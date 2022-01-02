#pragma once

#include "rs-graphics-core/colour-space.hpp"
#include "rs-graphics-core/maths.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-format/enum.hpp"
#include "rs-format/format.hpp"
#include <functional>
#include <limits>
#include <ostream>
#include <string>
#include <type_traits>

namespace RS::Graphics::Core {

    RS_DEFINE_ENUM_CLASS(ColourLayout, int, 0,
        forward,
        forward_alpha,
        alpha_forward,
        reverse,
        reverse_alpha,
        alpha_reverse
    )

    template <typename VT, typename CS, ColourLayout CL> class Colour;

    namespace Detail {

        template <typename CS, char CH, int Offset = 0>
        struct ColourSpaceChannelIndex {
            static constexpr int cs_channels = int(CS::channels.size());
            static constexpr int get() noexcept {
                if constexpr (Offset >= cs_channels)
                    return -1;
                else if constexpr (CS::channels[Offset] == CH)
                    return Offset;
                else
                    return ColourSpaceChannelIndex<CS, CH, Offset + 1>::get();
            }
        };

        template <typename CS, char CH, ColourLayout CL>
        struct ColourChannelIndex {
            static constexpr int cs_channels = int(CS::channels.size());
            static constexpr int cs_index = ColourSpaceChannelIndex<CS, CH>::get();
            static constexpr int get() noexcept {
                if constexpr (cs_index == -1)
                    return -1;
                else if constexpr (CL == ColourLayout::forward || CL == ColourLayout::forward_alpha)
                    return cs_index;
                else if constexpr (CL == ColourLayout::alpha_forward)
                    return cs_index + 1;
                else if constexpr (CL == ColourLayout::reverse || CL == ColourLayout::reverse_alpha)
                    return cs_channels - cs_index - 1;
                else
                    return cs_channels - cs_index;
            }
        };

        template <typename CS, char CH, ColourLayout CL>
        constexpr int colour_channel_index = ColourChannelIndex<CS, CH, CL>::get();

        template <typename VT, typename CS, ColourLayout CL, bool IsLinear = CS::is_linear>
        class ColourArithmetic {
        private:
            static constexpr bool ca_has_alpha = CL != ColourLayout::forward && CL != ColourLayout::reverse;
            static constexpr int ca_channels = int(CS::channels.size()) + int(ca_has_alpha);
            using ca_colour = Colour<VT, CS, CL>;
            using ca_vector = Vector<VT, ca_channels>;
        public:
            friend constexpr ca_colour operator+(ca_colour c) noexcept { return c; }
            friend constexpr ca_colour operator-(ca_colour c) noexcept { return ca_colour(- c.vec_); }
            friend constexpr ca_colour operator+(ca_colour a, ca_colour b) noexcept { return ca_colour(a.vec_ + b.vec_); }
            friend constexpr ca_colour operator-(ca_colour a, ca_colour b) noexcept { return ca_colour(a.vec_ - b.vec_); }
            friend constexpr ca_colour operator*(ca_colour a, VT b) noexcept { return ca_colour(a.vec_ * b); }
            friend constexpr ca_colour operator*(VT a, ca_colour b) noexcept { return ca_colour(a * b.vec_); }
            friend constexpr ca_colour operator/(ca_colour a, VT b) noexcept { return ca_colour(a.vec_ / b); }
            friend constexpr ca_colour operator*(ca_colour a, ca_vector b) noexcept { return ca_colour(a.vec_ * b); }
            friend constexpr ca_colour operator*(ca_vector a, ca_colour b) noexcept { return ca_colour(a * b.vec_); }
            friend constexpr ca_colour operator/(ca_colour a, ca_vector b) noexcept { return ca_colour(a.vec_ / b); }
            friend constexpr ca_colour& operator+=(ca_colour& a, ca_colour b) noexcept { return a = a + b; }
            friend constexpr ca_colour& operator-=(ca_colour& a, ca_colour b) noexcept { return a = a - b; }
            friend constexpr ca_colour& operator*=(ca_colour& a, VT b) noexcept { return a = a * b; }
            friend constexpr ca_colour& operator/=(ca_colour& a, VT b) noexcept { return a = a / b; }
            friend constexpr ca_colour& operator*=(ca_colour& a, ca_vector b) noexcept { return a = a * b; }
            friend constexpr ca_colour& operator/=(ca_colour& a, ca_vector b) noexcept { return a = a / b; }
        };

