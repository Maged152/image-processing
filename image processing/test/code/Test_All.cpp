#include "../test/test.h"

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

		return res;
	}
}