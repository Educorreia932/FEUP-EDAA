import pandas as pd
import matplotlib
import matplotlib.pyplot as plt

df1 = pd.read_csv('hmm-vstripes.csv', index_col=0)

df2 = pd.read_csv('hmm-dijkstra-s.csv', index_col=0)
df3 = pd.read_csv('hmm-dijkstra-sd.csv', index_col=0)
df4 = pd.read_csv('hmm-astar.csv', index_col=0)
df5 = pd.read_csv('hmm-astar-d.csv', index_col=0)
df6 = pd.read_csv('hmm-astarfew.csv', index_col=0)
df7 = pd.read_csv('hmm-astarfew-d.csv', index_col=0)

# print(df1['VStripes'].mean()/1e9)
# print(df2['Dijkstra-s'].mean()/1e9)
# print(df3['Dijkstra-sd'].mean()/1e9)
# print(df4['A*'].mean()/1e9)

fig, ax = plt.subplots()
ax.boxplot([
    df1['VStripes'],
    df2['Dijkstra-s'],
    df3['Dijkstra-sd'],
    df4['A*'],
    df5['A*-d'],
    df6['A*few'],
    df7['A*few-d'],
], showmeans=True, whis=1000000)
ax.set_ylim(1e3, 1e11)
# ax.set_title("2-d tree memory usage")
# ax.set_xlabel("Number of points in set ($N$)")
ax.set_ylabel("Execution time ($t$/ms)")
ax.get_yaxis().set_major_formatter(matplotlib.ticker.FuncFormatter(lambda x, p: f'{x/1e6:.1f}'))
plt.yscale('log')
ax.grid('on', which='minor', axis='y')
ax.grid('on', which='major', axis='y')
fig.tight_layout()

plt.savefig("hmm-vstripes.png", dpi=600)
plt.savefig("hmm-vstripes.svg")

print("\\begin{tabular}{l | r r r r r}")
print("    \\textbf{Method} & \\textbf{Q1} & \\textbf{Median} & \\textbf{Q3} & \\textbf{Mean} & \\textbf{N}\\\\ \\hline")
print(f"    VStripes    & {df1['VStripes'   ].quantile(0.25)/1e9:.6f} & {df1['VStripes'   ].quantile(0.5)/1e9:.6f} & {df1['VStripes'   ].quantile(0.75)/1e9:.6f} & {df1['VStripes'   ].mean()/1e9:.6f} & {df1['VStripes'   ].size}\\\\ \\hline")
print(f"    A*          & {df4['A*'         ].quantile(0.25)/1e9:.6f} & {df4['A*'         ].quantile(0.5)/1e9:.6f} & {df4['A*'         ].quantile(0.75)/1e9:.6f} & {df4['A*'         ].mean()/1e9:.6f} & {df4['A*'         ].size}\\\\")
print(f"    Dijkstra    & {df2['Dijkstra-s' ].quantile(0.25)/1e9:.6f} & {df2['Dijkstra-s' ].quantile(0.5)/1e9:.6f} & {df2['Dijkstra-s' ].quantile(0.75)/1e9:.6f} & {df2['Dijkstra-s' ].mean()/1e9:.6f} & {df2['Dijkstra-s' ].size}\\\\")
print(f"    A*few       & {df6['A*few'      ].quantile(0.25)/1e9:.6f} & {df6['A*few'      ].quantile(0.5)/1e9:.6f} & {df6['A*few'      ].quantile(0.75)/1e9:.6f} & {df6['A*few'      ].mean()/1e9:.6f} & {df6['A*few'      ].size}\\\\")
print(f"    A*-d        & {df5['A*-d'       ].quantile(0.25)/1e9:.6f} & {df5['A*-d'       ].quantile(0.5)/1e9:.6f} & {df5['A*-d'       ].quantile(0.75)/1e9:.6f} & {df5['A*-d'       ].mean()/1e9:.6f} & {df5['A*-d'       ].size}\\\\")
print(f"    Dijkstra-d  & {df3['Dijkstra-sd'].quantile(0.25)/1e9:.6f} & {df3['Dijkstra-sd'].quantile(0.5)/1e9:.6f} & {df3['Dijkstra-sd'].quantile(0.75)/1e9:.6f} & {df3['Dijkstra-sd'].mean()/1e9:.6f} & {df3['Dijkstra-sd'].size}\\\\")
print(f"    A*few-d     & {df7['A*few-d'    ].quantile(0.25)/1e9:.6f} & {df7['A*few-d'    ].quantile(0.5)/1e9:.6f} & {df7['A*few-d'    ].quantile(0.75)/1e9:.6f} & {df7['A*few-d'    ].mean()/1e9:.6f} & {df7['A*few-d'    ].size}\\\\")
print("\\end{tabular}")

plt.show()
