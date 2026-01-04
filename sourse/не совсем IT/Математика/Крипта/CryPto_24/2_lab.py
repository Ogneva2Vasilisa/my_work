import random

def jacobi(a, n):
    g = 1
    while a != 0:
        if a == 1:
            return g
        a1 = a
        k = 0
        while a1 % 2 == 0:
            a1 = a1 // 2
            k += 1
        s = 0
        if k % 2 == 0:
            s = 1
        else:
            if n % 8 == 1 or n % 8 == 7:
                s = 1
            elif n % 8 == 3 or n % 8 == 5:
                s = -1
        if a1 == 1:
            return g * s
        if n % 4 == 3 and a1 % 4 == 3:
            s = -s
        a = n % a1
        n = a1
        g = g * s
    return 0

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


# ---------------------------------------

def fermat_test(n, k = 5):
    if n <= 1:
        return False
    for _ in range(k):
        a = random.randint(2, n-2)
        if pow(a, n-1, n) != 1:
            return False
    return True


def s_sh_test(n, k= 5):
  for _ in range(k):
    a = random.randint(2, n - 2)
    print(a)
    r = pow(a, (n - 1) // 2, n)
    if r != 1 and r != n - 1:
        print("NOD")
        return False
    jacobian =  (n+jacobi(a, n))%n
    if jacobian == 0 or r != jacobian:
        print("yacob")
        return False
  return True

def m_r_test(n,k=5):
  t=n-1
  s=0
  while t%2==0:
    s=s+1
    t=t//2
  for _ in range(k):
    flag=0
    a = random.randint(2, n - 2)
    print(a)
    x=pow(a,t,n)
    if x==1 or x==n-1:
      continue
    for i in range(s):
      x=pow(x,2,n)
      if x==1:
          print(1)
          return False
      if x==n-1:
        flag=1
        print(2)
        break
    if flag:
        return False
    continue
  return True

Car=[]

def Carmaikl():
    for i in range(0, 10):
        l=random.randint(100000000000000000000000000000, 1000000000000000000000000000000)
        n = (12 * l + 5) * (36 * l + 13) * (48 * l + 17)
        res = fermat_test(n)
        if res == 0:
            Car.append(l)

        n = (180 * l + 7) * (300 * l + 11) * (360 * l + 13) * (1200 * l + 41)
        res = fermat_test(n)
        if res == 0:
           Car.append(l)

# Пример использования

arr = [16070823815128249241, 2290355555399088701026891353837564906637, 213477488771126468401005871273198647209,
       9515192514416879753572488045399842777615799930917626533425236717814790844754041]

Carmaikl()

for number in arr:
  if fermat_test(number,k=6):
    print(f"{number} вероятно, простое число.")
  else:
    print(f"{number} составное число.")
  if s_sh_test(number,k=6):
    print(f"{number} вероятно, простое число.")
  else:
    print(f"{number} составное число.")
  if m_r_test(number,k=6):
    print(f"{number} вероятно, простое число.")
  else:
    print(f"{number} составное число.")
  print()

for number in Car[::2]:
  print("--------------------number-----------------------")
  if fermat_test(number,k=6):
    print(f"{number} вероятно, простое число.")
  else:
    print(f"{number} составное число.")
  if s_sh_test(number,k=6):
    print(f"{number} вероятно, простое число.")
  else:
    print(f"{number} составное число.")
  if m_r_test(number,k=6):
    print(f"{number} вероятно, простое число.")
  else:
    print(f"{number} составное число.")