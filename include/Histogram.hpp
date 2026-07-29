#pragma once

#include "common/types.hpp"
#include <algorithm>

namespace qlm
{
	template<ImageFormat frmt, typename T>
	struct Histogram_t 
	{
		static constexpr int num_channels = (frmt == ImageFormat::GRAY) ? 1 : 3;
		// Dynamically calculate the number of elements based on T
    	static constexpr int tot_elements = static_cast<int>(std::numeric_limits<T>::max() - std::numeric_limits<T>::lowest()) + 1;

		// Histogram data: dynamically allocated based on channel count
		std::array<std::vector<int>, num_channels> hist;

		// Constructor to initialize the histogram arrays
		Histogram_t()
		{
			for (auto& channel_hist : hist) 
			{
				channel_hist.resize(tot_elements, 0);  // Allocate and initialize to zero
			}
		}

		// Get the cumulative sum of the histogram
		Histogram_t<frmt, T> CumulativeHistogram() const
		{
			Histogram_t<frmt, T> cumsum;

			for (int c = 0; c < num_channels; ++c)
			{
				cumsum.hist[c][0] = hist[c][0];
				for (int i = 1; i < tot_elements; ++i)
				{
					cumsum.hist[c][i] = cumsum.hist[c][i - 1] + hist[c][i];
				}
			}

			return cumsum;
		}

		 // Reset the histogram data
		void Reset()
		{
			for (auto& channel_hist : hist)
			{
				std::fill(channel_hist.begin(), channel_hist.end(), 0);
			}
		}
	};

	template<ImageFormat frmt, pixel_t T>
	Histogram_t<frmt, T> Histogram(
		const Image<frmt, T>& in);

}