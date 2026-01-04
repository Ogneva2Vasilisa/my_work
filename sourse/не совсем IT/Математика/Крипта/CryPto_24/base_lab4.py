import random

def power_mod(a, x, p):
    """Возведение в степень по модулю."""
    res = 1
    a %= p
    while x > 0:
        if x % 2 == 1:
            res = (res * a) % p
        a = (a * a) % p
        x //= 2
    return res

def factorize(n, factor_base):
    """Простая факторизация в заданную базу.  Для больших чисел нужна более продвинутая факторизация."""
    factors = {}
    for p in factor_base:
        while n % p == 0:
            factors[p] = factors.get(p, 0) + 1
            n //= p
    if n > 1:
        return None  # Число не полностью разложилось в данной базе
    return factors


def solve_discrete_logarithm_index_calculus(a, b, p, factor_base):
    n = len(factor_base)
    matrix = [[0] * (n + 1) for _ in range(n)]
    vector_b = [0] * n

    # Этап 1: Собираем линейные уравнения
    i = 0
    while i < n:
        # Генерируем случайное число j
        j = random.randint(1, p - 2)
        x = power_mod(a, j, p)
        factors = factorize(x, factor_base)
        if factors:
            row = [0] * (n+1)
            for k, count in factors.items():
                index = factor_base.index(k)
                row[index] = count
            row[n] = j
            matrix[i] = row
            i += 1

    # Этап 2: Решаем систему линейных уравнений (упрощенное решение - Гауссово исключение)
    # (Для больших систем необходим более эффективный метод решения)
    for i in range(n):
        pivot = matrix[i][i]
        if pivot == 0:
            # Обработка случая, когда пивот равен нулю (нужно переставить строки)
            # В упрощенной версии пропускаем этот случай
            continue
        for j in range(i + 1, n):
            factor = matrix[j][i] // pivot
            for k in range(i, n + 1):
                matrix[j][k] -= factor * matrix[i][k]

    # Этап 3: Вычисляем дискретный логарифм
    # (Упрощенное вычисление - обратный проход)
    for i in range(n - 1, -1, -1):
        vector_b[i] = matrix[i][n]
        for j in range(i + 1, n):
            vector_b[i] -= matrix[i][j] * vector_b[j]
        vector_b[i] //= matrix[i][i]


    #Этап 4:  Вычисляем дискретный логарифм b
    factors_b = factorize(b, factor_base)
    if factors_b:
        log_b = 0
        for k, count in factors_b.items():
            index = factor_base.index(k)
            log_b += count * vector_b[index]
        return log_b % (p-1) #модуль p-1, так как порядок группы p-1
    else:
        return None


# Пример использования:
a = 2
b = 10
p = 101  # Простое число
factor_base = [2, 3, 5, 7] #Маленькая база - для демонстрации.  Для больших чисел нужна большая база и более сложная факторизация.

x = solve_discrete_logarithm_index_calculus(a, b, p, factor_base)

if x is not None:
    print(f"Дискретный логарифм: {x}")
else:
    print("Решение не найдено.")

