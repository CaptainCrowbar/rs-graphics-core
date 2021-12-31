#pragma once

#include "rs-graphics-core/maths.hpp"
#include "rs-graphics-core/matrix.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-format/enum.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <type_traits>

namespace RS::Graphics::Core {

    RS_DEFINE_ENUM_CLASS(ChannelMode, uint8_t, 0,
        unconstrained,  // Any real value
        non_negative,   // Non-negative real value
        unit,           // Unit interval [0,1]
        circle,         // Circular interval [0,1)
    )

    struct ChannelSpec {
        char id;
        ChannelMode mode;
    };

    namespace Detail {

        template <typename T>
        constexpr bool is_channel_in_gamut(T t, ChannelMode cm) noexcept {
            static_assert(std::is_floating_point_v<T>);
            switch (cm) {
                case ChannelMode::non_negative:  return t >= 0;
                case ChannelMode::unit:          return t >= 0 && t <= 1;
                case ChannelMode::circle:        return t >= 0 && t < 1;
                default:                         return true;
            }
        };

        template <typename ColourSpace, typename T, int N>
        constexpr bool is_colour_in_gamut(Vector<T, N> colour) noexcept {
            static_assert(std::is_floating_point_v<T>);
            static_assert(N == ColourSpace::n_channels);
            for (int i = 0; i < N; ++i)
                if (! is_channel_in_gamut(colour[i], ColourSpace::channels[i].mode))
                    return false;
            return true;
        }

        template <typename T>
        constexpr void clamp_channel(T& t, ChannelMode cm) noexcept {
            static_assert(std::is_floating_point_v<T>);
            switch (cm) {
                case ChannelMode::non_negative:
                    if (t < 0)
                        t = 0;
                    break;
                case ChannelMode::unit:
                    if (t < 0)
                        t = 0;
                    else if (t > 1)
                        t = 1;
                    break;
                case ChannelMode::circle:
                    t = fraction(t);
                    break;
                default:
                    break;
            }
        };

        template <typename ColourSpace, typename T, int N>
        constexpr void clamp_colour(Vector<T, N>& colour) noexcept {
            static_assert(std::is_floating_point_v<T>);
            static_assert(N == ColourSpace::n_channels);
            for (int i = 0; i < N; ++i)
                clamp_channel(colour[i], ColourSpace::channels[i].mode);
        }

    }

    // Colour space concept:
    // - using base = [colour space]
    // - static constexpr int n_channels
    // - static constexpr std::array<ChannelSpec, n_channels> channels
    // - Vector<T, n_channels> from_base(Vector<T, 3> colour) const
    // - Vector<T, 3> to_base(Vector<T, n_channels> colour) const

