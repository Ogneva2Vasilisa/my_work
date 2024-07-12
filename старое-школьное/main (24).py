#ввод матрицы через n и матрицу
n=int(input())
a=[]
stolb=[]
stroc=[]
for i in range(n):
    a.append([int(j) for j in input().split()])
    
# ---осн код------
for i in range(0, n):
    if not ( 1 in a[i]):
        stroc.append(i+1)
    for j in range(0, n):
        if (a[i][j]==1) and  not (j in stolb):
            stolb.append(j+1)
            break
        
print(len(stolb))
for i in range(len(stolb)):
    print(stolb[i])
print(len(stroc))
for i in range(len(stroc)):
    print(stroc[i])