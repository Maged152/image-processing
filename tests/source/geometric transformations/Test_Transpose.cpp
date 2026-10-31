#include "test_common.hpp"

TEST(Test_shakhbat_cv, Transpose)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/Transpose/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumberOfChannels() == 1)
		alpha = false;


	// do the operation
	t.Start();
	auto out = qlm::Transpose(in);
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

// The in-place overload (plus the round-trip property T(T(x)) == x) is never
// exercised by the reference-based test above.
TEST(Test_shakhbat_cv, Transpose_InPlaceAndRoundTrip)
{
	const std::string folder_path = test::example_folder + "Geometric Transformations/Transpose/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// allocating result (note the swapped dimensions)
	auto ref_out = qlm::Transpose(in);
	EXPECT_EQ(ref_out.Width(), in.Height());
	EXPECT_EQ(ref_out.Height(), in.Width());

	// in-place on a swapped-dimension buffer with different content
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> out(ref_out.Width(), ref_out.Height());
	qlm::Transpose(in, out);

	test::CompareImages(out, ref_out);

	// round-trip: transposing the result again restores the original image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> back(in.Width(), in.Height());
	qlm::Transpose(out, back);

	test::CompareImages(back, in);
}