        template <typename VT, typename CS, ColourLayout CL>
        class ColourArithmetic<VT, CS, CL, false> {};

        template <typename T>
        using FloatingChannelType =
            std::conditional_t<std::is_floating_point_v<T>, T,
            std::conditional_t<(sizeof(T) < sizeof(float)), float,
            std::conditional_t<(sizeof(T) < sizeof(double)), double, long double>>>;

        template <typename T1, typename T2>
        using WorkingChannelType =
            std::conditional_t<(sizeof(FloatingChannelType<T1>) >= sizeof(FloatingChannelType<T2>)),
            FloatingChannelType<T1>, FloatingChannelType<T2>>;

        template <typename WT, typename T1>
        constexpr WT channel_to_working_type(T1 c, T1 scale) noexcept {
            return WT(c) / WT(scale);
        }

        template <typename T2, typename WT>
        constexpr T2 working_type_to_channel(WT w, T2 scale) noexcept {
            w *= WT(scale);
            if constexpr (std::is_integral_v<T2>)
                return const_round<T2>(w);
            else
                return T2(w);
        }

        template <typename T2, typename T1>
        constexpr T2 round_channel(T1 t) noexcept {
            if constexpr (std::is_integral_v<T2>)
                return const_round<T2>(t);
            else
                return T2(t);
        }

    }

    // Don't use single letter template parameters here

