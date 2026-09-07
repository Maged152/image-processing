#include "BoxFilter.hpp"
#include "SepFilter2D.hpp"
#include <cassert>

namespace qlm
{
	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	void BoxFilter(
				const Image<frmt, src_t>& in,
				      Image<frmt, dst_t>& out,
		        const int filter_x_size,
		        const int filter_y_size,
		        const bool normalize,
		        const  BorderMode<frmt, src_t>& border_mode,
		        const Rectangle<int>& roi)
	{
		// validate that the output has the same dimensions as the input
		assert(in.width == out.width && in.height == out.height);

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

		SepFilter2D<frmt, src_t, dst_t>(in, out, kernel, border_mode, roi);
	}

	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	Image<frmt, dst_t> BoxFilter(
				const Image<frmt, src_t>& in,
		        const int filter_x_size,
		        const int filter_y_size,
		        const bool normalize,
		        const  BorderMode<frmt, src_t>& border_mode,
		        const Rectangle<int>& roi)
	{
		Image<frmt, dst_t> out(in.width, in.height);

		BoxFilter(in, out, filter_x_size, filter_y_size, normalize, border_mode, roi);

		return out;
	}
}

// Explicit instantiation for RGB , uint8_t, uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::BoxFilter<qlm::ImageFormat::RGB, uint8_t, uint8_t>(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&,
	const int filter_x_size,
	const int filter_y_size,
	const bool normalize,
	const BorderMode<qlm::ImageFormat::RGB, uint8_t>&,
	const Rectangle<int>&);
template void
qlm::BoxFilter<qlm::ImageFormat::RGB, uint8_t, uint8_t>(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&,
	qlm::Image<qlm::ImageFormat::RGB, uint8_t>&,
	const int filter_x_size,
	const int filter_y_size,
	const bool normalize,
	const BorderMode<qlm::ImageFormat::RGB, uint8_t>&,
	const Rectangle<int>&);
// Explicit instantiation for RGB , uint8_t, int16_t
template qlm::Image<qlm::ImageFormat::RGB, int16_t>
qlm::BoxFilter<qlm::ImageFormat::RGB, uint8_t, int16_t>(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&,
	const int filter_x_size,
	const int filter_y_size,
	const bool normalize,
	const BorderMode<qlm::ImageFormat::RGB, uint8_t>&,
	const Rectangle<int>&);
template void
qlm::BoxFilter<qlm::ImageFormat::RGB, uint8_t, int16_t>(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&,
	qlm::Image<qlm::ImageFormat::RGB, int16_t>&,
	const int filter_x_size,
	const int filter_y_size,
	const bool normalize,
	const BorderMode<qlm::ImageFormat::RGB, uint8_t>&,
	const Rectangle<int>&);
// Explicit instantiation for GRAY, uint8_t , uint8_t
template qlm::Image<qlm::ImageFormat::GRAY, uint8_t>
qlm::BoxFilter<qlm::ImageFormat::GRAY, uint8_t, uint8_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	const int filter_x_size,
	const int filter_y_size,
	const bool normalize,
	const BorderMode<qlm::ImageFormat::GRAY, uint8_t>&,
	const Rectangle<int>&);
template void
qlm::BoxFilter<qlm::ImageFormat::GRAY, uint8_t, uint8_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	const int filter_x_size,
	const int filter_y_size,
	const bool normalize,
	const BorderMode<qlm::ImageFormat::GRAY, uint8_t>&,
	const Rectangle<int>&);
// Explicit instantiation for GRAY, uint8_t, int16_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::BoxFilter<qlm::ImageFormat::GRAY, uint8_t, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	const int filter_x_size,
	const int filter_y_size,
	const bool normalize,
	const BorderMode<qlm::ImageFormat::GRAY, uint8_t>&,
	const Rectangle<int>&);
template void
qlm::BoxFilter<qlm::ImageFormat::GRAY, uint8_t, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	qlm::Image<qlm::ImageFormat::GRAY, int16_t>&,
	const int filter_x_size,
	const int filter_y_size,
	const bool normalize,
	const BorderMode<qlm::ImageFormat::GRAY, uint8_t>&,
	const Rectangle<int>&);
// Explicit instantiation for GRAY, int16_t, int16_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::BoxFilter<qlm::ImageFormat::GRAY, int16_t, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, int16_t>&,
	const int filter_x_size,
	const int filter_y_size,
	const bool normalize,
	const BorderMode<qlm::ImageFormat::GRAY, int16_t>&,
	const Rectangle<int>&);
template void
qlm::BoxFilter<qlm::ImageFormat::GRAY, int16_t, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, int16_t>&,
	qlm::Image<qlm::ImageFormat::GRAY, int16_t>&,
	const int filter_x_size,
	const int filter_y_size,
	const bool normalize,
	const BorderMode<qlm::ImageFormat::GRAY, int16_t>&,
	const Rectangle<int>&);
// Explicit instantiation for GRAY, float, float
template qlm::Image<qlm::ImageFormat::GRAY, float>
qlm::BoxFilter<qlm::ImageFormat::GRAY, float, float>(const qlm::Image<qlm::ImageFormat::GRAY, float>&,
	const int filter_x_size,
	const int filter_y_size,
	const bool normalize,
	const BorderMode<qlm::ImageFormat::GRAY, float>&,
	const Rectangle<int>&);
template void
qlm::BoxFilter<qlm::ImageFormat::GRAY, float, float>(const qlm::Image<qlm::ImageFormat::GRAY, float>&,
	qlm::Image<qlm::ImageFormat::GRAY, float>&,
	const int filter_x_size,
	const int filter_y_size,
	const bool normalize,
	const BorderMode<qlm::ImageFormat::GRAY, float>&,
	const Rectangle<int>&);
