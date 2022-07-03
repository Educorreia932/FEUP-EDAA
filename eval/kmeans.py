import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

df = pd.read_csv("kmeans.csv", index_col=0, header=None).T
df2 = df.ewm(span=4).mean()
df2['L'] = df['num_clusters']

fig = plt.figure()

Ls = [
    '10',
    '20',
    '30',
    '40',
]

colors = plt.cm.rainbow(np.linspace(0.0, 1.0, len(Ls)))

for i in range(len(Ls)):
    plt.plot(df2['L'], df2[Ls[i]], color=colors[i])

plt.title("$k$-means execution time")
plt.xlabel("Number of points in set ($N$)")
plt.ylabel("Execution time ($t$/s)")
plt.legend([f"$K={L}$" for L in Ls], ncol=2)
plt.gcf().text(0.01, 0.02, "Averaged 5 samples, 4-points exponential moving average", fontsize=8)

fig.tight_layout()

plt.savefig("kmeans.png", dpi=600)

plt.grid()
plt.show()
