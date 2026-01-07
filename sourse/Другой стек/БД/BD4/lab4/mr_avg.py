from pymongo import MongoClient
import time

client = MongoClient("mongodb://localhost:27017/")
db = client["lab3_mr"]

map_func = """
function() {
    emit("global", {sum: this.bytes_transferred, count: 1});
}
"""

reduce_func = """
function(key, values) {
    var s = 0, c = 0;
    values.forEach(v => { s += v.sum; c += v.count; });
    return {sum: s, count: c};
}
"""

finalize_func = """
function(key, reduced) {
    return reduced.sum / reduced.count;
}
"""

def run_mr_avg():
    print("MR AVG — средний трафик")

    start = time.time()

    db.command({
        "mapReduce": "logs",
        "map": map_func,
        "reduce": reduce_func,
        "finalize": finalize_func,
        "out": "mr_avg_traffic"
    })

    duration = time.time() - start
    print(f"Время: {duration:.2f} сек")

    print("\nСредний трафик:")
    print(db.mr_avg_traffic.find_one())

if __name__ == "__main__":
    run_mr_avg()
