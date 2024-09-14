#include "Equalization.hpp"
#include "Histogram.hpp"

namespace qlm
{
    template <ImageFormat frmt, pixel_t T>
    Image<frmt, T> Equalization(const Image<frmt, T> &in, const EqualizationFlag flag)
    {
        // output image
		Image<frmt, T> out {in.width, in.height};

        if (flag == EqualizationFlag::HISTOGRAM)
        {
            // calculate histogram
            const Histogram_t<frmt, T> histogram = Histogram(in);
            
            // calculate cumulative sum
            Histogram_t<frmt, T> cumsum;

            for(int c = 0; c < histogram.num_channels; c++)
            {
                cumsum.hist[c][0] = histogram.hist[c][0];

                for (int b = 1; b < histogram.tot_elements; b++)
                {
                    cumsum.hist[c][b] = cumsum.hist[c][b - 1] + histogram.hist[c][b];
                }
            }

            constexpr T max_val = std::numeric_limits<T>::max();
            const int tot_pixels = in.width * in.height;

            // transfer the image
            for (int y = 0; y < in.height; y++)
            {
                for (int x = 0; x < in.width; x++)
                {
                    const auto in_pix = in.GetPixel(x, y);
                    const auto out_pix =  (max_val * ( cumsum.hist[0][in_pix.v] - cumsum.hist[0][0])) / (tot_pixels - cumsum.hist[0][0]);

                    out.SetPixel(x, y, out_pix);
                }
            }
        }


        return out;
    }

    template Image<ImageFormat::GRAY, uint8_t> Equalization (const Image<ImageFormat::GRAY, uint8_t>&, const EqualizationFlag);
}