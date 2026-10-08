#include "test_common.hpp"

TEST(Test_shakhbat_cv, MatchTemplate)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Features Detection/MatchTemplate/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumberOfChannels() == 1)
		alpha = false;

	// load the template image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> templ;
	const bool load_temp = templ.Read(folder_path + "template.jpg");
	EXPECT_EQ(load_temp, true);

	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> mask{};
	mask.Create(templ.Width(), templ.Height(), qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t> {1});

	t.Start();
	auto out = qlm::MatchTemplate(in, templ, qlm::TemplateMatchFlag::SQDIFF, mask);
	t.End();


	// draw clusters
	auto [min_loc, max_loc] = qlm::MinMaxLoc(out);

	qlm::Pixel <qlm::ImageFormat::RGB, uint8_t> green{ 0, 255, 0 };
	qlm::Rectangle<int> rec{ {0, 0}, templ.Width(), templ.Height() };

	for (auto& p : min_loc)
	{
		rec.top_left = p;
		in = qlm::DrawRectangle(in, rec, green);
	}

	test::PrintTime(t);

	// reread output image
	in = test::ReReadImage(in);
	
	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
	const bool load_ref = ref.Read(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(in, ref);
}

TEST(Test_shakhbat_cv, MatchTemplate_SelfMatch)
{
	// template cut out of the input itself: SQDIFF must be 0 at the cut location
	const std::string folder_path = test::example_folder + "Features Detection/MatchTemplate/";

	qlm::Image<qlm::ImageFormat::RGB, uint8_t> rgb;
	ASSERT_TRUE(rgb.Read(folder_path + "input.jpg"));
	const auto in = qlm::ColorConvert<qlm::ImageFormat::GRAY>(rgb);

	const int tw = 32, th = 32, tx = 10, ty = 10;
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> templ(tw, th);
	for (int y = 0; y < th; y++)
		for (int x = 0; x < tw; x++)
			templ.SetPixel(x, y, in.GetPixel(tx + x, ty + y));

	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> mask(tw, th);
	for (int i = 0; i < tw * th; i++)
		mask.SetPixel(i, qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>(1));

	auto out = qlm::MatchTemplate(in, templ, qlm::TemplateMatchFlag::SQDIFF, mask);
	EXPECT_EQ(out.GetPixel(tx, ty).v, 0.0f);

	const auto [min_loc, max_loc] = qlm::MinMaxLoc(out);
	EXPECT_FALSE(min_loc.empty());
	if (!min_loc.empty())
	{
		EXPECT_EQ(min_loc[0].x, tx);
		EXPECT_EQ(min_loc[0].y, ty);
	}
}
