#pragma once

namespace qlm
{
	enum class ScaleMethod
	{
		NEAREST_NEIGHBOR,
		BILINEAR,
		BICUBIC
	};

	enum class BORDER
	{
		BORDER_CONSTANT,
		BORDER_REPLICATE,
		BORDER_REFLECT,
	};

	struct Line
	{
		int x0;
		int y0;
		int x1;
		int y1;
	};
	// line in the polar form
	struct LinePolar
	{
		float radius;
		float angle;
	};

	struct Kernel
	{
	private:
		float* data;
	public:
		int width;
		int height;
	public:
		Kernel(int M, int N) :width(M), height(N)
		{
			data = new float[height * width];
		}
		~Kernel()
		{
			delete data;
		}
	public:
		float Get(int x, int y)
		{
			return data[y * width + x];
		}

		void Set(int x, int y, float value)
		{
			data[y * width + x] = value;
		}
			
	};
}