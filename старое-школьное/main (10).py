#ввод матрицы через n и матрицу
n=int(input())
a=[]
t='NO'
for i in range(n):
    a.append([int(j) for j in input().split()])
    
#----------основной код-------------
for i in range (n):
    for j in range (n):
        if (i==j) and (a[i][j]!=0) and (t!='YES'):
            t='YES'
            break
print(t)
