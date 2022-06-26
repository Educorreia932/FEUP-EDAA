import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from scipy.cluster.hierarchy import dendrogram, linkage
from sklearn.cluster import AgglomerativeClustering

def plot_dendrogram(model, **kwargs):
    """Create linkage matrix and then plot the dendrogram"""

    # Create the counts of samples under each node
    counts = np.zeros(model.children_.shape[0])
    n_samples = len(model.labels_)

    for i, merge in enumerate(model.children_):
        current_count = 0

        for child_idx in merge:
            if child_idx < n_samples:
                current_count += 1  # Leaf node
        
            else:
                current_count += counts[child_idx - n_samples]

        counts[i] = current_count

    linkage_matrix = np.column_stack(
        [model.children_, model.distances_, counts]
    ).astype(float)

    # Plot the corresponding dendrogram
    dendrogram(linkage_matrix, **kwargs)


points_df = pd.read_csv("../res/map/processed/AMP.nodes", delimiter=" ", skiprows=[0], index_col=0, header=None)

# Array of points
X = np.array(list(zip(points_df[1], points_df[2])))[:100]

# setting distance_threshold=0 ensures we compute the full tree.
model = AgglomerativeClustering(distance_threshold=0, n_clusters=None)
model = model.fit(X)

plt.title("Hierarchical Clustering Dendrogram")
plot_dendrogram(model, truncate_mode="level")
plt.show()
