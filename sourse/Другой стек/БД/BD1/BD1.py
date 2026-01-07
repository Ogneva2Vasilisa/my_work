import psycopg2
import psycopg2.extensions
import time
import csv
import sys

# Параметры подключения к БД
databases = {
    "star": {
        "host": "localhost",
        "port": 5432,
        "dbname": "lab_1",
        "user": "postgres",
        "password": "postgres"
    },
    "snowflake": {
        "host": "localhost",
        "port": 5432,
        "dbname": "lab_1_2",
        "user": "postgres",
        "password": "postgres"
    }
}

# Список запросов для теста
queries = {
    "traffic_per_day": {
        "star": """SELECT t.year, t.month, t.day, SUM(f.bytes_transferred) AS total_bytes 
                    FROM fact_network_events f 
                    JOIN dim_time t ON f.timestamp_id = t.timestamp_id 
                    GROUP BY t.year, t.month, t.day ORDER BY t.year, t.month, t.day;
                    """,
        "snowflake": """SELECT 
                            d.day, d.month, d.year, 
                            SUM(f.bytes_transferred) AS total_bytes 
                        FROM fact_network_events f 
                        JOIN dim_time t ON f.timestamp_id = t.timestamp_id 
                        JOIN dim_date d ON t.date_id = d.date_id 
                        GROUP BY d.year, d.month, d.day 
                        ORDER BY d.year, d.month, d.day;
                        """
    },
    "events_per_day": {
        "star": """SELECT t.year, t.month, t.day, COUNT(*) AS event_count
                    FROM fact_network_events f
                    JOIN dim_time t ON f.timestamp_id = t.timestamp_id
                    GROUP BY t.year, t.month, t.day
                    ORDER BY t.year, t.month, t.day;
                    """,
        "snowflake": """SELECT d.date AS day, COUNT(*) AS event_count
                        FROM fact_network_events f
                        JOIN dim_time t ON f.timestamp_id = t.timestamp_id
                        JOIN dim_date d ON t.date_id = d.date_id
                        GROUP BY d.date
                        ORDER BY d.date;
                        """
    },
    "top_source_ips": {
        "star": """SELECT s.source_ip, SUM(f.bytes_transferred) AS total_bytes 
                    FROM fact_network_events f 
                    JOIN dim_source_ip s ON f.source_ip_id = s.source_ip_id 
                    GROUP BY s.source_ip ORDER BY total_bytes DESC LIMIT 5;
                    """,
        "snowflake": """SELECT s.source_ip, SUM(f.bytes_transferred) AS total_bytes 
                        FROM fact_network_events f 
                        JOIN dim_source_ip s ON f.source_ip_id = s.source_ip_id 
                        GROUP BY s.source_ip ORDER BY total_bytes DESC LIMIT 5;
                        """
    },
    "blocked_connections": {
        "star": """SELECT l.log_type, COUNT(*) AS blocked_count 
                    FROM fact_network_events f 
                    JOIN dim_log_type l ON f.log_type_id = l.log_type_id 
                    JOIN dim_action a ON f.action_id = a.action_id 
                    WHERE a.action = 'blocked' 
                    GROUP BY l.log_type;
                    """,
        "snowflake": """SELECT l.log_type, COUNT(*) AS blocked_count 
                        FROM fact_network_events f 
                        JOIN dim_log_type l ON f.log_type_id = l.log_type_id 
                        JOIN dim_action a ON f.action_id = a.action_id 
                        WHERE a.action = 'blocked' 
                        GROUP BY l.log_type;
                        """
    },
    "events_by_log_type_action": {
        "star": """SELECT 
                        l.log_type,
                        a.action,
                        COUNT(*) AS events_count 
                    FROM fact_network_events f 
                    JOIN dim_log_type l ON f.log_type_id = l.log_type_id 
                    JOIN dim_action a ON f.action_id = a.action_id 
                    GROUP BY l.log_type, a.action 
                    ORDER BY l.log_type, a.action;
                    """,
        "snowflake": """SELECT 
                            l.log_type,
                            a.action,
                            COUNT(*) AS events_count 
                        FROM fact_network_events f 
                        JOIN dim_log_type l ON f.log_type_id = l.log_type_id 
                        JOIN dim_action a ON f.action_id = a.action_id 
                        GROUP BY l.log_type, a.action 
                        ORDER BY l.log_type, a.action;
                        """
    },
    "traffic_by_protocol_threat": {
        "star": """SELECT p.protocol, t.threat_label, SUM(f.bytes_transferred) AS total_bytes
                    FROM fact_network_events f
                    JOIN dim_protocol p ON f.protocol_id = p.protocol_id
                    JOIN dim_threat_label t ON f.threat_label_id = t.threat_label_id
                    GROUP BY p.protocol, t.threat_label;
                    """,
        "snowflake": """SELECT p.protocol, t.threat_label, SUM(f.bytes_transferred) AS total_bytes
                        FROM fact_network_events f
                        JOIN dim_protocol p ON f.protocol_id = p.protocol_id
                        JOIN dim_threat_label t ON f.threat_label_id = t.threat_label_id
                        GROUP BY p.protocol, t.threat_label;
                        """
    }
}

