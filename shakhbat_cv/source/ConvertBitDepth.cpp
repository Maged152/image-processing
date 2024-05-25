#include "ConvertBitDepth.h"
#include <algorithm>
#include <cmath>

namespace qlm
{
	// change bit depth for a pixel
	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	Pixel<frmt, dst_t> ConvertBitDepth(const Pixel<frmt, src_t>& in)
	{
		// output pixel
		Pixel<frmt, dst_t> out;
		// min-max input
		const src_t src_min = std::numeric_limits<src_t>::lowest();
		const src_t src_max = std::numeric_limits<src_t>::max();
		// min-max output
		const dst_t dst_min = std::numeric_limits<dst_t>::lowest();
		const dst_t dst_max = std::numeric_limits<dst_t>::max();
		// check format
		if constexpr (frmt == ImageFormat::GRAY)
		{
			float v{ 0 }, a{ 0 };
			// normalize value
			v = (in.v - src_min) / (float)(src_max - src_min);
			a = (in.a - src_min) / (float)(src_max - src_min);
			// denormalize
			out.v = static_cast<dst_t>(v * (dst_max - dst_min) + dst_min);
			out.a = static_cast<dst_t>(a * (dst_max - dst_min) + dst_min);
		}
		else if constexpr (frmt == ImageFormat::RGB)
		{
			float r{ 0 }, g{ 0 }, b{ 0 }, a{ 0 };
			// normalize value
			r = (in.r - src_min) / (float)(src_max - src_min);
			g = (in.g - src_min) / (float)(src_max - src_min);
			b = (in.b - src_min) / (float)(src_max - src_min);
			a = (in.a - src_min) / (float)(src_max - src_min);
			// denormalize
			out.r = static_cast<dst_t>(r * (dst_max - dst_min) + dst_min);
			out.g = static_cast<dst_t>(g * (dst_max - dst_min) + dst_min);
			out.b = static_cast<dst_t>(b * (dst_max - dst_min) + dst_min);
			out.a = static_cast<dst_t>(a * (dst_max - dst_min) + dst_min);
		}
		else if constexpr (frmt == ImageFormat::HLS)
		{
			float h{ 0 }, l{ 0 }, s{ 0 }, a{ 0 };
			// normalize value
			h = (in.h - src_min) / (float)(src_max - src_min);
			l = (in.l - src_min) / (float)(src_max - src_min);
			s = (in.s - src_min) / (float)(src_max - src_min);
			a = (in.a - src_min) / (float)(src_max - src_min);
			// denormalize
			out.h = static_cast<dst_t>(h * (dst_max - dst_min) + dst_min);
			out.l = static_cast<dst_t>(l * (dst_max - dst_min) + dst_min);
			out.s = static_cast<dst_t>(s * (dst_max - dst_min) + dst_min);
			out.a = static_cast<dst_t>(a * (dst_max - dst_min) + dst_min);
		}
		else if constexpr (frmt == ImageFormat::HSV)
		{
			float h{ 0 }, v{ 0 }, s{ 0 }, a{ 0 };
			// normalize value
			h = (in.h - src_min) / (float)(src_max - src_min);
			v = (in.v - src_min) / (float)(src_max - src_min);
			s = (in.s - src_min) / (float)(src_max - src_min);
			a = (in.a - src_min) / (float)(src_max - src_min);
			// denormalize
			out.h = static_cast<dst_t>(h * (dst_max - dst_min) + dst_min);
			out.v = static_cast<dst_t>(v * (dst_max - dst_min) + dst_min);
			out.s = static_cast<dst_t>(s * (dst_max - dst_min) + dst_min);
			out.a = static_cast<dst_t>(a * (dst_max - dst_min) + dst_min);
		}
		else if constexpr (frmt == ImageFormat::YCrCb)
		{
			float y{ 0 }, cr{ 0 }, cb{ 0 }, a{ 0 };
			// normalize value
			y = (in.y - src_min) / (float)(src_max - src_min);
			cr = (in.cr - src_min) / (float)(src_max - src_min);
			cb = (in.cb - src_min) / (float)(src_max - src_min);
			a = (in.a - src_min) / (float)(src_max - src_min);
			// denormalize
			out.y = static_cast<dst_t>(y * (dst_max - dst_min) + dst_min);
			out.cr = static_cast<dst_t>(cr * (dst_max - dst_min) + dst_min);
			out.cb = static_cast<dst_t>(cb * (dst_max - dst_min) + dst_min);
			out.a = static_cast<dst_t>(a * (dst_max - dst_min) + dst_min);
		}
		
		return out;
	}
	// change bit depth for an image
	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	Image<frmt, dst_t> ConvertBitDepth(const Image<frmt, src_t>& in)
	{
		Image<frmt, dst_t> out{ in.width , in.height };

		for (int i = 0; i < in.height * in.width; i++)
		{
			auto out_pix = ConvertBitDepth<frmt, src_t, dst_t>(in.GetPixel(i));
			out.SetPixel(i, out_pix);

		}

		return out;
	}
}

// Explicit instantiation for GRAY, uint8_t, int16_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::ConvertBitDepth<qlm::ImageFormat::GRAY, uint8_t, int16_t>
	(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&);
// Explicit instantiation for GRAY, int16_t, uint8_t
template qlm::Image<qlm::ImageFormat::GRAY, uint8_t>
qlm::ConvertBitDepth<qlm::ImageFormat::GRAY, int16_t, uint8_t>
	(const qlm::Image<qlm::ImageFormat::GRAY, int16_t>&);

// Explicit instantiation for GRAY, uint8_t, int16_t
template qlm::Image<qlm::ImageFormat::RGB, int16_t>
qlm::ConvertBitDepth<qlm::ImageFormat::RGB, uint8_t, int16_t>
(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&);
// Explicit instantiation for RGB, int16_t, uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::ConvertBitDepth<qlm::ImageFormat::RGB, int16_t, uint8_t>
(const qlm::Image<qlm::ImageFormat::RGB, int16_t>&);


// Explicit instantiation for GRAY, uint8_t, int16_t
template qlm::Pixel<qlm::ImageFormat::GRAY, int16_t>
qlm::ConvertBitDepth<qlm::ImageFormat::GRAY, uint8_t, int16_t>
(const qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>&);
// Explicit instantiation for GRAY, int16_t, uint8_t
template qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>
qlm::ConvertBitDepth<qlm::ImageFormat::GRAY, int16_t, uint8_t>
(const qlm::Pixel<qlm::ImageFormat::GRAY, int16_t>&);

// Explicit instantiation for GRAY, uint8_t, int16_t
template qlm::Pixel<qlm::ImageFormat::RGB, int16_t>
qlm::ConvertBitDepth<qlm::ImageFormat::RGB, uint8_t, int16_t>
(const qlm::Pixel<qlm::ImageFormat::RGB, uint8_t>&);
// Explicit instantiation for RGB, int16_t, uint8_t
template qlm::Pixel<qlm::ImageFormat::RGB, uint8_t>
qlm::ConvertBitDepth<qlm::ImageFormat::RGB, int16_t, uint8_t>
(const qlm::Pixel<qlm::ImageFormat::RGB, int16_t>&);
