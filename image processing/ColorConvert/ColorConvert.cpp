#include "ColorConvert.h"



namespace qlm
{
	template<ImageFormat src_frmt, pixel_t src_t, ImageFormat dst_frmt, pixel_t dst_t>
	Image<dst_frmt, dst_t> ColorConvert(const Image<src_frmt, src_t>& in)
	{
		unsigned int width = in.Width();
		unsigned int height = in.Height();
		// create the output image
		Image<dst_frmt, dst_t> out;
		out.create(width, height);
		// check if they are the same format & same data type
		if constexpr (dst_frmt == src_frmt && std::is_same_v<src_t, dst_t>)
		{
			// copy
			out = in;
			
			return out;
		}

		Pixel<dst_frmt, dst_t> dst_pix;
		Pixel<dst_frmt, dst_t> src_pix;

		for (int y = 0; i < with * height; i++)
		{
			// input pixel
			src_pix = in.GetPixel(i);
			if constexpr (dst_frmt == src_frmt)
			{
				// cast
				dst_pix = src_pix;	
			}
			else if constexpr (src_frmt == ImageFormat::RGB && dst_frmt == ImageFormat::GRAY)
			{
				// RGB to GRAY
			}
			else if constexpr (src_frmt == ImageFormat::GRAY && dst_frmt == ImageFormat::RGB)
			{
				// GRAY to RGB
			}
			// set output pixel
			out.SetPixel(i, dst_pix);
		}

		return out;
	}
}
