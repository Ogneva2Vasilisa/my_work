from scapy.all import *
import sys

def analyze_packets(pcap_file):
    bots = []
    """
    Анализирует pcap файл и выводит информацию об ICMP пакетах
    """
    try:
        # Читаем pcap файл
        packets = rdpcap(pcap_file)
        print(f"Загружено пакетов: {len(packets)}")
        print("-" * 80)
        
        
        for i, packet in enumerate(packets):
            # Проверяем наличие IP и ICMP слоев
            if packet.haslayer(IP) and packet.haslayer(ICMP):
                ip_layer = packet[IP]
                icmp_layer = packet[ICMP]
                
                if packet.haslayer(UDP) or packet.haslayer(TCP) or packet.haslayer(DNS):
                    print(f"Подозрительный пакет с адреса {ip_layer.src}")
                    if ip_layer.src not in bots:
                        bots.append(ip_layer.src)

                
        print(f"Боты: {bots}")
    except FileNotFoundError:
        print(f"Ошибка: Файл {pcap_file} не найден")
    except Exception as e:
        print(f"Ошибка при чтении файла: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Использование: python main.py <pcap_file>")
        sys.exit(1)
    
    pcap_file = sys.argv[1]
    
    analyze_packets(pcap_file)
