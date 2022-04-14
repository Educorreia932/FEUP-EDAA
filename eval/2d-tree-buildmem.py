import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression

df = pd.read_csv('2d-tree-buildmem.csv', index_col=0)
df['lower'] = df.index*3*(8*2)
df['upper'] = df.index*3*(8*2)*2

X = df.index.values.reshape(-1,1) # this  has shape (XXX, 1) - it's 2D
Y = df['mem'].values.reshape(-1,1) # this  has shape (XXX, 1) - it's 2D
linear_regressor = LinearRegression()  # create object for the class
linear_regressor.fit(X,Y)  # perform linear regression
r2 = linear_regressor.score(X, Y, sample_weight=None)
df['linreg'] = linear_regressor.predict(X)

print(df)

fig = plt.figure()
ax = df['mem'].plot(figsize=(10,6), color=(0,0,0))
ax = df['lower'].plot(ax=ax, color=(0.6,0.6,0.6),lw=0.8)
ax = df['upper'].plot(ax=ax, color=(0.6,0.6,0.6),lw=0.8)
ax = df['linreg'].plot(ax=ax, color=(1,0,0), lw=0.8)
ax.fill_between(df.index, df['lower'], df['upper'], color=(0,0,0,0.10))
ax.set_xlim(0, 300000)
ax.set_ylim(0, 30000000)
ax.set_title("2-d tree memory usage")
ax.set_xlabel("Number of points in set ($N$)")
ax.set_ylabel("Memory usage (MB)")
plt.xticks(range(0, 300000+1, 20000))
ax.get_xaxis().set_major_formatter(matplotlib.ticker.FuncFormatter(lambda x, p: f'{x/1000:.0f}k' if x >= 1000 else '0'))
ax.get_yaxis().set_major_formatter(matplotlib.ticker.FuncFormatter(lambda x, p: f'{x/1000000:.0f}'))
plt.grid()
ax.text(190000, 21000000, f'$y = 48 x$', fontsize=10)
ax.text(250000, 10500000, f'$y = 24 x$', fontsize=10)
ax.text(200000, 16500000, f'$y = {linear_regressor.coef_[0][0]:.3f} x {linear_regressor.intercept_[0]/1000000:+.3f}$\n$r^2={r2:.3f}$', fontsize=10)
fig.tight_layout()

plt.savefig("2d-tree-buildmem.png", dpi=600)
plt.savefig("2d-tree-buildmem.svg")

plt.show()
