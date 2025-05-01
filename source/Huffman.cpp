#include "Huffman.hpp"
#include "Histogram.hpp"
#include <vector>
#include <queue>
#include <functional>

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

        HuffmanNode(const T& data, const int frequency,  HuffmanNode* left = nullptr, HuffmanNode* right = nullptr) : data(data), frequency(frequency), left(left), right(right) 
        {}
    };

    template <typename T>
    struct CompareHuffmanNode
    {
        bool operator()(const HuffmanNode<T>* left, const HuffmanNode<T>* right) const
        {
            return left->frequency > right->frequency; // Min-heap
        }
    };

    template <typename T>
    void Encode(const HuffmanNode<T> *const  current, const std::string str, std::unordered_map<T, std::string> &huffmanCode)
    {
        if (current == nullptr)
            return;

        // found a leaf node
        if (current->left == nullptr && current->right == nullptr)
            huffmanCode[current->data] = str;
        

        Encode(current->left, str + "0", huffmanCode);
        Encode(current->right, str + "1", huffmanCode);
    }

    template<ImageFormat frmt, pixel_t T>
    Huffman_t<frmt, T> HuffmanEncode(const Image<frmt, T>& in)
    {
        // Get the histogram of the image
        const auto freq =  Histogram(in);
       
        // Create nodes for each unique pixel value and push them into the priority queue
        constexpr int num_codes = freq.num_channels;
        std::priority_queue<HuffmanNode<T>*, std::vector<HuffmanNode<T>*>, CompareHuffmanNode<T>> huffman_queue[num_codes];

        for (int i = 0; i < num_codes; i++) 
        {
            for (int e = 0; e < freq.tot_elements; e++) 
            {
                const int hist_value = freq.hist[i][e];
                if (hist_value)
                    huffman_queue[i].push(new HuffmanNode<T>(e, hist_value));
            }
        }

        // Build the Huffman tree using a priority queue (min-heap)
        Huffman_t<frmt, T> out;
        HuffmanNode<T>* root [out.num_channels];
        
        for (int i = 0; i < num_codes; i++) 
        {
            while (huffman_queue[i].size() > 1)
            {
                HuffmanNode<T>* left = huffman_queue[i].top();
                huffman_queue[i].pop();

                HuffmanNode<T>* right = huffman_queue[i].top();
                huffman_queue[i].pop();

                huffman_queue[i].push(new HuffmanNode<T>(0, left->frequency + right->frequency, left, right));
            }

            root[i] = huffman_queue[i].top(); // The root of the tree
        }

        // Generate the Huffman codes by traversing the tree
        for (int i = 0; i < num_codes; i++) 
        {
            Encode(root[i], "", out.table[i]);
        }

        // Create the encoded string for each channel
        for (int i = 0; i < num_codes; i++) 
        {
            for (int e = 0; e < out.table[i].size(); e++) 
            {
                out.code[i] += out.table[i][e];
            }
        }

        // Free the dynamically allocated memory for the Huffman tree
        for (int i = 0; i < num_codes; i++) 
        {
            std::function<void(HuffmanNode<T>*)> deleteTree = [&](HuffmanNode<T>* node) {
                if (node == nullptr) return;
                deleteTree(node->left);
                deleteTree(node->right);
                delete node;
            };
            deleteTree(root[i]);
        }

        return out;
    }

    template Huffman_t<ImageFormat::RGB, uint8_t> HuffmanEncode(const Image<ImageFormat::RGB, uint8_t>&);
	template Huffman_t<ImageFormat::GRAY, uint8_t> HuffmanEncode(const Image<ImageFormat::GRAY, uint8_t>&);
}