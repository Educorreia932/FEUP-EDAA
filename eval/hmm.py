import pandas as pd
import matplotlib
import matplotlib.pyplot as plt

df1 = pd.read_csv('hmm-vstripes.csv', index_col=0)
df2 = pd.read_csv('hmm-astar.csv', index_col=0)
df3 = pd.read_csv('hmm-dijkstra.csv', index_col=0)

print(df1)
print(df2)
print(df3)

print(df1['VStripes'].mean()/1e9)
print(df2['A*'].mean()/1e9)
print(df3['Dijkstra'].mean()/1e9)

fig, ax = plt.subplots()
ax.boxplot([df1['VStripes'], df2['A*'], df3['Dijkstra']], showmeans=True, whis=1000000)
ax.set_ylim(1e3, 1e9)
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

plt.show()
