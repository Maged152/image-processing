# K-Means Clustering Algorithm

## Description
Performs clustering using the K-Means algorithm

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<Cluster<frmt, T>> KMeans(
		const Image<frmt, T>& in,
		const unsigned int k = 8,
		const KMeansInit init = KMeansInit::KMEANS_PLUS_PLUS,
		const int seed = 0,
		const int max_iter = 300,
		const T tol = 0);
}
```



## Parameters

| Name                  | Type          | Description                                                                                        |
|-----------------------|---------------|---------------------------------------------------------------------------------------------------|
| `in`                  | `Image`       | The input image.                                                                                   |
| `k`                   | `unsigned int`| The number of clusters to partition the pixels into. (default: 8).                                |
| `init`                | `KMeansInit`  | The initialization method for cluster centroids. Possible values:`RANDOM`, `KMEANS_PLUS_PLUS` (default: KMEANS_PLUS_PLUS).  |
| `seed`                | `int`         | The seed value for the random number generator used in k-means initialization. (default: 0).                             |
| `max_iter`            | `int`         | The maximum number of iterations allowed for the k-means algorithm to converge. (default: 300).                             |
| `tol`                 | `T`           | The tolerance value for early stopping. The algorithm stops if the squared difference between centroids in consecutive iterations is less than or equal to tol. (default: 0)  |



## Return Value
The function returns a vector of clusters of type `std::vector<Cluster<frmt, T>>`.
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
		struct Cluster
		{
			std::vector<Point<int>> pixels;
			Pixel<frmt, T> color;
		};
}
```

* [Example](../../../Examples/Features%20Detection/KMeans)
* [Implementation](../../../../code/KMeans/KMeans.cpp)
