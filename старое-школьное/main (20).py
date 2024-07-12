n=int(input())
a=[]
b1=0
b2=0
for i in range(n):
    a.append(list( map( int, input().split())))

for i in range(0, n):
    b1 = sum(a[i])
    b2 = 0
    for j in range(0, n):
        b2=b2+a[j][i]
    print(b2)
    print(b1)