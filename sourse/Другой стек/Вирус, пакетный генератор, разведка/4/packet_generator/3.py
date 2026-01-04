from scapy.all import *
import sys
from collections import defaultdict, Counter
import statistics
import json

def analyze_dns_behavior(pcap_file):
    """
    Анализирует DNS поведение каждого отправителя
    """
    try:
        packets = rdpcap(pcap_file)
        print(f"Загружено пакетов: {len(packets)}")
        print("=" * 80)
        
        # Структура для хранения данных
        ip_stats = defaultdict(lambda: {
            'domains': Counter(),
            'timestamps': defaultdict(list),
            'total_queries': 0,
            'unique_domains': set()
        })
        
        dns_server_ip = "192.168.50.88"
        output_data = {}
        
        # Собираем статистику
        for packet in packets:
            if packet.haslayer(IP) and packet.haslayer(UDP) and packet.haslayer(DNS):
                ip_layer = packet[IP]
                dns_layer = packet[DNS]
                
                if ip_layer.dst == dns_server_ip and dns_layer.qr == 0:
                    src_ip = ip_layer.src
                    timestamp = packet.time
                    
                    if dns_layer.qd:
                        query_name = ""
                        if hasattr(dns_layer.qd, 'qname'):
                            try:
                                query_name = dns_layer.qd.qname.decode('utf-8', errors='ignore')
                            except:
                                query_name = str(dns_layer.qd.qname)
                        else:
                            query_name = str(dns_layer.qd)
                        
                        query_name = query_name.rstrip('.').lower()
                        
                        ip_stats[src_ip]['domains'][query_name] += 1
                        ip_stats[src_ip]['timestamps'][query_name].append(timestamp)
                        ip_stats[src_ip]['total_queries'] += 1
                        ip_stats[src_ip]['unique_domains'].add(query_name)
        
        # Анализируем каждого отправителя и формируем выходные данные
        suspicious_ips = []
        normal_ips = []
        
        for ip, stats in ip_stats.items():
            domain_analysis = []
            
            for domain, count in stats['domains'].items():
                # Анализ временных интервалов
                interval_info = "все норм"
                if len(stats['timestamps'][domain]) > 1:
                    timestamps = sorted(stats['timestamps'][domain])
                    intervals = [timestamps[i] - timestamps[i-1] for i in range(1, len(timestamps))]
                    
                    if len(intervals) > 2:
                        stdev = statistics.stdev(intervals)
                        avg_interval = statistics.mean(intervals)
                        if stdev < avg_interval * 0.3:  # Регулярные запросы
                            interval_info = f"есть интервалы обращения ({avg_interval:.1f}±{stdev:.1f}сек)"
                
                domain_analysis.append(f'{domain}:{count} "{interval_info}"')
            
            # Сохраняем в выходные данные
            output_data[ip] = domain_analysis
            
            # Определяем подозрительность
            if is_suspicious_ip(ip, stats):
                suspicious_ips.append(ip)
            else:
                normal_ips.append(ip)
        
        # Записываем детальные данные в файл
        output_filename = "dns_analysis.txt"
        with open(output_filename, 'w', encoding='utf-8') as f:
            for ip, domains in output_data.items():
                f.write(f"{ip} {', '.join(domains)}\n")
        
        print(f"Детальный анализ записан в файл: {output_filename}")
        
        # Выводим сжатую статистику
        print_compact_statistics(ip_stats, suspicious_ips, normal_ips)
        
        return suspicious_ips
        
    except FileNotFoundError:
        print(f"Ошибка: Файл {pcap_file} не найден")
        return []
    except Exception as e:
        print(f"Ошибка при чтении файла: {e}")
        return []

