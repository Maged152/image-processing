#include "test_cv.h"

namespace test
{
	bool Test_All()
	{
		bool res = true;

		// filters
		res &= Test_Filter2D();
		res &= Test_SepFilter2D();
		res &= Test_Gaussian();
		res &= Test_Sobel();
		res &= Test_BoxFilter();

		// feature detection
		res &= Test_HoughLines();
		res &= Test_HarrisCorner();
		res &= Test_FAST();
		res &= Test_KMeans();
		res &= Test_MatchTemplate();

		// color manipulation
		res &= Test_ColorConvert();
		res &= Test_FloodFill();
		res &= Test_ConvertBitDepth();

		// geometric transformations
		res &= Test_Flip();
		res &= Test_Resize();
		res &= Test_Rotate();
		res &= Test_Translate();
		res &= Test_Transpose();
		res &= Test_WarpAffine();
		res &= Test_Scale();
		res &= Test_WarpPerspective();

		return res;
	}
}