#pragma once


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
  

    template <typename T>
    class HuffmanTree 
    {
        private:
            HuffmanNode<T>* root = nullptr;
            std::vector<HuffmanNode<T>*> nodes;
            int occupied = 0;
            int capacity = 0;

        public:
            HuffmanTree(const int size) : capacity(size), nodes(size) 
            {}

            ~HuffmanTree() 
            {
                Clear();
            }
        };
}