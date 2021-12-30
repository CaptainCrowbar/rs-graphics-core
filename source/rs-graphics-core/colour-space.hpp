#pragma once

#include "rs-graphics-core/maths.hpp"
#include "rs-graphics-core/matrix.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-format/enum.hpp"
#include <array>
#include <type_traits>

namespace RS::Graphics::Core {

    RS_DEFINE_ENUM_CLASS(ChannelMode, uint8_t, 0,
        unconstrained,  // Any real value
        non_negative,   // Non-negative real value
        unit,           // Unit interval [0,1]
        circle,         // Circular interval [0,2π)
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
                case ChannelMode::circle:        return t >= 0 && t < 2 * pi<T>;
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
                    t = euclidean_remainder(t, 2 * pi<T>);
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

        // http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
        // http://www.brucelindbloom.com/index.html?WorkingSpaceInfo.html

        template <typename T>
        constexpr Matrix<T, 3> linear_srgb_to_ciexyz_matrix() noexcept {
            return Matrix<T, 3, MatrixLayout::row> {
                T(0.4124564), T(0.3575761), T(0.1804375),
                T(0.2126729), T(0.7151522), T(0.0721750),
                T(0.0193339), T(0.1191920), T(0.9503041),
            };
        }

        template <typename T>
        constexpr Matrix<T, 3> ciexyz_to_linear_srgb_matrix() noexcept {
            return linear_srgb_to_ciexyz_matrix<T>().inverse();
        }

    }

    // Colour space concept:
    // - using base = [colour space]
    // - static constexpr int n_channels
    // - static constexpr std::array<ChannelSpec, n_channels> channels
    // - static Vector<T, n_channels> from_base(Vector<T, 3> colour)
    // - static Vector<T, 3> to_base(Vector<T, n_channels> colour)

    class CIEXYZ {
    public:
        using base = CIEXYZ;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'X', ChannelMode::non_negative },
            { 'Y', ChannelMode::non_negative },
            { 'Z', ChannelMode::non_negative },
        }};
        template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept { return colour; }
        template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept { return colour; }
    };

    template <int64_t m00, int64_t m01, int64_t m02,
        int64_t m10, int64_t m11, int64_t m12,
        int64_t m20, int64_t m21, int64_t m22,
        int64_t scale>
    class RGBWorkingSpace {
    public:
        using base = CIEXYZ;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'R', ChannelMode::unit },
            { 'G', ChannelMode::unit },
            { 'B', ChannelMode::unit },
        }};
        template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept {
            return xyz_to_rgb_matrix<T> * colour;
        }
        template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept {
            return rgb_to_xyz_matrix<T> * colour;
        }
    private:
        template <typename T> static constexpr Matrix<T, 3, MatrixLayout::row> rgb_to_xyz_matrix = {
            T(m00) / T(scale), T(m01) / T(scale), T(m02) / T(scale),
            T(m10) / T(scale), T(m11) / T(scale), T(m12) / T(scale),
            T(m20) / T(scale), T(m21) / T(scale), T(m22) / T(scale),
        };
        template <typename T> static constexpr Matrix<T, 3, MatrixLayout::row> xyz_to_rgb_matrix = rgb_to_xyz_matrix<T>.inverse();
    };

    // http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
    // http://www.brucelindbloom.com/index.html?WorkingSpaceInfo.html

    using LinearRGB = RGBWorkingSpace<
        4'124'564,  3'575'761,  1'804'375,
        2'126'729,  7'151'522,  721'750,
        193'339,    1'191'920,  9'503'041,
        10'000'000
    >;

    // class LinearRGB {
    // public:
    //     using base = CIEXYZ;
    //     static constexpr int n_channels = 3;
    //     static constexpr std::array<ChannelSpec, n_channels> channels = {{
    //         { 'R', ChannelMode::unit },
    //         { 'G', ChannelMode::unit },
    //         { 'B', ChannelMode::unit },
    //     }};
    //     template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
    //     template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    // };

    //     template <typename T>
    //     Vector<T, 3> LinearRGB::from_base(Vector<T, 3> colour) noexcept {
    //         return Detail::ciexyz_to_linear_srgb_matrix<T>() * colour;
    //     }

    //     template <typename T>
    //     Vector<T, 3> LinearRGB::to_base(Vector<T, 3> colour) noexcept {
    //         return Detail::linear_srgb_to_ciexyz_matrix<T>() * colour;
    //     }

    class CIELab {
    public:
        using base = CIEXYZ;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'L', ChannelMode::unit },
            { 'a', ChannelMode::unconstrained },
            { 'b', ChannelMode::unconstrained },
        }};
        template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <typename T>
        Vector<T, 3> CIELab::from_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

        template <typename T>
        Vector<T, 3> CIELab::to_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

    class CIELuv {
    public:
        using base = CIEXYZ;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'L', ChannelMode::non_negative },
            { 'u', ChannelMode::unconstrained },
            { 'v', ChannelMode::unconstrained },
        }};
        template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <typename T>
        Vector<T, 3> CIELuv::from_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

        template <typename T>
        Vector<T, 3> CIELuv::to_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

    class CIExyY {
    public:
        using base = CIEXYZ;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'x', ChannelMode::unit },
            { 'y', ChannelMode::unit },
            { 'Y', ChannelMode::non_negative },
        }};
        template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <typename T>
        Vector<T, 3> CIExyY::from_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            T sum = colour[0] + colour[1] + colour[2];
            out[0] = colour[0] / sum;
            out[1] = colour[1] / sum;
            out[2] = colour[1];
            return out;
        }

        template <typename T>
        Vector<T, 3> CIExyY::to_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            T scale = colour[2] / colour[1];
            out[0] = scale * colour[0];
            out[1] = colour[2];
            out[2] = scale * (1 - colour[0] - colour[1]);
            return out;
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
        template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <typename T>
        Vector<T, 3> HSL::from_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

        template <typename T>
        Vector<T, 3> HSL::to_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

    class HSV {
    public:
        using base = LinearRGB;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'H', ChannelMode::unit },
            { 'S', ChannelMode::unit },
            { 'V', ChannelMode::circle },
        }};
        template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <typename T>
        Vector<T, 3> HSV::from_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

        template <typename T>
        Vector<T, 3> HSV::to_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

    class sRGB {
    public:
        using base = LinearRGB;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'R', ChannelMode::unit },
            { 'G', ChannelMode::unit },
            { 'B', ChannelMode::unit },
        }};
        template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <typename T>
        Vector<T, 3> sRGB::from_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

        template <typename T>
        Vector<T, 3> sRGB::to_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

}
