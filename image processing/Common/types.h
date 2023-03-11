#pragma once

namespace qlm
{
	enum class ScaleMethod
	{
		NEAREST_NEIGHBOR,
		BILINEAR,
		BICUBIC
	};

	struct Line
	{
		int x0;
		int y0;
		int x1;
		int y1;
	};
	// line in the polar form
	struct LinePolar
	{
		float radius;
		float angle;
	};
}