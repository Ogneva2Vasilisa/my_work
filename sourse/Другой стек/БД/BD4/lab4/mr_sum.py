from pymongo import MongoClient
import time

client = MongoClient("mongodb://localhost:27017/")
db = client["lab3_mr"]

map_func = """
function() {
    emit(this.source_ip, this.bytes_transferred);
}
"""

reduce_func = """
function(key, values) {
    return Array.sum(values);
}
"""

def run_mr_sum():
    print("MR SUM — суммарный трафик по IP")

    start = time.time()

    db.command({
        "mapReduce": "logs",
        "map": map_func,
        "reduce": reduce_func,
        "out": "mr_sum_traffic"
    })

    duration = time.time() - start
    print(f"Время: {duration:.2f} сек")

    # топ 5 IP
    print("\nТоп 5 IP:")
    for d in db.mr_sum_traffic.find().sort("value", -1).limit(5):
        print(d)

if __name__ == "__main__":
    run_mr_sum()
