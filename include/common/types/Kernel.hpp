#pragma once

namespace qlm
{
	// 2d kernel
	class Kernel
	{
	private:
		float* data;
		const int width;
		const int height;
	public:
		// Constructor
		Kernel(int M, int N) :width(M), height(N)
		{
			data = new float[height * width];
		}
		// Move Constructor
		Kernel(Kernel&& other) noexcept : width(other.width), height(other.height), data(other.data)
		{
			other.data = nullptr;
		}
		// Copy Constructor
		Kernel(const Kernel& other) : width(other.width), height(other.height)
		{
			data = new float[height * width];
			std::memcpy(data, other.data, height * width * sizeof(float));
		}
		// Move Assignment Operator
		Kernel& operator=(Kernel&& other) noexcept
		{
			if (this != &other)
			{
				const_cast<int&>(width) = other.width;
				const_cast<int&>(height) = other.height;
				data = other.data;
				other.data = nullptr;
			}
			return *this;
		}
		// Copy Assignment Operator
		Kernel& operator=(const Kernel& other)
		{
			if (this != &other)
			{
				const_cast<int&>(width) = other.width;
				const_cast<int&>(height) = other.height;
				if (data != nullptr)
					delete[] data;
				data = new float[height * width];
				std::memcpy(data, other.data, height * width * sizeof(float));
			}
			return *this;
		}
		// Destructor
		~Kernel()
		{
			if (data != nullptr)
				delete[] data;
		}
	public:
		float Get(int x, int y) const
		{
			if (x >= 0 && x < width && y >= 0 && y < height)
			{
				return data[y * width + x];
			}
			return 0.0f; //default
		}

		void Set(int x, int y, float value)
		{
			if (x >= 0 && x < width && y >= 0 && y < height)
			{
				data[y * width + x] = value;
			}
		}

		int Width() const
		{
			return width;
		}

		int Height() const
		{
			return height;
		}
	};

	// 1d kernel
	class Kernel1D : private Kernel
	{
	private:
		const int length;
	public:
		// Constructor
		Kernel1D(int length) : Kernel(1u, length), length(length)
		{}
		// Copy Constructor
		Kernel1D(const Kernel1D& other) : Kernel(other), length(other.length)
		{
		}
		// Move Constructor
		Kernel1D(Kernel1D&& other) noexcept : Kernel(std::move(other)), length(other.length)
		{
		}
		// Copy Assignment Operator
		Kernel1D& operator=(const Kernel1D& other)
		{
			if (this != &other)
			{
				Kernel::operator=(other);
				const_cast<int&>(length) = other.length;
			}
			return *this;
		}
		// Move Assignment Operator
		Kernel1D& operator=(Kernel1D&& other) noexcept
		{
			if (this != &other)
			{
				Kernel::operator=(std::move(other));
				const_cast<int&>(length) = other.length;
			}
			return *this;
		}

	public:

		float Get(int idx)  const
		{
			return Kernel::Get(0, idx);
		}

		void Set(int idx, float value)
		{
			Kernel::Set(0, idx, value);
		}

		int Length() const
		{
			return length;
		}
	};

	// separable kernel
	class SepKernel
	{
	public:
		Kernel1D x_ker;
		Kernel1D y_ker;
	public:
		SepKernel(int len_x, int len_y) : x_ker(len_x), y_ker(len_y)
		{}
	};
}