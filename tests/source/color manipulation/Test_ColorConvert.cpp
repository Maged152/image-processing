#include "test_common.hpp"

TEST(Test_shakhbat_cv, ColorConvert)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Color Manipulation/ColorConvert/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumberOfChannels() == 3)
		alpha = false;
	
	// do the operation
	// RGB to HSV
	t.Start();
	auto out = qlm::ColorConvert<qlm::ImageFormat::HSV>(in);
	t.End();

	// red to green
	for (int i = 0; i < out.Height() * out.Width(); i++)
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
	auto out2 = qlm::ColorConvert<qlm::ImageFormat::RGB>(out);

	test::PrintTime(t);

	// reread output image
	out2 = test::ReReadImage(out2);

	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
	const bool load_ref = ref.Read(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(out2, ref);
}

TEST(Test_shakhbat_cv, ColorConvert_RoundTrip)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Color Manipulation/ColorConvert/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	in.RandomInit(0);
	

	t.Start();

	// RGB -> HSV -> RGB (8-bit quantization: see tolerances below)
	auto hsv = qlm::ColorConvert<qlm::ImageFormat::HSV>(in);
	auto rgb_from_hsv = qlm::ColorConvert<qlm::ImageFormat::RGB>(hsv);

	// RGB -> HLS -> RGB
	auto hls = qlm::ColorConvert<qlm::ImageFormat::HLS>(in);
	auto rgb_from_hls = qlm::ColorConvert<qlm::ImageFormat::RGB>(hls);

	// RGB -> YCrCb -> RGB
	auto ycrcb = qlm::ColorConvert<qlm::ImageFormat::YCrCb>(in);
	auto rgb_from_ycrcb = qlm::ColorConvert<qlm::ImageFormat::RGB>(ycrcb);

	// GRAY -> RGB -> GRAY
	auto gray = qlm::ColorConvert<qlm::ImageFormat::GRAY>(in);
	auto rgb_from_gray = qlm::ColorConvert<qlm::ImageFormat::RGB>(gray);
	auto gray_back = qlm::ColorConvert<qlm::ImageFormat::GRAY>(rgb_from_gray);

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

	// gray->rgb->gray: GRAY->RGB is an exact copy, RGB->GRAY applies the luma weights
	// (sum 1.0, but float rounding can shift by 1)
	std::cout << "[round-trip] GRAY\n";
	const qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t> tol_gray{ 1 };
	test::CompareImages(gray_back, gray, tol_gray);
}

