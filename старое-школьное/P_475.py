n,m=[int(x) for x in input().split()] 
a=[]
for i in range(n):
    w=[0]*n
    a.append(w)
for i in range(m):
    s,p=[int(x) for x in input().split()]
    a[s-1][p-1]=1
    a[p-1][s-1]=1
w=[]    
for i in range(n):
    w.append(sum(a[i]))
k=0
for i in range(n-1):
    if w[i]!=w[i+1]:
        k+=1
if k>0: print('NO')
else: print('YES')