    template <typename VT, typename CS = LinearRGB, ColourLayout CL = ColourLayout::forward_alpha>
    class Colour:
    public Detail::ColourArithmetic<VT, CS, CL> {

    public:

        static_assert(std::is_arithmetic_v<VT>);

        static constexpr int colour_space_channels = int(CS::channels.size());
        static constexpr int alpha_index = CL == ColourLayout::alpha_forward || CL == ColourLayout::alpha_reverse ? 0 :
            CL == ColourLayout::forward_alpha || CL == ColourLayout::reverse_alpha ? colour_space_channels : -1;
        static constexpr bool has_alpha = alpha_index != -1;
        static constexpr int channels = colour_space_channels + int(has_alpha);
        static constexpr bool is_hdr = std::is_floating_point_v<VT>;
        static constexpr ColourLayout layout = CL;
        static constexpr VT scale = is_hdr || CS::shape == ColourSpace::any ? VT(1) : std::numeric_limits<VT>::max();

        using colour_space = CS;
        using iterator = VT*;
        using const_iterator = const VT*;
        using value_type = VT;
        using vector_type = Vector<VT, channels>;
        using partial_vector_type = Vector<VT, colour_space_channels>;

        constexpr Colour() noexcept {}
        explicit constexpr Colour(VT x) noexcept: vec_(x) { if constexpr (has_alpha) alpha() = scale; }
        template <typename V2 = VT> constexpr Colour(V2 x, std::enable_if_t<has_alpha, V2> a) noexcept: vec_(x) { alpha() = a; }
        explicit constexpr Colour(vector_type v) noexcept: vec_(v) {}

        template <typename... Args>
        constexpr Colour(VT x,
            std::enable_if_t<sizeof...(Args) + 2 == channels && (std::is_convertible_v<Args, VT> && ...), VT> y,
            Args... args) noexcept:
            vec_(VT(x), VT(y), VT(args)...) {}

        template <typename... Args>
        constexpr Colour(VT x,
            std::enable_if_t<has_alpha && sizeof...(Args) + 2 == colour_space_channels && (std::is_convertible_v<Args, VT> && ...), VT> y,
            Args... args) noexcept:
            vec_(VT(x), VT(y), VT(args)..., scale) {}

        template <typename V2 = VT>
        constexpr VT& alpha(std::enable_if<Detail::SfinaeBoolean<V2, has_alpha>::value>* = nullptr) noexcept {
            return vec_[alpha_index];
        }

        constexpr const VT& alpha() const noexcept {
            if constexpr (has_alpha)
                return vec_[alpha_index];
            else
                return scale;
        }

        #define RS_GRAPHICS_COLOUR_CHANNEL(Ch, Lit) \
            template <typename V2 = VT> \
            constexpr VT& Ch(std::enable_if<Detail::SfinaeBoolean<V2, \
                    Detail::colour_channel_index<CS, Lit, CL> != -1>::value>* = nullptr) noexcept { \
                return vec_[Detail::colour_channel_index<CS, Lit, CL>]; \
            } \
            template <typename V2 = VT> \
            constexpr const VT& Ch(std::enable_if<Detail::SfinaeBoolean<V2, \
                    Detail::colour_channel_index<CS, Lit, CL> != -1>::value>* = nullptr) const noexcept { \
                return vec_[Detail::colour_channel_index<CS, Lit, CL>]; \
            }

        RS_GRAPHICS_COLOUR_CHANNEL(a, 'a')  RS_GRAPHICS_COLOUR_CHANNEL(A, 'A')
        RS_GRAPHICS_COLOUR_CHANNEL(b, 'b')  RS_GRAPHICS_COLOUR_CHANNEL(B, 'B')
        RS_GRAPHICS_COLOUR_CHANNEL(c, 'c')  RS_GRAPHICS_COLOUR_CHANNEL(C, 'C')
        RS_GRAPHICS_COLOUR_CHANNEL(d, 'd')  RS_GRAPHICS_COLOUR_CHANNEL(D, 'D')
        RS_GRAPHICS_COLOUR_CHANNEL(e, 'e')  RS_GRAPHICS_COLOUR_CHANNEL(E, 'E')
        RS_GRAPHICS_COLOUR_CHANNEL(f, 'f')  RS_GRAPHICS_COLOUR_CHANNEL(F, 'F')
        RS_GRAPHICS_COLOUR_CHANNEL(g, 'g')  RS_GRAPHICS_COLOUR_CHANNEL(G, 'G')
        RS_GRAPHICS_COLOUR_CHANNEL(h, 'h')  RS_GRAPHICS_COLOUR_CHANNEL(H, 'H')
        RS_GRAPHICS_COLOUR_CHANNEL(i, 'i')  RS_GRAPHICS_COLOUR_CHANNEL(I, 'I')
        RS_GRAPHICS_COLOUR_CHANNEL(j, 'j')  RS_GRAPHICS_COLOUR_CHANNEL(J, 'J')
        RS_GRAPHICS_COLOUR_CHANNEL(k, 'k')  RS_GRAPHICS_COLOUR_CHANNEL(K, 'K')
        RS_GRAPHICS_COLOUR_CHANNEL(l, 'l')  RS_GRAPHICS_COLOUR_CHANNEL(L, 'L')
        RS_GRAPHICS_COLOUR_CHANNEL(m, 'm')  RS_GRAPHICS_COLOUR_CHANNEL(M, 'M')
        RS_GRAPHICS_COLOUR_CHANNEL(n, 'n')  RS_GRAPHICS_COLOUR_CHANNEL(N, 'N')
        RS_GRAPHICS_COLOUR_CHANNEL(o, 'o')  RS_GRAPHICS_COLOUR_CHANNEL(O, 'O')
        RS_GRAPHICS_COLOUR_CHANNEL(p, 'p')  RS_GRAPHICS_COLOUR_CHANNEL(P, 'P')
        RS_GRAPHICS_COLOUR_CHANNEL(q, 'q')  RS_GRAPHICS_COLOUR_CHANNEL(Q, 'Q')
        RS_GRAPHICS_COLOUR_CHANNEL(r, 'r')  RS_GRAPHICS_COLOUR_CHANNEL(R, 'R')
        RS_GRAPHICS_COLOUR_CHANNEL(s, 's')  RS_GRAPHICS_COLOUR_CHANNEL(S, 'S')
        RS_GRAPHICS_COLOUR_CHANNEL(t, 't')  RS_GRAPHICS_COLOUR_CHANNEL(T, 'T')
        RS_GRAPHICS_COLOUR_CHANNEL(u, 'u')  RS_GRAPHICS_COLOUR_CHANNEL(U, 'U')
        RS_GRAPHICS_COLOUR_CHANNEL(v, 'v')  RS_GRAPHICS_COLOUR_CHANNEL(V, 'V')
        RS_GRAPHICS_COLOUR_CHANNEL(w, 'w')  RS_GRAPHICS_COLOUR_CHANNEL(W, 'W')
        RS_GRAPHICS_COLOUR_CHANNEL(x, 'x')  RS_GRAPHICS_COLOUR_CHANNEL(X, 'X')
        RS_GRAPHICS_COLOUR_CHANNEL(y, 'y')  RS_GRAPHICS_COLOUR_CHANNEL(Y, 'Y')
        RS_GRAPHICS_COLOUR_CHANNEL(z, 'z')  RS_GRAPHICS_COLOUR_CHANNEL(Z, 'Z')

        VT& operator[](int i) noexcept { return vec_[i]; }
        const VT& operator[](int i) const noexcept { return vec_[i]; }

        constexpr VT& cs(int i) noexcept { return vec_[space_to_layout_index(i)]; }
        constexpr const VT& cs(int i) const noexcept { return vec_[space_to_layout_index(i)]; }

        constexpr vector_type as_vector() const noexcept { return vec_; }
        constexpr partial_vector_type partial_vector() const noexcept { return partial_vector_type(vec_.begin()); }

        constexpr VT* begin() noexcept { return vec_.begin(); }
        constexpr const VT* begin() const noexcept { return vec_.begin(); }
        constexpr VT* end() noexcept { return vec_.end(); }
        constexpr const VT* end() const noexcept { return vec_.end(); }

        constexpr void clamp() noexcept {
            if constexpr (has_alpha) {
                auto pv = partial_vector();
                clamp_colour<CS>(pv, scale);
                int offset = int(CL == ColourLayout::alpha_forward || CL == ColourLayout::alpha_reverse);
                for (int i = 0; i < colour_space_channels; ++i)
                    vec_[i + offset] = pv[i];
                if (alpha() < 0)
                    alpha() = 0;
                else if (alpha() > scale)
                    alpha() = scale;
            } else {
                clamp_colour<CS>(vec_, scale);
            }
        }

        constexpr Colour clamped() const noexcept { auto c = *this; c.clamp(); return c; }

        constexpr bool is_clamped() const noexcept {
            if (! is_colour_in_gamut<CS>(partial_vector(), scale))
                return false;
            if constexpr (has_alpha)
                if (alpha() < 0 || alpha() > scale)
                    return false;
            return true;
        }

        constexpr bool empty() const noexcept { return false; }
        constexpr size_t size() const noexcept { return size_t(channels); }

        size_t hash() const noexcept { return vec_.hash(); }

        template <typename V2 = VT>
        constexpr Colour multiply_alpha(std::enable_if<Detail::SfinaeBoolean<V2,
                has_alpha && CS::is_linear && CS::is_rgb
                && CS::shape == ColourSpace::unit>::value>* = nullptr) const noexcept {
            using FT = Detail::FloatingChannelType<VT>;
            auto result = *this;
            FT factor = FT(alpha()) / FT(scale);
            for (auto& x: result.vec_)
                x = Detail::round_channel<VT>(factor * x);
            result.alpha() = alpha();
            return result;
        }

        template <typename V2 = VT>
        constexpr Colour unmultiply_alpha(std::enable_if<Detail::SfinaeBoolean<V2,
                has_alpha && CS::is_linear && CS::is_rgb
                && CS::shape == ColourSpace::unit>::value>* = nullptr) const noexcept {
            using FT = Detail::FloatingChannelType<VT>;
            auto result = *this;
            FT factor = FT(scale) / FT(alpha());
            for (auto& x: result.vec_)
                x = Detail::round_channel<VT>(factor * x);
            result.alpha() = alpha();
            return result;
        }

        std::string str(RS::Format::FormatSpec spec = {}) const { return vec_.str(spec); }
        friend std::ostream& operator<<(std::ostream& out, const Colour& c) { return out << c.str(); }

        template <typename V2 = VT>
            static Colour black(std::enable_if<Detail::SfinaeBoolean<V2, CS::is_rgb>::value>* = nullptr) noexcept
            { return Colour(0); }
        template <typename V2 = VT>
            static Colour white(std::enable_if<Detail::SfinaeBoolean<V2, CS::is_rgb>::value>* = nullptr) noexcept
            { return Colour(scale); }
        template <typename V2 = VT>
            static Colour red(std::enable_if<Detail::SfinaeBoolean<V2, CS::is_rgb>::value>* = nullptr) noexcept
            { return {scale, 0, 0}; }
        template <typename V2 = VT>
            static Colour yellow(std::enable_if<Detail::SfinaeBoolean<V2, CS::is_rgb>::value>* = nullptr) noexcept
            { return {scale, scale, 0}; }
        template <typename V2 = VT>
            static Colour green(std::enable_if<Detail::SfinaeBoolean<V2, CS::is_rgb>::value>* = nullptr) noexcept
            { return {0, scale, 0}; }
        template <typename V2 = VT>
            static Colour cyan(std::enable_if<Detail::SfinaeBoolean<V2, CS::is_rgb>::value>* = nullptr) noexcept
            { return {0, scale, scale}; }
        template <typename V2 = VT>
            static Colour blue(std::enable_if<Detail::SfinaeBoolean<V2, CS::is_rgb>::value>* = nullptr) noexcept
            { return {0, 0, scale}; }
        template <typename V2 = VT>
            static Colour magenta(std::enable_if<Detail::SfinaeBoolean<V2, CS::is_rgb>::value>* = nullptr) noexcept
            { return {scale, 0, scale}; }

        friend constexpr bool operator==(Colour a, Colour b) noexcept { return a.vec_ == b.vec_; }
        friend constexpr bool operator!=(Colour a, Colour b) noexcept { return ! (a == b); }

    private:

        friend class Detail::ColourArithmetic<VT, CS, CL>;

        vector_type vec_;

        static constexpr int space_to_layout_index(int cs) noexcept {
            if constexpr (CL <= ColourLayout::forward_alpha)
                return cs;
            else if constexpr (CL == ColourLayout::alpha_forward)
                return (cs + 1) % channels;
            else if constexpr (CL == ColourLayout::reverse_alpha)
                return (2 * colour_space_channels - cs) % channels;
            else
                return channels - cs - 1;
        }

    };

