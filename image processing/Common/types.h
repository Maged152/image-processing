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

	class Kernel
	{
	private:
		float* data;
	public:
		const unsigned int width;
		const unsigned int height;
	
		Kernel(unsigned int M, unsigned int N) :width(M), height(N)
		{
			data = new float[height * width];
		}
		~Kernel()
		{
			if (data != nullptr)
				delete[] data;
		}
	
		float Get(int x, int y) const
		{
			return data[y * width + x];
		}

		void Set(int x, int y, float value)
		{
			data[y * width + x] = value;
		}
	};

	class Kernel1D : protected Kernel
	{
	public:
		Kernel1D(unsigned int length): Kernel(1u, length), length(length)
		{}
	public:
		const unsigned int length;

		float Get(int idx)  const
		{
			return Kernel::Get(idx, 0);
		}

		void Set(int idx, float value) 
		{
			Kernel::Set(idx, 0, value);
		}
	};
}