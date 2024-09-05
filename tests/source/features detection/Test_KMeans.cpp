#include "test_common.h"

TEST(Test_shakhbat_cv, KMeans)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Features Detection/KMeans/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 1)
		alpha = false;

	const size_t k = 4;
	const qlm::KMeansInit init = qlm::KMeansInit::KMEANS_PLUS_PLUS;

	// do the operation
	t.start();
	auto clusters = qlm::KMeans(in, k, init);
	t.end();


	// draw clusters
	auto out = qlm::DrawCluster(in, clusters);

	test::PrintTime(t);

	// reread output image
	out = test::ReReadImage(out);
	
	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
	const bool load_ref = ref.LoadFromFile(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(out, ref);
}