// Direct known-value validation of each color space conversion with 1x1 probes.
// Expected values below are derived from the implementation formulas
// (truncating cast on 8-bit output; hue stored as H/2).
TEST(Test_shakhbat_cv, ColorConvert_KnownValues)
{
	using RGB8 = qlm::Pixel<qlm::ImageFormat::RGB, uint8_t>;

	// ---------- pure red (255, 0, 0) ----------
	{
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> in(1, 1);
		in.SetPixel(0, RGB8(255, 0, 0));

		auto hsv = qlm::ColorConvert<qlm::ImageFormat::HSV>(in).GetPixel(0);
		EXPECT_EQ(hsv.h, 0);
		EXPECT_EQ(hsv.s, 255);
		EXPECT_EQ(hsv.v, 255);

		auto gray = qlm::ColorConvert<qlm::ImageFormat::GRAY>(in).GetPixel(0);
		EXPECT_EQ(gray.v, 76);
	}

	// ---------- pure green (0, 255, 0) ----------
	{
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> in(1, 1);
		in.SetPixel(0, RGB8(0, 255, 0));

		auto hsv = qlm::ColorConvert<qlm::ImageFormat::HSV>(in).GetPixel(0);
		EXPECT_EQ(hsv.h, 60);
		EXPECT_EQ(hsv.s, 255);
		EXPECT_EQ(hsv.v, 255);

		auto gray = qlm::ColorConvert<qlm::ImageFormat::GRAY>(in).GetPixel(0);
		EXPECT_EQ(gray.v, 149);
	}

	// ---------- pure blue (0, 0, 255) ----------
	{
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> in(1, 1);
		in.SetPixel(0, RGB8(0, 0, 255));

		auto hsv = qlm::ColorConvert<qlm::ImageFormat::HSV>(in).GetPixel(0);
		EXPECT_EQ(hsv.h, 120);
		EXPECT_EQ(hsv.s, 255);
		EXPECT_EQ(hsv.v, 255);

		auto gray = qlm::ColorConvert<qlm::ImageFormat::GRAY>(in).GetPixel(0);
		EXPECT_EQ(gray.v, 29);
	}

	// ---------- white (255, 255, 255) ----------
	{
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> in(1, 1);
		in.SetPixel(0, RGB8(255, 255, 255));

		auto hsv = qlm::ColorConvert<qlm::ImageFormat::HSV>(in).GetPixel(0);
		EXPECT_EQ(hsv.h, 0);
		EXPECT_EQ(hsv.s, 0);
		EXPECT_EQ(hsv.v, 255);

		auto hls = qlm::ColorConvert<qlm::ImageFormat::HLS>(in).GetPixel(0);
		EXPECT_EQ(hls.h, 0);
		EXPECT_EQ(hls.l, 255);
		EXPECT_EQ(hls.s, 0);

		auto gray = qlm::ColorConvert<qlm::ImageFormat::GRAY>(in).GetPixel(0);
		EXPECT_EQ(gray.v, 255);
	}

	// ---------- black (0, 0, 0) ----------
	{
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> in(1, 1);
		in.SetPixel(0, RGB8(0, 0, 0));

		auto hsv = qlm::ColorConvert<qlm::ImageFormat::HSV>(in).GetPixel(0);
		EXPECT_EQ(hsv.h, 0);
		EXPECT_EQ(hsv.s, 0);
		EXPECT_EQ(hsv.v, 0);

		auto hls = qlm::ColorConvert<qlm::ImageFormat::HLS>(in).GetPixel(0);
		EXPECT_EQ(hls.h, 0);
		EXPECT_EQ(hls.l, 0);
		EXPECT_EQ(hls.s, 0);

		// (0, delta, delta) encodes black exactly: YCrCb->RGB of it is exact too
		auto ycc = qlm::ColorConvert<qlm::ImageFormat::YCrCb>(in).GetPixel(0);
		EXPECT_EQ(ycc.y, 0);
		EXPECT_EQ(ycc.cr, 128);
		EXPECT_EQ(ycc.cb, 128);

		auto ycc_img = qlm::ColorConvert<qlm::ImageFormat::YCrCb>(in);
		auto rgb = qlm::ColorConvert<qlm::ImageFormat::RGB>(ycc_img).GetPixel(0);
		EXPECT_EQ(rgb.r, 0);
		EXPECT_EQ(rgb.g, 0);
		EXPECT_EQ(rgb.b, 0);

		auto gray = qlm::ColorConvert<qlm::ImageFormat::GRAY>(in).GetPixel(0);
		EXPECT_EQ(gray.v, 0);
	}

	// ---------- medium gray (128, 128, 128) ----------
	{
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> in(1, 1);
		in.SetPixel(0, RGB8(128, 128, 128));

		auto hsv = qlm::ColorConvert<qlm::ImageFormat::HSV>(in).GetPixel(0);
		EXPECT_EQ(hsv.h, 0);
		EXPECT_EQ(hsv.s, 0);
		EXPECT_TRUE(hsv.v >= 127 && hsv.v <= 128);

		auto hls = qlm::ColorConvert<qlm::ImageFormat::HLS>(in).GetPixel(0);
		EXPECT_EQ(hls.h, 0);
		EXPECT_EQ(hls.s, 0);
		EXPECT_TRUE(hls.l >= 127 && hls.l <= 128);

		// (128-delta,...) float rounding can land one below 128 for Y/Cr/Cb
		auto ycc = qlm::ColorConvert<qlm::ImageFormat::YCrCb>(in).GetPixel(0);
		EXPECT_TRUE(ycc.y >= 127 && ycc.y <= 129);
		EXPECT_TRUE(ycc.cr >= 127 && ycc.cr <= 129);
		EXPECT_TRUE(ycc.cb >= 127 && ycc.cb <= 129);

		auto gray = qlm::ColorConvert<qlm::ImageFormat::GRAY>(in).GetPixel(0);
		EXPECT_TRUE(gray.v >= 127 && gray.v <= 128);
	}

	// ---------- pure red in HLS (L rounding is implementation-specific) ----------
	{
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> in(1, 1);
		in.SetPixel(0, RGB8(255, 0, 0));

		auto hls = qlm::ColorConvert<qlm::ImageFormat::HLS>(in).GetPixel(0);
		EXPECT_EQ(hls.h, 0);
		EXPECT_EQ(hls.s, 255);
		EXPECT_TRUE(hls.l >= 127 && hls.l <= 128);
	}
}