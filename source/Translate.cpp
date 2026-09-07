#include "Translate.hpp"
#include <cassert>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T, typename S>
	void Translate(const Image<frmt, T>& in, Image<frmt, T>& out, const Point<S>& displacement, const Pixel<frmt, T>& pix, const Rectangle<int>& roi)
	{
		// Translate image by (dx, dy):
    	// out(x, y) = in(x + dx, y + dy)

		// validate that the output has the same dimensions as the input
		assert(in.width == out.width && in.height == out.height);

		int img_width = in.width;
		int img_height = in.height;

		auto [start_w, start_h, end_w, end_h] = ROI(roi, img_width, img_height);

		// loop over the working area of the output image
		for (int y = start_h; y < end_h; y++)
		{
			for (int x = start_w; x < end_w; x++)
			{
				// translate the pixel coordinates by the displacement vector
				S in_x = x - displacement.x;
				S in_y = y - displacement.y;

				// Check if the new coordinates are within the bounds of the output image.
				if (in_x < img_width && in_x >= 0 && in_y < img_height && in_y >= 0)
				{
					Pixel<frmt, T> out_pix;
					if constexpr(std::is_same_v<S, int>)
					{
						out_pix = in.GetPixel(in_x, in_y);
					}
					else
					{
						// Perform bilinear interpolation
						out_pix = BilinearInterpolation(in, in_x, in_y, BorderMode<frmt, T>{});
					}

					out.SetPixel(x, y, out_pix);
				}
				else
				{
					out.SetPixel(x, y, pix);
				}
			}
		}
	}

	template<ImageFormat frmt, pixel_t T, typename S>
	Image<frmt, T> Translate(const Image<frmt, T>& in, const Point<S>& displacement, const Pixel<frmt, T>& pix, const Rectangle<int>& roi)
	{
		// output image
		Image<frmt, T> out = Image<frmt, T>{};
		out.Create(in.width, in.height, pix);

		Translate(in, out, displacement, pix, roi);

		return out;
	}


	template Image<ImageFormat::RGB, uint8_t>  Translate(const Image<ImageFormat::RGB, uint8_t>&, const Point<int>&, const Pixel< ImageFormat::RGB, uint8_t >&, const Rectangle<int>&);
	template Image<ImageFormat::GRAY, uint8_t> Translate(const Image<ImageFormat::GRAY, uint8_t>&, const Point<int>&, const Pixel<ImageFormat::GRAY, uint8_t>&, const Rectangle<int>&);

	template Image<ImageFormat::RGB, uint8_t>  Translate(const Image<ImageFormat::RGB, uint8_t>&, const Point<float>&, const Pixel< ImageFormat::RGB, uint8_t >&, const Rectangle<int>&);
	template Image<ImageFormat::GRAY, uint8_t> Translate(const Image<ImageFormat::GRAY, uint8_t>&, const Point<float>&, const Pixel<ImageFormat::GRAY, uint8_t>&, const Rectangle<int>&);

	template void Translate(const Image<ImageFormat::RGB, uint8_t>&, Image<ImageFormat::RGB, uint8_t>&, const Point<int>&, const Pixel< ImageFormat::RGB, uint8_t >&, const Rectangle<int>&);
	template void Translate(const Image<ImageFormat::GRAY, uint8_t>&, Image<ImageFormat::GRAY, uint8_t>&, const Point<int>&, const Pixel<ImageFormat::GRAY, uint8_t>&, const Rectangle<int>&);

	template void Translate(const Image<ImageFormat::RGB, uint8_t>&, Image<ImageFormat::RGB, uint8_t>&, const Point<float>&, const Pixel< ImageFormat::RGB, uint8_t >&, const Rectangle<int>&);
	template void Translate(const Image<ImageFormat::GRAY, uint8_t>&, Image<ImageFormat::GRAY, uint8_t>&, const Point<float>&, const Pixel<ImageFormat::GRAY, uint8_t>&, const Rectangle<int>&);
}
