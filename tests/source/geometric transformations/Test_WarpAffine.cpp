#include "test_common.hpp"

TEST(Test_shakhbat_cv, WarpAffine)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/WarpAffine/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumberOfChannels() == 1)
		alpha = false;

	float scale = 0.5f;
	float angle = 30.0f;
	const int dst_width = in.Width();
	const int dst_height = in.Height();

	const qlm::Point<int> center{ (int)in.Width() / 2, (int)in.Height() / 2 };

	qlm::AffineMatrix mat = qlm::GetRotationMatrix(center, angle, scale);

	auto border_mode = qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{};
	border_mode.border_type = qlm::BorderType::BORDER_REFLECT;

	// do the operation
	t.Start();
	auto out = qlm::WarpAffine(in, mat, dst_width, dst_height, qlm::InterpolationFlag::BILINEAR, border_mode);
	t.End();

	test::PrintTime(t);

	// reread output image
	out = test::ReReadImage(out);

	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
	const bool load_ref = ref.Read(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(out, ref);
}

TEST(Test_shakhbat_cv, WarpAffine_Identity)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/WarpAffine/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// identity transform: the source points are mapped onto themselves
	const qlm::Point<int> src[3] =
	{
		{0, 0}, {in.Width() - 1, 0}, {0, in.Height() - 1}
	};
	const qlm::Point<int> dst[3] =
	{
		{0, 0}, {in.Width() - 1, 0}, {0, in.Height() - 1}
	};

	qlm::AffineMatrix mat = qlm::GetAffineTransform(src, dst);

	auto border_mode = qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{};

	// do the operation
	t.Start();
	auto out = qlm::WarpAffine(in, mat, in.Width(), in.Height(), qlm::InterpolationFlag::BILINEAR, border_mode);
	t.End();

	test::PrintTime(t);

	// the output must be identical to the input
	test::CompareImages(out, in);
}
TEST(Test_shakhbat_cv, GetAffineTransform_Shear)
{
	// known shear: (x, y) -> (x + y, y)
	const qlm::Point<int> src[3] =
	{
		{0, 0}, {10, 0}, {0, 10}
	};
	const qlm::Point<int> dst[3] =
	{
		{0, 0}, {10, 0}, {10, 10}
	};

	qlm::AffineMatrix mat = qlm::GetAffineTransform(src, dst);

	// first row: m00 m01 m02
	EXPECT_NEAR(mat.GetElement(0, 0), 1.0f, 1e-3f);
	EXPECT_NEAR(mat.GetElement(0, 1), 1.0f, 1e-3f);
	EXPECT_NEAR(mat.GetElement(0, 2), 0.0f, 1e-3f);
	// second row: m10 m11 m12
	EXPECT_NEAR(mat.GetElement(1, 0), 0.0f, 1e-3f);
	EXPECT_NEAR(mat.GetElement(1, 1), 1.0f, 1e-3f);
	EXPECT_NEAR(mat.GetElement(1, 2), 0.0f, 1e-3f);
}

// Direct tests for the Gaussian-elimination kernel qlm::Solve (common helper
// used by GetAffineTransform/GetHomography; it had a dimension-check bug).
TEST(Test_shakhbat_cv, Solve_WellConditioned)
{
	// 3x3 system A*x = b with solution x = (1, 2, 3)
	// A = [[2,1,1],[1,3,2],[1,0,0]], b = (7, 13, 1)
	qlm::Matrix aug(3, 4);
	const float aug_data[12] =
	{
		2.0f, 1.0f, 1.0f, 7.0f,
		1.0f, 3.0f, 2.0f, 13.0f,
		1.0f, 0.0f, 0.0f, 1.0f
	};
	for (int i = 0; i < 12; i++)
	{
		aug.Set(i, aug_data[i]);
	}

	qlm::Matrix sol(3, 1);
	qlm::Solve(aug, sol);

	EXPECT_NEAR(sol.Get(0, 0), 1.0f, 1e-4f);
	EXPECT_NEAR(sol.Get(1, 0), 2.0f, 1e-4f);
	EXPECT_NEAR(sol.Get(2, 0), 3.0f, 1e-4f);
}

TEST(Test_shakhbat_cv, Solve_NeedsRowSwap)
{
	// the first row cannot be the pivot row; Solve must swap rows internally
	// A = [[0,1],[1,0]], x = (2, 3), b = (3, 2)
	qlm::Matrix aug(2, 3);
	const float aug_data[6] =
	{
		0.0f, 1.0f, 3.0f,
		1.0f, 0.0f, 2.0f
	};
	for (int i = 0; i < 6; i++)
	{
		aug.Set(i, aug_data[i]);
	}

	qlm::Matrix sol(2, 1);
	qlm::Solve(aug, sol);

	EXPECT_NEAR(sol.Get(0, 0), 2.0f, 1e-4f);
	EXPECT_NEAR(sol.Get(1, 0), 3.0f, 1e-4f);
}

TEST(Test_shakhbat_cv, Solve_BadDimensions)
{
	// an augmented matrix must be rows x (rows + 1); sol must be rows x 1.
	// Solve must reject the input and leave the solution untouched.
	qlm::Matrix aug(3, 3);  // wrong: columns != rows + 1
	const float aug_data[9] =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
	for (int i = 0; i < 9; i++)
	{
		aug.Set(i, aug_data[i]);
	}

	const float sentinel = 42.0f;
	qlm::Matrix sol(3, 1);
	sol.Set(0, 0, sentinel);
	sol.Set(1, 0, sentinel);
	sol.Set(2, 0, sentinel);

	qlm::Solve(aug, sol);

	EXPECT_EQ(sol.Get(0, 0), sentinel);
	EXPECT_EQ(sol.Get(1, 0), sentinel);
	EXPECT_EQ(sol.Get(2, 0), sentinel);
}
