from scapy.all import *
import random

# Количество пакетов
NUM_PACKETS = 11000

# Список "виртуальных" IP для генерации
source_ips = [f"192.168.1.{i}" for i in range(1, 51)]
dest_ips = [f"10.0.0.{i}" for i in range(1, 51)]

packets = []

for _ in range(NUM_PACKETS):
    src = random.choice(source_ips)
    dst = random.choice(dest_ips)
    payload_size = random.randint(20, 150)  # случайный размер полезной нагрузки

    # Генерируем простой IP-пакет с ICMP
    packet = IP(src=src, dst=dst)/ICMP()/Raw(load="X"*payload_size)
    packets.append(packet)

# Сохраняем в pcap
wrpcap("synthetic_traffic.pcap", packets)
print(f"Создан файл synthetic_traffic.pcap с {NUM_PACKETS} пакетами")
