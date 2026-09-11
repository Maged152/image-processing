#include "EqualizeHist.hpp"
#include "Histogram.hpp"

namespace qlm
{
    template <ImageFormat frmt, pixel_t T>
    Image<frmt, T> EqualizeHist(const Image<frmt, T> &in)
    {
        // calculate histogram
        const Histogram_t<frmt, T> histogram = Histogram(in);
        
        // calculate cumulative sum
        Histogram_t<frmt, T> cumsum = histogram.CumulativeHistogram();

        constexpr T max_val = std::numeric_limits<T>::max();
        const int tot_pixels = in.width * in.height;

        // constant image: denominator (tot_pixels - cumsum[0]) is zero -> return a copy
        if (tot_pixels == cumsum.hist[0][0])
        {
            return in;
        }

        // output image
		Image<frmt, T> out {in.width, in.height};

        // transfer the image
        for (int y = 0; y < in.height; y++)
        {
            for (int x = 0; x < in.width; x++)
            {
                const auto in_pix = in.GetPixel(x, y);
                const auto out_pix = static_cast<T>((static_cast<long long>(max_val) * (cumsum.hist[0][in_pix.v] - cumsum.hist[0][0])) / (tot_pixels - cumsum.hist[0][0]));

                out.SetPixel(x, y, out_pix);
            }
        }

        return out;
    }

    template Image<ImageFormat::GRAY, uint8_t> EqualizeHist (const Image<ImageFormat::GRAY, uint8_t>&);
}