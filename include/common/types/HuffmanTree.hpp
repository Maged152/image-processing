#pragma once
#include <string>
#include <unordered_map>

namespace qlm
{
    template<ImageFormat frmt, pixel_t T>
    struct Huffman_t
    {
        std::unordered_map<T, std::string> table;
        std::string code;
    };

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
    class HuffmanTree 
    {
        private:
            HuffmanNode<T>* root = nullptr;
            std::vector<HuffmanNode<T>*> nodes;

        public:
            HuffmanTree(const Image<frmt, T>& in ,const Histogram_t<frmt, T>& hist);
            ~HuffmanTree();

            Huffman_t Encode() const;
    };
}