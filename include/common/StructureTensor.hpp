#pragma once

#include "common/types.hpp"

namespace qlm
{
	// Computes the data needed by corner detectors (Harris / Shi-Tomasi / Lucas-Kanade):
	// Sobel gradients I_x, I_y and the box-filtered structure tensor components
	// S_xx = sum(I_x^2), S_yy = sum(I_y^2), S_xy = sum(I_x * I_y)
	// The outputs are full-size images. The sums inside the requested roi are
	// exact: the gradients/products are computed over the roi extended by half
	// the block size (halo), so a box-filter window centered in the roi never
	// reads uninitialized pixels.
	void StructureTensor(
		const Image<ImageFormat::GRAY, uint8_t>& in,
		const int gradient_size,
		const int block_x_size,
		const int block_y_size,
				Image<ImageFormat::GRAY, int16_t>& I_x,
				Image<ImageFormat::GRAY, int16_t>& I_y,
				Image<ImageFormat::GRAY, float>& S_xx,
				Image<ImageFormat::GRAY, float>& S_yy,
				Image<ImageFormat::GRAY, float>& S_xy,
		const BorderMode<ImageFormat::GRAY, uint8_t>& border_mode = BorderMode<ImageFormat::GRAY, uint8_t>{},
		const BorderMode<ImageFormat::GRAY, float>& box_border_mode = BorderMode<ImageFormat::GRAY, float>{},
		const Rectangle<int>& roi = Rectangle<int>{});

	// Minimum eigen value of the 2x2 structure tensor [[ixx, ixy], [ixy, iyy]].
	// This is the Shi-Tomasi corner response
	float MinEigenValue(const float S_xx, const float S_xy, const float S_yy, const float normalization = 1.0f);

	// Harris corner response of the 2x2 structure tensor [[ixx, ixy], [ixy, iyy]].
	// R = (det - k * trace^2) * normalization
	float HarrisResponse(const float S_xx, const float S_yy, const float S_xy, const float k, const float normalization = 1.0f);

	// Computes the per-pixel corner response from the structure tensor sums
	float CornerResponse(
		const float S_xx,
		const float S_yy,
		const float S_xy,
		const float k,
		const float normalization = 1.0f,
		const bool is_harris = true);

	Image<ImageFormat::GRAY, float> CornerResponse(
		const Image<ImageFormat::GRAY, float>& S_xx,
		const Image<ImageFormat::GRAY, float>& S_yy,
		const Image<ImageFormat::GRAY, float>& S_xy,
		const float k,
		const float normalization = 1.0f,
		const bool is_harris = true,
		const Rectangle<int>& roi = Rectangle<int>{});

} // namespace qlm