import numpy as np
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
from math import log2

df = pd.read_csv('deepvstripes-buildtime.csv', index_col=0, header=None).T
df2 = df.ewm(span=4).mean()
df2['L'] = df['L']

print()
print(df2)

fig = plt.figure(figsize=(10,6))

Ls = [
    '1',
    '3',
    '6',
    '9',
    '12',
    '15',
    '18',
    '21',
    '24',
]

colors = plt.cm.rainbow(np.linspace(0.0, 1.0, len(Ls)))

for i in range(len(Ls)):
    plt.plot(df2['L'], df2[Ls[i]], color=colors[i])
plt.xlim(0, 300000)
plt.ylim(0, 0.6e9)
plt.title("DeepVStripes build execution time ($d=0.0003$)")
plt.xlabel("Number of points in set ($N$)")
plt.ylabel("Build time ($t$/s)")
plt.xticks(range(0, 300000+1, 20000))
plt.gca().get_xaxis().set_major_formatter(matplotlib.ticker.FuncFormatter(lambda x, p: f'{x/1000:.0f}k' if x >= 1000 else '0'))
plt.gca().get_yaxis().set_major_formatter(matplotlib.ticker.FuncFormatter(lambda y, p: f'{y/1000000000:.1f}' if y > 0 else '0'))
plt.grid()
plt.legend([f"$L={L}$" for L in Ls], ncol=2)
fig.tight_layout()

plt.savefig("deepvstripes-buildtime.png", dpi=600)

plt.show()
