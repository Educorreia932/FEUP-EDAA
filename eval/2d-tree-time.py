import networkx as nx
import random
import pandas as pd
import matplotlib
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

df = pd.read_csv('2d-tree-time.csv', index_col=0, header=None)
df['median'] = df.quantile(axis=1)        .ewm(span=8).mean()
df['q1'    ] = df.quantile(axis=1, q=0.25).ewm(span=8).mean()
df['q3'    ] = df.quantile(axis=1, q=0.75).ewm(span=8).mean()
df['min'   ] = df.min(axis=1)             .ewm(span=8).mean()
df['max'   ] = df.max(axis=1)             .ewm(span=8).mean()
df['mean'  ] = df.mean(axis=1)            .ewm(span=8).mean()

print(df)

fig = plt.figure()
ax = df['median'].plot(figsize=(10,6), color=(0,0,0))
ax.plot(df.index, df['mean'], '--', color=(0,0,0), lw=0.85)
ax = df['q1'    ].plot(ax=ax         , color=(0.2,0.2,0.2))
ax = df['q3'    ].plot(ax=ax         , color=(0.2,0.2,0.2))
ax = df['min'   ].plot(ax=ax         , color=(0.4,0.4,0.4))
# ax = df['max'   ].plot(ax=ax         , color=(0.4,0.4,0.4))
ax.fill_between(df.index, df['q1'], df['q3'], color=(253/255,184/255,19/255,0.75))
# ax.fill_between(df.index, df['min'], df['max'], color=(1,0,0,0.2))
ax.set_xlim(-2000, 300000)
ax.set_ylim(0, 350)
ax.set_title("2-d tree query execution time")
ax.set_xlabel("Number of points in set ($N$)")
ax.set_ylabel("Query time ($t$/ns)")
plt.xticks(range(0, 300000, 20000))
ax.get_xaxis().set_major_formatter(matplotlib.ticker.FuncFormatter(lambda x, p: f'{x/1000:.0f}k' if x >= 1000 else '0'))
plt.grid()
fig.tight_layout()

plt.savefig("2d-tree-time.svg")

plt.show()
