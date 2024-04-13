#include "SeamCarving.h"
#include "Sobel.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> SeamCarving(const Image<frmt, T>& in, 
		const size_t width, const size_t height,
		const EnergyFlag energy_fun, const OrderFlag order)
	{
		return Image<frmt, T>();
	}
}