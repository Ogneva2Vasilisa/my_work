int main() {
    int x = 10;
    int y = 20;
    int result = x + y;

    if (x > y) {
        result = x - y;
    } else {
        result = y - x;
    }

    while (x < 15) {
        x = x + 1;
    }

    for (int i = 0; i < 5; i = i + 1) {
        result = result + i;
    }

    return result;
}

