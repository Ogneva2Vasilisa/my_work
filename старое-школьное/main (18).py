n, m = map(int,input().split())
a =[[0]*n for i in range (n)]
b = []
bol = 'NO'
for i in range (0,m):
    b.append(list(map(int, input().split()) ))
#print(rebra)
r=0
while r!=m:
    if a[b[r][0]-1][b[r][1]-1]==1:
        bol='YES'
    else: a[b[r][0]-1][b[r][1]-1]=1
    r=r+1
#for i in range (n):
#matrix    print(*a[i])
print(bol)