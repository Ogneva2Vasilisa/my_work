from pymongo import MongoClient
import random
import time
from datetime import datetime, timedelta
import sys

client = MongoClient("mongodb://localhost:27017/")
db = client["lab3_mr"]
logs = db["logs"]

def generate_logs(n):
    logs.drop()

    print(f"Создание {n} записей...")
    start = time.time()

    base = datetime(2024, 1, 1)

    batch = []
    for i in range(n):
        doc = {
            "timestamp": (base + timedelta(minutes=i)).isoformat(),
            "source_ip": f"192.168.1.{random.randint(1, 50)}",
            "bytes_transferred": random.randint(100, 10000)
        }
        batch.append(doc)

        if len(batch) == 5000:
            logs.insert_many(batch)
            batch.clear()

    if batch:
        logs.insert_many(batch)

    print(f"Готово за {time.time() - start:.2f} сек")

if __name__ == "__main__":
    # Аргумент: python generate_data.py 10000
    if len(sys.argv) > 1:
        size = int(sys.argv[1])
    else:
        size = 10000

    generate_logs(size)
