#pragma once
#include "common/types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<ImageFormat::GRAY, float> MatchTemplate(
		const Image<frmt, T>& in,
		const Image<frmt, T>& templ,
		const TemplateMatchFlag mode,
		const Image<ImageFormat::GRAY, uint8_t>& mask);
}