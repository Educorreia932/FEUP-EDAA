from operator import index
import numpy as np
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
from math import log2, sqrt
from scipy.stats import chi, chi2, kstest

matplotlib.rcParams.update({'font.size': 14})

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
df.sort_values(by=['delta_match_obs'], inplace=True)
df['delta_match_obs2'] = df['delta_match_obs'].map(lambda x: x*x)

mu_X = df['delta_match_obs2'].mean()
sigma_X = df['delta_match_obs2'].std()

print(f"mu_d = {df['delta_match_obs'].mean():.6f}")
print(f"mu_X' = {mu_X:.6f}")
print(f"sigma_X' = {sigma_X:.6f}")

DoF = 2
binWidth=0.5

sigma_Z_1 = sqrt(mu_X/DoF)
sigma_Z_2 = sigma_X**0.5 * (1/(2 * DoF))**0.25
print(sigma_Z_1, sigma_Z_2)

print(df)

f = lambda x: kstest(df['delta_match_obs']/x, 'chi', args=(DoF,)).pvalue
l,r = ternary_search(5.30, 5.32, 0.000000001, f)

# Ks = list(np.arange(5.30, 5.32, 0.01))
# for x in Ks:
#     ks = kstest(df['delta_match_obs']/x, 'chi', args=(DoF,))
#     print(f"{x}\t{ks.statistic}\t{ks.pvalue}")

sigma_Z = (l+r)/2
ks = kstest(df['delta_match_obs']/sigma_Z, 'chi', args=(DoF,))
print(f"{sigma_Z}\t{ks.statistic}\t{ks.pvalue}")

N = len(df.index)
print(N)
df['chi1'] = chi.pdf(df['delta_match_obs']/sigma_Z_1, DoF)/sigma_Z_1 * N*binWidth # *16000
df['chi2'] = chi.pdf(df['delta_match_obs']/sigma_Z_2, DoF)/sigma_Z_2 * N*binWidth # *16000
df['chi'] = chi.pdf(df['delta_match_obs']/sigma_Z, DoF)/sigma_Z * N*binWidth # *16000

fig = plt.figure(figsize=(10,6))

ax = df['delta_match_obs'].hist(figsize=(10,6), bins=[i*binWidth for i in range(0, 501, 1)], label="Data")
df.plot(x='delta_match_obs', y='chi2', ax=ax, label=f"$\chi$ fit ($DoF=2$, $\sigma_Z = {sigma_Z_2:.6f}$)")
df.plot(x='delta_match_obs', y='chi1', ax=ax, label=f"$\chi$ fit ($DoF=2$, $\sigma_Z = {sigma_Z_1:.6f}$)")
df.plot(x='delta_match_obs', y='chi', ax=ax, label=f"$\chi$ fit ($DoF=2$, $\sigma_Z = {sigma_Z:.6f}$)")

plt.xlim(0, 30)
plt.xlabel("Distance between observations and matches ($d$/m)")
plt.ylabel("Number of cases")

plt.title("Distance between observations and matches (NN)")
plt.grid()

plt.legend()

fig.tight_layout()

plt.savefig("nn-delta-match-obs.png", dpi=600)
plt.savefig("nn-delta-match-obs.svg")

plt.show()
