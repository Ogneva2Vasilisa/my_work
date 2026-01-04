import math
import time
from sympy import discrete_log

TIME = 60 * 60 * 8

def powmode(x, y, n):
	r = 1

	while (y):
		if (y & 0x01):
			r = (r * x) % n

		x = (x * x) % n
		y = y >> 1

	return r

def Gelfond(a, b, p, q):
	start = time.time()

	s = (int)(q**(0.5)) + 1

	s1 = p - 1
	s1 -= s
	a1 = powmode(a, s1, p)

	f_x=open("x.txt",'w')
	f_y=open("y.txt",'w')

	i = 0
	while i != s:
		if time.time() - start > TIME:
			print(time.time())
			print( i)
			break
		f_x.write(str((b * powmode(a1, i, p)) % p)+'\n')
		f_y.write(str(powmode(a, i, p) % p)+'\n')
		i += 1

	f_x.close()
	f_y.close()
	f_x = open("x.txt", 'r')
	f_y = open("y.txt", 'r')
	print("Базы построены!")

	i, j = 0, 0
	x=f_x.readline()
	y=f_y.readline()
	while x != y and i != s:
		if time.time() - start > TIME:
			print(time.time())
			f_x.close()
			f_y.close()
			return 0, x, y, i, j

		y = f_y.readline()
		j += 1
		if j == s:
			j = 0
			f_y.close()
			f_y = open("y.txt", 'r')
			y = f_y.readline()
			i += 1
			x = f_x.readline()

	z = (i * s + j) % p
	print("ответ - ", z)
	f_x.close()
	f_y.close()


a = 17
b = 19
p = 50091896122438801343
q = 25045948061219400671

#19

# a = 2
# b = 7
# p = 137
# q = 68

#45

# a = 11
# b = 14
# p = 137
# q = 68

#6

print(Gelfond(a,b,p,q))

# if x is not None:
#     print(f"Дискретный логарифм x: {x}")
#     print(f"Проверка: {pow(a, x, p)} == {b} (mod {p})")
# else:
#     print("Дискретный логарифм не найден в заданном диапазоне.")
#     print(f"Время работы: {end_time - start_time:.2f} секунд")
#     print(f"Выполнено итераций: {iterations}")
