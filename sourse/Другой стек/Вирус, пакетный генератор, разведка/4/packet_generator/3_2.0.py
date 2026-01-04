import sys
import json
from scapy.all import rdpcap
from scapy.layers.dns import DNSQR
from scapy.layers.inet import IP, UDP
from collections import defaultdict

def analyze_dns_traffic_by_volume(pcap_file, threshold=0.8):
    try:
        packets = rdpcap(pcap_file)
        print(f"Загружено пакетов: {len(packets)}")
        
        # Определяем диапазоны сетей
        lvs1_range = range(10, 36)  # 192.168.50.10 - 192.168.50.35
        lvs2_range = range(50, 71)  # 192.168.50.50 - 192.168.50.70
        
        # DNS сервер (исключаем из анализа)
        dns_server = "192.168.50.88"
        
        # Структуры для подсчета
        ip_domain_counts = defaultdict(lambda: defaultdict(int))  # ip -> domain -> count
        ip_total_counts = defaultdict(int)  # ip -> total count
        
        dns_count = 0
        filtered_count = 0
        lvs1_requests = 0
        lvs2_requests = 0
        
        for packet in packets:
            if (packet.haslayer(UDP) and packet[UDP].dport == 53 and 
                packet.haslayer(DNSQR) and packet.haslayer(IP)):
                
                dns_count += 1
                ip_src = packet[IP].src
                
                # Пропускаем пакеты от DNS сервера
                if ip_src == dns_server:
                    filtered_count += 1
                    continue
                
                # Определяем, из какой сети запрос
                ip_last_octet = int(ip_src.split('.')[-1])
                
                # ИГНОРИРУЕМ все запросы от ЛВС2
                if ip_last_octet in lvs2_range:
                    filtered_count += 1
                    lvs2_requests += 1
                    continue
                
                # Анализируем только запросы от ЛВС1
                if ip_last_octet in lvs1_range:
                    lvs1_requests += 1
                    domain = packet[DNSQR].qname.decode('utf-8').rstrip('.')
                    
                    # Увеличиваем счетчики
                    ip_domain_counts[ip_src][domain] += 1
                    ip_total_counts[ip_src] += 1
        
        print(f"Найдено DNS запросов: {dns_count}")
        print(f"Запросов от ЛВС1 (10-35): {lvs1_requests}")
        print(f"Запросов от ЛВС2 (50-70): {lvs2_requests} (игнорируются)")
        print(f"Запросов от DNS сервера: {filtered_count - lvs2_requests} (игнорируются)")
        print(f"Уникальных IP в ЛВС1: {len(ip_total_counts)}")
        
        # Находим подозрительные пары (домен ≥ threshold от общего трафика IP)
        suspicious_pairs = []
        
        for ip in ip_total_counts:
            total = ip_total_counts[ip]
            domain_counts = ip_domain_counts[ip]
            
            # Сортируем домены по количеству запросов (по убыванию)
            sorted_domains = sorted(domain_counts.items(), key=lambda x: x[1], reverse=True)
            
            # Берем самый частый домен
            if sorted_domains:
                top_domain, top_count = sorted_domains[0]
                percentage = top_count / total
                
                if percentage >= threshold:
                    suspicious_pairs.append({
                        'ip': ip,
                        'domain': top_domain,
                        'domain_requests': top_count,
                        'total_requests': total,
                        'percentage': percentage,
                        'other_domains': len(sorted_domains) - 1
                    })
        
        print(f"\nПОДОЗРИТЕЛЬНЫЕ ПАРЫ (домен ≥{threshold*100:.0f}% трафика IP)")
        print("="*80)
        
        if not suspicious_pairs:
            print("Подозрительной активности не найдено")
            return []
        
        # Сортируем по проценту трафика (по убыванию)
        suspicious_pairs.sort(key=lambda x: x['percentage'], reverse=True)
        
        print(f"Найдено подозрительных пар: {len(suspicious_pairs)}")
        print("Обнаружена возможная бот-активность в ЛВС1!")
        print("-" * 80)
        
        for i, pair in enumerate(suspicious_pairs, 1):
            print(f"{i:2d}. ПОДОЗРИТЕЛЬНЫЙ БОТ")
            print(f"    IP: {pair['ip']}")
            print(f"    Домен: {pair['domain']}")
            print(f"    Запросы к домену: {pair['domain_requests']}")
            print(f"    Всего запросов: {pair['total_requests']}")
            print(f"    Доля трафика: {pair['percentage']*100:.1f}%")
            print(f"    Других доменов: {pair['other_domains']}")
            print()
        
        # Выводим статистику
        unique_ips = set(pair['ip'] for pair in suspicious_pairs)
        print(f"IP адреса с подозрительной активностью: {', '.join(sorted(unique_ips))}")
        print(f"Всего затронуто IP: {len(unique_ips)}")
        
        # Сохраняем результаты в JSON
        output_file = "dns_volume_analysis.json"
        with open(output_file, 'w') as f:
            json.dump({
                'threshold': threshold,
                'suspicious_pairs': suspicious_pairs,
                'statistics': {
                    'total_dns_requests': dns_count,
                    'lvs1_requests': lvs1_requests,
                    'lvs2_requests': lvs2_requests,
                    'unique_lvs1_ips': len(ip_total_counts),
                    'suspicious_ips': len(unique_ips)
                }
            }, f, indent=2, default=float)
        
        print(f"\nРезультаты сохранены в {output_file}")
        
        return suspicious_pairs
        
    except Exception as e:
        print(f"Ошибка при анализе pcap файла: {e}")
        return []

