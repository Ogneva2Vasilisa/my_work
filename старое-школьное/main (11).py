#ввод матрицы через n и матрицу
n=int(input())
a=[]
t=0
for i in range(0,n):
    a.append([int(j) for j in input().split()])
    
#----------основной код-------------
for i in range (0,n):

    for j in range (i,n):
        if (a[i][j]!=0):
            t=t+1
print(t)
