#include "Resize/Resize.h"
#include "Scale/Scale.h"

template<qlm::ImageFormat frmt, qlm::pixel_t T>
qlm::Image<frmt, T> qlm::Resize(const qlm::Image<frmt, T>& in, const unsigned int width, const unsigned int height, qlm::ScaleMethod method)
{
	unsigned int img_width = in.Width();
	unsigned int img_height = in.Height();
	// calculate the scale factors
	float scale_x = static_cast<float>(width) / img_width;
	float scale_y = static_cast<float>(height) / img_height;

	return qlm::Scale(in, method, scale_x, scale_y);
}

// Explicit instantiation for RGB , uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::Resize<qlm::ImageFormat::RGB, uint8_t>(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>& ,
											const unsigned int,
											const unsigned int,
											qlm::ScaleMethod);
// Explicit instantiation for RGB , int16_t
template qlm::Image<qlm::ImageFormat::RGB, int16_t>
qlm::Resize<qlm::ImageFormat::RGB, int16_t>(const qlm::Image<qlm::ImageFormat::RGB, int16_t>&,
											const unsigned int,
											const unsigned int,
											qlm::ScaleMethod);
// Explicit instantiation for GRAY , uint8_t
template qlm::Image<qlm::ImageFormat::GRAY, uint8_t>
qlm::Resize<qlm::ImageFormat::GRAY, uint8_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
											 const unsigned int,
											 const unsigned int,
											 qlm::ScaleMethod);
// Explicit instantiation for GRAY , uint8_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::Resize<qlm::ImageFormat::GRAY, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, int16_t>&,
	                                         const unsigned int,
	                                         const unsigned int,
	                                         qlm::ScaleMethod);
