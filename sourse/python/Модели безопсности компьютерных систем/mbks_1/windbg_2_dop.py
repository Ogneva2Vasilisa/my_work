import os
import subprocess
from threading import Thread
from queue import Queue, Empty
import time

# Конфигурация
SEPARATORS = list(b",:=;%./\"")

ERROR_LOG = "error_log.json"
MAX_CONSECUTIVE_ERRORS = 1
BACKUP_FILE = "uuu.xlsx.original"
TARGET_FILE = "uuu.xlsx"
EXCEL_PATH = r"C:\Program Files\Microsoft Office\root\Office16\EXCEL.EXE"  # Путь к Excel
drrun_path = r"C:\Users\Vasilisa\Downloads\DynamoRIO-Windows-11.90.20168\bin64\drrun.exe"

def replace_byte_in_file(filename, offset, new_byte):
    """Замена байта в файле"""
    try:
        with open(filename, 'rb') as f:
            data = bytearray(f.read())
        
        if offset >= len(data):
            print(f"Ошибка: смещение {offset} превышает размер файла")
            return False

        old_byte = data[offset]
        data[offset] = new_byte
        
        with open(filename, 'wb') as f:
            f.write(data)
        
        print(f"Замена: 0x{old_byte:02X} → 0x{new_byte:02X} по смещению {offset}")
        return True

    except Exception as e:
        print(f"Ошибка замены: {str(e)}")
        return False

def run_windbg_trace():
    """Запуск трассировки в WinDbg"""
    try:
        if not os.path.exists(EXCEL_PATH):
            print(f"Ошибка: {EXCEL_PATH} не найден!")
            return False

        windbg_path = r"C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\windbg.exe"
        if not os.path.exists(windbg_path):
            print("WinDbg не найден! Установите Windows SDK")
            return False

        log_file = f"trace_{int(time.time())}.log"
        cmd = [
            windbg_path,
            '-g',
            '-c',
            f'bp /1 @$exentry; .logopen {log_file}; .logclose; q',
            EXCEL_PATH
        ]

        print(f"[+] Запуск трассировки (лог: {log_file})...")
        subprocess.run(cmd, check=True)
        print("[+] Трассировка завершена")
        return True

    except subprocess.CalledProcessError as e:
        print(f"Ошибка WinDbg: {e}")
        return False

def run_windbg_coverage(log_dir):
    """Анализ покрытия через WinDbg"""
    try:
        
        log_filename = "coverage.log"
        
        log_p="e://mbks_1/"+ log_dir
        log_p=log_p + "/" + log_filename

        windbg_path = r"C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\windbg.exe"
        cmd = [
            windbg_path,
            '-g',
            '-c',
            f'bp /1 @$exentry; .logopen /t /u {log_p}; .logclose; q',
            EXCEL_PATH
        ]

        print("[+] Анализ покрытия...")
        subprocess.run(cmd, check=True)
        
        if os.path.exists("coverage.log"):
            with open("coverage.log", 'r') as f:
                print(f.read())
            return True
        return False

    except Exception as e:
        print(f"Ошибка: {str(e)}")
        return False

def main2():
    while True:
        print("\nМеню:")
        print("1 - Заменить байт в config_15")
        print("2 - Трассировка в WinDbg (регистры + стек)")
        print("3 - Анализ покрытия в WinDbg")
        print("q - Выход")

        choice = input("> ").lower()
        
        if choice == '1':
            try:
                offset = int(input("Смещение: "))
                new_byte = int(input("Новый байт (hex): "), 16)
                replace_byte_in_file("config_15", offset, new_byte)
            except ValueError:
                print("Некорректный ввод!")
        
        elif choice == '2':
            run_windbg_trace()
        
        elif choice == '3':
            run_windbg_coverage()
        
        elif choice == 'q':
            break



if __name__ == "__main__":
    main2()