    template <typename VT1, typename CS1, ColourLayout CL1,
        typename VT2, typename CS2, ColourLayout CL2>
    void convert_colour(Colour<VT1, CS1, CL1> in, Colour<VT2, CS2, CL2>& out) noexcept {

        using C1 = Colour<VT1, CS1, CL1>;
        using C2 = Colour<VT2, CS2, CL2>;

        if constexpr (std::is_same_v<VT1, VT2> && std::is_same_v<CS1, CS2> && CL1 == CL2) {

            out = in;

        } else if constexpr (std::is_same_v<VT1, VT2> && std::is_same_v<CS1, CS2>) {

            for (int i = 0; i < C1::colour_space_channels; ++i)
                out.cs(i) = in.cs(i);
            if constexpr (C2::has_alpha)
                out.alpha() = in.alpha();

        } else {

            using WT = Detail::WorkingChannelType<VT1, VT2>;
            using WC1 = Colour<WT, CS1, ColourLayout::forward_alpha>;
            using WC2 = Colour<WT, CS2, ColourLayout::forward_alpha>;

            // WT is always floating point, so the scale of WC1/2 is always 1

            WC1 wc1;
            for (int i = 0; i < C1::colour_space_channels; ++i)
                wc1.cs(i) = Detail::channel_to_working_type<WT>(in.cs(i), C1::scale);
            wc1.alpha() = Detail::channel_to_working_type<WT>(in.alpha(), C1::scale);

            auto pvec2 = convert_colour_space<CS1, CS2>(wc1.partial_vector());

            WC2 wc2;
            for (int i = 0; i < C2::colour_space_channels; ++i)
                wc2[i] = pvec2[i];
            wc2.alpha() = wc1.alpha();

            for (int i = 0; i < C2::channels; ++i)
                out.cs(i) = Detail::working_type_to_channel(wc2.cs(i), C2::scale);

        }

    }

