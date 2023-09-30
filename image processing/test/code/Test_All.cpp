#include "../test/test.h"

namespace test
{
	bool Test_All()
	{
		bool res = true;

		// filters
		res &= Test_Filter2D();

		return res;
	}
}