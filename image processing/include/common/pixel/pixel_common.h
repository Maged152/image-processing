#pragma once
#include <concepts>
#include <algorithm>

namespace qlm
{
    // image formates
    enum class ImageFormat
    {
        GRAY,
        RGB,
        YCrCb,
        HSV,
        HLS
    };

    // Supported pixel types
    template <class T>
    concept pixel_t = std::same_as<T, uint8_t> ||
        std::same_as<T, int16_t> ||
        std::same_as<T, uint16_t> ||
        std::floating_point<T>;
    // Arithmetic data type
    template<typename T>
    concept arithmetic_t = std::is_arithmetic_v<T>;
    // Next larger data type
    template <typename T>
    using wider_t =
        typename std::conditional<std::is_same_v<T, std::uint8_t>, std::uint16_t,
        typename std::conditional<std::is_same_v<T, std::int8_t>, std::int16_t,
        typename std::conditional<std::is_same_v<T, std::uint16_t>, std::uint32_t,
        typename std::conditional<std::is_same_v<T, std::int16_t>, std::int32_t,
        typename std::conditional<std::is_same_v<T, std::uint32_t>, std::uint64_t,
        typename std::conditional<std::is_same_v<T, std::int32_t>, std::int64_t,
        typename std::conditional<std::is_same_v<T, float>, double, double
        >::type
        >::type
        >::type
        >::type
        >::type
        >::type
        >::type;
    // Make a type signed
    template<typename T>
    using signed_t =
        typename std::conditional<std::is_integral<T>::value, std::make_signed<T>, T>::type;
    // upgrade type
    template <pixel_t T, arithmetic_t T2>
    using cast_t =
        typename std::conditional<(std::is_floating_point_v<T2> || std::is_floating_point_v<T>), double,
        typename std::conditional<((sizeof(T) > sizeof(T2)) && (std::is_signed_v<T2> || std::is_signed_v<T>)), wider_t<signed_t<T>>,
        typename std::conditional<((sizeof(T) < sizeof(T2)) && (std::is_signed_v<T2> || std::is_signed_v<T>)), wider_t<signed_t<T2>>,
        typename std::conditional<(sizeof(T) > sizeof(T2)), wider_t<T>, wider_t<T2>
        >::type
        >::type
        >::type
    >::type;

    // Forward declaration of Pixel class
    template<ImageFormat frmt, pixel_t T>
    class Pixel;

