#pragma once

#include <array>
#include <vector>
#include <iostream>
#include <string>

template <typename T, int N>
struct KDNode 
{
    std::array<T, N> data;
    KDNode* left;
    KDNode* right;
    int axis;

    // Default constructor
    KDNode() : left(nullptr), right(nullptr), axis(0) 
    {}

    // Parameterized constructor
    KDNode(const std::array<T, N>& data, int axis = 0) : data(data), left(nullptr), right(nullptr), axis(axis) 
    {}

    bool operator<(const KDNode& other) const 
    {
        return data[axis] < other.data[axis];
    }

    void Print() const 
    {
        std::cout << "(";
        for (int i = 0; i < N; ++i) 
        {
            std::cout << data[i];
            if (i < N - 1) 
            {
                std::cout << ", ";
            }
        }
        std::cout << ")";
    }
};

template <typename T, int N>
class KDTree 
{
    private:
        KDNode<T, N>* root = nullptr;
        std::vector<KDNode<T, N>> nodes;
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

    void Print() const
    {
        PrintTree(root);
    }

    private:
        KDNode<T, N>* Insert(KDNode<T, N>* node, const std::array<T, N>& point, const int depth) 
        {
            if (node == nullptr) 
            {
                if (occupied < capacity) 
                {
                    nodes[occupied] = KDNode<T, N>(point, depth % N);
                    return &nodes[occupied++];
                } 
                else 
                {
                    nodes.push_back(KDNode<T, N>(point, depth % N));
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
      
        // bool find(KDNode<T, N>* node, const std::array<T, N>& point, int depth) const 
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

        void PrintTree(KDNode<T, N>* root, std::string prefix = "", bool is_right = true) const
        {
            if (root == nullptr) return;

            std::cout << prefix;

            std::cout <<  "|--" << (is_right ? "R: " : "L: ");

            root->Print();
            std::cout << std::endl;

            // Recur for left and right subtrees with updated prefix
            PrintTree(root->right, prefix + (is_right ? "|   " : "    "), true);
            PrintTree(root->left, prefix + (is_right ? "|   " : "    "), false);
        }
};