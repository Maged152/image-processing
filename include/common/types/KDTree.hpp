#pragma once

#include <array>
#include <vector>

template <int N, typename T>
struct KDNode 
{
    std::array<T, N> data;
    KDNode* left;
    KDNode* right;
    int axis;

    KDNode(const std::array<T, N>& data, int axis = 0) : data(data), left(nullptr), right(nullptr), axis(axis) 
    {}

    bool operator<(const KDNode& other) const 
    {
        return data[axis] < other.data[axis];
    }
};

template <int N, typename T>
class KDTree 
{
    private:
        KDNode<N, T>* root = nullptr;
        std::vector<KDNode<N, T>> nodes;
        int occupied = 0;
        int capacity = 0;

    public:
        KDTree(const int size) : capacity(size), nodes(size)
        {}

        ~KDTree() 
        {}

        void Build(const std::vector<std::array<T, N>>& points) 
        {
            Clear();

            if (points.empty()) 
            {
                return;
            }

            if (points.size() > capacity) 
            {
                nodes.resize(points.size());
                capacity = points.size();
            }

            for (const auto& point : points) 
            {
                Insert(point);
            }
        }
        
        void Insert(const std::array<T, N>& point) 
        {
            root = Insert(root, point, 0);
        }

        // bool find(const std::array<T, N>& point) const 
        // {
        //     return find(&nodes[0], point, 0);
        // }

    private:
        KDNode<N, T>* Insert(KDNode<N, T>* node, const std::array<T, N>& point, const int depth) 
        {
            if (node == nullptr) 
            {
                if (occupied < capacity) 
                {
                    nodes[occupied] = KDNode<N, T>(point, depth % N);
                    return &nodes[occupied++];
                } 
                else 
                {
                    nodes.push_back(KDNode<N, T>(point, depth % N));
                    capacity = nodes.capacity();
                    occupied++;
                    return &nodes.back();
                }
            }
            
            const int axis = node->axis;

            if (point[axis] < node->data[axis]) 
            {
                node->left = Insert(node->left, point, depth + 1);
            } 
            else 
            {
                node->right = Insert(node->right, point, depth + 1);
            }

            return node;
        }
      
        // bool find(KDNode<N, T>* node, const std::array<T, N>& point, int depth) const 
        // {
        //     if (node == nullptr) 
        //     {
        //         return false;
        //     }

        //     if (node->data == point) 
        //     {
        //         return true;
        //     }

        //     int axis = depth % N;
        //     if (point[axis] < node->data[axis]) 
        //     {
        //         return find(node->left, point, depth + 1);
        //     } 
        //     else 
        //     {
        //         return find(node->right, point, depth + 1);
        //     }
        // }

        void Clear() 
        {
            occupied = 0;
            root = nullptr;
        }

};