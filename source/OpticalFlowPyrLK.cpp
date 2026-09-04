#include "OpticalFlowPyrLK.hpp"
#include "common/solve.hpp"
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
        const int num_layers = max_level + 1;
        const float scale = 0.5f;
		const int filter_size = 5;
		const float sigma = 0.8f;
		const BorderMode<ImageFormat::GRAY, T>& border_mode {BorderType::BORDER_REFLECT, Pixel<ImageFormat::GRAY, T>{0}};
        const BorderMode<ImageFormat::GRAY, float>& border_mode_f {BorderType::BORDER_REFLECT, Pixel<ImageFormat::GRAY, float>{0}};

        const Pyramid<ImageFormat::GRAY, T> pyr_prev_img = GaussianPyramid(prev_img, num_layers, scale, filter_size, sigma, border_mode);
        const Pyramid<ImageFormat::GRAY, T> pyr_next_img = GaussianPyramid(next_img, num_layers, scale, filter_size, sigma, border_mode);

        constexpr int gradient_size = 3;
        
        // flow vector
        std::vector<Point<float>> flow(prev_pts.size());
        for (int i = 0; i < prev_pts.size(); i++)
        {
            // initial guess
            flow[i] = (next_pts[i].point - prev_pts[i].point) / static_cast<float>(1 << max_level);
        }

        for (int level = max_level; level >= 0; level--)
        {
            const Image<ImageFormat::GRAY, T> img_prev_l = pyr_prev_img.layers[level];
            const Image<ImageFormat::GRAY, T> img_next_l = pyr_next_img.layers[level];

            const Image<ImageFormat::GRAY, int16_t> I_x = SobelX<T, int16_t>(img_prev_l, gradient_size);
		    const Image<ImageFormat::GRAY, int16_t> I_y = SobelY<T, int16_t>(img_prev_l, gradient_size);

            const Image<ImageFormat::GRAY, float> I_xx = qlm::Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(I_x, I_x, 1.0f, OverFlowFlag::WRAP);
            const Image<ImageFormat::GRAY, float> I_yy = qlm::Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(I_y, I_y, 1.0f, OverFlowFlag::WRAP);
            const Image<ImageFormat::GRAY, float> I_xy = qlm::Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(I_x, I_y, 1.0f, OverFlowFlag::WRAP);

            const Image<ImageFormat::GRAY, float> S_xx = BoxFilter<ImageFormat::GRAY, float, float>(I_xx, win_size.width, win_size.height, false);
            const Image<ImageFormat::GRAY, float> S_yy = BoxFilter<ImageFormat::GRAY, float, float>(I_yy, win_size.width, win_size.height, false);
            const Image<ImageFormat::GRAY, float> S_xy = BoxFilter<ImageFormat::GRAY, float, float>(I_xy, win_size.width, win_size.height, false);

            const float level_scale = static_cast<float>(1 << level);

            for (int i = 0; i < prev_pts.size(); i++)
            {
                if (next_pts[i].status == KPStatusFlag::UNTRACKED) continue;

                const Point<float> prev_pt_loc = prev_pts[i].point / level_scale;

                // unified skip: at the finest level the point is abandoned,
                // at coarser levels the flow estimate is carried to the next level
                auto skip_point = [&]()
                {
                    if (level == 0) next_pts[i].status = KPStatusFlag::UNTRACKED;
                    else { flow[i].x *= 2.0f; flow[i].y *= 2.0f; }
                };

                // calculates the minimum eigen value
                const float ixx = BilinearInterpolation(S_xx, prev_pt_loc.x, prev_pt_loc.y, border_mode_f).v;
                const float ixy = BilinearInterpolation(S_xy, prev_pt_loc.x, prev_pt_loc.y, border_mode_f).v;
                const float iyy = BilinearInterpolation(S_yy, prev_pt_loc.x, prev_pt_loc.y, border_mode_f).v;

                // Sobel + non-normalized box-sum produce λ_min in raw (gradient^2) units, not
                // OpenCV's FLT_SCALE-adjusted units. So min_eig_threshold must be chosen in these units.
                // e.g. min_eig_threshold ≈ 1e3 when win = 21x21 (matches OpenCV's 1e-4 behavior).
                const float min_eigenvalue = MinEigenValue(ixx, ixy, iyy, static_cast<float>(win_size.width * win_size.height));
                if (min_eigenvalue < min_eig_threshold)
                {
                    skip_point();
                    continue;
                }

                const float denominator = ixx * iyy - ixy * ixy;

                // Ill-conditioned structure tensor: flat region or single-direction edge.
                // Inverting it would blow up the Newton step, so abandon this point.
                if (std::abs(denominator) < FLT_EPSILON)
                {
                    skip_point();
                    continue;
                }

                const float inv_denom = 1.0f / denominator;

                // Iterative Newton-Raphson
                for(int k = 0; k < criteria.max_count; k++)
                {
                    // displacement for the current iteration
                    const Image<ImageFormat::GRAY, T> img_nex_k = Translate(img_next_l,  Point<float>{-flow[i].x, -flow[i].y});
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
                
                    const float ixt = BilinearInterpolation(S_xt, prev_pt_loc.x, prev_pt_loc.y, border_mode_f).v;
                    const float iyt = BilinearInterpolation(S_yt, prev_pt_loc.x, prev_pt_loc.y, border_mode_f).v;

                    const float du = (ixy * iyt - iyy * ixt) * inv_denom;
                    const float dv = (ixy * ixt - ixx * iyt) * inv_denom;

                    // guess for the next iteration
                    flow[i].x += du;
                    flow[i].y += dv;

                    if (du * du + dv * dv < criteria.epsilon * criteria.epsilon)
                    {
                        break;
                    }
                }

                // final optical flow for the current level
                if (level == 0)
                {
                    // write the full-res flow into the output point
                    next_pts[i].point.x = prev_pts[i].point.x + flow[i].x;
                    next_pts[i].point.y = prev_pts[i].point.y + flow[i].y;

                    // bounds / status check
                    if (next_pts[i].point.x < 0 || next_pts[i].point.x >= next_img.width ||
                        next_pts[i].point.y < 0 || next_pts[i].point.y >= next_img.height)
                        next_pts[i].status = KPStatusFlag::UNTRACKED;
                }
                else 
                {
                    flow[i] = flow[i] * 2.0f;
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