def analyze_dns_traffic(pcap_file):
    try:
        packets = rdpcap(pcap_file)
        print(f"Загружено пакетов: {len(packets)}")
        
        # Определяем диапазоны сетей
        lvs1_range = range(10, 36)  # 192.168.50.10 - 192.168.50.35
        lvs2_range = range(50, 71)  # 192.168.50.50 - 192.168.50.70
        
        # DNS сервер (исключаем из анализа)
        dns_server = "192.168.50.88"
        
        # Структура для хранения данных: { "ip.src домен": {"intervals": [], "first_ts": 0, "last_ts": 0} }
        intervals_data = defaultdict(lambda: {"intervals": [], "first_ts": None, "last_ts": None, "count": 0})
        
        # Временное хранилище последних времен для каждой пары ip-домен
        last_timestamps = {}
        
        dns_count = 0
        filtered_count = 0
        lvs1_requests = 0
        lvs2_requests = 0
        
        for packet in packets:
            if (packet.haslayer(UDP) and packet[UDP].dport == 53 and 
                packet.haslayer(DNSQR) and packet.haslayer(IP)):
                
                dns_count += 1
                timestamp = packet.time
                ip_src = packet[IP].src
                
                # Пропускаем пакеты от DNS сервера
                if ip_src == dns_server:
                    filtered_count += 1
                    continue
                
                # Определяем, из какой сети запрос
                ip_last_octet = int(ip_src.split('.')[-1])
                
                # ИГНОРИРУЕМ все запросы от ЛВС2
                if ip_last_octet in lvs2_range:
                    filtered_count += 1
                    lvs2_requests += 1
                    continue
                
                # Анализируем только запросы от ЛВС1
                if ip_last_octet in lvs1_range:
                    lvs1_requests += 1
                    domain = packet[DNSQR].qname.decode('utf-8').rstrip('.')
                    
                    # Ключ для идентификации пары ip-домен
                    key = f"{ip_src} {domain}"
                    
                    data = intervals_data[key]
                    data["count"] += 1
                    
                    if data["first_ts"] is None:
                        data["first_ts"] = timestamp
                    
                    data["last_ts"] = timestamp
                    
                    # Если уже был запрос к этому домену с этого IP, вычисляем интервал
                    if key in last_timestamps:
                        interval = timestamp - last_timestamps[key]
                        data["intervals"].append(interval)
                    
                    # Обновляем время последнего запроса
                    last_timestamps[key] = timestamp
        
        print(f"Найдено DNS запросов: {dns_count}")
        print(f"Запросов от ЛВС1 (10-35): {lvs1_requests}")
        print(f"Запросов от ЛВС2 (50-70): {lvs2_requests} (игнорируются)")
        print(f"Запросов от DNS сервера: {filtered_count - lvs2_requests} (игнорируются)")
        print(f"Уникальных пар IP-домен от ЛВС1: {len(intervals_data)}")
        
        # Фильтруем: оставляем только те, где 4 или больше интервалов
        filtered_data = {key: data for key, data in intervals_data.items() 
                         if len(data["intervals"]) >= 4}
        
        print(f"Подозрительных пар с 4+ интервалами: {len(filtered_data)}")
        
        # Сохраняем в JSON файл
        output_file = "dns_intervals.json"
        with open(output_file, 'w') as f:
            json.dump(filtered_data, f, indent=2, default=float)
        
        
        # Ищем одинаковые промежутки
        find_regular_intervals(filtered_data)
        
        return filtered_data
        
    except Exception as e:
        print(f"Ошибка при анализе pcap файла: {e}")
        return {}

