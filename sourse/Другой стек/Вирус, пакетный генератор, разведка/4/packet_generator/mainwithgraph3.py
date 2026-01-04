#!/usr/bin/env python3
import socket
import struct
import threading
import time
from scapy.all import *
from scapy.layers.inet import IP, TCP, UDP, ICMP
from scapy.layers.l2 import Ether, ARP
from scapy.packet import Raw
import netifaces
import ipaddress
import random
import subprocess
import platform
import re
import tkinter as tk
from tkinter import ttk, messagebox, scrolledtext
import sys
import json

class PacketGeneratorGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Packet Generator - Генератор сетевых пакетов")
        self.root.geometry("1200x800")
        
        # Инициализация бэкенда
        self.generator = PacketGenerator()
        
        # Хранилище последовательностей пакетов
        self.packet_sequences = {}  # name -> list of packets
        self.current_sequence = []
        self.current_sequence_name = ""
        
        # Создание интерфейса
        self.create_widgets()
        
        # Автоматическое обнаружение интерфейсов при запуске
        self.root.after(100, self.auto_discover_interfaces)
    
    def create_widgets(self):
        """Создание всех элементов интерфейса"""
        # Создаем notebook для вкладок
        notebook = ttk.Notebook(self.root)
        notebook.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # Вкладка 1: Интерфейсы
        self.tab_interfaces = ttk.Frame(notebook)
        notebook.add(self.tab_interfaces, text="Интерфейсы")
        
        # Вкладка 2: Создание пакетов
        self.tab_packets = ttk.Frame(notebook)
        notebook.add(self.tab_packets, text="Создание пакетов")
        
        # Вкладка 3: Последовательности
        self.tab_sequences = ttk.Frame(notebook)
        notebook.add(self.tab_sequences, text="Последовательности")
        
        # Вкладка 4: Отправка
        self.tab_send = ttk.Frame(notebook)
        notebook.add(self.tab_send, text="Отправка пакетов")
        
        # Вкладка 5: Логи
        self.tab_logs = ttk.Frame(notebook)
        notebook.add(self.tab_logs, text="Логи и диагностика")
        
        self.setup_interfaces_tab()
        self.setup_packets_tab()
        self.setup_sequences_tab()
        self.setup_send_tab()
        self.setup_logs_tab()
    
    def setup_interfaces_tab(self):
        """Настройка вкладки интерфейсов"""
        # Фрейм для управления интерфейсами
        frame_control = ttk.LabelFrame(self.tab_interfaces, text="Управление интерфейсами", padding=10)
        frame_control.pack(fill=tk.X, padx=5, pady=5)
        
        ttk.Button(frame_control, text="Обнаружить интерфейсы", 
                  command=self.discover_interfaces).pack(side=tk.LEFT, padx=5)
        
        # Фрейм для списка интерфейсов
        frame_list = ttk.LabelFrame(self.tab_interfaces, text="Доступные интерфейсы", padding=10)
        frame_list.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # Таблица интерфейсов
        columns = ("name", "ip", "mac", "status")
        self.tree_interfaces = ttk.Treeview(frame_list, columns=columns, show="headings", height=10)
        
        self.tree_interfaces.heading("name", text="Интерфейс")
        self.tree_interfaces.heading("ip", text="IP адрес")
        self.tree_interfaces.heading("mac", text="MAC адрес")
        self.tree_interfaces.heading("status", text="Статус")
        
        self.tree_interfaces.column("name", width=200)
        self.tree_interfaces.column("ip", width=150)
        self.tree_interfaces.column("mac", width=150)
        self.tree_interfaces.column("status", width=100)
        
        # Scrollbar для таблицы
        scrollbar = ttk.Scrollbar(frame_list, orient=tk.VERTICAL, command=self.tree_interfaces.yview)
        self.tree_interfaces.configure(yscrollcommand=scrollbar.set)
        
        self.tree_interfaces.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # Привязка события выбора
        self.tree_interfaces.bind('<<TreeviewSelect>>', self.on_interface_select)
        
        # Фрейм для информации о выбранном интерфейсе
        frame_selected = ttk.LabelFrame(self.tab_interfaces, text="Выбранный интерфейс", padding=10)
        frame_selected.pack(fill=tk.X, padx=5, pady=5)
        
        self.selected_interface_var = tk.StringVar(value="Не выбран")
        ttk.Label(frame_selected, textvariable=self.selected_interface_var, 
                 font=('Arial', 10, 'bold')).pack()
    
    def setup_packets_tab(self):
        """Настройка вкладки создания пакетов"""
        # Основной фрейм с прокруткой
        main_frame = ttk.Frame(self.tab_packets)
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        canvas = tk.Canvas(main_frame)
        scrollbar = ttk.Scrollbar(main_frame, orient=tk.VERTICAL, command=canvas.yview)
        self.scrollable_frame = ttk.Frame(canvas)
        
        self.scrollable_frame.bind(
            "<Configure>",
            lambda e: canvas.configure(scrollregion=canvas.bbox("all"))
        )
        
        canvas.create_window((0, 0), window=self.scrollable_frame, anchor="nw")
        canvas.configure(yscrollcommand=scrollbar.set)
        
        canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # Ethernet настройки
        frame_ether = ttk.LabelFrame(self.scrollable_frame, text="Ethernet заголовок", padding=10)
        frame_ether.pack(fill=tk.X, padx=5, pady=5)
        
        ttk.Label(frame_ether, text="MAC отправителя:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.eth_src = ttk.Entry(frame_ether, width=20)
        self.eth_src.grid(row=0, column=1, padx=5, pady=2)
        self.eth_src.insert(0, "авто")
        
        ttk.Label(frame_ether, text="MAC получателя:").grid(row=0, column=2, sticky=tk.W, padx=5, pady=2)
        self.eth_dst = ttk.Entry(frame_ether, width=20)
        self.eth_dst.grid(row=0, column=3, padx=5, pady=2)
        self.eth_dst.insert(0, "авто")
        
        # IP настройки
        frame_ip = ttk.LabelFrame(self.scrollable_frame, text="IP заголовок", padding=10)
        frame_ip.pack(fill=tk.X, padx=5, pady=5)
        
        # Строка 1
        ttk.Label(frame_ip, text="IP отправителя:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.ip_src = ttk.Entry(frame_ip, width=15)
        self.ip_src.grid(row=0, column=1, padx=5, pady=2)
        self.ip_src.insert(0, "192.168.1.100")
        
        ttk.Label(frame_ip, text="IP получателя:").grid(row=0, column=2, sticky=tk.W, padx=5, pady=2)
        self.ip_dst = ttk.Entry(frame_ip, width=15)
        self.ip_dst.grid(row=0, column=3, padx=5, pady=2)
        self.ip_dst.insert(0, "192.168.1.1")
        
        ttk.Label(frame_ip, text="TTL:").grid(row=0, column=4, sticky=tk.W, padx=5, pady=2)
        self.ip_ttl = ttk.Entry(frame_ip, width=10)
        self.ip_ttl.grid(row=0, column=5, padx=5, pady=2)
        self.ip_ttl.insert(0, "64")
        
        # Строка 2
        ttk.Label(frame_ip, text="Версия:").grid(row=1, column=0, sticky=tk.W, padx=5, pady=2)
        self.ip_version = ttk.Entry(frame_ip, width=10)
        self.ip_version.grid(row=1, column=1, padx=5, pady=2)
        self.ip_version.insert(0, "4")
        
        ttk.Label(frame_ip, text="Длина заголовка:").grid(row=1, column=2, sticky=tk.W, padx=5, pady=2)
        self.ip_ihl = ttk.Entry(frame_ip, width=10)
        self.ip_ihl.grid(row=1, column=3, padx=5, pady=2)
        self.ip_ihl.insert(0, "5")
        
        ttk.Label(frame_ip, text="TOS:").grid(row=1, column=4, sticky=tk.W, padx=5, pady=2)
        self.ip_tos = ttk.Entry(frame_ip, width=10)
        self.ip_tos.grid(row=1, column=5, padx=5, pady=2)
        self.ip_tos.insert(0, "0")
        
        # Строка 3
        ttk.Label(frame_ip, text="ID:").grid(row=2, column=0, sticky=tk.W, padx=5, pady=2)
        self.ip_id = ttk.Entry(frame_ip, width=10)
        self.ip_id.grid(row=2, column=1, padx=5, pady=2)
        self.ip_id.insert(0, "1")
        
        ttk.Label(frame_ip, text="Флаги:").grid(row=2, column=2, sticky=tk.W, padx=5, pady=2)
        self.ip_flags = ttk.Entry(frame_ip, width=10)
        self.ip_flags.grid(row=2, column=3, padx=5, pady=2)
        self.ip_flags.insert(0, "0")
        
        ttk.Label(frame_ip, text="Смещение:").grid(row=2, column=4, sticky=tk.W, padx=5, pady=2)
        self.ip_frag = ttk.Entry(frame_ip, width=10)
        self.ip_frag.grid(row=2, column=5, padx=5, pady=2)
        self.ip_frag.insert(0, "0")
        
        # Строка 4
        ttk.Label(frame_ip, text="Контр. сумма:").grid(row=3, column=0, sticky=tk.W, padx=5, pady=2)
        self.ip_chksum = ttk.Entry(frame_ip, width=10)
        self.ip_chksum.grid(row=3, column=1, padx=5, pady=2)
        self.ip_chksum.insert(0, "авто")
        
        ttk.Label(frame_ip, text="Протокол:").grid(row=3, column=2, sticky=tk.W, padx=5, pady=2)
        self.ip_proto = ttk.Entry(frame_ip, width=10)
        self.ip_proto.grid(row=3, column=3, padx=5, pady=2)
        self.ip_proto.insert(0, "6")  # TCP по умолчанию
        
        # Выбор протокола
        frame_proto = ttk.LabelFrame(self.scrollable_frame, text="Транспортный протокол", padding=10)
        frame_proto.pack(fill=tk.X, padx=5, pady=5)
        
        self.protocol_var = tk.StringVar(value="tcp")
        
        ttk.Radiobutton(frame_proto, text="TCP", variable=self.protocol_var, 
                       value="tcp", command=self.on_protocol_change).grid(row=0, column=0, sticky=tk.W)
        ttk.Radiobutton(frame_proto, text="UDP", variable=self.protocol_var, 
                       value="udp", command=self.on_protocol_change).grid(row=0, column=1, sticky=tk.W)
        ttk.Radiobutton(frame_proto, text="ICMP Request", variable=self.protocol_var, 
                       value="icmp_req", command=self.on_protocol_change).grid(row=0, column=2, sticky=tk.W)
        ttk.Radiobutton(frame_proto, text="ICMP Reply", variable=self.protocol_var, 
                       value="icmp_rep", command=self.on_protocol_change).grid(row=0, column=3, sticky=tk.W)
        
        # TCP настройки
        self.frame_tcp = ttk.LabelFrame(self.scrollable_frame, text="TCP заголовок", padding=10)
        
        # Строка 1 TCP
        ttk.Label(self.frame_tcp, text="Порт источник:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.tcp_sport = ttk.Entry(self.frame_tcp, width=10)
        self.tcp_sport.grid(row=0, column=1, padx=5, pady=2)
        self.tcp_sport.insert(0, "12345")
        
        ttk.Label(self.frame_tcp, text="Порт назначения:").grid(row=0, column=2, sticky=tk.W, padx=5, pady=2)
        self.tcp_dport = ttk.Entry(self.frame_tcp, width=10)
        self.tcp_dport.grid(row=0, column=3, padx=5, pady=2)
        self.tcp_dport.insert(0, "80")
        
        ttk.Label(self.frame_tcp, text="Seq номер:").grid(row=0, column=4, sticky=tk.W, padx=5, pady=2)
        self.tcp_seq = ttk.Entry(self.frame_tcp, width=10)
        self.tcp_seq.grid(row=0, column=5, padx=5, pady=2)
        self.tcp_seq.insert(0, "0")
        
        # Строка 2 TCP
        ttk.Label(self.frame_tcp, text="Ack номер:").grid(row=1, column=0, sticky=tk.W, padx=5, pady=2)
        self.tcp_ack = ttk.Entry(self.frame_tcp, width=10)
        self.tcp_ack.grid(row=1, column=1, padx=5, pady=2)
        self.tcp_ack.insert(0, "0")
        
        ttk.Label(self.frame_tcp, text="Смещение данных:").grid(row=1, column=2, sticky=tk.W, padx=5, pady=2)
        self.tcp_dataofs = ttk.Entry(self.frame_tcp, width=10)
        self.tcp_dataofs.grid(row=1, column=3, padx=5, pady=2)
        self.tcp_dataofs.insert(0, "5")
        
        ttk.Label(self.frame_tcp, text="Зарезервировано:").grid(row=1, column=4, sticky=tk.W, padx=5, pady=2)
        self.tcp_reserved = ttk.Entry(self.frame_tcp, width=10)
        self.tcp_reserved.grid(row=1, column=5, padx=5, pady=2)
        self.tcp_reserved.insert(0, "0")
        
        # Строка 3 TCP
        ttk.Label(self.frame_tcp, text="Флаги:").grid(row=2, column=0, sticky=tk.W, padx=5, pady=2)
        self.tcp_flags = ttk.Entry(self.frame_tcp, width=10)
        self.tcp_flags.grid(row=2, column=1, padx=5, pady=2)
        self.tcp_flags.insert(0, "S")
        
        ttk.Label(self.frame_tcp, text="Окно:").grid(row=2, column=2, sticky=tk.W, padx=5, pady=2)
        self.tcp_window = ttk.Entry(self.frame_tcp, width=10)
        self.tcp_window.grid(row=2, column=3, padx=5, pady=2)
        self.tcp_window.insert(0, "8192")
        
        ttk.Label(self.frame_tcp, text="Контр. сумма:").grid(row=2, column=4, sticky=tk.W, padx=5, pady=2)
        self.tcp_chksum = ttk.Entry(self.frame_tcp, width=10)
        self.tcp_chksum.grid(row=2, column=5, padx=5, pady=2)
        self.tcp_chksum.insert(0, "авто")
        
        # Строка 4 TCP
        ttk.Label(self.frame_tcp, text="Указатель срочности:").grid(row=3, column=0, sticky=tk.W, padx=5, pady=2)
        self.tcp_urgptr = ttk.Entry(self.frame_tcp, width=10)
        self.tcp_urgptr.grid(row=3, column=1, padx=5, pady=2)
        self.tcp_urgptr.insert(0, "0")
        
        # UDP настройки
        self.frame_udp = ttk.LabelFrame(self.scrollable_frame, text="UDP заголовок", padding=10)
        
        ttk.Label(self.frame_udp, text="Порт источник:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.udp_sport = ttk.Entry(self.frame_udp, width=10)
        self.udp_sport.grid(row=0, column=1, padx=5, pady=2)
        self.udp_sport.insert(0, "12345")
        
        ttk.Label(self.frame_udp, text="Порт назначения:").grid(row=0, column=2, sticky=tk.W, padx=5, pady=2)
        self.udp_dport = ttk.Entry(self.frame_udp, width=10)
        self.udp_dport.grid(row=0, column=3, padx=5, pady=2)
        self.udp_dport.insert(0, "53")
        
        ttk.Label(self.frame_udp, text="Длина:").grid(row=0, column=4, sticky=tk.W, padx=5, pady=2)
        self.udp_len = ttk.Entry(self.frame_udp, width=10)
        self.udp_len.grid(row=0, column=5, padx=5, pady=2)
        self.udp_len.insert(0, "8")
        
        ttk.Label(self.frame_udp, text="Контр. сумма:").grid(row=1, column=0, sticky=tk.W, padx=5, pady=2)
        self.udp_chksum = ttk.Entry(self.frame_udp, width=10)
        self.udp_chksum.grid(row=1, column=1, padx=5, pady=2)
        self.udp_chksum.insert(0, "авто")
        
        # ICMP настройки
        self.frame_icmp = ttk.LabelFrame(self.scrollable_frame, text="ICMP заголовок", padding=10)
        
        ttk.Label(self.frame_icmp, text="Тип:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.icmp_type = ttk.Entry(self.frame_icmp, width=10)
        self.icmp_type.grid(row=0, column=1, padx=5, pady=2)
        self.icmp_type.insert(0, "8")
        
        ttk.Label(self.frame_icmp, text="Код:").grid(row=0, column=2, sticky=tk.W, padx=5, pady=2)
        self.icmp_code = ttk.Entry(self.frame_icmp, width=10)
        self.icmp_code.grid(row=0, column=3, padx=5, pady=2)
        self.icmp_code.insert(0, "0")
        
        ttk.Label(self.frame_icmp, text="Контр. сумма:").grid(row=0, column=4, sticky=tk.W, padx=5, pady=2)
        self.icmp_chksum = ttk.Entry(self.frame_icmp, width=10)
        self.icmp_chksum.grid(row=0, column=5, padx=5, pady=2)
        self.icmp_chksum.insert(0, "авто")
        
        ttk.Label(self.frame_icmp, text="ID:").grid(row=1, column=0, sticky=tk.W, padx=5, pady=2)
        self.icmp_id = ttk.Entry(self.frame_icmp, width=10)
        self.icmp_id.grid(row=1, column=1, padx=5, pady=2)
        self.icmp_id.insert(0, "1")
        
        ttk.Label(self.frame_icmp, text="Sequence:").grid(row=1, column=2, sticky=tk.W, padx=5, pady=2)
        self.icmp_seq = ttk.Entry(self.frame_icmp, width=10)
        self.icmp_seq.grid(row=1, column=3, padx=5, pady=2)
        self.icmp_seq.insert(0, "1")
        
        # Данные пакета
        frame_data = ttk.LabelFrame(self.scrollable_frame, text="Данные пакета", padding=10)
        frame_data.pack(fill=tk.X, padx=5, pady=5)
        
        self.packet_data = tk.Text(frame_data, height=4, width=80)
        self.packet_data.pack(fill=tk.X, padx=5, pady=5)
        
        # Кнопки управления
        frame_buttons = ttk.Frame(self.scrollable_frame)
        frame_buttons.pack(fill=tk.X, padx=5, pady=10)
        
        ttk.Button(frame_buttons, text="Показать пакет", 
                  command=self.show_packet).pack(side=tk.LEFT, padx=5)
        ttk.Button(frame_buttons, text="Добавить в последовательность", 
                  command=self.add_to_sequence).pack(side=tk.LEFT, padx=5)
        ttk.Button(frame_buttons, text="Сбросить значения", 
                  command=self.reset_fields).pack(side=tk.LEFT, padx=5)
    
    def setup_sequences_tab(self):
        """Настройка вкладки последовательностей пакетов"""
        # Основной фрейм
        main_frame = ttk.Frame(self.tab_sequences)
        main_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # Левая панель - управление последовательностями
        left_frame = ttk.LabelFrame(main_frame, text="Управление последовательностями", padding=10)
        left_frame.pack(side=tk.LEFT, fill=tk.Y, padx=5, pady=5)
        
        # Название последовательности
        ttk.Label(left_frame, text="Название последовательности:").pack(anchor=tk.W, pady=2)
        self.sequence_name = ttk.Entry(left_frame, width=20)
        self.sequence_name.pack(fill=tk.X, pady=5)
        
        # Кнопки управления
        ttk.Button(left_frame, text="Создать новую", 
                  command=self.create_new_sequence).pack(fill=tk.X, pady=2)
        #ttk.Button(left_frame, text="Сохранить последовательность", 
        #          command=self.save_sequence).pack(fill=tk.X, pady=2)
        #ttk.Button(left_frame, text="Загрузить последовательность", 
        #          command=self.load_sequence).pack(fill=tk.X, pady=2)
        ttk.Button(left_frame, text="Удалить последовательность", 
                  command=self.delete_sequence).pack(fill=tk.X, pady=2)
        
        # Список последовательностей
        ttk.Label(left_frame, text="Сохраненные последовательности:").pack(anchor=tk.W, pady=(10, 2))
        self.sequences_listbox = tk.Listbox(left_frame, height=10)
        self.sequences_listbox.pack(fill=tk.BOTH, expand=True, pady=5)
        self.sequences_listbox.bind('<<ListboxSelect>>', self.on_sequence_select)
        
        # Правая панель - текущая последовательность
        right_frame = ttk.LabelFrame(main_frame, text="Текущая последовательность", padding=10)
        right_frame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # Информация о текущей последовательности
        self.current_sequence_info = ttk.Label(right_frame, text="Последовательность не выбрана", 
                                             font=('Arial', 10, 'bold'))
        self.current_sequence_info.pack(anchor=tk.W, pady=5)
        
        # Таблица пакетов в последовательности
        columns = ("index", "type", "src", "dst", "sport", "dport")
        self.sequence_tree = ttk.Treeview(right_frame, columns=columns, show="headings", height=12)
        
        self.sequence_tree.heading("index", text="#")
        self.sequence_tree.heading("type", text="Тип")
        self.sequence_tree.heading("src", text="Источник")
        self.sequence_tree.heading("dst", text="Назначение")
        self.sequence_tree.heading("sport", text="Порт ист.")
        self.sequence_tree.heading("dport", text="Порт назн.")
        
        self.sequence_tree.column("index", width=40)
        self.sequence_tree.column("type", width=80)
        self.sequence_tree.column("src", width=120)
        self.sequence_tree.column("dst", width=120)
        self.sequence_tree.column("sport", width=80)
        self.sequence_tree.column("dport", width=80)
        
        # Scrollbar для таблицы
        scrollbar = ttk.Scrollbar(right_frame, orient=tk.VERTICAL, command=self.sequence_tree.yview)
        self.sequence_tree.configure(yscrollcommand=scrollbar.set)
        
        self.sequence_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # Кнопки управления пакетами в последовательности
        button_frame = ttk.Frame(right_frame)
        button_frame.pack(fill=tk.X, pady=5)
        
        ttk.Button(button_frame, text="Удалить пакет", 
                  command=self.remove_from_sequence).pack(side=tk.LEFT, padx=2)
        #ttk.Button(button_frame, text="Переместить вверх", 
        #          command=self.move_packet_up).pack(side=tk.LEFT, padx=2)
        #ttk.Button(button_frame, text="Переместить вниз", 
        #          command=self.move_packet_down).pack(side=tk.LEFT, padx=2)
        #ttk.Button(button_frame, text="Очистить последовательность", 
        #          command=self.clear_sequence).pack(side=tk.LEFT, padx=2)
    
    def setup_send_tab(self):
        """Настройка вкладки отправки"""
        # Настройки отправки
        frame_send = ttk.LabelFrame(self.tab_send, text="Настройки отправки", padding=10)
        frame_send.pack(fill=tk.X, padx=5, pady=5)
        
        ttk.Label(frame_send, text="Количество пакетов:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.send_count = ttk.Entry(frame_send, width=10)
        self.send_count.grid(row=0, column=1, padx=5, pady=2)
        self.send_count.insert(0, "1")
        
        ttk.Label(frame_send, text="Задержка (сек):").grid(row=0, column=2, sticky=tk.W, padx=5, pady=2)
        self.send_delay = ttk.Entry(frame_send, width=10)
        self.send_delay.grid(row=0, column=3, padx=5, pady=2)
        self.send_delay.insert(0, "0")
        
        ttk.Label(frame_send, text="Повторов последовательности:").grid(row=0, column=4, sticky=tk.W, padx=5, pady=2)
        self.sequence_repeats = ttk.Entry(frame_send, width=10)
        self.sequence_repeats.grid(row=0, column=5, padx=5, pady=2)
        self.sequence_repeats.insert(0, "1")
        
        # Кнопки отправки
        frame_buttons = ttk.Frame(frame_send)
        frame_buttons.grid(row=1, column=0, columnspan=6, pady=10)
        
        ttk.Button(frame_buttons, text="Отправить текущий пакет", 
                  command=self.send_current_packet).pack(side=tk.LEFT, padx=5)
        ttk.Button(frame_buttons, text="Отправить последовательность", 
                  command=self.send_sequence).pack(side=tk.LEFT, padx=5)
        #ttk.Button(frame_buttons, text="Быстрая отправка (Ping)", 
        #          command=self.quick_send).pack(side=tk.LEFT, padx=5)
        ttk.Button(frame_buttons, text="Остановить отправку", 
                  command=self.stop_sending).pack(side=tk.LEFT, padx=5)
        
        # Статус отправки
        self.send_status = ttk.Label(frame_send, text="Готов к отправке", foreground="green")
        self.send_status.grid(row=2, column=0, columnspan=6, pady=5)
        
        # Прогресс бар
        self.progress = ttk.Progressbar(frame_send, mode='determinate')
        self.progress.grid(row=3, column=0, columnspan=6, sticky=tk.EW, pady=5)
        
        # Превью пакета/последовательности
        frame_preview = ttk.LabelFrame(self.tab_send, text="Превью", padding=10)
        frame_preview.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        self.packet_preview = scrolledtext.ScrolledText(frame_preview, height=15, width=80)
        self.packet_preview.pack(fill=tk.BOTH, expand=True)
        
        # Флаг для остановки отправки
        self.stop_sending_flag = False
    
    def setup_logs_tab(self):
        """Настройка вкладки логов"""
        frame_logs = ttk.LabelFrame(self.tab_logs, text="Логи работы", padding=10)
        frame_logs.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        self.log_text = scrolledtext.ScrolledText(frame_logs, height=20, width=80)
        self.log_text.pack(fill=tk.BOTH, expand=True)
        
        # Кнопки управления логами
        frame_log_buttons = ttk.Frame(frame_logs)
        frame_log_buttons.pack(fill=tk.X, pady=5)
        
        ttk.Button(frame_log_buttons, text="Очистить логи", 
                  command=self.clear_logs).pack(side=tk.LEFT, padx=5)
        ttk.Button(frame_log_buttons, text="Сохранить логи", 
                  command=self.save_logs).pack(side=tk.LEFT, padx=5)
    
    # ===== МЕТОДЫ ДЛЯ РАБОТЫ С ПОСЛЕДОВАТЕЛЬНОСТЯМИ =====
    
    def create_new_sequence(self):
        """Создание новой последовательности"""
        name = self.sequence_name.get().strip()
        if not name:
            messagebox.showwarning("Предупреждение", "Введите название последовательности!")
            return
        
        if name in self.packet_sequences:
            if not messagebox.askyesno("Подтверждение", 
                                     f"Последовательность '{name}' уже существует. Перезаписать?"):
                return
        
        self.packet_sequences[name] = []
        self.current_sequence_name = name
        self.current_sequence = []
        self.update_sequences_list()
        self.update_sequence_display()
        self.log_message(f"Создана новая последовательность: {name}")
    
    def add_to_sequence(self):
        """Добавление текущего пакета в последовательность"""
        if not self.current_sequence_name:
            messagebox.showwarning("Предупреждение", "Сначала создайте или выберите последовательность!")
            return
        
        packet = self.create_packet_from_gui()
        if not packet:
            return
        
        self.current_sequence.append(packet)
        self.packet_sequences[self.current_sequence_name] = self.current_sequence
        self.update_sequence_display()
        self.log_message(f"Пакет добавлен в последовательность '{self.current_sequence_name}'")
    
    def remove_from_sequence(self):
        """Удаление выбранного пакета из последовательности"""
        selection = self.sequence_tree.selection()
        if not selection:
            messagebox.showwarning("Предупреждение", "Выберите пакет для удаления!")
            return
        
        item = self.sequence_tree.item(selection[0])
        index = int(item['values'][0]) - 1
        
        if 0 <= index < len(self.current_sequence):
            self.current_sequence.pop(index)
            self.packet_sequences[self.current_sequence_name] = self.current_sequence
            self.update_sequence_display()
            self.log_message(f"Пакет #{index+1} удален из последовательности")
    
    def move_packet_up(self):
        """Перемещение пакета вверх в последовательности"""
        self._move_packet(-1)
    
    def move_packet_down(self):
        """Перемещение пакета вниз в последовательности"""
        self._move_packet(1)
    
    def _move_packet(self, direction):
        """Вспомогательный метод для перемещения пакета"""
        selection = self.sequence_tree.selection()
        if not selection:
            return
        
        item = self.sequence_tree.item(selection[0])
        index = int(item['values'][0]) - 1
        new_index = index + direction
        
        if 0 <= new_index < len(self.current_sequence):
            self.current_sequence[index], self.current_sequence[new_index] = \
                self.current_sequence[new_index], self.current_sequence[index]
            self.packet_sequences[self.current_sequence_name] = self.current_sequence
            self.update_sequence_display()
            self.log_message(f"Пакет #{index+1} перемещен")
    
    def clear_sequence(self):
        """Очистка текущей последовательности"""
        if self.current_sequence:
            if messagebox.askyesno("Подтверждение", "Очистить текущую последовательность?"):
                self.current_sequence.clear()
                self.packet_sequences[self.current_sequence_name] = self.current_sequence
                self.update_sequence_display()
                self.log_message("Последовательность очищена")
    
    def on_sequence_select(self, event):
        """Обработка выбора последовательности из списка"""
        selection = self.sequences_listbox.curselection()
        if selection:
            name = self.sequences_listbox.get(selection[0])
            self.current_sequence_name = name
            self.current_sequence = self.packet_sequences.get(name, [])
            self.sequence_name.delete(0, tk.END)
            self.sequence_name.insert(0, name)
            self.update_sequence_display()
    
    def update_sequences_list(self):
        """Обновление списка последовательностей"""
        self.sequences_listbox.delete(0, tk.END)
        for name in self.packet_sequences.keys():
            self.sequences_listbox.insert(tk.END, name)
    
    def update_sequence_display(self):
        """Обновление отображения текущей последовательности"""
        self.sequence_tree.delete(*self.sequence_tree.get_children())
        
        if self.current_sequence_name:
            self.current_sequence_info.config(
                text=f"Последовательность: {self.current_sequence_name} ({len(self.current_sequence)} пакетов)")
            
            for i, packet in enumerate(self.current_sequence, 1):
                # Определяем тип пакета и основные параметры
                pkt_type = "Unknown"
                src = "N/A"
                dst = "N/A"
                sport = "N/A"
                dport = "N/A"
                
                if IP in packet:
                    src = packet[IP].src
                    dst = packet[IP].dst
                    
                    if TCP in packet:
                        pkt_type = "TCP"
                        sport = packet[TCP].sport
                        dport = packet[TCP].dport
                    elif UDP in packet:
                        pkt_type = "UDP"
                        sport = packet[UDP].sport
                        dport = packet[UDP].dport
                    elif ICMP in packet:
                        pkt_type = "ICMP"
                        sport = packet[ICMP].type
                        dport = packet[ICMP].code
                
                self.sequence_tree.insert("", tk.END, values=(
                    i, pkt_type, src, dst, sport, dport
                ))
        else:
            self.current_sequence_info.config(text="Последовательность не выбрана")
    
    def save_sequence(self):
        """Сохранение последовательности в файл"""
        if not self.current_sequence_name:
            messagebox.showwarning("Предупреждение", "Нет активной последовательности для сохранения!")
            return
        
        try:
            filename = f"sequence_{self.current_sequence_name}_{time.strftime('%Y%m%d_%H%M%S')}.pcap"
            packets = self.packet_sequences[self.current_sequence_name]
            wrpcap(filename, packets)
            self.log_message(f"Последовательность '{self.current_sequence_name}' сохранена в {filename}")
            messagebox.showinfo("Успех", f"Последовательность сохранена в {filename}")
        except Exception as e:
            self.log_message(f"Ошибка сохранения последовательности: {e}", "error")
            messagebox.showerror("Ошибка", f"Не удалось сохранить последовательность: {e}")
    
    def load_sequence(self):
        """Загрузка последовательности из файла"""
        # Здесь можно добавить диалог выбора файла
        messagebox.showinfo("Информация", "Функция загрузки последовательности в разработке")
    
    def delete_sequence(self):
        """Удаление последовательности"""
        if not self.current_sequence_name:
            messagebox.showwarning("Предупреждение", "Выберите последовательность для удаления!")
            return
        
        if messagebox.askyesno("Подтверждение", 
                             f"Удалить последовательность '{self.current_sequence_name}'?"):
            del self.packet_sequences[self.current_sequence_name]
            self.current_sequence_name = ""
            self.current_sequence = []
            self.sequence_name.delete(0, tk.END)
            self.update_sequences_list()
            self.update_sequence_display()
            self.log_message(f"Последовательность удалена")
    
    def send_sequence(self):
        """Отправка текущей последовательности"""
        if not self.generator.selected_interface:
            messagebox.showwarning("Предупреждение", "Сначала выберите интерфейс!")
            return
        
        if not self.current_sequence:
            messagebox.showwarning("Предупреждение", "Текущая последовательность пуста!")
            return
        
        try:
            count = int(self.sequence_repeats.get())
            delay = float(self.send_delay.get())
            
            self.log_message(f"Начало отправки последовательности '{self.current_sequence_name}' ({count} повторов)...")
            self.stop_sending_flag = False
            
            def send_sequence_thread():
                try:
                    total_packets = len(self.current_sequence) * count
                    packet_counter = 0
                    
                    for repeat in range(count):
                        if self.stop_sending_flag:
                            break
                            
                        self.root.after(0, lambda: self.log_message(f"Повтор {repeat+1}/{count}"))
                        
                        for i, packet in enumerate(self.current_sequence):
                            if self.stop_sending_flag:
                                break
                            
                            packet_counter += 1
                            self.root.after(0, lambda pc=packet_counter, tp=total_packets: 
                                          self.update_progress(pc, tp))
                            self.root.after(0, lambda: self.send_status.config(
                                text=f"Отправка {packet_counter}/{total_packets}", foreground="blue"))
                            
                            self.generator.send_packets([packet], 1, 0)
                            self.root.after(0, lambda: self.log_message(
                                f"Пакет {i+1}/{len(self.current_sequence)} отправлен (повтор {repeat+1})"))
                            
                            if delay > 0 and (i < len(self.current_sequence) - 1 or repeat < count - 1):
                                time.sleep(delay)
                    
                    if not self.stop_sending_flag:
                        self.root.after(0, lambda: self.send_status.config(
                            text="Отправка завершена", foreground="green"))
                        self.root.after(0, lambda: self.log_message(
                            f"Последовательность отправлена {count} раз(а)"))
                        self.root.after(0, lambda: self.progress.config(value=0))
                        
                except Exception as e:
                    self.root.after(0, lambda: self.log_message(f"Ошибка отправки последовательности: {e}", "error"))
                    self.root.after(0, lambda: self.send_status.config(text="Ошибка отправки", foreground="red"))
            
            threading.Thread(target=send_sequence_thread, daemon=True).start()
            
        except ValueError as e:
            self.log_message(f"Ошибка в параметрах отправки: {e}", "error")
            messagebox.showerror("Ошибка", "Проверьте правильность введенных параметров")

    # Остальные методы остаются без изменений...
    def on_protocol_change(self):
        """Обработка изменения протокола"""
        protocol = self.protocol_var.get()
        
        # Скрываем все фреймы
        self.frame_tcp.pack_forget()
        self.frame_udp.pack_forget()
        self.frame_icmp.pack_forget()
        
        # Обновляем поле протокола в IP
        if protocol == 'tcp':
            self.ip_proto.delete(0, tk.END)
            self.ip_proto.insert(0, "6")
            self.frame_tcp.pack(fill=tk.X, padx=5, pady=5)
        elif protocol == 'udp':
            self.ip_proto.delete(0, tk.END)
            self.ip_proto.insert(0, "17")
            self.frame_udp.pack(fill=tk.X, padx=5, pady=5)
        elif protocol == 'icmp_req':
            self.ip_proto.delete(0, tk.END)
            self.ip_proto.insert(0, "1")
            self.icmp_type.delete(0, tk.END)
            self.icmp_type.insert(0, "8")  # Echo Request
            self.icmp_code.delete(0, tk.END)
            self.icmp_code.insert(0, "0")
            self.frame_icmp.pack(fill=tk.X, padx=5, pady=5)
        elif protocol == 'icmp_rep':
            self.ip_proto.delete(0, tk.END)
            self.ip_proto.insert(0, "1")
            self.icmp_type.delete(0, tk.END)
            self.icmp_type.insert(0, "0")  # Echo Reply
            self.icmp_code.delete(0, tk.END)
            self.icmp_code.insert(0, "0")
            self.frame_icmp.pack(fill=tk.X, padx=5, pady=5)
    
    def reset_fields(self):
        """Сброс полей к значениям по умолчанию"""
        self.ip_src.delete(0, tk.END)
        self.ip_src.insert(0, "192.168.1.100")
        self.ip_dst.delete(0, tk.END)
        self.ip_dst.insert(0, "192.168.1.1")
        self.ip_ttl.delete(0, tk.END)
        self.ip_ttl.insert(0, "64")
        
        self.log_message("Поля сброшены к значениям по умолчанию")
    
    def create_packet_from_gui(self):
        """Создание пакета на основе настроек GUI"""
        try:
            # Ethernet
            src_mac = None if self.eth_src.get() == "авто" else self.eth_src.get()
            dst_mac = None if self.eth_dst.get() == "авто" else self.eth_dst.get()
            
            ether = self.generator.create_ethernet_header(
                self.ip_src.get(), self.ip_dst.get(), src_mac, dst_mac
            )
            
            # IP с всеми полями
            ip_params = {
                'src': self.ip_src.get(),
                'dst': self.ip_dst.get(),
                'ttl': int(self.ip_ttl.get()),
                'version': int(self.ip_version.get()),
                'ihl': int(self.ip_ihl.get()),
                'tos': int(self.ip_tos.get()),
                'id': int(self.ip_id.get()),
                'flags': int(self.ip_flags.get()),
                'frag': int(self.ip_frag.get()),
                'proto': int(self.ip_proto.get())
            }
            
            if self.ip_chksum.get() != "авто":
                ip_params['chksum'] = int(self.ip_chksum.get())
            
            ip_layer = self.generator.create_ip_packet(**ip_params)
            packet = ether / ip_layer
            
            # Транспортный уровень
            protocol = self.protocol_var.get()
            
            if protocol == 'tcp':
                tcp_params = {
                    'sport': int(self.tcp_sport.get()),
                    'dport': int(self.tcp_dport.get()),
                    'seq': int(self.tcp_seq.get()),
                    'ack': int(self.tcp_ack.get()),
                    'dataofs': int(self.tcp_dataofs.get()),
                    'reserved': int(self.tcp_reserved.get()),
                    'flags': self.tcp_flags.get(),
                    'window': int(self.tcp_window.get()),
                    'urgptr': int(self.tcp_urgptr.get())
                }
                
                if self.tcp_chksum.get() != "авто":
                    tcp_params['chksum'] = int(self.tcp_chksum.get())
                    
                transport = self.generator.create_tcp_packet(**tcp_params)
                
            elif protocol == 'udp':
                udp_params = {
                    'sport': int(self.udp_sport.get()),
                    'dport': int(self.udp_dport.get()),
                    'len': int(self.udp_len.get())
                }
                
                if self.udp_chksum.get() != "авто":
                    udp_params['chksum'] = int(self.udp_chksum.get())
                    
                transport = self.generator.create_udp_packet(**udp_params)
                
            elif protocol in ['icmp_req', 'icmp_rep']:
                icmp_params = {
                    'type': int(self.icmp_type.get()),
                    'code': int(self.icmp_code.get()),
                    'id': int(self.icmp_id.get()),
                    'seq': int(self.icmp_seq.get())
                }
                
                if self.icmp_chksum.get() != "авто":
                    icmp_params['chksum'] = int(self.icmp_chksum.get())
                    
                icmp_type_str = "echo_request" if protocol == 'icmp_req' else "echo_reply"
                transport = self.generator.create_icmp_packet(icmp_type_str, **icmp_params)
            
            packet = packet / transport
            
            # Данные
            data_text = self.packet_data.get(1.0, tk.END)
            #data_text = self.packet_data.get(1.0, tk.END).strip()
            if data_text == "\n":
                data_text = None
            if data_text:
                packet = packet / Raw(load=data_text.encode() if isinstance(data_text, str) else data_text)
            
            return packet
            
        except Exception as e:
            self.log_message(f"Ошибка создания пакета: {e}", "error")
            messagebox.showerror("Ошибка", f"Не удалось создать пакет: {e}")
            return None

    def log_message(self, message, level="info"):
        """Добавление сообщения в лог"""
        timestamp = time.strftime("%H:%M:%S")
        if level == "error":
            tag = "ERROR"
            color = "red"
        elif level == "warning":
            tag = "WARN"
            color = "orange"
        else:
            tag = "INFO"
            color = "black"
        
        log_entry = f"[{timestamp}] [{tag}] {message}\n"
        
        self.log_text.insert(tk.END, log_entry)
        self.log_text.see(tk.END)
        
        # Вывод в консоль для отладки
        print(f"[{tag}] {message}")
    
    def clear_logs(self):
        """Очистка логов"""
        self.log_text.delete(1.0, tk.END)
    
    def save_logs(self):
        """Сохранение логов в файл"""
        try:
            filename = f"packet_generator_log_{time.strftime('%Y%m%d_%H%M%S')}.txt"
            with open(filename, 'w', encoding='utf-8') as f:
                f.write(self.log_text.get(1.0, tk.END))
            self.log_message(f"Логи сохранены в файл: {filename}")
            messagebox.showinfo("Успех", f"Логи сохранены в {filename}")
        except Exception as e:
            self.log_message(f"Ошибка сохранения логов: {e}", "error")
            messagebox.showerror("Ошибка", f"Не удалось сохранить логи: {e}")
    
    def auto_discover_interfaces(self):
        """Автоматическое обнаружение интерфейсов при запуске"""
        self.log_message("Автоматическое обнаружение интерфейсов...")
        self.discover_interfaces()
    
    def discover_interfaces(self):
        """Обнаружение интерфейсов"""
        try:
            self.tree_interfaces.delete(*self.tree_interfaces.get_children())
            self.generator.discover_interfaces()
            
            for iface_name, details in self.generator.interfaces.items():
                status = "Доступен" if details['ip'] != 'N/A' else "Недоступен"
                self.tree_interfaces.insert("", tk.END, values=(
                    iface_name, details['ip'], details['mac'], status
                ))
            
            self.log_message(f"Обнаружено {len(self.generator.interfaces)} интерфейсов")
            
        except Exception as e:
            self.log_message(f"Ошибка обнаружения интерфейсов: {e}", "error")
            messagebox.showerror("Ошибка", f"Не удалось обнаружить интерфейсы: {e}")
    
    def on_interface_select(self, event):
        """Обработка выбора интерфейса"""
        selection = self.tree_interfaces.selection()
        if selection:
            item = self.tree_interfaces.item(selection[0])
            iface_name = item['values'][0]
            self.generator.selected_interface = iface_name
            self.selected_interface_var.set(f"Выбран: {iface_name}")
            self.log_message(f"Выбран интерфейс: {iface_name}")
    
    def show_packet(self):
        """Показать созданный пакет"""
        packet = self.create_packet_from_gui()
        if packet:
            # Обновляем превью
            self.packet_preview.delete(1.0, tk.END)
            
            # Форматируем вывод пакета
            packet_str = f"=== Созданный пакет ===\n"
            packet_str += f"Время: {time.strftime('%Y-%m-%d %H:%M:%S')}\n"
            packet_str += f"Размер: {len(packet)} байт\n"
            packet_str += "=" * 50 + "\n"
            
            # Используем scapy для красивого вывода
            from io import StringIO
            import sys
            
            old_stdout = sys.stdout
            sys.stdout = captured_output = StringIO()
            
            packet.show()
            
            sys.stdout = old_stdout
            packet_str += captured_output.getvalue()
            
            self.packet_preview.insert(1.0, packet_str)
            self.log_message("Пакет создан и отображен в превью")
    
    def send_current_packet(self):
        """Отправка текущего пакета"""
        if not self.generator.selected_interface:
            messagebox.showwarning("Предупреждение", "Сначала выберите интерфейс!")
            return
        
        packet = self.create_packet_from_gui()
        if not packet:
            return
        
        try:
            count = int(self.send_count.get())
            delay = float(self.send_delay.get())
            
            self.log_message(f"Начало отправки {count} пакетов...")
            self.stop_sending_flag = False
            
            # Запускаем отправку в отдельном потоке
            def send_thread():
                try:
                    for i in range(count):
                        if self.stop_sending_flag:
                            self.root.after(0, lambda: self.log_message("Отправка остановлена"))
                            break
                        
                        self.root.after(0, lambda i=i: self.update_progress(i, count))
                        self.root.after(0, lambda: self.send_status.config(text=f"Отправка {i+1}/{count}", foreground="blue"))
                        
                        self.generator.send_packets([packet], 1, 0)
                        
                        if delay > 0 and i < count - 1:
                            time.sleep(delay)
                    
                    if not self.stop_sending_flag:
                        self.root.after(0, lambda: self.send_status.config(text="Отправка завершена", foreground="green"))
                        self.root.after(0, lambda: self.log_message(f"Отправлено {count} пакетов"))
                        self.root.after(0, lambda: self.progress.config(value=0))
                        
                except Exception as e:
                    self.root.after(0, lambda: self.log_message(f"Ошибка отправки: {e}", "error"))
                    self.root.after(0, lambda: self.send_status.config(text="Ошибка отправки", foreground="red"))
            
            threading.Thread(target=send_thread, daemon=True).start()
            
        except ValueError as e:
            self.log_message(f"Ошибка в параметрах отправки: {e}", "error")
            messagebox.showerror("Ошибка", "Проверьте правильность введенных параметров")
    
    def quick_send(self):
        """Быстрая отправка ping пакетов"""
        if not self.generator.selected_interface:
            messagebox.showwarning("Предупреждение", "Сначала выберите интерфейс!")
            return
        
        try:
            dst_ip = self.ip_dst.get() or "8.8.8.8"
            count = 4
            
            self.log_message(f"Быстрая отправка ping на {dst_ip}...")
            
            # Создаем ICMP пакет
            if self.generator.selected_interface in self.generator.interfaces:
                src_ip = self.generator.interfaces[self.generator.selected_interface]['ip']
            else:
                src_ip = "192.168.1.100"
            
            ether = self.generator.create_ethernet_header(src_ip, dst_ip)
            ip = self.generator.create_ip_packet(src=src_ip, dst=dst_ip)
            icmp = self.generator.create_icmp_packet("echo_request")
            
            packet = ether / ip / icmp
            
            def ping_thread():
                try:
                    for i in range(count):
                        if self.stop_sending_flag:
                            break
                        
                        self.root.after(0, lambda i=i: self.update_progress(i, count))
                        self.generator.send_packets([packet], 1, 0)
                        self.root.after(0, lambda: self.log_message(f"Ping {i+1}/{count} отправлен"))
                        time.sleep(1)
                    
                    if not self.stop_sending_flag:
                        self.root.after(0, lambda: self.log_message("Быстрая отправка завершена"))
                        self.root.after(0, lambda: self.progress.config(value=0))
                        
                except Exception as e:
                    self.root.after(0, lambda: self.log_message(f"Ошибка быстрой отправки: {e}", "error"))
            
            threading.Thread(target=ping_thread, daemon=True).start()
            
        except Exception as e:
            self.log_message(f"Ошибка быстрой отправки: {e}", "error")
    
    def stop_sending(self):
        """Остановка отправки пакетов"""
        self.stop_sending_flag = True
        self.send_status.config(text="Остановка...", foreground="orange")
        self.log_message("Запрошена остановка отправки")
    
    def update_progress(self, current, total):
        """Обновление прогресс бара"""
        progress = (current / total) * 100
        self.progress.config(value=progress)


class PacketGenerator:
    def __init__(self):
        self.interfaces = {}
        self.scapy_interfaces = []
        self.selected_interface = None
        self.arp_cache = {}
        
    def get_scapy_interfaces(self):
        # Получаем интерфейсы
        try:
            return get_if_list()
        except:
            return []
    
    def get_interface_details(self, iface_name):
        details = {
            'ip': 'N/A',
            'mac': 'N/A',
            'description': iface_name
        }   
        return details
    
    def discover_interfaces(self):
        print("Поиск интерфейсов...")
        
        self.scapy_interfaces = self.get_scapy_interfaces()
        
        if not self.scapy_interfaces:
            print("Не найдено интерфейсов")
            return
        
        print(f"Найдено интерфейсов: {len(self.scapy_interfaces)}")
        
        for i, iface_name in enumerate(self.scapy_interfaces):
            details = self.get_interface_details(iface_name)
            details['ip'] = get_if_addr(iface_name)
            details['mac'] = get_if_hwaddr(iface_name)
            self.interfaces[iface_name] = details
            
            print(f"{i+1}. Интерфейс: {iface_name}")
            print(f"   IP: {details['ip']}")
            print(f"   MAC: {details['mac']}")
            print(f"   Описание: {details['description']}")
            print("-" * 50)
    
    def get_mac_by_ip(self, ip_address):
        # Получение MAC-адреса по IP
        if ip_address in self.arp_cache:
            return self.arp_cache[ip_address]
            
        try:
            print(f"Выполняем ARP запрос для {ip_address}...")
            ans, unans = arping(ip_address, timeout=2, verbose=0)
            if ans:
                mac = ans[0][1].hwsrc
                self.arp_cache[ip_address] = mac
                print(f"Найден MAC для {ip_address}: {mac}")
                return mac
        except Exception as e:
            print(f"ARP запрос не удался: {e}")
            
        random_mac = "02:00:00:%02x:%02x:%02x" % (random.randint(0, 255), 
                                                 random.randint(0, 255), 
                                                 random.randint(0, 255))
        print(f"Используем случайный MAC для {ip_address}: {random_mac}")
        self.arp_cache[ip_address] = random_mac
        return random_mac
    
    def create_ethernet_header(self, src_ip, dst_ip, custom_src_mac=None, custom_dst_mac=None):
        # Создание Ethernet заголовка
        if custom_src_mac:
            src_mac = custom_src_mac
        else:
            if self.selected_interface in self.interfaces:
                src_mac = self.interfaces[self.selected_interface]['mac']
                if src_mac == 'N/A':
                    src_mac = "00:11:22:33:44:55"
            else:
                src_mac = "00:11:22:33:44:55"
            
        if custom_dst_mac:
            dst_mac = custom_dst_mac
        else:
            dst_mac = self.get_mac_by_ip(dst_ip)
            
        print(f"Ethernet: {src_mac} -> {dst_mac}")
        return Ether(src=src_mac, dst=dst_mac)
    
    def create_ip_packet(self, **kwargs):
        # Создание IP пакета
        ip_layer = IP()
        
        if 'src' in kwargs: ip_layer.src = kwargs['src']
        if 'dst' in kwargs: ip_layer.dst = kwargs['dst']
        if 'ttl' in kwargs: ip_layer.ttl = kwargs['ttl']
        if 'proto' in kwargs: ip_layer.proto = kwargs['proto']
        if 'version' in kwargs: ip_layer.version = kwargs['version']
        if 'ihl' in kwargs: ip_layer.ihl = kwargs['ihl']
        if 'tos' in kwargs: ip_layer.tos = kwargs['tos']
        if 'len' in kwargs: ip_layer.len = kwargs['len']
        if 'id' in kwargs: ip_layer.id = kwargs['id']
        if 'flags' in kwargs: ip_layer.flags = kwargs['flags']
        if 'frag' in kwargs: ip_layer.frag = kwargs['frag']
        if 'chksum' in kwargs: ip_layer.chksum = kwargs['chksum']
        
        return ip_layer
    
    def create_tcp_packet(self, **kwargs):
        # Создание TCP пакета
        tcp_layer = TCP()
        
        if 'sport' in kwargs: tcp_layer.sport = kwargs['sport']
        if 'dport' in kwargs: tcp_layer.dport = kwargs['dport']
        if 'seq' in kwargs: tcp_layer.seq = kwargs['seq']
        if 'ack' in kwargs: tcp_layer.ack = kwargs['ack']
        if 'flags' in kwargs: tcp_layer.flags = kwargs['flags']
        if 'window' in kwargs: tcp_layer.window = kwargs['window']
        if 'dataofs' in kwargs: tcp_layer.dataofs = kwargs['dataofs']
        if 'reserved' in kwargs: tcp_layer.reserved = kwargs['reserved']
        if 'urgptr' in kwargs: tcp_layer.urgptr = kwargs['urgptr']
        if 'chksum' in kwargs: tcp_layer.chksum = kwargs['chksum']
        
        return tcp_layer
    
    def create_udp_packet(self, **kwargs):
        # Создание UDP пакета
        udp_layer = UDP()
        
        if 'sport' in kwargs: udp_layer.sport = kwargs['sport']
        if 'dport' in kwargs: udp_layer.dport = kwargs['dport']
        if 'len' in kwargs: udp_layer.len = kwargs['len']
        if 'chksum' in kwargs: udp_layer.chksum = kwargs['chksum']
        
        return udp_layer
    
    def create_icmp_packet(self, icmp_type, **kwargs):
        # Создание ICMP пакета
        if icmp_type == "echo_request":
            icmp_layer = ICMP(type=8, code=0)
        elif icmp_type == "echo_reply":
            icmp_layer = ICMP(type=0, code=0)
        else:
            icmp_layer = ICMP()
        
        if 'type' in kwargs: icmp_layer.type = kwargs['type']
        if 'code' in kwargs: icmp_layer.code = kwargs['code']
        if 'chksum' in kwargs: icmp_layer.chksum = kwargs['chksum']
        if 'id' in kwargs: icmp_layer.id = kwargs['id']
        if 'seq' in kwargs: icmp_layer.seq = kwargs['seq']
        
        return icmp_layer
    
    def send_packets(self, packets, count=1, delay=0):
        # Отправка пакетов
        if not self.selected_interface:
            print("Ошибка: Интерфейс не выбран!")
            return
            
        try:
            print(f"Отправка через интерфейс: {self.selected_interface}")
            for i in range(count):
                for packet in packets:
                    print(f"Отправка пакета {i+1}/{count}...")
                    sendp(packet, iface=self.selected_interface, verbose=False)
                    print(f"Пакет {i+1}/{count} отправлен")
                    if delay > 0:
                        time.sleep(delay)
        except Exception as e:
            print(f"Ошибка отправки: {e}")
    
    def test_interface(self):
        # Тестирование интерфейса
        if not self.selected_interface:
            print("Интерфейс не выбран!")
            return False
            
        print(f"Тестирование интерфейса: {self.selected_interface}")
        
        try:
            test_packet = Ether() / IP(dst="8.8.8.8", ttl=64) / ICMP()
            sendp(test_packet, iface=self.selected_interface, verbose=False)
            print("Тестовый пакет отправлен успешно!")
            return True
        except Exception as e:
            print(f"Тест не пройден: {e}")
            return False

def main():
    """Запуск графического интерфейса"""
    try:
        root = tk.Tk()
        app = PacketGeneratorGUI(root)
        root.mainloop()
    except Exception as e:
        print(f"Ошибка запуска GUI: {e}")
        print("Запуск в консольном режиме...")

if __name__ == "__main__":
    main()