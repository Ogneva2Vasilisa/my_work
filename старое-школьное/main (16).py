#ввод матрицы через n и матрицу
n=int(input())
a=[]
 
for i in range(0,n):
    a.append([int(j) for j in input().split()])
    
#----------основной код-------------
for i in range (0,n):
    for j in range (0,n):
        if (a[i][j]!=0):
            print(i+1, j+1)