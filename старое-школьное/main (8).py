#ввод матрицы через n и матрицу
n=int(input())
a=[]
t='YES'
for i in range(n):
    a.append([int(j) for j in input().split()])
    
#----------основной код-------------
for i in range (n):
    for j in range (n):
        if (t!='NO') and (a[i][j]!=a[j][i]):
            t='NO'
            break
print(t)
