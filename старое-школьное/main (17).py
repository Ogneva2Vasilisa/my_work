#ввод матрицы через n и матрицу
n=int(input())
a=[]
t=0
for i in range(n):
    a.append([int(j) for j in input().split()])
    
#----------основной код-------------
for i in range (n):
    t=0
    for j in range (n):
        if (a[i][j]!=0):
            t=t+1
    print(t)