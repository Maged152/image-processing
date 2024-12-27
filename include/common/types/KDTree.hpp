#pragma once

#include <array>
#include <vector>
#include <iostream>
#include <string>

namespace qlm
{
    template <typename T, int N>
    struct KDNode 
    {
        std::array<T, N> data;
        KDNode* left = nullptr;
        KDNode* right = nullptr;
        int axis = 0;
        int index = -1;

        KDNode()
        {}

        KDNode(const std::array<T, N>& data, int axis = 0, int index = -1) : data(data), left(nullptr), right(nullptr), axis(axis), index(index) 
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

            using dist_t = qlm::wider_t<qlm::signed_t<T>>;

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

            int NearestPoint(const std::array<T, N>& point) const 
            {
                KDNode<T, N>* best_node = nullptr;
                dist_t best_dist = std::numeric_limits<dist_t>::max();

                NearestPoint(root, point, best_node, best_dist);

                return best_node->index;
            }

            std::vector<int> RadiusSearch(const std::array<T, N>& point, dist_t radius) const 
            {
                std::vector<int> indices;
                RadiusSearch(root, point, radius, indices);
                return indices;
            }

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
                        nodes[occupied] = KDNode<T, N>(point, depth % N, occupied);
                        return &nodes[occupied++];
                    } 
                    else 
                    {
                        nodes.push_back(KDNode<T, N>(point, depth % N, occupied));
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

            dist_t SquaredEuclideanDistance(const std::array<T, N>& p0, const std::array<T, N>& p1) const 
            {
                dist_t sum = 0;
                for (int i = 0; i < N; ++i) 
                {
                    const dist_t diff = p0[i] - p1[i];
                    sum += diff * diff;
                }

                return sum;
            }
            
            void NearestPoint(KDNode<T, N>* node, const std::array<T, N>& query, KDNode<T, N>*& best_node, dist_t& best_dist) const 
            {
                if (node == nullptr) 
                {
                    return;
                }

                // calculate distance
                const dist_t dist = SquaredEuclideanDistance(node->data, query);

                // Update the best point and distance if closer
                if (dist < best_dist) 
                {
                    best_node = node;
                    best_dist = dist;
                }

                // Determine which subtree to search first
                const int axis = node->axis;
                KDNode<T, N>* good_side = nullptr;
                KDNode<T, N>* bad_side = nullptr;

                if (query[axis] < node->data[axis]) 
                {
                    good_side = node->left;
                    bad_side = node->right;
                } 
                else 
                {
                    good_side = node->right;
                    bad_side = node->left;
                }
                
                // search the good side
                NearestPoint(good_side, query, best_node, best_dist);

                // search the bad side if needed
                const dist_t plane_dist = node->data[axis] - query[axis];
                if (plane_dist * plane_dist < best_dist) 
                {
                    NearestPoint(bad_side, query, best_node, best_dist);
                }
            }
    
            void RadiusSearch(KDNode<T, N>* node, const std::array<T, N>& query, dist_t radius, std::vector<int>& indices) const 
            {
                if (node == nullptr) 
                {
                    return;
                }

                // calculate distance
                const dist_t dist = SquaredEuclideanDistance(node->data, query);

                // If the point is within the radius, add its index to the result
                if (dist <= radius * radius) 
                {
                    indices.push_back(node->index);
                }

                // Determine which subtree to search first
                const int axis = node->axis;
                KDNode<T, N>* good_side = nullptr;
                KDNode<T, N>* bad_side = nullptr;

                if (query[axis] < node->data[axis]) 
                {
                    good_side = node->left;
                    bad_side = node->right;
                } 
                else 
                {
                    good_side = node->right;
                    bad_side = node->left;
                }
                
                // search the good side
                RadiusSearch(good_side, query, radius, indices);

                // search the bad side if needed
                const dist_t plane_dist = node->data[axis] - query[axis];
                if (plane_dist * plane_dist <= radius * radius) 
                {
                    RadiusSearch(bad_side, query, radius, indices);
                }
            }
    };
}