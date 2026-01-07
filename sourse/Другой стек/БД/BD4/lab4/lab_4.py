import os
import time
import csv

sizes = [1000, 10000, 100000, 1000000, 5000000]

# Создаем CSV файлы с заголовками
with open('mr_sum_results.csv', 'w', newline='', encoding='utf-8') as f:
    writer = csv.writer(f)
    writer.writerow(['Размер данных', 'Время выполнения'])

with open('mr_avg_results.csv', 'w', newline='', encoding='utf-8') as f:
    writer = csv.writer(f)
    writer.writerow(['Размер данных', 'Время выполнения'])

for size in sizes:
    print("\n" + "="*60)
    print(f"ГЕНЕРАЦИЯ ДАННЫХ: {size}")
    print("="*60)
    os.system(f"python generate_data.py {size}")

    print("\n-- MR SUM --")
    start_time = time.time()
    os.system("python mr_sum.py")
    end_time = time.time()
    sum_time = end_time - start_time
    
    # Сохраняем результат MR SUM
    with open('mr_sum_results.csv', 'a', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow([size, sum_time])

    print("\n-- MR AVG --")
    start_time = time.time()
    os.system("python mr_avg.py")
    end_time = time.time()
    avg_time = end_time - start_time
    
    # Сохраняем результат MR AVG
    with open('mr_avg_results.csv', 'a', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow([size, avg_time])

print(f"\nРезультаты сохранены в mr_sum_results.csv и mr_avg_results.csv")