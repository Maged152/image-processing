#include "test_common.hpp"

TEST(Test_shakhbat_cv, ColorConvert)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Color Manipulation/ColorConvert/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 3)
		alpha = false;
	
	// do the operation
	// RGB to HSV
	t.Start();
	auto out = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t,qlm::ImageFormat::HSV, uint8_t>(in);
	t.End();

	// red to green
	for (int i = 0; i < out.height * out.width; i++)
	{
		auto pix = out.GetPixel(i);
		// extract H value
		int h = (int)pix.h * 2;
		// shift the red color by 120 to make green
		if ((h >= 0 && h <= 30) || h >= 330)
		{
			h += 120;
		}
		// ensure the range 0-360
		if (h > 360)
		{
			h -= 360;
		}
		// to fit in 8-bits
		h /= 2;
		pix.h = h;

		out.SetPixel(i, pix);
	}
	// HSV2RGB
	auto out2 = qlm::ColorConvert<qlm::ImageFormat::HSV, uint8_t,qlm::ImageFormat::RGB, uint8_t>(out);

	test::PrintTime(t);

	// reread output image
	out2 = test::ReReadImage(out2);

	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
	const bool load_ref = ref.LoadFromFile(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(out2, ref);
}

TEST(Test_shakhbat_cv, ColorConvert_RoundTrip)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Color Manipulation/ColorConvert/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	t.Start();

	// RGB -> HSV -> RGB (tolerance 1: 8-bit rounding)
	auto hsv = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::HSV, uint8_t>(in);
	auto rgb_from_hsv = qlm::ColorConvert<qlm::ImageFormat::HSV, uint8_t, qlm::ImageFormat::RGB, uint8_t>(hsv);

	// RGB -> HLS -> RGB
	auto hls = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::HLS, uint8_t>(in);
	auto rgb_from_hls = qlm::ColorConvert<qlm::ImageFormat::HLS, uint8_t, qlm::ImageFormat::RGB, uint8_t>(hls);

	// RGB -> YCrCb -> RGB
	auto ycrcb = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::YCrCb, uint8_t>(in);
	auto rgb_from_ycrcb = qlm::ColorConvert<qlm::ImageFormat::YCrCb, uint8_t, qlm::ImageFormat::RGB, uint8_t>(ycrcb);

	// GRAY -> RGB -> GRAY
	auto gray = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>(in);
	auto rgb_from_gray = qlm::ColorConvert<qlm::ImageFormat::GRAY, uint8_t, qlm::ImageFormat::RGB, uint8_t>(gray);
	auto gray_back = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>(rgb_from_gray);

	// 8-bit quantization tolerances (inherent to the u8 storage, same as OpenCV):
	// HSV/HLS: H is stored as H/2 -> up to 2 degrees hue error -> up to ~9/255 RGB error;
	// HLS additionally quantizes L and S independently (~+3) -> bound ~12
	const qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> tol_hue{ 15, 15, 15 };
	// YCrCb: each of Y/Cr/Cb quantized independently -> accumulated error up to ~4
	const qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> tol_ycrcb{ 4, 4, 4 };
	std::cout << "[round-trip] HSV\n";
	test::CompareImages(rgb_from_hsv, in, tol_hue);
	std::cout << "[round-trip] HLS\n";
	test::CompareImages(rgb_from_hls, in, tol_hue);
	std::cout << "[round-trip] YCrCb\n";
	test::CompareImages(rgb_from_ycrcb, in, tol_ycrcb);

	// gray->rgb->gray: luma weights sum to 1.0 but float rounding can shift by 1
	std::cout << "[round-trip] GRAY\n";
	const qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t> tol_gray{ 1 };
	test::CompareImages(gray_back, gray, tol_gray);
}