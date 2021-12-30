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
                    t -= int64_t(t);
                    if (t < 0)
                        t += 1;
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
            { 'L', ChannelMode::unit },
            { 'a', ChannelMode::unconstrained },
            { 'b', ChannelMode::unconstrained },
        }};
        template <typename T> constexpr Vector<T, 3> from_base(Vector<T, 3> colour) const noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }
        template <typename T> constexpr Vector<T, 3> to_base(Vector<T, 3> colour) const noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
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
            // TODO
            (void)colour;
            return out;
        }
        template <typename T> constexpr Vector<T, 3> to_base(Vector<T, 3> colour) const noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }
    };

    namespace Detail {

        template <typename T>
        constexpr void rgb_to_hcv(T r, T g, T b, T& h, T& c, T& v) noexcept {
            v = std::max({r, g, b});
            c = v - std::min({r, g, b});
            h = c == 0 ? T(0) : v == r ? (g - b) / c : v == g ? (b - r) / c + 2 : (r - g) / c + 4;
            h = euclidean_remainder(h, T(6)) / 6;
        }

        template <typename T>
        constexpr void hcm_to_rgb(T h, T c, T m, T& r, T& g, T& b) noexcept {
            h *= 6;
            T x = c * (1 - std::abs(euclidean_remainder(h, T(2)) - 1));
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
            { 'H', ChannelMode::unit },
            { 'S', ChannelMode::unit },
            { 'L', ChannelMode::circle },
        }};
        template <typename T> constexpr Vector<T, 3> from_base(Vector<T, 3> colour) const noexcept {
            Vector<T, 3> out;
            T c, v;
            Detail::rgb_to_hcv(colour[0], colour[1], colour[2], out[0], c, v);
            out[2] = (T(2) * v - c) / T(2);
            out[1] = c == T(0) ? T(0) : c / (T(1) - std::abs(T(2) * out[2] - T(1)));
            return out;
        }
        template <typename T> constexpr Vector<T, 3> to_base(Vector<T, 3> colour) const noexcept {
            Vector<T, 3> out;
            T c = (T(1) - std::abs(T(2) * colour[2] - T(1))) * colour[1];
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
            { 'H', ChannelMode::unit },
            { 'S', ChannelMode::unit },
            { 'V', ChannelMode::circle },
        }};
        template <typename T> constexpr Vector<T, 3> from_base(Vector<T, 3> colour) const noexcept {
            Vector<T, 3> out;
            T c;
            Detail::rgb_to_hcv(colour[0], colour[1], colour[2], out[0], c, out[2]);
            out[1] = c == T(0) ? T(0) : c / out[2];
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
