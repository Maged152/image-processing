#pragma once

namespace qlm
{
	enum class RotateFlag
	{
		ROTATE_90_CLOCKWISE,
		ROTATE_180,
		ROTATE_90_COUNTERCLOCKWISE,
	};

	enum class FlipFlag
	{
		FLIP_VERTICAL_AXIS,
		FLIP_HORIZONTAL_AXIS,
		FLIP_VERTICAL_HORIZONTAL_AXIS,
	};

	enum class KMeansInit
	{
		RANDOM,
		KMEANS_PLUS_PLUS
	};

	enum class TemplateMatchFlag
	{
		SQDIFF,
		SQDIFF_NORMED,
		CCORR,
		CCORR_NORMED
		//CCOEFF,
		//CCOEFF_NORMED
	};

	enum class OrderFlag
	{
		WIDTH_FIRST,
		HEIGHT_FIRST
	};

	enum class EnergyFlag
	{
		FORWARD,
		BACKWARD
	};

    enum class EqualizationFlag
	{
		HISTOGRAM,
		ADAPTIVE_HISTOGRAM
	};
}