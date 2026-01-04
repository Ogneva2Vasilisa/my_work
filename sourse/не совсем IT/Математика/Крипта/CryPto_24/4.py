import math
import time
import random

B = [-1, 2]

TIME = 60 * 60 * 8

# from math import sqrt

def prim(n):
    lk = 0
    for i in range(3, n + 1, 2):
        if (i > 10) and (i % 10 == 5):
            continue
        for j in range(B[1], len(B)):
            if j > int((math.sqrt(i)) + 1):
                B.append(i)
                lk = lk + 1
                break
            if i % j == 0:
                break
        else:
            B.append(i)
            lk = lk + 1
    return lk


def Gauss(A, B, X, n):
    k = 0
    while k < n:
        max_elem = abs(A[k][k])
        index = k
        for i in range(k + 1, n):
            if abs(A[i][k]) > max_elem:
                max_elem = abs(A[i][k])
                index = i

        if max_elem == 0.0:
            return -1

        for j in range(n):
            temp = A[k][j]
            A[k][j] = A[index][j]
            A[index][j] = temp

        temp = B[k]
        B[k] = B[index]
        B[index] = temp

        for i in range(k, n):
            temp = A[i][k]
            if abs(temp) == 0:
                continue
            for j in range(n):
                A[i][j] /= temp
            B[i] /= temp
            if i == k:
                continue
            for j in range(n):
                A[i][j] -= A[k][j]
            B[i] -= B[k]
        k += 1

    for k in range(n - 1, -1, -1):
        X[k] = B[k]
        for i in range(k):
            B[i] -= A[i][k] * X[k]
    return 1


def smooth_num(num, vector, size):
    for k in range(size):
        vector[k] = 0
    i = 1
    while i != size:
        if num % B[i] == 0:
            num /= B[i]
            vector[i] += 1
            if num == 1:
                return 1
        else:
            i += 1
    return 0


def contain(elem, vector):
    for k in vector:
        if k == elem:
            return 1
    return 0


def decomp_base(a, b, p, q):
    start = time.time()
    # ph <= M = L(n)^(0.5), где L(n) = exp(sqrt(ln(n) * ln(ln(n))))
    e = math.e
    ln_p = math.log(p, e)
    limiter = math.sqrt(int(pow(e, math.sqrt(ln_p * math.log(ln_p, e)))))
    # генерируем простые числа, ограниченные limiter
    # h - количество простых чисел в базе

    h = prim(int(limiter)) + 1

    # print B
    # h = h - 1
    print("len(B) = ", h + 1)

    print("Base created, time: ", time.time() - StartTime)
    print(B)
    u_vector = []
    x_vector = []
    beta_vector = []
    alfa_matrix = []

    for k in range(h + 1):  # инициализация
        alfa_matrix.append([0] * (h + 1))
        u_vector.append(0)
        beta_vector.append(0)
        x_vector.append(0)
    # случайным выбором показателей ui найдем множество
    # B-гладких чисел
    i = 0
    while i < h + 1:
        if time.time() - start > TIME:
            print(time.time())
            return None
        ui = random.randint(2, p - 1)
        bi = pow(a, ui, p)  # a^(ui)mod(p)
        # проверяем, является ли bi B-гладким
        if smooth_num(bi, alfa_matrix[i], h + 1) == 0:  # если сгенерированное - не B-гладкое
            for j in range(h + 1):
                alfa_matrix[i][j] = 0  # в случае неудачи - обнуление вектора флаго
            # проверяем, является ли p - bi B-гладким
            if smooth_num(p - bi, alfa_matrix[i], h + 1) == 1:
                if contain(ui, u_vector):
                    for j in range(h + 1):
                        alfa_matrix[i][j] = 0
                    continue

                alfa_matrix[i][0] = 1  # если такого элемента нет - добавить его в вектор B-гладких
                u_vector[i] = ui
                i += 1
                # print "u_i - ", ui
                # print bi
        else:  # если сгенерированное - B - гладкое
            if contain(ui, u_vector):  # если уже добавлено
                for j in range(h + 1):
                    alfa_matrix[i][j] = 0
                continue
            u_vector[i] = ui  # иначе добавить
            print("u_i - ", ui)
            print("alfa_matrix - ", alfa_matrix[i])
            print("b_i - ", bi)
            i += 1
    print("Smooth numbers found, time: ", time.time() - StartTime)
    v = 1
    while True:
        if time.time() - start > TIME:
            print(time.time())
            return None
        bv = pow(b, v, p)
        if smooth_num(bv, beta_vector, h + 1) == 1:
            break
        else:
            v += 1
            for i in range(h + 1):
                beta_vector[i] = 0
    print("time:", time.time() - StartTime)
    print("v:", v)
    # решаем СЛАУ методом Гауссова исключения
    # def Gauss(A, B, X, n):
    status = Gauss(alfa_matrix, u_vector, x_vector, h + 1)
    if status < 0:
        print("Error Gauss")
        return -1
    xv = 0
    for i in range(h + 1):
        xv = (xv + beta_vector[i] * x_vector[i]) % q
    if int(xv) != xv:
        print("Incorrect solution")
        return -1
    print("xv = ", xv)
    while xv % v != 0:
        xv += q
    x = (xv / v) % q
    if pow(a, int(x), p) != b:
        print("Incorrect solution")
        return -1
    print("x = ", x)
    return 1


a = 17
b = 19
p = 50091896122438801343
q = 25045948061219400671

# a = 11
# b = 14
# p = 137
# q = 68

d = -1
StartTime = time.time()
while d < 0:
    d = decomp_base(a, b, p, q)
    print("New iteration")
WorkTime = time.time() - StartTime
print("Total Time: ", WorkTime)
