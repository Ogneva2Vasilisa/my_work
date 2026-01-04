import time

def binary_gcd(a, b):
    u, v, s, t, r = 1, 0, 0, 1, 0
    while (a % 2 == 0) and (b % 2 == 0):
        a, b, r = a // 2, b // 2, r + 1
    alpha, beta = a, b
    while (a % 2 == 0):
        a = a // 2
        if (u % 2 == 0) and (v % 2 == 0):
            u, v = u // 2, v // 2
        else:
            u, v = (u + beta) // 2, (v - alpha) // 2
    i = 0
    while a != b:
        if (b % 2 == 0):
            b = b // 2
            if (s % 2 == 0) and (t % 2 == 0):
                s, t = s // 2, t // 2
            else:
                s, t = (s + beta) // 2, (t - alpha) // 2
        elif b < a:
            a, b, u, v, s, t = b, a, s, t, u, v
        else:
            b, s, t = b - a, s - u, t - v
        i+=1
    return (2 ** r) * a, s, t


def gcd_ex(a, b):
    x = [1, 0]
    y = [0, 1]
    r = [a, b]
    i = 0
    while (r[-1] != 0):
        q = r[-2] // r[-1]
        x.append(x[-2] - q * x[-1])
        y.append(y[-2] - q * y[-1])
        r.append(r[-2] % r[-1])
        i += 1
    return r[-2], x[-2], y[-2]


def gcd_us(a, b):
    x = [1, 0]
    y = [0, 1]
    r = [a, b]
    i = 0

    while (r[-1] != 0):
        q = r[-2] // r[-1]
        if abs(r[-1]) / 2 <= r[-2] % r[-1]:
            if r[-1] > 0:
                q = q + 1
            else:
                q = q - 1
            r.append(r[-2] % r[-1] - abs(r[-1]))
        else:
            r.append(r[-2] % r[-1])
        x.append(x[-2] - q * x[-1])
        y.append(y[-2] - q * y[-1])
        i += 1
    return r[-2], x[-2], y[-2]


# ------------------------------------------------------main-------------------------------------------------------
a1 = [3, 5]
a2 = [48, 4096]
a3 = [40272697491475079892551833825168366592769844561060802116192162121030989699413161,
      27408111317231573879642453726577761671869473672147412389100161500456568162811601]
with open("gcd_ex.txt", 'a') as f:
    print("1---- ", gcd_ex(a1[0], a1[1])[0])
    d, x, y = gcd_ex(a1[0], a1[1])
    print("a1[0]*x+a1[1]*y =", a1[0] * x + a1[1] * y)
    print("d = ", d, ', x = ', x, ', y = ', y)

    print("2----- " + str(gcd_ex(max(a2[0], a2[1]), min(a2[0], a2[1]))) + "\n")
    d, x, y = gcd_ex(a2[0], a2[1])
    print("a2[0]*x+a2[1]*y =", a2[0] * x + a2[1] * y)
    print("d = ", d, ', x = ', x, ', y = ', y)

    print("3---- " + str(gcd_ex(max(a3[0], a3[1]), min(a3[0], a3[1]))[0]) + "\n")
    d, x, y = gcd_ex(a3[0], a3[1])
    print("a3[0]*x+a3[1]*y =", a3[0] * x + a3[1] * y)
    print("d = ", d, ', x = ', x, ', y = ', y)
with open("binary_gcd.txt", 'a') as f:
    print("1---- " + str(binary_gcd(a1[0], a1[1])[0]) + "\n")
    d, x, y = binary_gcd(a1[0], a1[1])
    print("a1[0]*x+a1[1]*y =", a1[0] * x + a1[1] * y)
    print("d = ", d, ', x = ', x, ', y = ', y)

    print("2----- " + str(binary_gcd(a2[0], a2[1])) + "\n")
    d, x, y = binary_gcd(a2[0], a2[1])
    print("a2[0]*x+a2[1]*y =", a2[0] * x + a2[1] * y)
    print("d = ", d, ', x = ', x, ', y = ', y)

    print("3---- " + str(binary_gcd(a3[0], a3[1])) + "\n")
    d, x, y = binary_gcd(a3[0], a3[1])
    print("a3[0]*x+a3[1]*y =", a3[0] * x + a3[1] * y)
    print("d = ", d, ', x = ', x, ', y = ', y)
with open("gcd_us.txt", 'a') as f:
    print("1---- " + str(gcd_us(max(a1[0], a1[1]), min(a1[0], a1[1]))[0]) + "\n")
    d, x, y = gcd_us(a1[0], a1[1])
    print("a1[0]*x+a1[1]*y =", a1[0] * x + a1[1] * y)
    print("d = ", d, ', x = ', x, ', y = ', y)

    print("2----- " + str(gcd_us(max(a2[0], a2[1]), min(a2[0], a2[1]))[0]) + "\n")
    d, x, y = gcd_us(a2[0], a2[1])
    print("a2[0]*x+a2[1]*y =", a2[0] * x + a2[1] * y)
    print("d = ", d, ', x = ', x, ', y = ', y)

    print("3---- " + str(gcd_us(max(a3[0], a3[1]), min(a3[0], a3[1]))[0]) + "\n")
    d, x, y = gcd_us(a3[0], a3[1])
    print("a3[0]*x+a3[1]*y =", a3[0] * x + a3[1] * y)
    print("d = ", d, ', x = ', x, ', y = ', y)

# -----------------------------------------------------замер скорости-------------------------------------------------

a = [(a1[0], a1[1]), (a2[0], a2[1]), (a3[0], a3[1])]
for num1, num2 in a:
    print(num1, num2)
    start = time.time()
    for i in range(10000):
        gcd_ex(num1, num2)
    result = time.time() - start
    print("gcd_ex time : {:>.3f}".format(result) + " seconds")
    start = time.time()
    for i in range(10000):
        binary_gcd(num1, num2)
    result = time.time() - start
    print("binary_gcd time: {:>.3f}".format(result) + " seconds")
    start = time.time()
    for i in range(10000):
        gcd_us(num1, num2)
    result = time.time() - start
    print("gcd_us time: {:>.3f}".format(result) + " seconds")