runs = 10  # Кол-во прогонов
results = []

# Функция для проверки подключения к базе данных
def test_connection(db_name, conn_params):
    """Проверяет подключение к базе данных и выводит детальную информацию об ошибке"""
    try:
        print(f"Проверка подключения к базе {db_name}...")
        print(f"Параметры: host={conn_params['host']}, port={conn_params['port']}, dbname={conn_params['dbname']}, user={conn_params['user']}")
        
        # Пробуем подключиться
        conn = psycopg2.connect(**conn_params)
        conn.close()
        print(f"[OK] Подключение к базе {db_name} успешно!")
        return True
        
    except psycopg2.OperationalError as e:
        print(f"[ERROR] Ошибка подключения к базе {db_name}:")
        print(f"   Код ошибки: {e.pgcode}")
        print(f"   Сообщение: {e}")
        
        return False
        
    except Exception as e:
        print(f"[ERROR] Неожиданная ошибка при подключении к базе {db_name}: {e}")
        return False

# Функция для выполнения одного запроса на всех базах
def test_query_on_all_databases(query_name, query_versions):
    query_results = {}
    
    for db_name, conn_params in available_databases.items():
        # Проверяем, есть ли версия запроса для данной схемы
        if db_name not in query_versions:
            print(f"Предупреждение: Запрос {query_name} не найден для схемы {db_name}")
            continue
        
        try:
            with psycopg2.connect(**conn_params) as conn:
                with conn.cursor() as cur:
                    sql = query_versions[db_name]
                    times = []
                    for i in range(runs):
                        start = time.time()
                        cur.execute(sql)
                        cur.fetchall()
                        end = time.time()
                        times.append(end - start)
                    avg_time = sum(times) / runs
                    min_time = min(times)
                    max_time = max(times)
                    query_results[db_name] = {
                        'avg_time': avg_time,
                        'min_time': min_time,
                        'max_time': max_time
                    }
                    print(f"Готово: {query_name} ({db_name})")
        except psycopg2.Error as e:
            print(f"Ошибка подключения к базе {db_name} для запроса {query_name}: {e}")
        except Exception as e:
            print(f"Неожиданная ошибка при тестировании {query_name} на {db_name}: {e}")
    
    # Сохраняем результаты для этого запроса
    results[query_name] = query_results

# Проверяем подключения ко всем базам перед началом тестирования
print("=== ПРОВЕРКА ПОДКЛЮЧЕНИЙ К БАЗАМ ДАННЫХ ===\n")
available_databases = {}

for db_name, conn_params in databases.items():
    if test_connection(db_name, conn_params):
        available_databases[db_name] = conn_params
    print()  # Пустая строка для разделения

if not available_databases:
    print("[ERROR] Нет доступных баз данных для тестирования!")
    sys.exit(1)

print(f"[OK] Доступны для тестирования: {list(available_databases.keys())}\n")

# Запускаем тесты по каждому запросу
for query_name, query_versions in queries.items():
    print(f"=== Тестирование запроса: {query_name} ===")
    test_query_on_all_databases(query_name, query_versions)

# Сохраняем результаты в CSV с европейским форматом
with open("query_performance.csv", "w", newline="", encoding="utf-8") as f:
    writer = csv.writer(f, delimiter=';')
    writer.writerow([
        "Query", 
        "Avg_time_star", "Min_time_star", "Max_time_star",
        "Avg_time_snowflake", "Min_time_snowflake", "Max_time_snowflake"
    ])
    
    # Форматируем числа с запятой как разделителем дробной части
    for query_name, query_results in results.items():
        # Получаем результаты для star и snowflake
        star_data = query_results.get('star', {})
        snowflake_data = query_results.get('snowflake', {})
        
        formatted_row = [
            query_name,  # Query name
            f"{star_data.get('avg_time', 0):.6f}".replace('.', ','),      # Avg_time_star
            f"{star_data.get('min_time', 0):.6f}".replace('.', ','),      # Min_time_star
            f"{star_data.get('max_time', 0):.6f}".replace('.', ','),      # Max_time_star
            f"{snowflake_data.get('avg_time', 0):.6f}".replace('.', ','), # Avg_time_snowflake
            f"{snowflake_data.get('min_time', 0):.6f}".replace('.', ','), # Min_time_snowflake
            f"{snowflake_data.get('max_time', 0):.6f}".replace('.', ',')  # Max_time_snowflake
        ]
        writer.writerow(formatted_row)

print("Тестирование завершено. Результаты сохранены в query_performance.csv")
