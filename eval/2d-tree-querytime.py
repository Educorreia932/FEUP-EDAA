import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from math import log2

df = pd.read_csv('2d-tree-querytime.csv', index_col=0, header=None)
df2 = pd.DataFrame(index = df.index)
df2['x-adjusted'] = df.index.map(lambda x: log2(x))
df2['median'] = df.quantile(axis=1)        .ewm(span=8).mean()
df2['q1'    ] = df.quantile(axis=1, q=0.25).ewm(span=8).mean()
df2['q3'    ] = df.quantile(axis=1, q=0.75).ewm(span=8).mean()
df2['min'   ] = df.min(axis=1)             .ewm(span=8).mean()
df2['max'   ] = df.max(axis=1)             .ewm(span=8).mean()
df2['mean'  ] = df.mean(axis=1)            .ewm(span=8).mean()

X = df2['x-adjusted'].values.reshape(-1,1) # this  has shape (XXX, 1) - it's 2D
Y = df2['mean'].values.reshape(-1,1) # this  has shape (XXX, 1) - it's 2D
linear_regressor = LinearRegression()  # create object for the class
linear_regressor.fit(X,Y)  # perform linear regression
r2 = linear_regressor.score(X, Y, sample_weight=None)
df2['linreg'] = linear_regressor.predict(X)

print(df)
print(df2)

fig = plt.figure()
ax = df2['median'].plot(figsize=(10,6), color=(0,0,0))
ax.plot(df2.index, df2['mean'], '--', color=(0,0,0), lw=0.85)
ax.fill_between(df2.index, df2['q1'], df2['q3'], color=(253/255,184/255,19/255,0.75))
ax = df2['min'   ].plot(ax=ax         , color=(0.4,0.4,0.4))
ax = df2['q1'    ].plot(ax=ax         , color=(196/255, 145/255, 22/255))
ax = df2['q3'    ].plot(ax=ax         , color=(196/255, 145/255, 22/255))
ax = df2['linreg'].plot(ax=ax         , color=(1,0,0), lw=0.8)
# ax = df2['max'   ].plot(ax=ax         , color=(0.4,0.4,0.4))
# ax.fill_between(df2.index, df2['min'], df2['max'], color=(1,0,0,0.2))
ax.set_xlim(-2000, 300000)
ax.set_ylim(0, 350)
ax.set_title("2-d tree query execution time")
ax.set_xlabel("Number of points in set ($N$)")
ax.set_ylabel("Query time ($t$/ns)")
plt.xticks(range(0, 300000+1, 20000))
ax.get_xaxis().set_major_formatter(matplotlib.ticker.FuncFormatter(lambda x, p: f'{x/1000:.0f}k' if x >= 1000 else '0'))
plt.grid()
ax.legend(["Median", "Mean", "IQR", "Minimum"])
ax.text(170000, 115, f'$y = {linear_regressor.coef_[0][0]:.3f} x\' {linear_regressor.intercept_[0]:+.3f}$\n$r^2={r2:.3f}$', fontsize=10)
ax.text(50000, 325, f'$x\'=\log_2{{x}}$', fontsize=10)

plt.gcf().text(0.01, 0.02, "100,000 queries, averaged 10 runs,\n8-points exponential moving average", fontsize=8)

fig.tight_layout()

plt.savefig("2d-tree-querytime.png", dpi=600)
plt.savefig("2d-tree-querytime.svg")

plt.show()
