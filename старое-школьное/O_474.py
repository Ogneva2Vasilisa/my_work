n=int(input())
a=[]
for i in range(n):
    a.append(list(map(int,input().split())))

zax=[]
isx=[]
for i in range(n):
    isx.append(sum(a[i]))
    w=0
    for j in range(n):
        w+=a[j][i]
    zax.append(w)
        
ki=0
kz=0
lisx=[]
lzax=[]
for i in range (n):
    if isx[i]==0:
        ki+=1
        lisx.append(i)
        
    if zax[i]==0:
        kz+=1
        lzax.append(i)

print(kz)#кол-во истоков
for i in range (kz):
    print(lzax[i]+1)

print(ki)#кол-во стоков
for i in range (ki):
    print(lisx[i]+1)   

    



