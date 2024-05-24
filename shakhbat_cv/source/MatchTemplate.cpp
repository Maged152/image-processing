#include "MatchTemplate.h"
#include <functional>
#include <cmath>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<ImageFormat::GRAY, float> MatchTemplate(
		const Image<frmt, T>& in,
		const Image<frmt, T>& templ,
		const TemplateMatchFlag mode,
		const Image<ImageFormat::GRAY, uint8_t>& mask)
	{
		Image<ImageFormat::GRAY, float> out{ in.width - templ.width + 1, in.height - templ.height + 1};

		std::function<float(Pixel<frmt, T>, Pixel<frmt, T>)> mt_metric;

		auto mt_sqdiff = [](const Pixel<frmt, T>& in1, const Pixel<frmt, T>& in2)
		{
			float ret{ 0 };
			if constexpr (frmt == ImageFormat::GRAY)
			{
				float diff = static_cast<float>(in1.v) - in2.v;
				ret = diff * diff;
			}
			else if constexpr (frmt == ImageFormat::RGB)
			{
				float diff_r = static_cast<float>(in1.r) - in2.r;
				float diff_g = static_cast<float>(in1.g) - in2.g;
				float diff_b = static_cast<float>(in1.b) - in2.b;

				ret = diff_r * diff_r + diff_g * diff_g + diff_b * diff_b;
			}
			return ret;
		};

		auto mt_ccorr = [](const Pixel<frmt, T>& in1, const Pixel<frmt, T>& in2)
		{
			float ret{ 0 };
			if constexpr (frmt == ImageFormat::GRAY)
			{
				ret = static_cast<float>(in1.v) * in2.v;
			}
			else if constexpr (frmt == ImageFormat::RGB)
			{
				ret += static_cast<float>(in1.r) * in2.r;
				ret += static_cast<float>(in1.g) * in2.g;
				ret += static_cast<float>(in1.b) * in2.b;

			}
			return ret;
		};

		bool do_normlization{ false };
		switch (mode)
		{
		case TemplateMatchFlag::SQDIFF:
			mt_metric = mt_sqdiff;
			break;

		case TemplateMatchFlag::CCORR:
			mt_metric = mt_ccorr;
			break;

		case TemplateMatchFlag::SQDIFF_NORMED:
			mt_metric = mt_sqdiff;
			do_normlization = true;
			break;

		case TemplateMatchFlag::CCORR_NORMED:
			mt_metric = mt_ccorr;
			do_normlization = true;
			break;
		}

		if (do_normlization)
		{
			float temple_deno{ 0.0f };
			for (int j = 0; j < templ.height; j++)
			{
				for (int i = 0; i < templ.width; i++)
				{
					auto templ_pix = templ.GetPixel(i, j);
					temple_deno += mt_ccorr(templ_pix, templ_pix);
				}
			}

			for (int y = 0; y < out.height; y++)
			{
				for (int x = 0; x < out.width; x++)
				{
					// Reset the sum array for each pixel
					float sum{ 0.0f };
					float src_deno{ 0.0f };

					for (int j = 0; j < templ.height; j++)
					{
						for (int i = 0; i < templ.width; i++)
						{
							if (mask.GetPixel(i, j).v)
							{
								// get the pixel
								auto src_pix = in.GetPixel(x + i, y + j);
								auto templ_pix = templ.GetPixel(i, j);

								sum += mt_metric(src_pix, templ_pix);
								src_deno += mt_ccorr(src_pix, src_pix);
							}
						}
					}
					// store the output
					out.SetPixel(x, y, sum / std::sqrt(src_deno * temple_deno));
				}
			}
		}
		else
		{
			for (int y = 0; y < out.height; y++)
			{
				for (int x = 0; x < out.width; x++)
				{
					// Reset the sum array for each pixel
					float sum{ 0.0f };

					for (int j = 0; j < templ.height; j++)
					{
						for (int i = 0; i < templ.width; i++)
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
		}
		
		return out;
	}

	template Image<ImageFormat::GRAY, float> MatchTemplate(
		const Image<ImageFormat::RGB, uint8_t>&,
		const Image<ImageFormat::RGB, uint8_t>&,
		const TemplateMatchFlag,
		const Image<ImageFormat::GRAY, uint8_t>&);

	template Image<ImageFormat::GRAY, float> MatchTemplate(
		const Image<ImageFormat::GRAY, uint8_t>&,
		const Image<ImageFormat::GRAY, uint8_t>&,
		const TemplateMatchFlag,
		const Image<ImageFormat::GRAY, uint8_t>&);
}