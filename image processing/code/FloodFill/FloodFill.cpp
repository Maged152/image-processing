#include "FloodFill/FloodFill.h"
#include<queue>
#include<cmath>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> FloodFill(const Image<frmt, T>& in, const Point<int>& seed_point, const Pixel<frmt, T>& fill_color, const Pixel<frmt, T>& threshold)
	{
		const unsigned int img_width = in.Width();
		const unsigned int img_height = in.Height();

		Image<frmt, T> out{ in };
		std::queue<Point<int>> q;

		Pixel<frmt, T> seed_pixel{ in.GetPixel(seed_point.x, seed_point.y) };
		Pixel<frmt, T> neighbour_pixel;

		if (AbsDiff(fill_color, seed_pixel) <= threshold)
		{
			return out;
		}

		// mark seed point as visited
		out.SetPixel(seed_point.x, seed_point.y, fill_color);
		// enqueue seed point
		q.push(seed_point);

		Point<int> seed;
		Point<int> neighbour[4]; // up, down, left, and right
		const Point<int> offset[4] = {
			Point<int>{0 , -1},
			Point<int>{0 ,  1},
			Point<int>{-1,  0},
			Point<int>{1 ,  0},
		};

		auto get_neighbours = [&neighbour, &offset](const Point<int>&seed)
		{
			neighbour[0] = seed + offset[0];
			neighbour[1] = seed + offset[1];
			neighbour[2] = seed + offset[2];
			neighbour[3] = seed + offset[3];
		};

		while (!q.empty())
		{
			// dequeue
			seed = q.front();
			q.pop();
			// get the neighbours
			get_neighbours(seed);
			// mark them as visited
			for (int i = 0; i < 4; i++)
			{
				// boundary check
				if (neighbour[i].x >= 0 && neighbour[i].x < img_width &&
					neighbour[i].y >= 0 && neighbour[i].y < img_height)
				{
					neighbour_pixel = out.GetPixel(neighbour[i].x, neighbour[i].y);
					// color check
					if (AbsDiff(neighbour_pixel, seed_pixel) <= threshold)
					{
						// mark as visited
						out.SetPixel(neighbour[i].x, neighbour[i].y, fill_color);
						// enqueue
						q.push(neighbour[i]);
					}

				}
			}
			
		}
		return out;
	}


	template Image<ImageFormat::RGB, uint8_t> FloodFill(const Image<ImageFormat::RGB, uint8_t>&, const Point<int>&, const Pixel<ImageFormat::RGB, uint8_t>&, const Pixel<ImageFormat::RGB, uint8_t>&);
}
