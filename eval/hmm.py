import pandas as pd
import matplotlib
import matplotlib.pyplot as plt

df1 = pd.read_csv('hmm-vstripes.csv', index_col=0)

df4 = pd.read_csv('hmm-astar.csv', index_col=0)
df2 = pd.read_csv('hmm-dijkstra-s.csv', index_col=0)
df6 = pd.read_csv('hmm-astarfew.csv', index_col=0)
df5 = pd.read_csv('hmm-astar-d.csv', index_col=0)
df3 = pd.read_csv('hmm-dijkstra-sd.csv', index_col=0)
df7 = pd.read_csv('hmm-astarfew-d.csv', index_col=0)
df8 = pd.read_csv('hmm-viterbi.csv', index_col=0)
df9 = pd.read_csv('hmm-viterbi-o.csv', index_col=0)

fig1, ax1 = plt.subplots(figsize=(2.1,6))
bp = ax1.boxplot([
    df1['VStripes']/1e9,
], labels=[
    'VStripes'
], showmeans=True, whis=1000000, widths=0.5, patch_artist=True, meanprops={"marker":"x","markeredgecolor":"black"})
ax1.set_ylim(1e-4, 1e2)
ax1.set_title("Fixed-radius NN\nexecution time")
ax1.set_ylabel("Execution time ($t$/s)")
plt.yscale('log')
ax1.grid('on', which='minor', axis='y')
ax1.grid('on', which='major', axis='y')
for element in ['boxes', 'whiskers', 'fliers', 'means', 'medians', 'caps']: plt.setp(bp[element], color='black')
for box in bp['boxes']: box.set(facecolor=(0,0,1,0.5))
fig1.tight_layout()
plt.savefig("hmm-nn.png", dpi=600)
plt.savefig("hmm-nn.svg")

fig2, ax2 = plt.subplots(figsize=(7,6))
bp = ax2.boxplot([
    df4['A*'            ]/1e9,
    df2['Dijkstra-s'    ]/1e9,
    df6['A*few'         ]/1e9,
    df5['A*-d'          ]/1e9,
    df3['Dijkstra-sd'   ]/1e9,
    df7['A*few-d'       ]/1e9,
], labels=[
    'A*',
    'Dijkstra',
    'A*few',
    'A*-d',
    'Dijkstra-d',
    'A*few-d',
],
showmeans=True, whis=1000000, widths=0.5, patch_artist=True, meanprops={"marker":"x","markeredgecolor":"black"})
ax2.set_ylim(1e-4, 1e2)
ax2.set_title("Shortest paths\nexecution time")
plt.yscale('log')
ax2.grid('on', which='minor', axis='y')
ax2.grid('on', which='major', axis='y')
for element in ['boxes', 'whiskers', 'fliers', 'means', 'medians', 'caps']: plt.setp(bp[element], color='black')
for box in bp['boxes']: box.set(facecolor=(1,0,0,0.5))
ax2.set_yticklabels([])
fig2.tight_layout()
plt.savefig("hmm-shortestpath.png", dpi=600)
plt.savefig("hmm-shortestpath.svg")

fig3, ax3 = plt.subplots(figsize=(2.5,6))
bp = ax3.boxplot([
    df8['Viterbi'   ]/1e9,
    df9['Viterbi-o' ]/1e9,
], labels=[
    'Viterbi',
    'Viterbi (opt.)',
], showmeans=True, whis=1000000, widths=0.5, patch_artist=True, meanprops={"marker":"x","markeredgecolor":"black"})
ax3.set_ylim(1e-4, 1e2)
ax3.set_title("Viterbi\nexecution time")
plt.yscale('log')
ax3.grid('on', which='minor', axis='y')
ax3.grid('on', which='major', axis='y')
for element in ['boxes', 'whiskers', 'fliers', 'means', 'medians', 'caps']: plt.setp(bp[element], color='black')
for box in bp['boxes']: box.set(facecolor=(0,1,0,0.5))
ax3.set_yticklabels([])
fig3.tight_layout()
plt.savefig("hmm-viterbi.png", dpi=600)
plt.savefig("hmm-viterbi.svg")

