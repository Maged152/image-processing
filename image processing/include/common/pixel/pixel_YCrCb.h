#pragma once
#include "common/pixel/pixel_common.h"

namespace qlm
{
    // Specialization for YCrCb format
    template<qlm::pixel_t T>
    class qlm::Pixel<qlm::ImageFormat::YCrCb, T>
    {
    public:
        T y, cr, cb, a;
    private:
        const T min_value = std::numeric_limits<T>::lowest();
        const T max_value = std::numeric_limits<T>::max();
    public:
        Pixel() : y(0), cr(0), cb(0), a(std::numeric_limits<T>::max()) {}
        Pixel(T luminance, T chromaR, T chromaB) : y(luminance), cr(chromaR), cb(chromaB), a(std::numeric_limits<T>::max()) {}
        Pixel(T luminance, T chromaR, T chromaB, T alpha) : y(luminance), cr(chromaR), cb(chromaB), a(alpha) {}
        // Copy constructor
        Pixel(const Pixel& other) : y(other.y), cr(other.cr), cb(other.cb), a(other.a) {}
        // Move constructor
        Pixel(Pixel&& other) noexcept : y(std::move(other.y)), cr(std::move(other.cr)), cb(std::move(other.cb)), a(std::move(other.a)) {}
    public:
        // Assignment operator
        Pixel& operator=(const Pixel& other)
        {
            if (this != &other)
            {
                y = other.y;
                cr = other.cr;
                cb = other.cb;
                a = other.a;
            }
            return *this;
        }
        // Move operator
        Pixel& operator=(Pixel&& other) noexcept
        {
            if (this != &other)
            {
                y = std::move(other.y);
                cr = std::move(other.cr);
                cb = std::move(other.cb);
                a = std::move(other.a);
            }
            return *this;
        }
        // Addition operator
        Pixel operator+(const Pixel& other) const
        {
            Pixel result;
            // ranges may be changed
            result.y = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(y + other.y, min_value, max_value));
            result.cr = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(cr + other.cr, min_value, max_value));
            result.cb = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(cb + other.cb, min_value, max_value));
            result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a + other.a, min_value, max_value));
            return result;
        }
        // Subtraction operator
        Pixel operator-(const Pixel& other) const
        {
            Pixel result;
            result.y = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(y - other.y, min_value, max_value));
            result.cr = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(cr - other.cr, min_value, max_value));
            result.cb = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(cb - other.cb, min_value, max_value));
            result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a - other.a, min_value, max_value));
            return result;
        }
        // Multiplication operator
        Pixel operator*(const Pixel& other) const
        {
            Pixel result;
            result.y = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(y * other.y, min_value, max_value));
            result.cr = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(cr * other.cr, min_value, max_value));
            result.cb = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(cb * other.cb, min_value, max_value));
            result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a * other.a, min_value, max_value));
            return result;
        }
        // Cast operator
        template<qlm::pixel_t T2>
        operator Pixel<qlm::ImageFormat::YCrCb, T2>() const
        {
            Pixel<qlm::ImageFormat::YCrCb, T2> res;

            T2 min_val = std::numeric_limits<T2>::lowest();
            T2 max_val = std::numeric_limits<T2>::max();

            res.y = static_cast<T2>(std::clamp<qlm::cast_t<T, T2>>(y, min_val, max_val));
            res.cr = static_cast<T2>(std::clamp<qlm::cast_t<T, T2>>(cr, min_val, max_val));
            res.cb = static_cast<T2>(std::clamp<qlm::cast_t<T, T2>>(cb, min_val, max_val));
            res.a = static_cast<T2>(std::clamp<qlm::cast_t<T, T2>>(a, min_val, max_val));

            return res;
        }
    public:
        void Set(T Y, T Cr, T Cb, T alpha = std::numeric_limits<T>::max())
        {
            y = Y;
            cr = Cr;
            cb = Cb;
            a = alpha;
        }

        void Set(T val, T alpha = std::numeric_limits<T>::max())
        {
            y = val;
            cr = val;
            cb = val;
            a = alpha;
        }

        template<arithmetic_t T2>
        void MAC(const Pixel& other, const T2 coeff)
        {
            y = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(y + other.y * coeff, min_value, max_value));
            cr = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(cr + other.cr * coeff, min_value, max_value));
            cb = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(cb + other.cb * coeff, min_value, max_value));
            a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(a + other.a * coeff, min_value, max_value));
        }
    };
}