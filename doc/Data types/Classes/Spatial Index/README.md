# Spatial Index

The kd-tree types used for the neighborhood queries of `DBSCAN` and `KMeans`.

## KDNode&lt;T, N&gt;

A kd-tree node over N-dimensional points. Defined in
[`include/common/types/KDTree.hpp`](../../../../include/common/types/KDTree.hpp).

```c++
template <typename T, int N>
struct KDNode
{
    std::array<T, N> data;       // the point coordinates
    KDNode* left  = nullptr;
    KDNode* right = nullptr;
    int axis  = 0;               // splitting axis of the node
    int index = -1;              // index of the point among the inserted points
};
```

## KDTree&lt;T, N&gt;

A kd-tree over `T`-typed, N-dimensional points.

```c++
template <typename T, int N>
class KDTree
{
public:
    KDTree(const int size);                       // pre-allocate storage for size nodes

    void                 Build(const std::vector<std::array<T, N>>& points);
    void                 Insert(const std::array<T, N>& point);

    int                  NearestPoint(const std::array<T, N>& point) const;                 // index of the nearest point
    std::vector<int>     RadiusSearch(const std::array<T, N>& point,
                                      typename qlm::wider_t<qlm::signed_t<T>> radius) const; // indices within radius

    void                 Print() const;
};
```

* Distances are computed in a wider signed type (`wider_t<signed_t<T>>`); the query
  functions return the **index** of a point, not the point itself.
* Used by `DBSCAN` (region queries) and `KMeans` (cluster assignment).


