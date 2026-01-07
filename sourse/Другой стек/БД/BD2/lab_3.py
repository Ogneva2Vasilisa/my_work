from pymongo import MongoClient
import time
import json
from datetime import datetime

# --- Подключение к БД ---
client = MongoClient("mongodb://localhost:27017/")
db = client["cybersecurity_db_lab3"]
logs = db["logs"]

# --- Глобальные переменные для замера времени ---
execution_times = {
    "mr1": 0,
    "mr2": 0, 
    "mr3": 0,
    "total_chain": 0
}

# --- Утилиты для замера времени ---
def log_execution_time(step_name, start_time, end_time):
    duration = end_time - start_time
    execution_times[step_name] = duration
    print(f"{step_name}: {duration:.2f} сек")
    return duration

# --- MR-1: СРЕДНЕСУТОЧНЫЙ трафик по дням ---
def mr1_traffic_per_day():
    start = time.time()
    
    # Сначала считаем количество записей и суммарный трафик по дням
    map_func1 = """
    function() {
        var dt = new Date(this.timestamp);
        var key = {
            year: dt.getFullYear(),
            month: dt.getMonth() + 1,
            day: dt.getDate()
        };
        emit(key, { count: 1, total_bytes: this.bytes_transferred });
    }
    """
    
    reduce_func1 = """
    function(key, values) {
        var result = { count: 0, total_bytes: 0 };
        values.forEach(function(value) {
            result.count += value.count;
            result.total_bytes += value.total_bytes;
        });
        return result;
    }
    """
    
    result = db.command({
        "mapReduce": "logs",
        "map": map_func1,
        "reduce": reduce_func1,
        "out": {"replace": "mr_daily_stats"}
    })
    
    # Затем вычисляем средний трафик за день
    map_func2 = """
    function() {
        var avg_bytes_per_day = this.value.total_bytes / this.value.count;
        emit(this._id, avg_bytes_per_day);
    }
    """
    
    reduce_func2 = """
    function(key, values) {
        return values[0]; // Для одной даты всегда одно значение
    }
    """
    
    result = db.command({
        "mapReduce": "mr_daily_stats",
        "map": map_func2,
        "reduce": reduce_func2,
        "out": {"replace": "mr_traffic_per_day"}
    })
    
    # Статистика по дням
    days_count = db.mr_traffic_per_day.count_documents({})
    print(f"Проанализировано дней: {days_count}")
    
    end = time.time()
    return log_execution_time("mr1", start, end)

# --- MR-2: выявление аномальных дней (Map-Reduce с scope) ---
def mr2_anomalous_days():
    start = time.time()
    
    # Сначала вычисляем средний трафик через агрегацию
    avg_result = db.mr_traffic_per_day.aggregate([
        {"$group": {"_id": None, "avgBytes": {"$avg": "$value"}}}
    ]).next()
    avg_traffic = avg_result["avgBytes"]
    
    print(f"Средний трафик за день: {avg_traffic:.2f} байт")
    
    map_func = """
    function() {
        if (this.value > avg_traffic) {
            emit(this._id, {
                date: this._id,
                traffic: this.value,
                anomaly_score: (this.value - avg_traffic) / avg_traffic
            });
        }
    }
    """
    
    reduce_func = """
    function(key, values) {
        return values[0]; // В данном случае reduce не нужен, но обязателен для MR
    }
    """
    
    result = db.command({
        "mapReduce": "mr_traffic_per_day",
        "map": map_func,
        "reduce": reduce_func,
        "out": {"replace": "mr_anomalous_days"},
        "scope": {"avg_traffic": avg_traffic}
    })
    
    # Подсчет результатов
    anomalous_count = db.mr_anomalous_days.count_documents({})
    print(f"Найдено аномальных дней: {anomalous_count}")
    
    end = time.time()
    return log_execution_time("mr2", start, end)

