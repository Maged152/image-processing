#include "OpticalFlowPyrLK.hpp"
#include "GaussianPyramid.hpp"
#include "Sobel.hpp"
#include "BoxFilter.hpp"
#include "Subtract.hpp"
#include "Translate.hpp"
#include "Multiply.hpp"

namespace qlm
{
    template <pixel_t T>
    std::vector<KeyPoint<float>> OpticalFlowPyrLK(
        const Image<ImageFormat::GRAY, T> &prev_img, const Image<ImageFormat::GRAY, T> &next_img, 
        const std::vector<KeyPoint<float>> &prev_pts, const std::vector<KeyPoint<float>> &initial_guess, 
        const Size &win_size, const int max_level, const TermCriteria &criteria,const double min_eig_threshold)
    {
        constexpr float FLT_EPSILON = 1e-6f;
        std::vector<KeyPoint<float>> next_pts = (prev_pts.size() == initial_guess.size()) ? initial_guess : prev_pts;
        
        // create image pyramid for the input frames
        const Pyramid<ImageFormat::GRAY, T> pyr_prev_img = GaussianPyramid(prev_img, max_level);
        const Pyramid<ImageFormat::GRAY, T> pyr_next_img = GaussianPyramid(next_img, max_level);

        constexpr int sobel_kernel_size = 3;

        for (int level = max_level - 1; level >= 0; level--)
        {
            const Image<ImageFormat::GRAY, T> img_prev_l = pyr_prev_img.layers[level];
            const Image<ImageFormat::GRAY, T> img_next_l = pyr_next_img.layers[level];

            const Image<ImageFormat::GRAY, int16_t> I_x = SobelX<T, int16_t>(img_prev_l, sobel_kernel_size);
		    const Image<ImageFormat::GRAY, int16_t> I_y = SobelY<T, int16_t>(img_prev_l, sobel_kernel_size);

            const Image<ImageFormat::GRAY, float> I_xx = qlm::Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(I_x, I_x, 1.0f, OverFlowFlag::WRAP);
            const Image<ImageFormat::GRAY, float> I_yy = qlm::Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(I_y, I_y, 1.0f, OverFlowFlag::WRAP);
            const Image<ImageFormat::GRAY, float> I_xy = qlm::Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(I_x, I_y, 1.0f, OverFlowFlag::WRAP);

            const Image<ImageFormat::GRAY, float> S_xx = BoxFilter<ImageFormat::GRAY, float, float>(I_xx, win_size.width, win_size.height, false);
            const Image<ImageFormat::GRAY, float> S_yy = BoxFilter<ImageFormat::GRAY, float, float>(I_yy, win_size.width, win_size.height, false);
            const Image<ImageFormat::GRAY, float> S_xy = BoxFilter<ImageFormat::GRAY, float, float>(I_xy, win_size.width, win_size.height, false);

            const float level_scale = static_cast<float>(1 << level);

            for (int i = 0; i < prev_pts.size(); i++)
            {
                const Point<float> prev_pt_loc = prev_pts[i].point / level_scale;
                const Point<float> next_pt_loc = next_pts[i].point / level_scale;

                const int x_loc = std::round(prev_pt_loc.x);
                const int y_loc = std::round(prev_pt_loc.y);

                if (next_pts[i].status == KPStatusFlag::UNTRACKED)
                {
                    continue;
                }

                //  calculates the minimum eigen value
                const float ixx = S_xx.GetPixel(x_loc, y_loc).v;
                const float ixy = S_xy.GetPixel(x_loc, y_loc).v;
                const float iyy = S_yy.GetPixel(x_loc, y_loc).v;

                float d = (ixx - iyy) * (ixx - iyy) + 4.0f * ixy * ixy;
                d = std::max(d, 0.0f); // Ensure non-negative value for sqrt
                const float min_eigenvalue = ((ixx + iyy - std::sqrt(d)) * 0.5f) / (win_size.width * win_size.height);

                // Sobel + non-normalized box-sum produce λ_min in raw (gradient^2) units, not
                // OpenCV's FLT_SCALE-adjusted units. So min_eig_threshold must be chosen in these units.
                // e.g. min_eig_threshold ≈ 1e3 when win = 21x21 (matches OpenCV's 1e-4 behavior).
                if (min_eigenvalue < min_eig_threshold)
                {
                    if (level == 0) next_pts[i].status = KPStatusFlag::UNTRACKED;
                    continue;  // coarser levels just skip, keep point
                }

                const float denominator = ixx * iyy - ixy * ixy;

                // Ill-conditioned structure tensor: flat region or single-direction edge.
                // Inverting it would blow up the Newton step, so abandon this point.
                if (std::abs(denominator) < FLT_EPSILON)
                {
                    if (level == 0) next_pts[i].status = KPStatusFlag::UNTRACKED;
                    continue;                 // skip this keypoint entirely at this level
                }

                const float inv_denom = 1.0f / denominator;

                // initial guess for the next point
                float u = next_pt_loc.x - prev_pt_loc.x;
                float v = next_pt_loc.y - prev_pt_loc.y;

                // Iterative Newton-Raphson
                for(int k = 0; k < criteria.max_count; k++)
                {
                    // displacement for the current iteration
                    const Image<ImageFormat::GRAY, T> img_nex_k = Translate(img_next_l,  Point<float>{-u, -v});
                    const Image<ImageFormat::GRAY, int16_t> I_t = Subtract<ImageFormat::GRAY, T, int16_t>(img_nex_k, img_prev_l);

                    const Image<ImageFormat::GRAY, float> I_xt = qlm::Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(I_x, I_t, 1.0f, OverFlowFlag::WRAP);
                    const Image<ImageFormat::GRAY, float> I_yt = qlm::Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(I_y, I_t, 1.0f, OverFlowFlag::WRAP);

                    const Image<ImageFormat::GRAY, float> S_xt = BoxFilter<ImageFormat::GRAY, float, float>(I_xt, win_size.width, win_size.height, false);
                    const Image<ImageFormat::GRAY, float> S_yt = BoxFilter<ImageFormat::GRAY, float, float>(I_yt, win_size.width, win_size.height, false);

                    /*
                        estimate the optical flow :-

                        u = (S_xy * S_yt - S_yy * S_xt) / (S_xx * S_yy - S_xy * S_xy)
                        v = (S_xy * S_xt - S_xx * S_yt) / (S_xx * S_yy - S_xy * S_xy)
                    */
                
                    const float ixt = S_xt.GetPixel(x_loc, y_loc).v;
                    const float iyt = S_yt.GetPixel(x_loc, y_loc).v;

                    const float du = (ixy * iyt - iyy * ixt) * inv_denom;
                    const float dv = (ixy * ixt - ixx * iyt) * inv_denom;

                    // guess for the next iteration
                    u += du;
                    v += dv;

                    if (du * du + dv * dv < criteria.epsilon * criteria.epsilon)
                    {
                        break;
                    }
                }
               
                // final optical flow for the current level
                next_pts[i].point.x = prev_pts[i].point.x + u * level_scale;
                next_pts[i].point.y = prev_pts[i].point.y + v * level_scale;

                // check if the next point is within the image bounds
                if (next_pts[i].point.x < 0 || next_pts[i].point.x >= next_img.width || next_pts[i].point.y < 0 || next_pts[i].point.y >= next_img.height)
                {
                    next_pts[i].status = KPStatusFlag::UNTRACKED;
                }
            }
        }

        return next_pts;
    }

    template std::vector<KeyPoint<float>> OpticalFlowPyrLK<uint8_t>(
        const Image<ImageFormat::GRAY, uint8_t> &prev_img, const Image<ImageFormat::GRAY, uint8_t> &next_img, 
        const std::vector<KeyPoint<float>> &prev_pts, const std::vector<KeyPoint<float>> &initial_guess, 
        const Size &win_size, const int max_level, const TermCriteria &criteria, const double min_eig_threshold);
} 