    class CIEXYZ {
    public:
        using base = CIEXYZ;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'X', ChannelMode::unit },
            { 'Y', ChannelMode::unit },
            { 'Z', ChannelMode::unit },
        }};
        template <typename T> constexpr Vector<T, 3> from_base(Vector<T, 3> colour) const noexcept { return colour; }
        template <typename T> constexpr Vector<T, 3> to_base(Vector<T, 3> colour) const noexcept { return colour; }
    };

    class CIExyY {
    public:
        using base = CIEXYZ;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'x', ChannelMode::unit },
            { 'y', ChannelMode::unit },
            { 'Y', ChannelMode::unit },
        }};
        template <typename T> constexpr Vector<T, 3> from_base(Vector<T, 3> colour) const noexcept {
            Vector<T, 3> out;
            T sum = colour.x() + colour.y() + colour.z();
            if (sum != 0) {
                out.x() = colour.x() / sum;
                out.y() = colour.y() / sum;
                out.z() = colour.y();
            }
            return out;
        }
        template <typename T> constexpr Vector<T, 3> to_base(Vector<T, 3> colour) const noexcept {
            Vector<T, 3> out;
            T scale = colour.z() / colour.y();
            out.x() = scale * colour.x();
            out.y() = colour.z();
            out.z() = scale * (1 - colour.x() - colour.y());
            return out;
        }
    };

    template <int64_t M00, int64_t M01, int64_t M02,
        int64_t M10, int64_t M11, int64_t M12,
        int64_t M20, int64_t M21, int64_t M22,
        int64_t Divisor>
    class WorkingSpace {
    public:
        using base = CIEXYZ;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'R', ChannelMode::unit },
            { 'G', ChannelMode::unit },
            { 'B', ChannelMode::unit },
        }};
        template <typename T> constexpr Vector<T, 3> from_base(Vector<T, 3> colour) const noexcept {
            return xyz_to_rgb_matrix<T> * colour;
        }
        template <typename T> constexpr Vector<T, 3> to_base(Vector<T, 3> colour) const noexcept {
            return rgb_to_xyz_matrix<T> * colour;
        }
    private:
        template <typename T> static constexpr auto rgb_to_xyz_matrix = Matrix<T, 3, MatrixLayout::row>
            (T(M00), T(M01), T(M02), T(M10), T(M11), T(M12), T(M20), T(M21), T(M22)) / T(Divisor);
        template <typename T> static constexpr auto xyz_to_rgb_matrix = rgb_to_xyz_matrix<T>.inverse();
    };

    template <typename WorkingSpace, int64_t GammaNumerator, int64_t GammaDenominator>
    class NonlinearSpace {
    public:
        using base = WorkingSpace;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'R', ChannelMode::unit },
            { 'G', ChannelMode::unit },
            { 'B', ChannelMode::unit },
        }};
        template <typename T> Vector<T, 3> from_base(Vector<T, 3> colour) const noexcept {
            static constexpr T inverse_gamma = T(GammaDenominator) / T(GammaNumerator);
            for (auto& c: colour)
                c = std::pow(std::max(c, T(0)), inverse_gamma);
            return colour;
        }
        template <typename T> Vector<T, 3> to_base(Vector<T, 3> colour) const noexcept {
            static constexpr T gamma = T(GammaNumerator) / T(GammaDenominator);
            for (auto& c: colour)
                c = std::pow(std::max(c, T(0)), gamma);
            return colour;
        }
    };

    // Source for matrices:
    // http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
    // http://www.brucelindbloom.com/index.html?WorkingSpaceInfo.html

    using LinearRGB = WorkingSpace<
         4'124'564,  3'575'761,  1'804'375,
         2'126'729,  7'151'522,    721'750,
           193'339,  1'191'920,  9'503'041,
        10'000'000
    >;

    class sRGB {
    public:
        using base = LinearRGB;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'R', ChannelMode::unit },
            { 'G', ChannelMode::unit },
            { 'B', ChannelMode::unit },
        }};
        template <typename T> Vector<T, 3> from_base(Vector<T, 3> colour) const noexcept {
            static constexpr T a = T(0.003'130'8);
            static constexpr T b = T(12.92);
            static constexpr T c = T(0.055);
            static constexpr T d = c + 1;
            static constexpr T e = 1 / T(2.4);
            for (auto& x: colour) {
                if (x < a)
                    x *= b;
                else
                    x = d * std::pow(x, e) - c;
            }
            return colour;
        }
        template <typename T> Vector<T, 3> to_base(Vector<T, 3> colour) const noexcept {
            static constexpr T a = T(0.040'45);
            static constexpr T b = 1 / T(12.92);
            static constexpr T c = T(0.055);
            static constexpr T d = 1 / (c + 1);
            static constexpr T e = T(2.4);
            for (auto& x: colour) {
                if (x < a)
                    x *= b;
                else
                    x = std::pow((x + c) * d, e);
            }
            return colour;
        }
    };

    using LinearAdobeRGB = WorkingSpace<
         5'767'309,  1'855'540,  1'881'852,
         2'973'769,  6'273'491,    752'741,
           270'343,    706'872,  9'911'085,
        10'000'000
    >;

    using AdobeRGB = NonlinearSpace<LinearAdobeRGB, 22, 10>;

    using LinearWideGamut = WorkingSpace<
         7'161'046,  1'009'296,  1'471'858,
         2'581'874,  7'249'378,    168'748,
                 0,    517'813,  7'734'287,
        10'000'000
    >;

    using WideGamut = NonlinearSpace<LinearWideGamut, 563, 256>;

    class CIELab {
    public:
        using base = CIEXYZ;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'L', ChannelMode::non_negative },
            { 'a', ChannelMode::unconstrained },
            { 'b', ChannelMode::unconstrained },
        }};
        template <typename T> Vector<T, 3> from_base(Vector<T, 3> colour) const noexcept {
            Vector<T, 3> out;
            colour /= illuminant<T>;
            out[0] = 116 * f(colour.y()) - 16;
            out[1] = 500 * (f(colour.x()) - f(colour.y()));
            out[2] = 200 * (f(colour.y()) - f(colour.z()));
            return out;
        }
        template <typename T> Vector<T, 3> to_base(Vector<T, 3> colour) const noexcept {
            Vector<T, 3> out;
            T lx = (colour[0] + 16) / 116;
            out.x() = inverse_f(lx + colour[1] / 500);
            out.y() = inverse_f(lx);
            out.z() = inverse_f(lx - colour[2] / 200);
            return out * illuminant<T>;
        }
    private:
        template <typename T> static constexpr Vector<T, 3> illuminant = {T(0.950489),T(1),T(1.088840)}; // D65
        template <typename T> static constexpr T delta = T(6) / T(29);
        template <typename T> static constexpr T c1 = delta<T> * delta<T> * delta<T>;
        template <typename T> static constexpr T c2 = 3 * delta<T> * delta<T>;
        template <typename T> static constexpr T c3 = T(4) / T(29);
        template <typename T> static T f(T t) noexcept {
            if (t <= c1<T>)
                return t / c2<T> + c3<T>;
            else
                return std::cbrt(t);
        }
        template <typename T> static T inverse_f(T t) noexcept {
            if (t <= delta<T>)
                return c2<T> * (t - c3<T>);
            else
                return t * t * t;
        }
    };

    class CIELuv {
    public:
        using base = CIEXYZ;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'L', ChannelMode::non_negative },
            { 'u', ChannelMode::unconstrained },
            { 'v', ChannelMode::unconstrained },
        }};
        template <typename T> constexpr Vector<T, 3> from_base(Vector<T, 3> colour) const noexcept {
            Vector<T, 3> out;
            T y = colour.y() / illuminant<T>.y();
            if (y <= c1<T>)
                out[0] = c3<T> * y;
            else
                out[0] = 116 * std::cbrt(y) - 16;
            out[1] = 13 * out[0] * (u_prime(colour) - u_prime_n<T>);
            out[2] = 13 * out[0] * (v_prime(colour) - v_prime_n<T>);
            return out;
        }
        template <typename T> constexpr Vector<T, 3> to_base(Vector<T, 3> colour) const noexcept {
            Vector<T, 3> out;
            T u = colour[1] / (13 * colour[0]) + u_prime_n<T>;
            T v = colour[2] / (13 * colour[0]) + v_prime_n<T>;
            if (colour[0] <= 8)
                out[1] = c4<T> * colour[0];
            else
                out[1] = std::pow((colour[0] + 16) / 116, T(3));
            out[1] *= illuminant<T>.y();
            out[0] = out[1] * 9 * u / (4 * v);
            out[2] = out[1] * (12 - 3 * u - 20 * v) / (4 * v);
            return out;
        }
    private:
        template <typename T> static constexpr Vector<T, 3> illuminant = {T(0.950489),T(1),T(1.088840)}; // D65
        template <typename T> static constexpr T delta = T(6) / T(29);
        template <typename T> static constexpr T c1 = delta<T> * delta<T> * delta<T>;
        template <typename T> static constexpr T c2 = T(29) / T(3);
        template <typename T> static constexpr T c3 = c2<T> * c2<T> * c2<T>;
        template <typename T> static constexpr T c4 = 1 / c3<T>;
        template <typename T> static constexpr T u_prime(Vector<T, 3> xyz) noexcept {
            return 4 * xyz.x() / (xyz.x() + 15 * xyz.y() + 3 * xyz.z());
        }
        template <typename T> static constexpr T v_prime(Vector<T, 3> xyz) noexcept {
            return 9 * xyz.y() / (xyz.x() + 15 * xyz.y() + 3 * xyz.z());
        }
        template <typename T> static constexpr T u_prime_n = u_prime(illuminant<T>);
        template <typename T> static constexpr T v_prime_n = v_prime(illuminant<T>);
    };

    namespace Detail {

        template <typename T>
        constexpr void rgb_to_hcv(T r, T g, T b, T& h, T& c, T& v) noexcept {
            v = std::max({r, g, b});
            c = v - std::min({r, g, b});
            if (c == 0)
                h = 0;
            else if (v == r)
                h = (g - b) / c;
            else if (v == g)
                h = (b - r) / c + 2;
            else
                h = (r - g) / c + 4;
            h = euclidean_remainder(h, T(6)) / 6;
        }

        template <typename T>
        constexpr void hcm_to_rgb(T h, T c, T m, T& r, T& g, T& b) noexcept {
            h *= 6;
            T x = c * (1 - const_abs(euclidean_remainder(h, T(2)) - 1));
            r = g = b = m;
            switch (int(h)) {
                case 0:   r += c; g += x; break;
                case 1:   r += x; g += c; break;
                case 2:   g += c; b += x; break;
                case 3:   g += x; b += c; break;
                case 4:   r += x; b += c; break;
                default:  r += c; b += x; break;
            }
        }

    }

    class HSL {
    public:
        using base = LinearRGB;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'H', ChannelMode::circle },
            { 'S', ChannelMode::unit },
            { 'L', ChannelMode::unit },
        }};
        template <typename T> constexpr Vector<T, 3> from_base(Vector<T, 3> colour) const noexcept {
            Vector<T, 3> out;
            T c, v;
            Detail::rgb_to_hcv(colour[0], colour[1], colour[2], out[0], c, v);
            out[2] = (T(2) * v - c) / T(2);
            if (c != 0)
                out[1] = c / (T(1) - const_abs(T(2) * out[2] - T(1)));
            return out;
        }
        template <typename T> constexpr Vector<T, 3> to_base(Vector<T, 3> colour) const noexcept {
            Vector<T, 3> out;
            T c = (T(1) - const_abs(T(2) * colour[2] - T(1))) * colour[1];
            T m = colour[2] - c / T(2);
            Detail::hcm_to_rgb(colour[0], c, m, out[0], out[1], out[2]);
            return out;
        }
    };

    class HSV {
    public:
        using base = LinearRGB;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'H', ChannelMode::circle },
            { 'S', ChannelMode::unit },
            { 'V', ChannelMode::unit },
        }};
        template <typename T> constexpr Vector<T, 3> from_base(Vector<T, 3> colour) const noexcept {
            Vector<T, 3> out;
            T c;
            Detail::rgb_to_hcv(colour[0], colour[1], colour[2], out[0], c, out[2]);
            if (c != 0)
                out[1] = c / out[2];
            return out;
        }
        template <typename T> constexpr Vector<T, 3> to_base(Vector<T, 3> colour) const noexcept {
            Vector<T, 3> out;
            T c = colour[2] * colour[1];
            T m = colour[2] - c;
            Detail::hcm_to_rgb(colour[0], c, m, out[0], out[1], out[2]);
            return out;
        }
    };

}
