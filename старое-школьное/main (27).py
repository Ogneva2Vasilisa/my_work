#ввод матрицы через n и матрицу
n = int(input()) 
a = [[int(j) for j in input().split()] for i in range(n)]
adj_list=[]

#------- перевод матрицы смежности в список смежности
for i in range(n):
    adj_list.append([])
    for j in range(n):
        if a[i][j]!=0:
            adj_list[i].append(j)
#print(adj_list)

#----------основной код-------------
visited = [False] * n # массив "посещена ли вершина?"
def dfs(v):
    global k
    visited[v] = True
    for w in adj_list[v]:
        if visited[w] == False:
            k=k+1
            dfs(w)
k=1
dfs(0)
if False in visited:
    print('NO')
else:
    print('YES')
