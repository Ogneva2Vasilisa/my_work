import random
import math
import time

TIME = 3600*2

def P_Pollard (N:int):
    p=[]
    start = time.time()
    x = random.randint(1, N-2)
    y = 1
    i = 0
    stage = 2
    while math.gcd(N, abs(x - y)) == 1:
        if len(p)>=6:
           p=p[1:]
        else:
            print(p)
        if i == stage:
            y = x
            stage = stage*2
        x = (x*x + 1) % N
        i = i + 1
        p.append([x, y, math.gcd(N, abs(x-y))])
        if time.time()-start >TIME:
            print(p)
            return math.gcd(N, abs(x - y)), p
    return math.gcd(N, abs(x-y)), p


def P_1_Pollard(n):
    p_1=[]
    start = time.time()
    # defining base
    a = 2
    # defining exponent
    i = 2
    while True:
        if len(p_1)>=6:
            p_1=p_1[1:]
        else:
            print(p_1)
        a = pow(a, i, n)
        d = math.gcd((a - 1), n)
        if d > 1:
            return d, p_1
        i += 1
        p_1.append([a, i])
        if time.time()-start>TIME:
            return d, p_1

arr=[10964196854883952884291224125248167170683191209976018709838010001159818615987299]
for number in arr:
  print(P_Pollard(number))
  print(P_1_Pollard(number))

# 19:42