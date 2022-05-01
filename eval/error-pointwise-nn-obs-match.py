from operator import index
import numpy as np
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
from math import log, log2, sqrt
from scipy.stats import expon

matplotlib.rcParams.update({'font.size': 12})
matplotlib.rcParams['agg.path.chunksize'] = 10000

def ternary_search(l, r, epsilon, f):
    while (r-l) > epsilon:
        delta = (r-l)/3
        print((l,r), delta)
        x1 = l + delta*1
        x2 = l + delta*2
        y1 = f(x1)
        y2 = f(x2)
        if(y1 >= y2): r = x2
        else:         l = x1
    return (l,r)

df = pd.read_csv('error-pointwise-nn.csv', index_col=False)
df = df[df.dmatch.notnull()]
df['dobs-dmatch'] = df['dmatch'] - df['dobs']
df['abs(dobs-dmatch)'] = abs(df['dmatch'] - df['dobs'])
df.sort_values(by=['abs(dobs-dmatch)'], inplace=True)

mu_X = df['abs(dobs-dmatch)'].mean()
sigma_X = df['abs(dobs-dmatch)'].std()
median_t = df['abs(dobs-dmatch)'].quantile(interpolation='nearest')
beta = sum([
    df['abs(dobs-dmatch)'].quantile(0.50, interpolation='nearest') / log(1/0.50)
])/1

print(f"mu_X: {mu_X:.6f}")
print(f"sigma_X: {sigma_X:.6f}")
print(f"beta: {beta:.6f}")

binWidth=1

N = len(df.index)
print(N)
df['exp'] = expon.pdf(df['abs(dobs-dmatch)']/beta)/beta * N*binWidth # *16000

print(df)

fig = plt.figure(figsize=(8,6))

ax = df['abs(dobs-dmatch)'].hist(figsize=(8,6), bins=[i*binWidth for i in range(-100, 101, 1)], label="Data")
df.plot(x='abs(dobs-dmatch)', y='exp', ax=ax, label=f"Exp. fit ($\\beta = {beta:.6f}$)")

plt.xlim(0, 50)
plt.ylim(0, 60000)
plt.xlabel("Difference between great-circle and shortest-path distance ($|d_{route} - d_{circle}|$/m)")
plt.ylabel("Number of cases")

plt.title("Difference between great-circle and shortest-path distance (NN)")
plt.grid()

plt.legend()

fig.tight_layout()

plt.savefig("nn-obs-match.png", dpi=600)
plt.savefig("nn-obs-match.svg")

plt.show()
