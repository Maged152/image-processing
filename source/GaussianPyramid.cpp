#include "GaussianPyramid.hpp"
#include "Gaussian.hpp"
#include "Scale.hpp"

namespace qlm
{
    template <ImageFormat frmt, pixel_t T>
    Pyramid<frmt, T> GaussianPyramid(const Image<frmt, T> &in, const size_t num_layers, const float scale, const size_t filter_size, const float sigma, const BorderMode<frmt, T> &border_mode)
    {
        Pyramid<frmt, T> pyr {num_layers, scale, filter_size, sigma};
        pyr.layers[0] = in;

        for (int layer = 1; layer < num_layers; layer++)
        {
            const auto gaussian_image = Gaussian(pyr.layers[layer - 1], filter_size, sigma, sigma, border_mode);
            pyr.layers[layer] = Scale(gaussian_image, InterpolationFlag::NEAREST_NEIGHBOR, scale, scale, border_mode);
        }

        return pyr;
    }

    template Pyramid<ImageFormat::RGB, uint8_t>
    GaussianPyramid<ImageFormat::RGB, uint8_t>(const Image<ImageFormat::RGB, uint8_t>&,
        const size_t,
        const float,
        const size_t,
        const float,
        const BorderMode<ImageFormat::RGB, uint8_t>&);

    template Pyramid<ImageFormat::GRAY, uint8_t>
    GaussianPyramid<ImageFormat::GRAY, uint8_t>(const Image<ImageFormat::GRAY, uint8_t>&,
        const size_t,
        const float,
        const size_t,
        const float,
        const BorderMode<ImageFormat::GRAY, uint8_t>&);
}