def is_suspicious_ip(ip, stats):
    """
    Определяет, является ли IP подозрительным
    """
    # Проверяем подозрительные домены
    suspicious_domains = [dom for dom in stats['domains'] if is_suspicious_domain(dom)]
    if len(suspicious_domains) >= 2:
        return True
    
    # Проверяем регулярные запросы (боты)
    for domain, timestamps in stats['timestamps'].items():
        if len(timestamps) > 2:
            sorted_ts = sorted(timestamps)
            intervals = [sorted_ts[i] - sorted_ts[i-1] for i in range(1, len(sorted_ts))]
            if len(intervals) > 1:
                stdev = statistics.stdev(intervals)
                avg_interval = statistics.mean(intervals)
                if stdev < avg_interval * 0.3:  # Регулярность
                    return True
    
    # Слишком много запросов
    if stats['total_queries'] > 50:
        return True
        
    return False

def is_suspicious_domain(domain):
    """
    Определяет подозрительные домены
    """
    if not domain:
        return False
        
    domain_lower = domain.lower()
    
    # Легитимные домены
    legitimate = [
        'google.com', 'microsoft.com', 'adobe.com', 'mozilla.com',
        'dropbox.com', 'avast.com', 'windowsupdate.com', 'facebook.com',
        'skype.com', 'ccleaner.com', 'gstatic.com'
    ]
    
    for legit in legitimate:
        if legit in domain_lower:
            return False
    
    # Подозрительные паттерны
    suspicious_patterns = [
        'd0wn.biz', 'random.dns', 'avahost.net', 'byethost',
        'pissworld.net', '4qh.net', 'biznetman.info'
    ]
    
    for pattern in suspicious_patterns:
        if pattern in domain_lower:
            return True
    
    # DGA признаки
    if len(domain) > 25:
        return True
        
    if sum(c.isdigit() for c in domain) > 4:
        return True
    
    return False

def print_compact_statistics(ip_stats, suspicious_ips, normal_ips):
    """
    Выводит сжатую статистику
    """
    print("\n" + "=" * 80)
    print("СЖАТАЯ СТАТИСТИКА:")
    print("=" * 80)
    
    # Общая статистика
    total_ips = len(ip_stats)
    total_queries = sum(stats['total_queries'] for stats in ip_stats.values())
    avg_queries_per_ip = total_queries / total_ips if total_ips > 0 else 0
    
    print(f"📊 ОБЩАЯ СТАТИСТИКА:")
    print(f"   Всего IP адресов: {total_ips}")
    print(f"   Всего DNS запросов: {total_queries}")
    print(f"   Среднее запросов на IP: {avg_queries_per_ip:.1f}")
    
    # Статистика по подозрительным IP
    print(f"\n🚨 ПОДОЗРИТЕЛЬНЫЕ IP ({len(suspicious_ips)}):")
    for ip in suspicious_ips:
        stats = ip_stats[ip]
        suspicious_domains = [dom for dom in stats['domains'] if is_suspicious_domain(dom)]
        print(f"   🔴 {ip} - {stats['total_queries']} запросов, {len(suspicious_domains)} подозрительных доменов")
    
    # Статистика по нормальным IP
    print(f"\n✅ НОРМАЛЬНЫЕ IP ({len(normal_ips)}):")
    if normal_ips:
        # Показываем только топ-5 по активности
        normal_with_activity = [(ip, ip_stats[ip]['total_queries']) for ip in normal_ips]
        normal_with_activity.sort(key=lambda x: x[1], reverse=True)
        
        for ip, queries in normal_with_activity[:5]:
            print(f"   {ip} - {queries} запросов")
        if len(normal_ips) > 5:
            print(f"   ... и еще {len(normal_ips) - 5} IP")
    else:
        print("   Нет нормальных IP")
    
    # Топ доменов по популярности
    print(f"\n🔝 ТОП-5 САМЫХ ПОПУЛЯРНЫХ ДОМЕНОВ:")
    all_domains = Counter()
    for stats in ip_stats.values():
        all_domains.update(stats['domains'])
    
    for domain, count in all_domains.most_common(5):
        status = "🚨" if is_suspicious_domain(domain) else "✅"
        print(f"   {status} {domain}: {count} запросов")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Использование: python dns_analyzer.py <pcap_file>")
        sys.exit(1)
    
    pcap_file = sys.argv[1]
    analyze_dns_behavior(pcap_file)