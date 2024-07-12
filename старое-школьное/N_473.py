n,m=[int(x) for x in input().split()] 
a=[]
for i in range(n):
    w=[0]*n
    a.append(w)
for i in range(m):
    s,p=[int(x) for x in input().split()]
    a[s-1][p-1]=1

for i in range(n):
    isx=sum(a[i])
    zax=0
    for j in range(n):
        zax+=a[j][i]
    print(zax)
    print(isx)
