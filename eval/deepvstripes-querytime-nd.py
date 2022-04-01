import numpy as np
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
from math import log2

df = pd.read_csv('deepvstripes-querytime-nd.csv', index_col=0, header=None).T
df2 = df.ewm(span=4).mean()
df2['d'] = df['d']

print()
print(df2)

fig = plt.figure(figsize=(10,6))

l = [
    '0.0001',
    '0.0002',
    '0.0003',
    '0.0004',
    '0.0005',
    '0.0006',
    '0.0007',
    '0.0008',
    '0.0009',
    '0.0010'
]

colors = plt.cm.rainbow(np.linspace(0.0, 1.0, len(l)))

for i in range(len(l)):
    plt.plot(df2['d'], df2[l[i]], color=colors[i])
plt.xlim(-2000, 300000)
plt.ylim(0, 300)
plt.title("DeepVStripes query execution time")
plt.xlabel("Number of points in set ($N$)")
plt.ylabel("Query time ($t$/ns)")
plt.xticks(range(0, 300000+1, 20000))
plt.gca().get_xaxis().set_major_formatter(matplotlib.ticker.FuncFormatter(lambda x, p: f'{x/1000:.0f}k' if x >= 1000 else '0'))
plt.grid()
plt.legend([f"$d={d}$" for d in l], ncol=2)

plt.gcf().text(0.01, 0.02, "100,000 queries, averaged 10 runs,\n4-points exponential moving average", fontsize=8)

fig.tight_layout()

plt.savefig("deepvstripes-querytime-nd.png", dpi=600)

plt.show()
