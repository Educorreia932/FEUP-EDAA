import pandas as pd
import matplotlib
import matplotlib.pyplot as plt

df = pd.read_csv('2d-tree-buildmem.csv', index_col=0)

print(df)

fig = plt.figure()
ax = df['mem'].plot(figsize=(10,6), color=(0,0,0))
ax.set_xlim(0, 300000)
ax.set_ylim(0, 30000000)
ax.set_title("2-d tree memory usage")
ax.set_xlabel("Number of points in set ($N$)")
ax.set_ylabel("Memory usage (MB)")
plt.xticks(range(0, 300000+1, 20000))
ax.get_xaxis().set_major_formatter(matplotlib.ticker.FuncFormatter(lambda x, p: f'{x/1000:.0f}k' if x >= 1000 else '0'))
ax.get_yaxis().set_major_formatter(matplotlib.ticker.FuncFormatter(lambda x, p: f'{x/1000000:.0f}'))
plt.grid()
fig.tight_layout()

plt.savefig("2d-tree-buildtime.svg")

plt.show()
