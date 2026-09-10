#include "test_common.hpp"

TEST(Test_shakhbat_cv, WarpPerspective)
{
	qlm::Timer<qlm::usec> t {};
	const std::string folder_path = test::example_folder + "Geometric Transformations/WarpPerspective/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 1)
		alpha = false;
	const int dst_width = 254;
	const int dst_height = 356;

	const qlm::Point<int> src[4] =
	{
		{224, 93}, {430, 137}, {164, 378}, {370, 424}
	};

	const qlm::Point<int> dst[4] =
	{
		{0, 0}, {dst_width, 0}, {0, dst_height}, {dst_width, dst_height}
	};

	qlm::PerspectiveMatrix mat = qlm::GetPerspectiveTransform(src, dst);

	auto border_mode = qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{};
	border_mode.border_type = qlm::BorderType::BORDER_REFLECT;

	// do the operation
	t.Start();
	auto out = qlm::WarpPerspective(in, mat, dst_width, dst_height, qlm::InterpolationFlag::BILINEAR, border_mode);
	t.End();

	test::PrintTime(t);

	// reread output image
	out = test::ReReadImage(out);

	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
	const bool load_ref = ref.LoadFromFile(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(out, ref);
}

TEST(Test_shakhbat_cv, WarpPerspective_Identity)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/WarpPerspective/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// identity transform: the source points are mapped onto themselves
	const qlm::Point<int> src[4] =
	{
		{0, 0}, {in.width - 1, 0}, {0, in.height - 1}, {in.width - 1, in.height - 1}
	};
	const qlm::Point<int> dst[4] =
	{
		{0, 0}, {in.width - 1, 0}, {0, in.height - 1}, {in.width - 1, in.height - 1}
	};

	qlm::PerspectiveMatrix mat = qlm::GetPerspectiveTransform(src, dst);

	auto border_mode = qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{};

	// do the operation
	t.Start();
	auto out = qlm::WarpPerspective(in, mat, in.width, in.height, qlm::InterpolationFlag::BILINEAR, border_mode);
	t.End();

	test::PrintTime(t);

	// the output must be identical to the input
	test::CompareImages(out, in);
}