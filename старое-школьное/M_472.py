n=int(input())
a=[]
for i in range(n):
    a.append(list(map(int,input().split())))

for i in range(n):
    isx=sum(a[i])
    zax=0
    for j in range(n):
        zax+=a[j][i]
    print(zax)
    print(isx)



