import matplotlib.pyplot as plt
import pandas as pd

from sklearn.linear_model import LinearRegression

df = pd.read_csv("hierarchical.csv", index_col=0, header=None)
df2 = pd.DataFrame(index=df.index)

df2["x-adjusted"] = df.index.map(lambda x: x ** 3)
df2["median"] = df.quantile(axis=1).ewm(span=4).mean()
df2["q1"] = df.quantile(axis=1, q=0.25).ewm(span=4).mean()
df2["q3"] = df.quantile(axis=1, q=0.75).ewm(span=4).mean()
df2["mean"] = df.mean(axis=1).ewm(span=4).mean()
df2["min"] = df.min(axis=1).ewm(span=4).mean()
df2["max"] = df.max(axis=1).ewm(span=4).mean()

X = df2['x-adjusted'].values.reshape(-1,1) 
Y = df2['mean'].values.reshape(-1, 1)  

linear_regressor = LinearRegression()  # Create object for the class
linear_regressor.fit(X, Y)             # Perform linear regression

r2 = linear_regressor.score(X, Y, sample_weight=None)
df2['linreg'] = linear_regressor.predict(X)

fig = plt.figure()

ax = df2["median"].plot(figsize=(10, 6), color=(0, 0, 0))
ax.plot(df.index, df2['mean'], '--', color=(0, 0, 0), lw=0.85)
ax.fill_between(df2.index, df2['q1'], df2['q3'], color=(253 / 255, 184 / 255, 19 / 255, 0.75))
ax = df2["min"].plot(ax=ax, color=(0.4, 0.4, 0.4))
ax = df2["max"].plot(ax=ax, color=(0.4, 0.4, 0.4))
ax = df2['linreg'].plot(ax=ax, color=(1, 0, 0), lw=0.8)

fig.tight_layout()

ax.set_title("Hierarchical Clustering execution time")
ax.legend(["Median", "Mean", "IQR", "Minimum", "Maximum"])

plt.xlabel("Number of points in set ($N$)")
plt.ylabel("Execution time ($t$/s)")

# plt.savefig("hierarchical.png", dpi=600)

plt.grid()
plt.show()