    using Rgb8 = Colour<uint8_t, LinearRGB, ColourLayout::forward>;
    using Rgb16 = Colour<uint16_t, LinearRGB, ColourLayout::forward>;
    using Rgbf = Colour<float, LinearRGB, ColourLayout::forward>;
    using Rgbd = Colour<double, LinearRGB, ColourLayout::forward>;
    using sRgb8 = Colour<uint8_t, sRGB, ColourLayout::forward>;
    using sRgb16 = Colour<uint16_t, sRGB, ColourLayout::forward>;
    using sRgbf = Colour<float, sRGB, ColourLayout::forward>;
    using sRgbd = Colour<double, sRGB, ColourLayout::forward>;
    using Rgba8 = Colour<uint8_t, LinearRGB, ColourLayout::forward_alpha>;
    using Rgba16 = Colour<uint16_t, LinearRGB, ColourLayout::forward_alpha>;
    using Rgbaf = Colour<float, LinearRGB, ColourLayout::forward_alpha>;
    using Rgbad = Colour<double, LinearRGB, ColourLayout::forward_alpha>;
    using sRgba8 = Colour<uint8_t, sRGB, ColourLayout::forward_alpha>;
    using sRgba16 = Colour<uint16_t, sRGB, ColourLayout::forward_alpha>;
    using sRgbaf = Colour<float, sRGB, ColourLayout::forward_alpha>;
    using sRgbad = Colour<double, sRGB, ColourLayout::forward_alpha>;

