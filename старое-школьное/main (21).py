n, m = map(int,input().split())
a =[[0]*n for i in range (n)]
b = []
for i in range (0,m):
    b.append(list(map(int, input().split()) ))
#print(rebra)
r=0
while r!=m:
    a[b[r][0]-1][b[r][1]-1]=1
    r=r+1


#------код--------
for i in range(0, n):
    b1 = sum(a[i])
    b2 = 0
    for j in range(0, n):
        b2=b2+a[j][i]
    print(b2)
    print(b1)