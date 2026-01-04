from scapy.all import *
import sys

def analyze_packets(pcap_file):
    sources_in_pool = set()
    
    try:
        packets = rdpcap(pcap_file)
        print(f"Загружено пакетов: {len(packets)}")
        
        for packet in packets:
            if packet.haslayer(IP):
                src_ip = packet[IP].src
                
                # Проверяем диапазон 192.168.50.10 - 192.168.50.35
                if src_ip.startswith("192.168.50."):
                    ip_last_octet = int(src_ip.split('.')[-1])
                    # if 10 <= ip_last_octet <= 35:
                    #     sources_in_pool.add(src_ip)
                    if 50 <= ip_last_octet <= 70:
                        sources_in_pool.add(src_ip)
        
        print(f"Найдено уникальных источников в пуле 192.168.50.10-35: {len(sources_in_pool)}")
        print("Список источников:")
        for ip in sorted(sources_in_pool):
            print(f"  - {ip}")
            
    except Exception as e:
        print(f"Ошибка: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Использование: python 3.py <pcap_file>")
        sys.exit(1)
    
    analyze_packets(sys.argv[1])