def are_intervals_equal(intervals, first_ts, last_ts, tolerance=0.1):

    if not intervals or len(intervals) < 4:
        return False, 0
    
    # Взять первый и последний таймстамп и поделить на количество интервалов
    # Количество интервалов = количество пакетов - 1
    num_intervals = len(intervals)
    total_time = last_ts - first_ts
    
    if num_intervals > 0 and total_time > 0:
        base_interval = total_time / num_intervals
    else:
        return False, 0
    
    equal_count = 0
    
    for interval in intervals:
        # Проверяем, отличается ли интервал от базового более чем на tolerance (10%)
        if abs(interval - base_interval) <= base_interval * tolerance:
            equal_count += 1
    
    # Считаем процент одинаковых интервалов
    percentage = equal_count / len(intervals)
    return percentage >= 0.7, base_interval  # 80% интервалов должны быть одинаковыми

def find_regular_intervals(intervals_data):
    """
    Находит пары IP-домен с одинаковыми промежутками времени
    """
    print("\n" + "="*80)
    print("ПОДОЗРИТЕЛЬНЫЕ ПАРЫ (БОТ-АКТИВНОСТЬ В ЛВС1)")
    print("="*80)
    
    regular_pairs = []
    
    for key, data in intervals_data.items():
        intervals = data["intervals"]
        first_ts = data["first_ts"]
        last_ts = data["last_ts"]
        
        is_regular, base_interval = are_intervals_equal(intervals, first_ts, last_ts)
        
        if is_regular:
            ip, domain = key.split(' ', 1)
            regular_pairs.append({
                'ip': ip,
                'domain': domain,
                'interval_count': len(intervals),
                'packet_count': data["count"],
                'base_interval': base_interval,
                'intervals': intervals
            })
    
    if not regular_pairs:
        print("Подозрительной активности не найдено")
        return
    
    # Сортируем по количеству интервалов
    regular_pairs.sort(key=lambda x: x['interval_count'], reverse=True)
    
    print(f"Найдено подозрительных пар: {len(regular_pairs)}")
    print("Обнаружена возможная бот-активность в ЛВС1!")
    print("-" * 80)
    
    for i, pair in enumerate(regular_pairs, 1):
        print(f"{i:2d}. ПОДОЗРИТЕЛЬНЫЙ БОТ")
        print(f"    IP: {pair['ip']}")
        print(f"    Домен: {pair['domain']}")
        print(f"    Пакетов: {pair['packet_count']}")
        print(f"    Интервалов: {pair['interval_count']}")
        print(f"    Средний период: {pair['base_interval']:.2f} сек")
        print(f"    Первые 10 интервалов: {[f'{x:.2f}' for x in pair['intervals'][:10]]}")
        print()
   
    unique_ips = set(pair['ip'] for pair in regular_pairs)
    print(f"IP адреса зараженных компьютеров: {', '.join(sorted(unique_ips))}")
    
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Использование: python bot_detector.py <pcap_file>")
        sys.exit(1)
    
    pcap_file = sys.argv[1]
    
    # Анализ трафика
    intervals_data = analyze_dns_traffic(pcap_file)
    intervals_data = analyze_dns_traffic_by_volume(pcap_file, threshold=0.7)