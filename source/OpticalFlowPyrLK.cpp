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

            // needed buffer for iterative Newton-Raphson
            Image<ImageFormat::GRAY, int16_t> I_t {img_prev_l.width, img_prev_l.height};
            Image<ImageFormat::GRAY, float> I_xt {img_prev_l.width, img_prev_l.height}, I_yt {img_prev_l.width, img_prev_l.height};
            Image<ImageFormat::GRAY, T> img_nex_k {img_prev_l.width, img_prev_l.height};
            Image<ImageFormat::GRAY, float> S_xt {img_prev_l.width, img_prev_l.height}, S_yt {img_prev_l.width, img_prev_l.height};

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

                // define ROI for the current point
                const int x0 = std::floor(prev_pt_loc.x);
                const int y0 = std::floor(prev_pt_loc.y);
                const int hw_x = win_size.width  / 2;   // = pad
                const int hw_y = win_size.height / 2;

                // region the Translate/Subtract/Multiply chain writes (covers the box-filter reads)
                const Rectangle<int> in_roi{ { x0 - hw_x, y0 - hw_y }, win_size.width + 1, win_size.height + 1 };

                // 2x2 region the BoxFilter must actually produce (the bilinear taps)
                const Rectangle<int> out_roi{ { x0, y0 }, 2, 2 };

                // Iterative Newton-Raphson
                for(int k = 0; k < criteria.max_count; k++)
                {
                    // displacement for the current iteration
                    qlm::Translate(img_next_l, img_nex_k, Point<float>{-flow[i].x, -flow[i].y}, Pixel<ImageFormat::GRAY, T>{}, in_roi);
                    qlm::Subtract(img_nex_k, img_prev_l, I_t, in_roi);

                    qlm::Multiply(I_x, I_t, I_xt, 1.0f, OverFlowFlag::WRAP, in_roi);
                    qlm::Multiply(I_y, I_t, I_yt, 1.0f, OverFlowFlag::WRAP, in_roi);

                    BoxFilter(I_xt, S_xt, win_size.width, win_size.height, false, BorderMode<ImageFormat::GRAY, float>{}, out_roi);
                    BoxFilter(I_yt, S_yt, win_size.width, win_size.height, false, BorderMode<ImageFormat::GRAY, float>{}, out_roi);

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
