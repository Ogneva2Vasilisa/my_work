#-----матрица из списка ребер-------
n, m = map(int,input().split())
a =[[0]*n for i in range (n)]
b = []
for i in range (0,m):
    b.append(list(map(int, input().split()) ))
#print(rebra)
r=0
while r!=m:
    a[b[r][0]-1][b[r][1]-1]=1
    a[b[r][1]-1][b[r][0]-1]=1
    r=r+1
    ## на выходе матрица a от 0,0 с размерностью м на м
#----------основной код-------------
for i in range (n):
    t=0
    for j in range (n):
        if (a[i][j]!=0):
            t=t+1
    print(t)