    static_assert(std::is_standard_layout_v<Rgb8>);
    static_assert(std::is_standard_layout_v<Rgb16>);
    static_assert(std::is_standard_layout_v<Rgbf>);
    static_assert(std::is_standard_layout_v<Rgbd>);
    static_assert(std::is_standard_layout_v<sRgb8>);
    static_assert(std::is_standard_layout_v<sRgb16>);
    static_assert(std::is_standard_layout_v<sRgbf>);
    static_assert(std::is_standard_layout_v<sRgbd>);
    static_assert(std::is_standard_layout_v<Rgba8>);
    static_assert(std::is_standard_layout_v<Rgba16>);
    static_assert(std::is_standard_layout_v<Rgbaf>);
    static_assert(std::is_standard_layout_v<Rgbad>);
    static_assert(std::is_standard_layout_v<sRgba8>);
    static_assert(std::is_standard_layout_v<sRgba16>);
    static_assert(std::is_standard_layout_v<sRgbaf>);
    static_assert(std::is_standard_layout_v<sRgbad>);

    static_assert(sizeof(Rgb8) == 3);
    static_assert(sizeof(Rgb16) == 6);
    static_assert(sizeof(Rgbf) == 12);
    static_assert(sizeof(Rgbd) == 24);
    static_assert(sizeof(sRgb8) == 3);
    static_assert(sizeof(sRgb16) == 6);
    static_assert(sizeof(sRgbf) == 12);
    static_assert(sizeof(sRgbd) == 24);
    static_assert(sizeof(Rgba8) == 4);
    static_assert(sizeof(Rgba16) == 8);
    static_assert(sizeof(Rgbaf) == 16);
    static_assert(sizeof(Rgbad) == 32);
    static_assert(sizeof(sRgba8) == 4);
    static_assert(sizeof(sRgba16) == 8);
    static_assert(sizeof(sRgbaf) == 16);
    static_assert(sizeof(sRgbad) == 32);

}

namespace std {

    template <typename VT, typename CS, RS::Graphics::Core::ColourLayout CL>
    struct hash<RS::Graphics::Core::Colour<VT, CS, CL>> {
        size_t operator()(const RS::Graphics::Core::Colour<VT, CS, CL>& c) const noexcept { return c.hash(); }
    };

}
