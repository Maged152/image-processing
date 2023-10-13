#include "../test.h"

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

		// feature detection
		res &= Test_HoughLines();
		res &= Test_HarrisCorner();

		// color manipulation
		res &= Test_ColorConvert();
		res &= Test_FloodFill();
		res &= Test_ConvertBitDepth();

		// geometric transformations
		res &= Test_Flip();
		res &= Test_Resize();
		res &= Test_Rotate();

		return res;
	}
}