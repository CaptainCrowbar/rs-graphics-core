#include "rs-graphics-core/colour-space.hpp"
#include "rs-graphics-core/vector.hpp"
#include <vector>

namespace RS::Graphics::Core::Test {

    // Source for CIE and nonlinear RGB colour space samples:
    // https://www.easyrgb.com/en/convert.php

    // Colours are selected to be in gamut for all colour spaces of interest.
    // Differences in 4th-5th decimal place are probably due to differences
    // in matrix approximations between this source and Lindbloom.

    struct Sample {
        Double3 sRGB;
        Double3 AdobeRGB;
        Double3 CIEXYZ;
        Double3 CIExyY;
        Double3 CIELab;
        Double3 CIELuv;
        Double3 HSL;
        Double3 HSV;
    };

    inline const std::vector<Sample>& samples() {
        static const std::vector<Sample> samples = {
            // sRGB             AdobeRGB                    CIEXYZ                      CIExyY                      CIELab                    CIELuv                     HSL                            HSV
            {{0.00,0.00,0.00},  {0.00000,0.00000,0.00000},  {0.00000,0.00000,0.00000},  {0.00000,0.00000,0.00000},  {0.000,0.000,0.000},      {0.000,0.000,0.000},       {0.000000,0.000000,0.000000},  {0.000000,0.000000,0.000000}},
            {{0.25,0.50,0.75},  {0.34673,0.49610,0.73614},  {0.19180,0.20160,0.52305},  {0.20929,0.21998,0.20160},  {52.018,0.093,-39.363},   {52.018,-25.362,-60.283},  {0.609011,0.822549,0.286699},  {0.609011,0.902636,0.522522}},
            {{0.25,0.75,0.50},  {0.46453,0.74442,0.50928},  {0.24645,0.39995,0.26667},  {0.26991,0.43803,0.39995},  {69.466,-49.554,22.224},  {69.466,-52.311,38.425},   {0.390990,0.822506,0.286702},  {0.390990,0.902610,0.522517}},
            {{0.50,0.25,0.75},  {0.44385,0.25813,0.73171},  {0.20076,0.11962,0.50676},  {0.24271,0.14462,0.11962},  {41.155,51.411,-56.449},  {41.155,16.371,-86.719},   {0.724326,0.822542,0.286701},  {0.724326,0.902631,0.522525}},
            {{0.50,0.75,0.25},  {0.58009,0.74442,0.29901},  {0.28430,0.42288,0.11477},  {0.34589,0.51448,0.42288},  {71.069,-40.908,55.644},  {71.069,-32.079,71.662},   {0.275685,0.822541,0.286707},  {0.275685,0.902631,0.522534}},
            {{0.50,0.25,0.75},  {0.44385,0.25813,0.73171},  {0.20076,0.11962,0.50676},  {0.24271,0.14462,0.11962},  {41.155,51.411,-56.449},  {41.155,16.371,-86.719},   {0.724326,0.822542,0.286701},  {0.724326,0.902631,0.522525}},
            {{0.75,0.25,0.50},  {0.65033,0.25813,0.48896},  {0.27233,0.16296,0.21957},  {0.41586,0.24885,0.16296},  {47.360,56.522,-8.041},   {47.360,76.888,-20.831},   {0.942342,0.822535,0.286699},  {0.942342,0.902628,0.522518}},
            {{0.75,0.50,0.25},  {0.68464,0.49610,0.27311},  {0.30123,0.26787,0.08396},  {0.46126,0.41017,0.26787},  {58.777,18.585,43.798},   {58.777,50.243,45.130},    {0.057661,0.822550,0.286693},  {0.057661,0.902636,0.522512}},
            {{0.85,0.85,0.85},  {0.84589,0.84589,0.84589},  {0.65779,0.69207,0.75355},  {0.31273,0.32902,0.69207},  {86.607,0.000,-0.000},    {86.607,-0.000,-0.000},    {0.527616,0.000018,0.692068},  {0.527616,0.000016,0.692074}},
        };
        return samples;
    }

}