    // pixels operations
    // Pixel * num 
    template<qlm::ImageFormat frmt, qlm::pixel_t T, qlm::arithmetic_t T2>
    qlm::Pixel<frmt, T> operator*(const qlm::Pixel<frmt, T>& c, T2 num)
    {
        qlm::Pixel<frmt, T> result;

        const T min_val = std::numeric_limits<T>::lowest();
        const T max_val = std::numeric_limits<T>::max();

        if constexpr (frmt == qlm::ImageFormat::GRAY)
        {
            result.v = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.v * num, min_val, max_val));
            result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.a * num, min_val, max_val));
        }
        else if constexpr (frmt == qlm::ImageFormat::RGB)
        {
            result.r = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.r * num, min_val, max_val));
            result.g = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.g * num, min_val, max_val));
            result.b = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.b * num, min_val, max_val));
            result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.a * num, min_val, max_val));
        }
        else if constexpr (frmt == qlm::ImageFormat::HLS)
        {
            result.h = ((qlm::cast_t<T, T2>)c.h * (qlm::cast_t<T, T2>)num) % 360;
            result.l = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.l * num, min_val, max_val));
            result.s = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.s * num, min_val, max_val));
            result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.a * num, min_val, max_val));
        }
        else
        {
            result.h = ((qlm::cast_t<T, T2>)c.h * (qlm::cast_t<T, T2>)num) % 360;
            result.v = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.v * num, min_val, max_val));
            result.s = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.s * num, min_val, max_val));
            result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.a * num, min_val, max_val));
        }

        return result;
    }

    // absolute difference
    template<qlm::ImageFormat frmt, qlm::pixel_t T>
    qlm::Pixel<frmt, T>  AbsDiff(const qlm::Pixel<frmt, T>& in1, const qlm::Pixel<frmt, T>& in2)
    {
        qlm::Pixel<frmt, T> result;

        using type_t = qlm::wider_t<qlm::signed_t<T>>;

        const T min_val = std::numeric_limits<T>::lowest();
        const T max_val = std::numeric_limits<T>::max();

        if constexpr (frmt == qlm::ImageFormat::GRAY)
        {
            result.v = static_cast<T>(std::clamp<type_t>(std::abs(in1.v - in2.v), min_val, max_val));
            result.a = static_cast<T>(std::clamp<type_t>(std::abs(in1.a - in2.a), min_val, max_val));
        }
        else if constexpr (frmt == qlm::ImageFormat::RGB)
        {
            result.r = static_cast<T>(std::clamp<type_t>(std::abs(in1.r - in2.r), min_val, max_val));
            result.g = static_cast<T>(std::clamp<type_t>(std::abs(in1.g - in2.g), min_val, max_val));
            result.b = static_cast<T>(std::clamp<type_t>(std::abs(in1.b - in2.b), min_val, max_val));
            result.a = static_cast<T>(std::clamp<type_t>(std::abs(in1.a - in2.a), min_val, max_val));
        }
        else if constexpr (frmt == qlm::ImageFormat::HLS)
        {
            result.h = (std::abs((type_t)in1.h - (type_t)in2.h)) % 360;
            result.l = static_cast<T>(std::clamp<type_t>(std::abs(in1.l - in2.l), min_val, max_val));
            result.s = static_cast<T>(std::clamp<type_t>(std::abs(in1.s - in2.s), min_val, max_val));
            result.a = static_cast<T>(std::clamp<type_t>(std::abs(in1.a - in2.a), min_val, max_val));
        }
        else
        {
            result.h = (std::abs((type_t)in1.h - (type_t)in2.h)) % 360;
            result.v = static_cast<T>(std::clamp<type_t>(std::abs(in1.v - in2.v), min_val, max_val));
            result.s = static_cast<T>(std::clamp<type_t>(std::abs(in1.s - in2.s), min_val, max_val));
            result.a = static_cast<T>(std::clamp<type_t>(std::abs(in1.a - in2.a), min_val, max_val));
        }

        return result;
    }

    // blend pixels
    // Function to blend two colors based on the given weight (0 to 1)
    template <ImageFormat frmt, pixel_t T>
    Pixel<frmt, T> BlendColors(const Pixel<frmt, T>& color1, const Pixel<frmt, T>& color2, float weight)
    {
        return color1 * weight + color2 * (1 - weight);
    }

    // l2 norm
    template<qlm::ImageFormat frmt, qlm::pixel_t T>
    uint64_t L2Norm(const qlm::Pixel<frmt, T>& in1, const qlm::Pixel<frmt, T>& in2)
    {
        uint64_t result {0};

        using type_t = qlm::wider_t<qlm::signed_t<T>>;


        if constexpr (frmt == qlm::ImageFormat::GRAY)
        {
            result = std::pow(((type_t)in1.v - in2.v), 2);
           
        }
        else if constexpr (frmt == qlm::ImageFormat::RGB)
        {
            result = std::pow(((type_t)in1.r - in2.r), 2);
            result += std::pow(((type_t)in1.g - in2.g), 2);
            result += std::pow(((type_t)in1.b - in2.b), 2);
        }
        else if constexpr (frmt == qlm::ImageFormat::HLS)
        {
            result = std::pow(((type_t)in1.h - in2.h), 2);
            result += std::pow(((type_t)in1.l - in2.l), 2));
            result += std::pow(((type_t)in1.s - in2.s), 2));
        }
        else
        {
            result = std::pow(((type_t)in1.h - in2.h), 2);
            result += std::pow(((type_t)in1.v - in2.v), 2));
            result += std::pow(((type_t)in1.s - in2.s), 2));
        }

        return std::sqrt(result);
    }
}