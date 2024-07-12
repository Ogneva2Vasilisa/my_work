import numpy as np
import matplotlib.pyplot as plt
import networkx as nx

inf = 999999999999999999
sum = 0


def my_draw1(arr):
    G = nx.from_numpy_array(np.array(arr), create_using=nx.DiGraph)
    edge_nodes = set(G)
    pos = nx.circular_layout(G.subgraph(edge_nodes))
    nx.draw(G, pos, with_labels=1, node_color='g', node_size=600)
    edge_labels = nx.get_edge_attributes(G, "weight")
    nx.draw_networkx_edge_labels(G, pos, edge_labels)
    plt.show()
def my_draw(arr, istok):
    G = nx.from_numpy_array(np.array(arr), create_using=nx.DiGraph)
    edge_nodes = set(G)
    pos = nx.bfs_layout(G.subgraph(edge_nodes), istok)
    nx.draw(G, pos, with_labels=1, node_color='g', node_size=600)
    edge_labels = nx.get_edge_attributes(G, "weight")
    nx.draw_networkx_edge_labels(G, pos, edge_labels)
    plt.show()


#with open("job_Var2.in",'r') as f:
with open("Dinic.in", 'r') as f:
    arr = list(t for t in f.read().split('\n'))
    arr = list(t.split(' ') for t in (arr[o] for o in range(len(arr))))
    # print(arr)
    for u in range(len(arr)):
        for i in range(len(arr[u])):
            if arr[u][i] == '*':
                arr[u][i] = str(0)
    arg1 = [int(arr[0][0]), int(arr[0][1])]
    arr = arr[1:]
    # print(arr)
    arr = list(map(int, (arr[o][y] for o in range(arg1[0]) for y in range(arg1[0]))))
    # print(arr)
    arr1 = list(arr[i * arg1[0]:(i + 1) * arg1[0]] for i in range(arg1[0]))
    # arr1.append(arr[15*arg1[0]:])
print(arg1)

arr = arr1
for i in range(arg1[0]):
    print(arr[i])
sources = []
stock = []
print("###############################")
for j in range(arg1[0]):
    if ([arr[i][j] for i in range(arg1[0])]) == [0 for i in range(arg1[0])]:
        sources.append(j)

for i in range(arg1[0]):
    if ([arr[i][j] for j in range(arg1[0])]) == [0 for j in range(arg1[0])]:
        stock.append(i)
print(sources, ' - sources')
print(stock, ' - stoks')
G1 = nx.from_numpy_array(np.array(arr), create_using=nx.DiGraph)
print(list(nx.bfs_layers(G1, sources)))
bfs_layers = list(nx.bfs_layers(G1, sources))
print(list(nx.dfs_tree(G1, sources[0])))

my_draw(arr, sources[0])


# Tree_undirected = [[0 for i in range(arg1[0])] for j in range(arg1[0])]
FLAG=1
while(FLAG):
    for i in stock:
        FLAG=FLAG and ([arr[j][i] for j in range(arg1[0])]!=[0 for i in range(arg1[0])])
    marked = (0)
    itoe = sources[0]
    layer_now = 0
    Tree = [[0 for i in range(arg1[0])] for j in range(arg1[0])]
    while marked != set(i for i in range(arg1[0])):
        for j in range(arg1[0]):
            if arr[itoe][j] != 0:
                if layer_now + 1 < len(bfs_layers) and j in bfs_layers[layer_now + 1]:
                    Tree[itoe][j] = arr[itoe][j]
                    # Tree_undirected[itoe][j] = arr[itoe][j]
                    # Tree_undirected[j][itoe] = arr[itoe][j]
        marked = marked + itoe
        if layer_now + 1 == len(bfs_layers):
            break
        # print(layer_now, bfs_layers[layer_now], bfs_layers[layer_now].index(itoe))
        if bfs_layers[layer_now].index(itoe) == len(bfs_layers[layer_now]) - 1:
            layer_now = layer_now + 1
            itoe = bfs_layers[layer_now][0]
        else:
            itoe = bfs_layers[layer_now][bfs_layers[layer_now].index(itoe) + 1]

    for i in range(arg1[0]):
        print(Tree[i])

    #my_draw(Tree, sources[0])
    # my_draw(Tree_undirected, sources[0])
    # G2 = nx.from_numpy_array(np.array(arr))
    # print(list(nx.chain_decomposition(G2, root=0)))
    print("#######find_threads#####")
    threads = []
    while [Tree[j][stock[0]] for j in range(arg1[0])] != [0 for i in range(arg1[0])]:
        l = inf
        itoe = stock[0]
        layer_now = len(bfs_layers) - 1
        chain = []
        k=0
        while (itoe not in sources):
            # print(itoe)
            for i in range(arg1[0]):
                if Tree[i][itoe] != 0 and i in bfs_layers[layer_now - 1]:
                    l = min(l, Tree[i][itoe])
                    chain.append([i, itoe])
                    layer_now = layer_now - 1
                    itoe = i
                    break
            k=k+1
            if(k>arg1[0]):

                break
        if (k<arg1[0]):
            threads.append([l, chain[::-1]])
            print(l, chain[::-1])
            for t in chain:
                Tree[t[0]][t[1]] = Tree[t[0]][t[1]] - l
                arr[t[0]][t[1]] = arr[t[0]][t[1]] - l
                arr[t[1]][t[0]] = arr[t[1]][t[0]] + l

            print()
            # for i in range(arg1[0]):
            #     print(arr[i])
        else:
            break
        if (threads==[]):
            break