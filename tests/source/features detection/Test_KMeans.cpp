#include "test_common.hpp"

TEST(Test_shakhbat_cv, KMeans)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Features Detection/KMeans/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumberOfChannels() == 1)
		alpha = false;

	const int k = 4;
	const qlm::KMeansInit init = qlm::KMeansInit::KMEANS_PLUS_PLUS;

	// do the operation
	t.Start();
	auto clusters = qlm::KMeans(in, k, init);
	t.End();


	// draw clusters
	auto out = qlm::DrawCluster(in, clusters);

	test::PrintTime(t);

	// reread output image
	out = test::ReReadImage(out);
	
	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
	const bool load_ref = ref.Read(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(out, ref);
}
TEST(Test_shakhbat_cv, KMeans_Synthetic)
{
	// 10x10 image: left half black, right half white -> exact centers {0,0,0} and {255,255,255}
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> in(10, 10);
		for (int y = 0; y < 10; y++)
			for (int x = 0; x < 10; x++)
				 in.SetPixel(x, y, x < 5 ? qlm::Pixel<qlm::ImageFormat::RGB, uint8_t>(0,0,0) : qlm::Pixel<qlm::ImageFormat::RGB, uint8_t>(255,255,255));
		for (const auto init : {qlm::KMeansInit::RANDOM, qlm::KMeansInit::KMEANS_PLUS_PLUS})
		{
			const auto clusters = qlm::KMeans(in, 2, init, 0, 50, static_cast<uint8_t>(0));
			ASSERT_EQ(clusters.size(), 2);
			int total = 0;
			for (const auto &c : clusters)
			{
				 total += static_cast<int>(c.pixels.size());
				const auto v = c.color.r;
				EXPECT_TRUE(v == 0 || v == 255) << (int)v;
			}
			EXPECT_EQ(total, 100);
		}
	}

