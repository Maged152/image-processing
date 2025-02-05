#include "Resize.hpp"
#include "Scale.hpp"

template<qlm::ImageFormat frmt, qlm::pixel_t T>
qlm::Image<frmt, T> qlm::Resize(const qlm::Image<frmt, T>& in, const int width, const int height, const qlm::InterpolationFlag method, const BorderMode<frmt, T>& border_mode)
{
	int img_width = in.width;
	int img_height = in.height;
	// calculate the scale factors
	float scale_x = static_cast<float>(width) / img_width;
	float scale_y = static_cast<float>(height) / img_height;

	return qlm::Scale(in, method, scale_x, scale_y, border_mode);
}

// Explicit instantiation for RGB , uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::Resize<qlm::ImageFormat::RGB, uint8_t>(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>& ,
											const int,
											const int,
											const qlm::InterpolationFlag,
											const BorderMode<qlm::ImageFormat::RGB, uint8_t>&);
// Explicit instantiation for GRAY , uint8_t
template qlm::Image<qlm::ImageFormat::GRAY, uint8_t>
qlm::Resize<qlm::ImageFormat::GRAY, uint8_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
											 const int,
											 const int,
											 const qlm::InterpolationFlag,
											 const BorderMode<qlm::ImageFormat::GRAY, uint8_t>&);

