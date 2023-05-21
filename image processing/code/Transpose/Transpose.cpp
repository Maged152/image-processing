#define _USE_MATH_DEFINES

#include "Transpose/Transposeh.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Transpose(const Image<frmt, T>& in)
	{
		
	}


	template Image<ImageFormat::RGB, uint8_t> Transpose(const Image<ImageFormat::RGB, uint8_t>&);
	template Image<ImageFormat::GRAY, uint8_t> Transpose(const Image<ImageFormat::GRAY, uint8_t>&);
	template Image<ImageFormat::RGB, int16_t> Transpose(const Image<ImageFormat::RGB, int16_t>&);
	template Image<ImageFormat::GRAY, int16_t> Transpose(const Image<ImageFormat::GRAY, int16_t>&);
}
