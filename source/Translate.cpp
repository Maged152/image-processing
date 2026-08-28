#include "Translate.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T, typename S>
	Image<frmt, T> Translate(const Image<frmt, T>& in, const Point<S>& displacement, const Pixel<frmt, T>& pix)
	{
		// Translate image by (dx, dy):
    	// out(x, y) = in(x + dx, y + dy)

		int img_width = in.width;
		int img_height = in.height;

		// output image
		Image<frmt, T> out = Image<frmt, T>{};
		out.Create(img_width, img_height, pix);

		// loop over the output image
		for (int y = 0; y < img_height; y++)
		{
			for (int x = 0; x < img_width; x++)
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
			}
		}
		
		return out;
	}


	template Image<ImageFormat::RGB, uint8_t>  Translate(const Image<ImageFormat::RGB, uint8_t>&, const Point<int>&, const Pixel< ImageFormat::RGB, uint8_t >&);
	template Image<ImageFormat::GRAY, uint8_t> Translate(const Image<ImageFormat::GRAY, uint8_t>&, const Point<int>&, const Pixel<ImageFormat::GRAY, uint8_t>&);

	template Image<ImageFormat::RGB, uint8_t>  Translate(const Image<ImageFormat::RGB, uint8_t>&, const Point<float>&, const Pixel< ImageFormat::RGB, uint8_t >&);
	template Image<ImageFormat::GRAY, uint8_t> Translate(const Image<ImageFormat::GRAY, uint8_t>&, const Point<float>&, const Pixel<ImageFormat::GRAY, uint8_t>&);
}
