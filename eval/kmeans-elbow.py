import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from scipy.spatial.distance import cdist

points_df = pd.read_csv("../res/map/processed/AMP.nodes", delimiter=" ", skiprows=[0], index_col=0, header=None)
clusters_df = pd.read_csv("kmeans-elbow.csv", index_col=0)

K = [10, 50, 100, 250]

# Array of points
X = np.array(list(zip(points_df[1], points_df[2])))

# Series of centroids grouped by number of clusters
centroids_series = clusters_df.groupby("Number of clusters").apply(lambda x: np.array(list(zip(
    x["Centroid (Latitude)"],
    x["Centroid (Longitude)"]
))))

D_k = [cdist(X, centroids, "euclidean") for centroids in centroids_series]
cIdx = [np.argmin(D, axis=1) for D in D_k]
dist = [np.min(D, axis=1) for D in D_k]
avgWithinSS = [sum(d) / X.shape[0] for d in dist]

kIdx = 2

# Plot elbow curve
fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(K, avgWithinSS, 'b*-')
ax.plot(K[kIdx], avgWithinSS[kIdx], marker='o', markersize=12, markeredgewidth=2, markeredgecolor='r', markerfacecolor='None')

plt.grid(True)
plt.xlabel('Number of clusters')
plt.ylabel('Average within-cluster sum of squares')
plt.title('Elbow for K-Means clustering')

plt.show()