#include "Huffman.hpp"
#include "Histogram.hpp"
#include <vector>
#include <queue>

namespace qlm
{
    template <typename T>
    struct HuffmanNode 
    {
        T data;
        int frequency;
        HuffmanNode* left = nullptr;
        HuffmanNode* right = nullptr;

        HuffmanNode() : data(0), frequency(0), left(nullptr), right(nullptr) 
        {}

        HuffmanNode(const T& data, const int frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) 
        {}

        bool operator<(const HuffmanNode& other) const 
        {
            return frequency > other.frequency; // Min-heap
        }
    };

    template<ImageFormat frmt, pixel_t T>
    Huffman_t<frmt, T> HuffmanEncode(const Image<frmt, T>& in)
    {
        // Get the histogram of the image
        const auto freq =  Histogram(in);
       
        // Create nodes for each unique pixel value and push them into the priority queue
        std::priority_queue<HuffmanNode> huffman_queue;

        for (const auto& pair : frequencyMap) {
            nodes.push_back(new HuffmanNode<T>(pair.first, pair.second));
        }

        // Build the Huffman tree using a priority queue (min-heap)
        std::priority_queue<HuffmanNode<T>*, std::vector<HuffmanNode<T>*>, std::greater<HuffmanNode<T>*>> minHeap(nodes.begin(), nodes.end());

        while (minHeap.size() > 1) {
            HuffmanNode<T>* left = minHeap.top();
            minHeap.pop();
            HuffmanNode<T>* right = minHeap.top();
            minHeap.pop();

            HuffmanNode<T>* newNode = new HuffmanNode<T>(0, left->frequency + right->frequency);
            newNode->left = left;
            newNode->right = right;

            minHeap.push(newNode);
        }

        root = minHeap.top(); // The root of the tree
    }

}