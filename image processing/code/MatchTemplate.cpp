#include "MatchTemplate.h"
#include <functional>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<ImageFormat::GRAY, float> MatchTemplate(
		const Image<frmt, T>& in,
		const Image<frmt, T>& templ,
		const TemplateMatchFlag mode,
		const Image<ImageFormat::GRAY, uint8_t>& mask)
	{
		Image<ImageFormat::GRAY, float> out{ in.Width() - templ.Width() + 1, in.Height() - templ.Height() + 1};

		std::function<float(Pixel<frmt, T>, Pixel<frmt, T>)> mt_metric;

		auto mt_sqdiff = [](Pixel<frmt, T> in1, Pixel<frmt, T> in2)
		{
			return static_cast<float>(L2Norm(in1, in2));
		};

		switch (mode)
		{
		case TemplateMatchFlag::SQDIFF:
			mt_metric = mt_sqdiff;
			break;
		}

		for (int y = 0; y < out.Height(); y++)
		{
			for (int x = 0; x < out.Width(); x++)
			{
				// Reset the sum array for each pixel
				float sum{ 0.0f };

				for (int j = 0; j < templ.Height(); j++)
				{
					for (int i = 0; i < templ.Width(); i++)
					{
						if (mask.GetPixel(i, j).v)
						{
							// get the pixel
							auto src_pix = in.GetPixel(x + i, y + j);
							auto templ_pix = templ.GetPixel(i, j);

							sum += mt_metric(src_pix, templ_pix);
						}
					}
				}
				// store the output
				out.SetPixel(x, y, sum);
			}
		}

		return out;
	}

	template Image<ImageFormat::GRAY, float> MatchTemplate(
		const Image<ImageFormat::RGB, uint8_t>&,
		const Image<ImageFormat::RGB, uint8_t>&,
		const TemplateMatchFlag,
		const Image<ImageFormat::GRAY, uint8_t>&);
}