from random import randint
from time import time
from math import gcd
import time

TIME = 60 * 60 * 8


def euler_function(n):
    return sum(gcd(i, n) == 1 for i in range(1, n))


def pollard_rho(a, b, p, q, f):
    while True:
        u = (randint(2, p - 1) % p)
        v = u
        logc, logd = u, v
        logc_x, logd_x = v, u
        c = pow(a, u, p) * pow(b, v, p) % p
        d = c
        count = 0
        half_p = p // 2
        start = time.time()
        while (1):
            print("\nstep = ", count)
            print("c =", c)
            print("d =", d)
            print("log(c) = ", logc, " + ", logc_x, "x")
            print("log(d) = ", logd, " + ", logd_x, "x" "\n")
            f.write(
                "\nstep = " + str(count) + "\nc =" + str(c) + "\nd =" + str(d) + "\nlog(c) = " + str(logc) + " + " + str(
                    logc_x) + "x" + "\nlog(d) = " +
                str(logd) + " + " + str(logd_x) + "x\n")

            # f(c) function
            if c < 0:
                if a * c % p < abs(a * c % p - p):
                    c = a * c % p
                else:
                    c = a * c % p - p
                logc += 1
            else:
                if b * c % p < abs(b * c % p - p):
                    c = b * c % p
                else:
                    c = b * c % p - p
                logc_x += 1

            # f(f(d)) function
            if d < 0:
                if a * d % p < abs(a * d % p - p):
                    d = a * d % p
                else:
                    d = a * d % p - p
                logd += 1
            else:
                if b * d % p < abs(b * d % p - p):
                    d = b * d % p
                else:
                    d = b * d % p - p
                logd_x += 1

            if d < 0:
                if a * d % p < abs(a * d % p - p):
                    d = a * d % p
                else:
                    d = a * d % p - p
                logd += 1
            else:
                if b * d % p < abs(b * d % p - p):
                    d = b * d % p
                else:
                    d = b * d % p - p
                logd_x += 1

            count += 1
            if c == d:
                print("\nstep = ", count - 1)
                print("c =", c)
                print("d =", d)
                print("log(c) = ", logc, " + ", logc_x, "x")
                print("log(d) = ", logd, " + ", logd_x, "x" "\n")
                break

            if time.time() - start > TIME:
                print(time.time())
                break

        a1, b1 = (logc_x - logd_x) % q, (logd - logc) % q
        print(logc, " + ", logc_x, "x", " = ", logd, " + ", logd_x, "x", " (mod ", q, ")")
        x = (pow(a1, euler_function(q) - 1, q) * b1) % q
        print("x ≡ ", x, " (mod ", q, ")")
        print("step count =", count)
        f.write(str(logc) + " + " + str(logc_x) + "x" + " = " + str(logd) + " + " + str(logd_x) + "x" + " (mod " + str(
            q) + ")" + "\nx = " + str(x) + " (mod " + str(q) + ")" +
                "\nstep count =" + str(count))
        print(pow(a, x, p), b)
        if pow(a, x, p) != b:
            continue
        else:
            break
    return


a = 2
b = 7
p = 137
q = 68

a = 11
b = 14
p = 137
q = 68

# a = 17
# b = 19
# p = 50091896122438801343
# q = 25045948061219400671
f = open("log.txt", "w")
# start_time = time()
pollard_rho(a, b, p, q, f)
f.close()

# a^x = b (mod p)
