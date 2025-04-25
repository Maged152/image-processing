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

        HuffmanNode(const T& data, const int frequency,  HuffmanNode* left = nullptr, HuffmanNode* right = nullptr) : data(data), frequency(frequency), left(left), right(right) 
        {}

        bool operator<(const HuffmanNode& other) const 
        {
            return frequency > other.frequency; // Min-heap
        }
    };

    template <typename T>
    void Encode(const HuffmanNode *const  current, const std::string str, std::unordered_map<T, string> &huffmanCode)
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
        std::priority_queue<HuffmanNode> huffman_queue[num_codes];

        for (int i = 0; i < num_codes; i++) 
        {
            for (int e = 0; e < freq.tot_elements; e++) 
            {
                huffman_queue[i].push(HuffmanNode(e, freq.hist[i][e]));
            }
        }

        // Build the Huffman tree using a priority queue (min-heap)
        Huffman_t<frmt, T> out;
        HuffmanNode* root[out.num_channels];
        
        for (int i = 0; i < num_codes; i++) 
        {
            while (huffman_queue[i].size() > 1)
            {
                HuffmanNode<T> left = huffman_queue[i].top();
                huffman_queue[i].pop();

                HuffmanNode<T> right = huffman_queue[i].top();
                huffman_queue[i].pop();

                huffman_queue[i].push(HuffmanNode((0, left.frequency + right.frequency, &left, &right)));
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
            for (int e = 0; e < freq.tot_elements; e++) 
            {
                out.code[i] += out.table[i][e];
            }
        }

        return out;
    }

    template Huffman_t<ImageFormat::RGB, uint8_t> HuffmanEncode(const Image<ImageFormat::RGB, uint8_t>&);
	template Huffman_t<ImageFormat::GRAY, uint8_t> HuffmanEncode(const Image<ImageFormat::GRAY, uint8_t>&);
}