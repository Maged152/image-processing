#include "ConvertBitDepth.hpp"
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

		// normalize helper with clamping (float rounding safety)
		const auto normalize = [src_min, src_max](const src_t value)
		{
			const float v = (value - src_min) / (float)(src_max - src_min);
			return std::clamp(v, 0.0f, 1.0f);
		};

		// denormalize helper
		const auto denormalize = [dst_min, dst_max](const float v)
		{
			return static_cast<dst_t>(v * (dst_max - dst_min) + dst_min);
		};

		// check format
		if constexpr (frmt == ImageFormat::GRAY)
		{
			out.v = denormalize(normalize(in.v));
		}
		else if constexpr (frmt == ImageFormat::RGB)
		{
			out.r = denormalize(normalize(in.r));
			out.g = denormalize(normalize(in.g));
			out.b = denormalize(normalize(in.b));
		}
		else if constexpr (frmt == ImageFormat::HLS)
		{
			out.h = denormalize(normalize(in.h));
			out.l = denormalize(normalize(in.l));
			out.s = denormalize(normalize(in.s));
		}
		else if constexpr (frmt == ImageFormat::HSV)
		{
			out.h = denormalize(normalize(in.h));
			out.v = denormalize(normalize(in.v));
			out.s = denormalize(normalize(in.s));
		}
		else if constexpr (frmt == ImageFormat::YCrCb)
		{
			out.y = denormalize(normalize(in.y));
			out.cr = denormalize(normalize(in.cr));
			out.cb = denormalize(normalize(in.cb));
		}

		out.a = denormalize(normalize(in.a));
		
		return out;
	}

	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	Image<frmt, dst_t> ConvertBitDepth(const Image<frmt, src_t>& in)
	{
		Image<frmt, dst_t> out{ in.Width() , in.Height() };

		for (int y = 0; y < in.Height(); y++)
		{
			for (int x = 0; x < in.Width(); x++)
			{
				auto out_pix = ConvertBitDepth<frmt, src_t, dst_t>(in.GetPixel(x, y));
				out.SetPixel(x, y, out_pix);
			}
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

// Explicit instantiation for GRAY, uint16_t, uint8_t
template qlm::Image<qlm::ImageFormat::GRAY, uint8_t>
qlm::ConvertBitDepth<qlm::ImageFormat::GRAY, uint16_t, uint8_t>
	(const qlm::Image<qlm::ImageFormat::GRAY, uint16_t>&);
