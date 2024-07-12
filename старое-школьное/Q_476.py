import sys
n,m=[int(x) for x in input().split()] 
a=[]
for i in range(n):
    w=[0]*n
    a.append(w)
for i in range(m):
    s,p=[int(x) for x in input().split()]
    a[s-1][p-1]=1
    a[p-1][s-1]=1

for i in range (n):
    for j in range (n):
        if i!=j and a[i][j]==0:
            print('NO')
            sys.exit()

print('YES')            
