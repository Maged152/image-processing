#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Huffman_t<frmt, T> HuffmanEncode(
		const Image<frmt, T>& in);

}