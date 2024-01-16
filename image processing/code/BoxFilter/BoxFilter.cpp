#include "BoxFilter/BoxFilter.h"
#include "SepFilter2D/SepFilter2D.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> BoxFilter(const Image<frmt, T>& in,
		                     const unsigned int filter_x_size,
		                     const unsigned int filter_y_size,
		                     const bool normalize, 
		                     const  BorderMode<frmt, T>& border_mode)
	{
		SepKernel kernel{ filter_x_size , filter_y_size };

		const float val_x = normalize ? 1.0f / filter_x_size : 1.0f;
		const float val_y = normalize ? 1.0f / filter_y_size : 1.0f;

		for (int i = 0; i < filter_x_size; i++)
		{
			kernel.x_ker.Set(i, val_x);
		}

		for (int i = 0; i < filter_y_size; i++)
		{
			kernel.y_ker.Set(i, val_y);
		}


		return SepFilter2D<frmt, T, T>(in, kernel, border_mode);
	}
}

// Explicit instantiation for RGB , uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::BoxFilter<qlm::ImageFormat::RGB, uint8_t>(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&,
	const unsigned int filter_x_size,
	const unsigned int filter_y_size,
	const bool normalize,
	const BorderMode<qlm::ImageFormat::RGB, uint8_t>&);
// Explicit instantiation for RGB , int16_t
template qlm::Image<qlm::ImageFormat::RGB, int16_t>
qlm::BoxFilter<qlm::ImageFormat::RGB, int16_t>(const qlm::Image<qlm::ImageFormat::RGB, int16_t>&,
	const unsigned int filter_x_size,
	const unsigned int filter_y_size,
	const bool normalize,
	const BorderMode<qlm::ImageFormat::RGB, int16_t>&);
// Explicit instantiation for GRAY , uint8_t
template qlm::Image<qlm::ImageFormat::GRAY, uint8_t>
qlm::BoxFilter<qlm::ImageFormat::GRAY, uint8_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	const unsigned int filter_x_size,
	const unsigned int filter_y_size,
	const bool normalize,
	const BorderMode<qlm::ImageFormat::GRAY, uint8_t>&);
// Explicit instantiation for GRAY , int16_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::BoxFilter<qlm::ImageFormat::GRAY, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, int16_t>&,
	const unsigned int filter_x_size,
	const unsigned int filter_y_size,
	const bool normalize,
	const BorderMode<qlm::ImageFormat::GRAY, int16_t>&);