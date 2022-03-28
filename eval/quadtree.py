import networkx as nx
import random
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns


# def gen_connected_graph(n : int):
#     G = nx.Graph()
#     G.add_nodes_from(list(range(n)))
    
#     while len(list(nx.connected_components(G))) > 1:
#         u = random.randrange(n)
#         v = random.randrange(n)

#         if u == v:
#             continue

#         G.add_edge(u, v)
    
#     return len(G.edges)

# res = []

# for n in range(10, 50):
#     print(f"Generating graphs of size {n}")
#     for _ in range(30):
#         res.append((n, gen_connected_graph(n)))

# dt = pd.DataFrame(res)

dt = pd.read_csv('quadtree.csv', index_col=0, header=None).T

print(dt)

ax = dt.boxplot(figsize=(15,7.5), rot=60, showmeans=True, whis=1000)
ax.set_ylim(0, 4000)
# ax.set_yscale('log')
plt.show()
