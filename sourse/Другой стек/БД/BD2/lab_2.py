import time
import csv
from pymongo import MongoClient

# Подключение к MongoDB
MONGO_URI = "mongodb://localhost:27017/"
DB_NAME = "cybersecurity_db"
COLLECTION = "logs"

client = MongoClient(MONGO_URI)
db = client[DB_NAME]
logs = db[COLLECTION]

# Количество прогонов каждого запроса
RUNS = 10

# Запросы MongoDB (aggregation pipelines)
queries = {
    "traffic_per_day": [
        {
            "$group": {
                "_id": {
                    "year": {"$year": "$timestamp"},
                    "month": {"$month": "$timestamp"},
                    "day": {"$dayOfMonth": "$timestamp"}
                },
                "total_bytes": {"$sum": "$bytes_transferred"}
            }
        },
        {"$sort": {"_id.year": 1, "_id.month": 1, "_id.day": 1}}
    ],

    "events_per_day": [
        {
            "$group": {
                "_id": {
                    "year": {"$year": "$timestamp"},
                    "month": {"$month": "$timestamp"},
                    "day": {"$dayOfMonth": "$timestamp"}
                },
                "event_count": {"$sum": 1}
            }
        },
        {"$sort": {"_id.year": 1, "_id.month": 1, "_id.day": 1}}
    ],

    "top_source_ips": [
        {
            "$group": {
                "_id": "$source_ip",
                "total_bytes": {"$sum": "$bytes_transferred"}
            }
        },
        {"$sort": {"total_bytes": -1}},
        {"$limit": 5}
    ],

    "blocked_connections": [
        {"$match": {"action_taken": "blocked"}},
        {
            "$group": {
                "_id": "$log_type",
                "blocked_count": {"$sum": 1}
            }
        }
    ],

    "events_by_log_type_action": [
        {
            "$group": {
                "_id": {
                    "log_type": "$log_type",
                    "action": "$action"
                },
                "events_count": {"$sum": 1}
            }
        },
        {"$sort": {"_id.log_type": 1, "_id.action": 1}}
    ],

    "traffic_by_protocol_threat": [
        {
            "$group": {
                "_id": {
                    "protocol": "$protocol",
                    "threat_label": "$threat_label"
                },
                "total_bytes": {"$sum": "$bytes_transferred"}
            }
        }
    ]
}

# Функция тестирования одного запроса
def test_query(name, pipeline):
    times = []
    for _ in range(RUNS):
        start = time.time()
        list(logs.aggregate(pipeline))
        end = time.time()
        times.append(end - start)

    return {
        "avg": sum(times) / RUNS,
        "min": min(times),
        "max": max(times)
    }

# Основной цикл
results = {}

print("\n=== Начинаем тестирование MongoDB ===\n")

for query_name, pipeline in queries.items():
    print(f"Тест: {query_name}")
    stats = test_query(query_name, pipeline)
    results[query_name] = stats
    print(f"  avg={stats['avg']:.4f}s  min={stats['min']:.4f}s  max={stats['max']:.4f}s")

# Сохранение в CSV
csv_filename = "mongodb_performance.csv"
with open(csv_filename, "w", newline="", encoding="utf-8") as f:
    writer = csv.writer(f, delimiter=';')
    writer.writerow(["Query", "Avg_time", "Min_time", "Max_time"])

    for name, stats in results.items():
        writer.writerow([
            name,
            f"{stats['avg']:.6f}".replace('.', ','),
            f"{stats['min']:.6f}".replace('.', ','),
            f"{stats['max']:.6f}".replace('.', ',')
        ])

print(f"\nГотово! Результаты сохранены в {csv_filename}")
