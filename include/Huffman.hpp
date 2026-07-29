#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
    struct Huffman_t
    {
		static constexpr int num_channels = (frmt == ImageFormat::GRAY) ? 1 : 3;

        std::unordered_map<T, std::string> table[num_channels];
        std::string code[num_channels];
    };
	
	template<ImageFormat frmt, pixel_t T>
	Huffman_t<frmt, T> HuffmanEncode(
		const Image<frmt, T>& in);

}