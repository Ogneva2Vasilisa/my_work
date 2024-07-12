#ввод матрицы через n и матрицу
n, m = map(int,(input().split()))
a=[]
adj_list = []
for i in range(n):
    a.append([int(j) for j in input().split()])
  
visited = [False] * n  # массив "посещена ли вершина?"
#------- перевод матрицы смежности в список смежности
for i in range(n):
    adj_list.append([])
    for j in range(n):
        if a[i][j]!=0:
            adj_list[i].append(j)
#print(*adj_list)
#----------основной код-------------
visited = [False] * n
component = [-1] * n  # для каждой вершины храним номер её компоненты
num_components = 0
def dfs(v):
    global k
    component[v] = num_components
    visited[v] = True
    for w in adj_list[v]:
        if visited[w] == False:
            k=k+1
            dfs(w)
k=1
dfs(m-1)

print(k)