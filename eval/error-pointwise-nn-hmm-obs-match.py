from operator import index
import numpy as np
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
from math import log, log2, sqrt
from scipy.stats import expon

matplotlib.rcParams.update({'font.size': 12})
matplotlib.rcParams['agg.path.chunksize'] = 10000

df1 = pd.read_csv('error-pointwise-nn.csv', index_col=False)
df1 = df1[df1.dmatch.notnull()]
df1['dobs-dmatch'] = df1['dmatch'] - df1['dobs']
df1['abs(dobs-dmatch)'] = abs(df1['dmatch'] - df1['dobs'])
df1.sort_values(by=['abs(dobs-dmatch)'], inplace=True)

df2 = pd.read_csv('error-pointwise-hmm.csv', index_col=False)
df2 = df2[df2.dmatch.notnull()]
df2['dobs-dmatch'] = df2['dmatch'] - df2['dobs']
df2['abs(dobs-dmatch)'] = abs(df2['dmatch'] - df2['dobs'])
df2.sort_values(by=['abs(dobs-dmatch)'], inplace=True)

print(df1)

fig, ax = plt.subplots(figsize=(10,3))
bp = ax.boxplot([
    df2['abs(dobs-dmatch)'],
    df1['abs(dobs-dmatch)'],
], labels=[
    'HMM',
    'NN',
],
vert=False, whis=1000000, widths=0.5, patch_artist=True, showmeans=True, meanprops={"marker":"x","markeredgecolor":"black"})

plt.xlim(1e-3, 1e5)
plt.xlabel("$|d_{route} - d_{circle}|$/m")
plt.xscale('log')
ax.grid('on', which='minor', axis='x')
ax.grid('on', which='major', axis='x')

for element in ['boxes', 'whiskers', 'fliers', 'means', 'medians', 'caps']: plt.setp(bp[element], color='black')
for box in bp['boxes']: box.set(facecolor=(0,0,1,0.5))

plt.title("$|d_{route} - d_{circle}|$")

fig.tight_layout()

plt.savefig("nn-hmm-obs-match.png", dpi=600)
plt.savefig("nn-hmm-obs-match.svg")

plt.show()
