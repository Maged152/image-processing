#include "HoughLines.hpp"
#include <iostream>

template<qlm::pixel_t T>
std::vector<qlm::LinePolar> qlm::HoughLines(const qlm::Image<qlm::ImageFormat::GRAY, T>& in, 
	const float rho, const float theta_step, const int threshold, const double min_theta, const double max_theta)
{
	// output lines
	std::vector<qlm::LinePolar> lines;

	// validate parameters
	if (rho <= 0.f || theta_step <= 0.f || min_theta >= max_theta)
	{
		std::cerr << "HoughLines: invalid parameters (rho and theta_step must be > 0, min_theta < max_theta)\n";
		return lines;
	}

	const int width = in.Width();
	const int height = in.Height();

	const int max_rho = std::round(std::sqrt(width * width + height * height));
	const int min_rho = -max_rho;

	int num_angle = std::round((max_theta - min_theta) / theta_step);
	// If the distance between the first angle and the last angle is approximately equal to pi, then the last angle will be removed
	// in order to prevent a line to be detected twice.
	if (num_angle > 1 && std::fabs(std::numbers::pi - num_angle * theta_step) < theta_step / 2)
	{
		--num_angle;
	}

	const int half = static_cast<int>(std::ceil(max_rho / rho));
	const int num_rho = 2 * half + 1;                  // middle bin = rho 0

	if (num_angle <= 0 || num_rho <= 0)
	{
		std::cerr << "HoughLines: invalid sizes, num_angle = " << num_angle
		          << ", num_rho = " << num_rho << '\n';
		return lines;
	}
	
	// trig tables, pre-scaled by 1/rho
	std::vector<float> sin_table(num_angle), cos_table(num_angle);
	for (int n = 0 ; n < num_angle; n++)
	{
		const float th = min_theta + n * theta_step;
		sin_table[n] = std::sin(th) / rho;
		cos_table[n] = std::cos(th) / rho;
	}

	// Discrete accumulator array
	qlm::Image<qlm::ImageFormat::GRAY, int32_t> acc; // x -> theta r, y -> r
	acc.Create(num_angle, num_rho, 0);

	// voting
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// check if exists
			const qlm::Pixel c = in.GetPixel(x, y);
			if (c.v != 0)
			{
				// loop over all angles
				for (int t = 0; t < num_angle; t++)
				{
					// calculate r
					const float r = x * cos_table[t] + y * sin_table[t];
					const int idx_r = std::round(r) + half;
					
					auto val = acc.GetPixel(t, idx_r);
					acc.SetPixel(t, idx_r, val.v + 1);
				}
			}
		}
	}

	struct Peak { int votes; int t; int r; };
	std::vector<Peak> peaks;
	
	// find lines
	for (int r = 0; r < acc.Height(); r++)
	{
		for (int t = 0; t < acc.Width(); t++)
		{
			const int c_pix = acc.GetPixel(t, r).v;

			// neighboring elements
			const int l_pix = acc.GetPixel(t - 1, r).v;
			const int r_pix = acc.GetPixel(t + 1, r).v;
			const int t_pix = acc.GetPixel(t, r - 1).v;
			const int b_pix = acc.GetPixel(t, r + 1).v;

			if (c_pix > threshold && 
				c_pix > l_pix && c_pix >= r_pix &&
				c_pix > t_pix && c_pix >= b_pix)
			{
				peaks.push_back({c_pix, t, r});
			}
			
		}
	}

	// sort by votes (descending), then theta, then rho 
	std::sort(peaks.begin(), peaks.end(), [](const Peak& a, const Peak& b) {
		if (a.votes != b.votes) return a.votes > b.votes;
		if (a.t != b.t)         return a.t < b.t;
		return a.r < b.r;
	});

	lines.reserve(peaks.size());
	for (const auto& p : peaks)
	{
		const float d     = (p.r - half) * rho;
		const float theta = static_cast<float>(min_theta + p.t * theta_step);

		lines.push_back({ d, theta });
	}
	
	return lines;
}

// Explicit instantiation for GRAY , uint8_t
template std::vector<qlm::LinePolar>
qlm::HoughLines<uint8_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
						 const float, const float, const int, const double, const double);
