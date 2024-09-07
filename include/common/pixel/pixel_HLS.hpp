#pragma once
#include "common/pixel/pixel_common.hpp"

namespace qlm
{
    // Specialization for HLS format
    template<pixel_t T>
    class Pixel<ImageFormat::HLS, T>
    {
    public:
        T h, l, s, a;
    private:
        const T min_value = std::numeric_limits<T>::lowest();
        const T max_value = std::numeric_limits<T>::max();
    public:
        Pixel() : h(0), l(0), s(0), a(std::numeric_limits<T>::max()) {}
        Pixel(T hue, T lightness, T saturation) : h(hue), l(lightness), s(saturation), a(std::numeric_limits<T>::max()) {}
        Pixel(T hue, T lightness, T saturation, T alpha) : h(hue), l(lightness), s(saturation), a(alpha) {}
        // Copy constructor
        Pixel(const Pixel& other) : h(other.h), l(other.l), s(other.s), a(other.a) {}
        // Move constructor
        Pixel(Pixel&& other) noexcept : h(std::move(other.h)), l(std::move(other.l)), s(std::move(other.s)), a(std::move(other.a)) {}
    public:
        // Assignment operator
        Pixel& operator=(const Pixel& other)
        {
            if (this != &other)
            {
                h = other.h;
                l = other.l;
                s = other.s;
                a = other.a;
            }
            return *this;
        }
        // Move operator
        Pixel& operator=(Pixel&& other) noexcept
        {
            if (this != &other)
            {
                h = std::move(other.h);
                l = std::move(other.l);
                s = std::move(other.s);
                a = std::move(other.a);
            }
            return *this;
        }
        // Addition operator
        Pixel operator+(const Pixel& other) const
        {
            Pixel result;
            result.h = (h + other.h) % 360;
            result.s = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(s + other.s, min_value, max_value));
            result.l = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(l + other.l, min_value, max_value));
            result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a + other.a, min_value, max_value));
            return result;
        }
        // Subtraction operator
        Pixel operator-(const Pixel& other) const
        {
            Pixel result;
            result.h = (h - other.h) % 360;
            result.s = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(s - other.s, min_value, max_value));
            result.l = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(l - other.l, min_value, max_value));
            result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a - other.a, min_value, max_value));
            return result;
        }
        // Multiplication operator
        Pixel operator*(const Pixel& other) const
        {
            Pixel result;
            result.h = ((qlm::cast_t < T, T>) h * (qlm::cast_t<T, T>)other.h) % 360;
            result.s = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(s * other.s, min_value, max_value));
            result.l = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(l * other.l, min_value, max_value));
            result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a * other.a, min_value, max_value));
            return result;
        }
        // Cast operator
        template<qlm::pixel_t T2>
        operator Pixel<qlm::ImageFormat::HLS, T2>() const
        {
            Pixel<qlm::ImageFormat::HLS, T2> res;

            T2 min_val = std::numeric_limits<T2>::lowest();
            T2 max_val = std::numeric_limits<T2>::max();

            res.h = static_cast<T2>(std::clamp<qlm::cast_t<T, T2>>(h, min_val, max_val));
            res.s = static_cast<T2>(std::clamp<qlm::cast_t<T, T2>>(s, min_val, max_val));
            res.l = static_cast<T2>(std::clamp<qlm::cast_t<T, T2>>(l, min_val, max_val));
            res.a = static_cast<T2>(std::clamp<qlm::cast_t<T, T2>>(a, min_val, max_val));

            return res;
        }
    public:
        void Set(T hue, T lim, T sat, T alpha = std::numeric_limits<T>::max())
        {
            h = hue;
            s = sat;
            l = lim;
            a = alpha;
        }

        void Set(T val, T alpha = std::numeric_limits<T>::max())
        {
            h = val;
            s = val;
            l = val;
            a = alpha;
        }

        template<arithmetic_t T2>
        void MAC(const Pixel& other, const T2 coeff)
        {
            h = ((qlm::cast_t<T, T2>)h + (qlm::cast_t<T, T2>)other.h * (qlm::cast_t<T, T2>)coeff) % 360;
            s = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(s + other.s * coeff, min_value, max_value));
            l = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(l + other.l * coeff, min_value, max_value));
            a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(a + other.a * coeff, min_value, max_value));
        }

        uint64_t SquaredEuclideanDistance(const Pixel& other)
        {
            int64_t diff_h = (int64_t)h - (int64_t)other.h;
            int64_t diff_s = (int64_t)s - (int64_t)other.s;
            int64_t diff_l = (int64_t)l - (int64_t)other.l;

            return static_cast<uint64_t>(diff_h * diff_h + diff_s * diff_s + diff_l * diff_l);

        }
    };
}