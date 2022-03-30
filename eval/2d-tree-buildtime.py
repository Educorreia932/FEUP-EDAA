import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from math import log2

df = pd.read_csv('2d-tree-buildtime.csv', index_col=0, header=None)
df2 = pd.DataFrame(index = df.index)
df2['t-avg'] = df.ewm(span=2).mean()
df2['x-adjusted'] = df2.index.map(lambda x: x * (log2(x)**2))
df2['lower'] = df2['x-adjusted']*3.3
df2['upper'] = df2['x-adjusted']*5.2

print((df2['t-avg']-df2['lower']).apply(lambda x: x < 0.0).sum()/df2['t-avg'].size)
print((df2['t-avg']-df2['upper']).apply(lambda x: x > 0.0).sum()/df2['t-avg'].size)

X = df2['x-adjusted'].values.reshape(-1,1) # this  has shape (XXX, 1) - it's2 2D
Y = df2['t-avg'].values.reshape(-1,1) # this  has shape (XXX, 1) - it's 2D2
linear_regressor = LinearRegression()  # create object for the class
linear_regressor.fit(X,Y)  # perform linear regression
r2 = linear_regressor.score(X, Y, sample_weight=None)
df2['linreg'] = linear_regressor.predict(X)

print(df)
print(df2)

fig = plt.figure()
ax = df2['t-avg'].plot(figsize=(10,6), color=(0,0,0))
ax = df2['lower'].plot(ax=ax, color=(0.6,0.6,0.6),lw=0.8)
ax = df2['upper'].plot(ax=ax, color=(0.6,0.6,0.6),lw=0.8)
ax = df2['linreg'].plot(ax=ax, color=(1,0,0), lw=0.8)
ax.fill_between(df2.index, df2['lower'], df2['upper'], color=(0,0,0,0.10))
ax.set_xlim(0, 300000)
ax.set_ylim(0, 0.5*1e9)
ax.set_title("2-d tree build execution time")
ax.set_xlabel("Number of points in set ($N$)")
ax.set_ylabel("Build time ($t$/s)")
plt.xticks(range(0, 300000+1, 20000))
ax.get_xaxis().set_major_formatter(matplotlib.ticker.FuncFormatter(lambda x, p: f'{x/1000:.0f}k' if x >= 1000 else '0'))
ax.get_yaxis().set_major_formatter(matplotlib.ticker.FuncFormatter(lambda x, p: f'{x/1e9:.1f}'))
plt.grid()
ax.text(205000, 0.38*1e9, f'$y = 5.2 x\'$', fontsize=10)
ax.text(230000, 0.22*1e9, f'$y = 3.3 x\'$', fontsize=10)
ax.text(180000, 0.29*1e9, f'$y = {linear_regressor.coef_[0][0]:.3f} x\' {linear_regressor.intercept_[0]/1000000:+.3f}$\n$r^2={r2:.3f}$', fontsize=10)
ax.text(5000, 0.47*1e9, f'$x\'=x (\log_2{{x}})^2$', fontsize=10)
fig.tight_layout()

plt.savefig("2d-tree-buildtime.png", dpi=600)

plt.show()
