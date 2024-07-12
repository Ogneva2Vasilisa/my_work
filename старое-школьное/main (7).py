s = input()
n = len(s)
F = [0]*(n+1)
F[0] = 1
F[1] = 1
for i in range(2, n+1):
    F[i] = F[i-1] + F[i-2]*int(int(s[i-2])!=0 and 0 < int(s[i-2:i]) <= 33)
print(F[-1])