#include "Sobel.h"

namespace qlm
{
	// generate coefficients for Sobel X
	Kernel1D GetDerivXKernel(int n)
	{
		
	}
	/************************************************/
	// generate coefficients for Sobel Y
	Kernel1D qlm::GetDerivYKernel(int n)
	{
		
	}
	/************************************************/
	// Sobel X operation
	template<pixel_t in_t, pixel_t out_t>
	Image<ImageFormat::GRAY, out_t> SobelX(const Image<ImageFormat::GRAY, in_t>& in, const unsigned int kernel_size, Border border_type, int border_value)
	{
		
	}
	/************************************************/
	// Sobel X operation
	template<pixel_t in_t, pixel_t out_t>
	Image<ImageFormat::GRAY, out_t> SobelY(const Image<ImageFormat::GRAY, in_t>& in, const unsigned int kernel_size, Border border_type, int border_value)
	{
	
	}
	/************************************************/
	// Sobel operation
	template<pixel_t in_t, pixel_t out_t>
	SobelDerivatives<in_t, out_t> Sobel(const Image<ImageFormat::GRAY, in_t>& in, const unsigned int kernel_size, Border border_type, int border_value)
	{
		
	}
}