# --- MR-3: трафик по source_ip для аномальных дней ---
def mr3_ips_in_anomalous_days():
    start = time.time()
    
    # Получаем аномальные даты для фильтрации
    anomalous_docs = list(db.mr_anomalous_days.find())
    anomalous_dates = []
    
    for doc in anomalous_docs:
        date_obj = doc["_id"]
        anomalous_dates.append({
            "year": date_obj["year"],
            "month": date_obj["month"], 
            "day": date_obj["day"]
        })
    
    print(f"Аномальные дни для анализа: {len(anomalous_dates)}")
    
    map_func = """
    function() {
        var dt = new Date(this.timestamp);
        var currentDate = {
            year: dt.getFullYear(),
            month: dt.getMonth() + 1,
            day: dt.getDate()
        };
        
        // Проверяем, есть ли текущая дата в аномальных
        for (var i = 0; i < anomalous_dates.length; i++) {
            var abnormal = anomalous_dates[i];
            if (currentDate.year === abnormal.year &&
                currentDate.month === abnormal.month &&
                currentDate.day === abnormal.day) {
                emit(this.source_ip, this.bytes_transferred);
                break;
            }
        }
    }
    """
    
    reduce_func = """
    function(key, values) {
        return Array.sum(values);
    }
    """
    
    result = db.command({
        "mapReduce": "logs",
        "map": map_func,
        "reduce": reduce_func,
        "out": {"replace": "mr_ips_in_anomalous_days"},
        "scope": {"anomalous_dates": anomalous_dates}
    })
    
    end = time.time()
    return log_execution_time("mr3", start, end)

# --- Детальный анализ производительности ---
def print_performance_report():
    print("\n" + "="*60)
    print("ОТЧЕТ О ПРОИЗВОДИТЕЛЬНОСТИ MAP-REDUCE ЦЕПОЧКИ")
    print("="*60)
    
    total_mr_time = execution_times["mr1"] + execution_times["mr2"] + execution_times["mr3"]
    
    print(f"Время выполнения MR-1:          {execution_times['mr1']:8.2f} сек ({execution_times['mr1']/total_mr_time*100:5.1f}%)")
    print(f"Время выполнения MR-2:          {execution_times['mr2']:8.2f} сек ({execution_times['mr2']/total_mr_time*100:5.1f}%)")
    print(f"Время выполнения MR-3:          {execution_times['mr3']:8.2f} сек ({execution_times['mr3']/total_mr_time*100:5.1f}%)")
    print("-" * 60)
    print(f"Общее время Map-Reduce операций: {total_mr_time:8.2f} сек")
    print(f"Общее время цепочки:            {execution_times['total_chain']:8.2f} сек")
    print(f"Накладные расходы:              {execution_times['total_chain'] - total_mr_time:8.2f} сек")
    
    # Статистика по коллекциям
    print("\n" + "="*60)
    print("СТАТИСТИКА КОЛЛЕКЦИЙ")
    print("="*60)
    
    collections_stats = [
        ("logs", "Исходные логи"),
        ("mr_traffic_per_day", "Трафик по дням"), 
        ("mr_anomalous_days", "Аномальные дни"),
        ("mr_ips_in_anomalous_days", "IP в аномальные дни")
    ]
    
    for coll_name, description in collections_stats:
        if coll_name in db.list_collection_names():
            count = db[coll_name].count_documents({})
            print(f"{description:25} {count:8} документов")

# --- Главная функция выполнения цепочки ---
def execute_map_reduce_chain():
    print("ЗАПУСК MAP-REDUCE ЦЕПОЧКИ НА 6 МЛН ЗАПИСЕЙ")
    print("="*50)
    
    # Статистика исходной коллекции
    total_logs = logs.count_documents({})
    print(f"Исходная коллекция: {total_logs:,} записей")
    
    chain_start = time.time()
    
    # Выполнение цепочки Map-Reduce
    try:
        print("\nШаг 1: Анализ трафика по дням...")
        mr1_time = mr1_traffic_per_day()
        
        print("\nШаг 2: Выявление аномальных дней...")
        mr2_time = mr2_anomalous_days()
        
        print("\nШаг 3: Анализ IP-адресов в аномальные дни...")
        mr3_time = mr3_ips_in_anomalous_days()
        
    except Exception as e:
        print(f"Ошибка выполнения цепочки: {e}")
        return
    
    chain_end = time.time()
    execution_times["total_chain"] = chain_end - chain_start
    
    # Вывод результатов
    print("\n" + "="*50)
    print("РЕЗУЛЬТАТЫ АНАЛИЗА")
    print("="*50)
    
    # Топ IP в аномальные дни
    print("\nТоп-10 IP по трафику в аномальные дни:")
    top_ips = db.mr_ips_in_anomalous_days.find().sort("value", -1).limit(10)
    for i, doc in enumerate(top_ips, 1):
        traffic_mb = doc['value'] / (1024 * 1024)
        print(f"{i:2}. IP: {doc['_id']:15} Трафик: {doc['value']:12} байт ({traffic_mb:6.1f} MB)")
    
    # Детальный отчет производительности
    print_performance_report()

# --- Запуск программы ---
if __name__ == "__main__":
    # Выполняем цепочку Map-Reduce
    execute_map_reduce_chain()