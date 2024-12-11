import cv2
import numpy as np
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt

# Load the image
image = cv2.imread('image.jpg')
image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

# Resize the image for faster processing
resized_image = cv2.resize(image, (100, 100), interpolation=cv2.INTER_AREA)

# Apply a Gaussian blur (convolution)
blurred_image = cv2.GaussianBlur(resized_image, (5, 5), 0)

# Reshape the image into a 2D array of pixels
pixels = blurred_image.reshape(-1, 3)

# Perform k-means clustering to find dominant colors
k = 5  # Number of dominant colors to find
kmeans = KMeans(n_clusters=k)
kmeans.fit(pixels)

# Extract dominant colors
dominant_colors = kmeans.cluster_centers_.astype(int)

# Get color percentages
labels, counts = np.unique(kmeans.labels_, return_counts=True)
percentages = counts / counts.sum()

# Display the dominant colors
for i, color in enumerate(dominant_colors):
    print(f"Color {i+1}: RGB{tuple(color)}, Percentage: {percentages[i]:.2%}")

# Visualize the dominant colors
plt.figure(figsize=(8, 2))
for i, color in enumerate(dominant_colors):
    plt.subplot(1, k, i+1)
    plt.imshow([[(color / 255)]])
    plt.axis('off')
plt.show()