print("\\begin{tabular}{l | r r r r r}")
print("    \\textbf{Method} & \\textbf{Q1} & \\textbf{Median} & \\textbf{Q3} & \\textbf{Mean} & \\textbf{N}\\\\ \\hline")
print(f"    VStripes            & \SI{{{df1['VStripes'   ].quantile(0.25)/1e9:.6f}}}{{}} & \SI{{{df1['VStripes'   ].quantile(0.5)/1e9:.6f}}}{{}} & \SI{{{df1['VStripes'   ].quantile(0.75)/1e9:.6f}}}{{}} & \SI{{{df1['VStripes'   ].mean()/1e9:.6f}}}{{}} & \SI{{{df1['VStripes'   ].size}}}{{}}\\\\ \\hline")
print(f"    A*                  & \SI{{{df4['A*'         ].quantile(0.25)/1e9:.6f}}}{{}} & \SI{{{df4['A*'         ].quantile(0.5)/1e9:.6f}}}{{}} & \SI{{{df4['A*'         ].quantile(0.75)/1e9:.6f}}}{{}} & \SI{{{df4['A*'         ].mean()/1e9:.6f}}}{{}} & \SI{{{df4['A*'         ].size}}}{{}}\\\\")
print(f"    Dijkstra            & \SI{{{df2['Dijkstra-s' ].quantile(0.25)/1e9:.6f}}}{{}} & \SI{{{df2['Dijkstra-s' ].quantile(0.5)/1e9:.6f}}}{{}} & \SI{{{df2['Dijkstra-s' ].quantile(0.75)/1e9:.6f}}}{{}} & \SI{{{df2['Dijkstra-s' ].mean()/1e9:.6f}}}{{}} & \SI{{{df2['Dijkstra-s' ].size}}}{{}}\\\\")
print(f"    A*few               & \SI{{{df6['A*few'      ].quantile(0.25)/1e9:.6f}}}{{}} & \SI{{{df6['A*few'      ].quantile(0.5)/1e9:.6f}}}{{}} & \SI{{{df6['A*few'      ].quantile(0.75)/1e9:.6f}}}{{}} & \SI{{{df6['A*few'      ].mean()/1e9:.6f}}}{{}} & \SI{{{df6['A*few'      ].size}}}{{}}\\\\")
print(f"    A*-d                & \SI{{{df5['A*-d'       ].quantile(0.25)/1e9:.6f}}}{{}} & \SI{{{df5['A*-d'       ].quantile(0.5)/1e9:.6f}}}{{}} & \SI{{{df5['A*-d'       ].quantile(0.75)/1e9:.6f}}}{{}} & \SI{{{df5['A*-d'       ].mean()/1e9:.6f}}}{{}} & \SI{{{df5['A*-d'       ].size}}}{{}}\\\\")
print(f"    Dijkstra-d          & \SI{{{df3['Dijkstra-sd'].quantile(0.25)/1e9:.6f}}}{{}} & \SI{{{df3['Dijkstra-sd'].quantile(0.5)/1e9:.6f}}}{{}} & \SI{{{df3['Dijkstra-sd'].quantile(0.75)/1e9:.6f}}}{{}} & \SI{{{df3['Dijkstra-sd'].mean()/1e9:.6f}}}{{}} & \SI{{{df3['Dijkstra-sd'].size}}}{{}}\\\\")
print(f"    A*few-d             & \SI{{{df7['A*few-d'    ].quantile(0.25)/1e9:.6f}}}{{}} & \SI{{{df7['A*few-d'    ].quantile(0.5)/1e9:.6f}}}{{}} & \SI{{{df7['A*few-d'    ].quantile(0.75)/1e9:.6f}}}{{}} & \SI{{{df7['A*few-d'    ].mean()/1e9:.6f}}}{{}} & \SI{{{df7['A*few-d'    ].size}}}{{}}\\\\ \\hline")
print(f"    Viterbi             & \SI{{{df8['Viterbi'    ].quantile(0.25)/1e9:.6f}}}{{}} & \SI{{{df8['Viterbi'    ].quantile(0.5)/1e9:.6f}}}{{}} & \SI{{{df8['Viterbi'    ].quantile(0.75)/1e9:.6f}}}{{}} & \SI{{{df8['Viterbi'    ].mean()/1e9:.6f}}}{{}} & \SI{{{df8['Viterbi'    ].size}}}{{}}\\\\")
print(f"    Viterbi (optimized) & \SI{{{df9['Viterbi-o'  ].quantile(0.25)/1e9:.6f}}}{{}} & \SI{{{df9['Viterbi-o'  ].quantile(0.5)/1e9:.6f}}}{{}} & \SI{{{df9['Viterbi-o'  ].quantile(0.75)/1e9:.6f}}}{{}} & \SI{{{df9['Viterbi-o'  ].mean()/1e9:.6f}}}{{}} & \SI{{{df9['Viterbi-o'  ].size}}}{{}}\\\\")
print("\\end{tabular}")

# plt.show()
