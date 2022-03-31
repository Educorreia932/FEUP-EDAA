import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('deepvstripes-querytime-d.csv', index_col=0)

print(df)

fig = plt.figure()
ax = df['t'].plot(figsize=(10,6), color=(0,0,0))
ax.set_xlim(0, 0.003)
ax.set_ylim(0, 2000)
ax.set_title("DeepVStripes query execution time")
ax.set_xlabel("Width of stripes ($d$/$^{\circ}$ lon)")
ax.set_ylabel("Query time ($t$/ns)")
# plt.xticks(range(0, 300000+1, 20000))
# ax.get_xaxis().set_major_formatter(matplotlib.ticker.FuncFormatter(lambda x, p: f'{x/1000:.0f}k' if x >= 1000 else '0'))
# ax.get_yaxis().set_major_formatter(matplotlib.ticker.FuncFormatter(lambda x, p: f'{x/1000000:.0f}'))
plt.grid()
fig.tight_layout()

# plt.savefig("deepvstripes-querytime-d.png", dpi=600)

plt.show()
