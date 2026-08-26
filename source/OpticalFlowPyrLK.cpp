#include "OpticalFlowPyrLK.hpp"
#include "GaussianPyramid.hpp"
#include "Sobel.hpp"
#include "BoxFilter.hpp"
#include "Subtract.hpp"
#include "Translate.hpp"

namespace qlm
{
    template <pixel_t T>
    std::vector<KeyPoint<float>> OpticalFlowPyrLK(
        const Image<ImageFormat::GRAY, T> &prev_img, const Image<ImageFormat::GRAY, T> &next_img, 
        const std::vector<KeyPoint<float>> &prev_pts, const std::vector<KeyPoint<float>> &initial_guess, 
        const Size &win_size, const TermCriteria &criteria, const int max_level, const double min_eig_threshold)
    {
        std::vector<KeyPoint<float>> next_pts = (prev_pts.size() == initial_guess.size()) ? initial_guess : prev_pts;
        
        // create image pyramid for the input frames
        const Pyramid<ImageFormat::GRAY, T> pyr_prev_img = GaussianPyramid(prev_img, max_level);
        const Pyramid<ImageFormat::GRAY, T> pyr_next_img = GaussianPyramid(next_img, max_level);

        constexpr int sobel_kernel_size = 3;

        for (int level = max_level - 1; level >= 0; level--)
        {
            const Image<ImageFormat::GRAY, T> img_prev_l = pyr_prev_img.layers[level];
            const Image<ImageFormat::GRAY, T> img_next_l = pyr_next_img.layers[level];

            Image<ImageFormat::GRAY, int16_t> I_x = SobelX<uint8_t, int16_t>(img_prev_l, sobel_kernel_size);
		    Image<ImageFormat::GRAY, int16_t> I_y = SobelY<uint8_t, int16_t>(img_prev_l, sobel_kernel_size);

            const Image<ImageFormat::GRAY, float> I_xx = qlm::Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(I_x, I_x, 1.0f, OverFlowFlag::WRAP);
            const Image<ImageFormat::GRAY, float> I_yy = qlm::Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(I_y, I_y, 1.0f, OverFlowFlag::WRAP);
            const Image<ImageFormat::GRAY, float> I_xy = qlm::Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(I_x, I_y, 1.0f, OverFlowFlag::WRAP);

            Image<ImageFormat::GRAY, float> S_xx = BoxFilter<ImageFormat::GRAY, float, float>(I_xx, win_size.width, win_size.height, false);
            Image<ImageFormat::GRAY, float> S_yy = BoxFilter<ImageFormat::GRAY, float, float>(I_yy, win_size.width, win_size.height, false);
            Image<ImageFormat::GRAY, float> S_xy = BoxFilter<ImageFormat::GRAY, float, float>(I_xy, win_size.width, win_size.height, false);

            const float level_scale = static_cast<float>(1 << level);

            for (int i = 0; i < prev_pts.size(); i++)
            {
                const int x_loc = static_cast<int>(next_pts[i].point.x / level_scale);
                const int y_loc = static_cast<int>(next_pts[i].point.y / level_scale);

                // initial guess for the next point
                float u = (initial_guess[i].point.x - next_pts[i].point.x) / level_scale;
                float v = (initial_guess[i].point.y - next_pts[i].point.y) / level_scale;

                // Iterative Newton-Raphson
                for(int k = 0; k < criteria.max_count; k++)
                {
                    // displacement for the current iteration
                    const Image<ImageFormat::GRAY, uint8_t> img_nex_k = Translate(img_next_l, (static_cast<int>(u), static_cast<int>(v)));
                    const Image<ImageFormat::GRAY, int16_t> I_t = Subtract<uint8_t, int16_t>(img_nex_k, img_prev_l);

                    const Image<ImageFormat::GRAY, float> I_xt = qlm::Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(I_x, I_t, 1.0f, OverFlowFlag::WRAP);
                    const Image<ImageFormat::GRAY, float> I_yt = qlm::Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(I_y, I_t, 1.0f, OverFlowFlag::WRAP);

                    const Image<ImageFormat::GRAY, float> S_xt = BoxFilter<ImageFormat::GRAY, float, float>(I_xt, win_size.width, win_size.height, false);
                    const Image<ImageFormat::GRAY, float> S_yt = BoxFilter<ImageFormat::GRAY, float, float>(I_yt, win_size.width, win_size.height, false);

                    /*
                        estimate the optical flow :-

                        u = (S_Ixy * S_Iyt - S_Iyy * S_Ixt) / (S_Ixx * S_Iyy - S_Ixy * S_Ixy)
                        v = (S_Ixy * S_Ixt - S_Ixx * S_Iyt) / (S_Ixx * S_Iyy - S_Ixy * S_Ixy)
                    */
                    const float denominator = (S_Ixx.GetPixel(x_loc, y_loc).v * S_Iyy.GetPixel(x_loc, y_loc).v - S_Ixy.GetPixel(x_loc, y_loc).v * S_Ixy.GetPixel(x_loc, y_loc).v);
                
                    const float du = (S_Ixy.GetPixel(x_loc, y_loc).v * S_Iyt.GetPixel(x_loc, y_loc).v - S_Iyy.GetPixel(x_loc, y_loc).v * S_Ixt.GetPixel(x_loc, y_loc).v) / denominator;
                    const float dv = (S_Ixy.GetPixel(x_loc, y_loc).v * S_Ixt.GetPixel(x_loc, y_loc).v - S_Ixx.GetPixel(x_loc, y_loc).v * S_Iyt.GetPixel(x_loc, y_loc).v) / denominator;

                    // guess for the next iteration
                    u += du;
                    v += dv;
                }
               
                // final optical for the current level
                next_pts[i].point.x += u * level_scale;
                next_pts[i].point.y += v * level_scale;
            }
        }

        return next_pts;
    }
} 
