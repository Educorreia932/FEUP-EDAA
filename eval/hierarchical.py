import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("hierarchical.csv", index_col=0, header=None)

df["median"] = df.quantile(axis=1).ewm(span=8).mean()
df['q1'] = df.quantile(axis=1, q=0.25).ewm(span=8).mean()
df['q3'] = df.quantile(axis=1, q=0.75).ewm(span=8).mean()
df['mean'] = df.mean(axis=1).ewm(span=8).mean()
df["min"] = df.min(axis=1).ewm(span=8).mean()
df["max"] = df.max(axis=1).ewm(span=8).mean()

fig = plt.figure()

ax = df["median"].plot(figsize=(10, 6), color=(0, 0, 0))
ax.plot(df.index, df['mean'], '--', color=(0, 0, 0), lw=0.85)
ax.fill_between(df.index, df['q1'], df['q3'], color=(253 / 255, 184 / 255, 19 / 255, 0.75))
ax = df["min"].plot(ax=ax, color=(0.4, 0.4, 0.4))
ax = df["max"].plot(ax=ax, color=(0.4, 0.4, 0.4))

fig.tight_layout()

plt.xlabel("Number of points in set ($N$)")
plt.ylabel("Exectuion time ($t$/s)")

plt.grid()
plt.show()
