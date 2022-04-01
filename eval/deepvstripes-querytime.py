import pandas as pd
import matplotlib
import matplotlib.pyplot as plt

df = pd.read_csv('deepvstripes-querytime.csv', index_col=0, header=None)
df2 = pd.DataFrame(index = df.index)
# df2['x-adjusted'] = df.index.map(lambda x: log2(x))
df2['median'] = df.quantile(axis=1)        .ewm(span=8).mean()
df2['q1'    ] = df.quantile(axis=1, q=0.25).ewm(span=8).mean()
df2['q3'    ] = df.quantile(axis=1, q=0.75).ewm(span=8).mean()
df2['min'   ] = df.min(axis=1)             .ewm(span=8).mean()
df2['max'   ] = df.max(axis=1)             .ewm(span=8).mean()
df2['mean'  ] = df.mean(axis=1)            .ewm(span=8).mean()

print(df)
print(df2)

fig = plt.figure()
ax = df2['median'].plot(figsize=(10,6), color=(0,0,0))
ax.plot(df2.index, df2['mean'], '--', color=(0,0,0), lw=0.85)
ax.fill_between(df2.index, df2['q1'], df2['q3'], color=(253/255,184/255,19/255,0.75))
ax = df2['min'   ].plot(ax=ax         , color=(0.4,0.4,0.4))
ax = df2['q1'    ].plot(ax=ax         , color=(196/255, 145/255, 22/255))
ax = df2['q3'    ].plot(ax=ax         , color=(196/255, 145/255, 22/255))
# ax = df2['linreg'].plot(ax=ax         , color=(1,0,0), lw=0.8)
# ax = df2['max'   ].plot(ax=ax         , color=(0.4,0.4,0.4))
# ax.fill_between(df2.index, df2['min'], df2['max'], color=(1,0,0,0.2))
ax.set_xlim(-2000, 300000)
ax.set_ylim(0, 350)
ax.set_title("DeepVStripes query execution time ($d=0.0003$)")
ax.set_xlabel("Number of points in set ($N$)")
ax.set_ylabel("Query time ($t$/ns)")
plt.xticks(range(0, 300000+1, 20000))
ax.get_xaxis().set_major_formatter(matplotlib.ticker.FuncFormatter(lambda x, p: f'{x/1000:.0f}k' if x >= 1000 else '0'))
plt.grid()
ax.legend(["Median", "Mean", "IQR", "Minimum"])

plt.gcf().text(0.01, 0.02, "100,000 queries, averaged 10 runs,\n8-points exponential moving average", fontsize=8)

fig.tight_layout()

plt.savefig("deepvstripes-querytime.png", dpi=600)

plt.show()
