// SPDX-License-Identifier: MPL-2.0
// Copyright © 2020 Skyline Team and Contributors (https://github.com/skyline-emu/)

#include <array>
#include <common.h>
#include "resampler.h"

namespace skyline::audio {
    /**
     * @brief This holds the coefficients of a single output frame
     */
    struct LutEntry {
        i32 a; //!< The coefficient for the first index
        i32 b; //!< The coefficient for the second index
        i32 c; //!< The coefficient for the third index
        i32 d; //!< The coefficient for the fourth index
    };

    // @fmt:off
    constexpr std::array<LutEntry, 128> CurveLut0 = {{
        {6600, 19426, 6722, 3},     {6479, 19424, 6845, 9},     {6359, 19419, 6968, 15},    {6239, 19412, 7093, 22},
        {6121, 19403, 7219, 28},    {6004, 19391, 7345, 34},    {5888, 19377, 7472, 41},    {5773, 19361, 7600, 48},
        {5659, 19342, 7728, 55},    {5546, 19321, 7857, 62},    {5434, 19298, 7987, 69},    {5323, 19273, 8118, 77},
        {5213, 19245, 8249, 84},    {5104, 19215, 8381, 92},    {4997, 19183, 8513, 101},   {4890, 19148, 8646, 109},
        {4785, 19112, 8780, 118},   {4681, 19073, 8914, 127},   {4579, 19031, 9048, 137},   {4477, 18988, 9183, 147},
        {4377, 18942, 9318, 157},   {4277, 18895, 9454, 168},   {4179, 18845, 9590, 179},   {4083, 18793, 9726, 190},
        {3987, 18738, 9863, 202},   {3893, 18682, 10000, 215},  {3800, 18624, 10137, 228},  {3709, 18563, 10274, 241},
        {3618, 18500, 10411, 255},  {3529, 18436, 10549, 270},  {3441, 18369, 10687, 285},  {3355, 18300, 10824, 300},
        {3269, 18230, 10962, 317},  {3186, 18157, 11100, 334},  {3103, 18082, 11238, 351},  {3022, 18006, 11375, 369},
        {2942, 17927, 11513, 388},  {2863, 17847, 11650, 408},  {2785, 17765, 11788, 428},  {2709, 17681, 11925, 449},
        {2635, 17595, 12062, 471},  {2561, 17507, 12198, 494},  {2489, 17418, 12334, 517},  {2418, 17327, 12470, 541},
        {2348, 17234, 12606, 566},  {2280, 17140, 12741, 592},  {2213, 17044, 12876, 619},  {2147, 16946, 13010, 647},
        {2083, 16846, 13144, 675},  {2020, 16745, 13277, 704},  {1958, 16643, 13409, 735},  {1897, 16539, 13541, 766},
        {1838, 16434, 13673, 798},  {1780, 16327, 13803, 832},  {1723, 16218, 13933, 866},  {1667, 16109, 14062, 901},
        {1613, 15998, 14191, 937},  {1560, 15885, 14318, 975},  {1508, 15772, 14445, 1013}, {1457, 15657, 14571, 1052},
        {1407, 15540, 14695, 1093}, {1359, 15423, 14819, 1134}, {1312, 15304, 14942, 1177}, {1266, 15185, 15064, 1221},
        {1221, 15064, 15185, 1266}, {1177, 14942, 15304, 1312}, {1134, 14819, 15423, 1359}, {1093, 14695, 15540, 1407},
        {1052, 14571, 15657, 1457}, {1013, 14445, 15772, 1508}, {975, 14318, 15885, 1560},  {937, 14191, 15998, 1613},
        {901, 14062, 16109, 1667},  {866, 13933, 16218, 1723},  {832, 13803, 16327, 1780},  {798, 13673, 16434, 1838},
        {766, 13541, 16539, 1897},  {735, 13409, 16643, 1958},  {704, 13277, 16745, 2020},  {675, 13144, 16846, 2083},
        {647, 13010, 16946, 2147},  {619, 12876, 17044, 2213},  {592, 12741, 17140, 2280},  {566, 12606, 17234, 2348},
        {541, 12470, 17327, 2418},  {517, 12334, 17418, 2489},  {494, 12198, 17507, 2561},  {471, 12062, 17595, 2635},
        {449, 11925, 17681, 2709},  {428, 11788, 17765, 2785},  {408, 11650, 17847, 2863},  {388, 11513, 17927, 2942},
        {369, 11375, 18006, 3022},  {351, 11238, 18082, 3103},  {334, 11100, 18157, 3186},  {317, 10962, 18230, 3269},
        {300, 10824, 18300, 3355},  {285, 10687, 18369, 3441},  {270, 10549, 18436, 3529},  {255, 10411, 18500, 3618},
        {241, 10274, 18563, 3709},  {228, 10137, 18624, 3800},  {215, 10000, 18682, 3893},  {202, 9863, 18738, 3987},
        {190, 9726, 18793, 4083},   {179, 9590, 18845, 4179},   {168, 9454, 18895, 4277},   {157, 9318, 18942, 4377},
        {147, 9183, 18988, 4477},   {137, 9048, 19031, 4579},   {127, 8914, 19073, 4681},   {118, 8780, 19112, 4785},
        {109, 8646, 19148, 4890},   {101, 8513, 19183, 4997},   {92, 8381, 19215, 5104},    {84, 8249, 19245, 5213},
        {77, 8118, 19273, 5323},    {69, 7987, 19298, 5434},    {62, 7857, 19321, 5546},    {55, 7728, 19342, 5659},
        {48, 7600, 19361, 5773},    {41, 7472, 19377, 5888},    {34, 7345, 19391, 6004},    {28, 7219, 19403, 6121},
        {22, 7093, 19412, 6239},    {15, 6968, 19419, 6359},    {9, 6845, 19424, 6479},     {3, 6722, 19426, 6600}}};

    constexpr std::array<LutEntry, 128> CurveLut1 = {{
        {-68, 32639, 69, -5},         {-200, 32630, 212, -15},      {-328, 32613, 359, -26},      {-450, 32586, 512, -36},
        {-568, 32551, 669, -47},      {-680, 32507, 832, -58},      {-788, 32454, 1000, -69},     {-891, 32393, 1174, -80},
        {-990, 32323, 1352, -92},     {-1084, 32244, 1536, -103},   {-1173, 32157, 1724, -115},   {-1258, 32061, 1919, -128},
        {-1338, 31956, 2118, -140},   {-1414, 31844, 2322, -153},   {-1486, 31723, 2532, -167},   {-1554, 31593, 2747, -180},
        {-1617, 31456, 2967, -194},   {-1676, 31310, 3192, -209},   {-1732, 31157, 3422, -224},   {-1783, 30995, 3657, -240},
        {-1830, 30826, 3897, -256},   {-1874, 30649, 4143, -272},   {-1914, 30464, 4393, -289},   {-1951, 30272, 4648, -307},
        {-1984, 30072, 4908, -325},   {-2014, 29866, 5172, -343},   {-2040, 29652, 5442, -362},   {-2063, 29431, 5716, -382},
        {-2083, 29203, 5994, -403},   {-2100, 28968, 6277, -424},   {-2114, 28727, 6565, -445},   {-2125, 28480, 6857, -468},
        {-2133, 28226, 7153, -490},   {-2139, 27966, 7453, -514},   {-2142, 27700, 7758, -538},   {-2142, 27428, 8066, -563},
        {-2141, 27151, 8378, -588},   {-2136, 26867, 8694, -614},   {-2130, 26579, 9013, -641},   {-2121, 26285, 9336, -668},
        {-2111, 25987, 9663, -696},   {-2098, 25683, 9993, -724},   {-2084, 25375, 10326, -753},  {-2067, 25063, 10662, -783},
        {-2049, 24746, 11000, -813},  {-2030, 24425, 11342, -844},  {-2009, 24100, 11686, -875},  {-1986, 23771, 12033, -907},
        {-1962, 23438, 12382, -939},  {-1937, 23103, 12733, -972},  {-1911, 22764, 13086, -1005}, {-1883, 22422, 13441, -1039},
        {-1855, 22077, 13798, -1072}, {-1825, 21729, 14156, -1107}, {-1795, 21380, 14516, -1141}, {-1764, 21027, 14877, -1176},
        {-1732, 20673, 15239, -1211}, {-1700, 20317, 15602, -1246}, {-1667, 19959, 15965, -1282}, {-1633, 19600, 16329, -1317},
        {-1599, 19239, 16694, -1353}, {-1564, 18878, 17058, -1388}, {-1530, 18515, 17423, -1424}, {-1495, 18151, 17787, -1459},
        {-1459, 17787, 18151, -1495}, {-1424, 17423, 18515, -1530}, {-1388, 17058, 18878, -1564}, {-1353, 16694, 19239, -1599},
        {-1317, 16329, 19600, -1633}, {-1282, 15965, 19959, -1667}, {-1246, 15602, 20317, -1700}, {-1211, 15239, 20673, -1732},
        {-1176, 14877, 21027, -1764}, {-1141, 14516, 21380, -1795}, {-1107, 14156, 21729, -1825}, {-1072, 13798, 22077, -1855},
        {-1039, 13441, 22422, -1883}, {-1005, 13086, 22764, -1911}, {-972, 12733, 23103, -1937},  {-939, 12382, 23438, -1962},
        {-907, 12033, 23771, -1986},  {-875, 11686, 24100, -2009},  {-844, 11342, 24425, -2030},  {-813, 11000, 24746, -2049},
        {-783, 10662, 25063, -2067},  {-753, 10326, 25375, -2084},  {-724, 9993, 25683, -2098},   {-696, 9663, 25987, -2111},
        {-668, 9336, 26285, -2121},   {-641, 9013, 26579, -2130},   {-614, 8694, 26867, -2136},   {-588, 8378, 27151, -2141},
        {-563, 8066, 27428, -2142},   {-538, 7758, 27700, -2142},   {-514, 7453, 27966, -2139},   {-490, 7153, 28226, -2133},
        {-468, 6857, 28480, -2125},   {-445, 6565, 28727, -2114},   {-424, 6277, 28968, -2100},   {-403, 5994, 29203, -2083},
        {-382, 5716, 29431, -2063},   {-362, 5442, 29652, -2040},   {-343, 5172, 29866, -2014},   {-325, 4908, 30072, -1984},
        {-307, 4648, 30272, -1951},   {-289, 4393, 30464, -1914},   {-272, 4143, 30649, -1874},   {-256, 3897, 30826, -1830},
        {-240, 3657, 30995, -1783},   {-224, 3422, 31157, -1732},   {-209, 3192, 31310, -1676},   {-194, 2967, 31456, -1617},
        {-180, 2747, 31593, -1554},   {-167, 2532, 31723, -1486},   {-153, 2322, 31844, -1414},   {-140, 2118, 31956, -1338},
        {-128, 1919, 32061, -1258},   {-115, 1724, 32157, -1173},   {-103, 1536, 32244, -1084},   {-92, 1352, 32323, -990},
        {-80, 1174, 32393, -891},     {-69, 1000, 32454, -788},     {-58, 832, 32507, -680},      {-47, 669, 32551, -568},
        {-36, 512, 32586, -450},      {-26, 359, 32613, -328},      {-15, 212, 32630, -200},      {-5, 69, 32639, -68}}};

    constexpr std::array<LutEntry, 128> CurveLut2 = {{
        {3195, 26287, 3329, -32},   {3064, 26281, 3467, -34},   {2936, 26270, 3608, -38},   {2811, 26253, 3751, -42},
        {2688, 26230, 3897, -46},   {2568, 26202, 4046, -50},   {2451, 26169, 4199, -54},   {2338, 26130, 4354, -58},
        {2227, 26085, 4512, -63},   {2120, 26035, 4673, -67},   {2015, 25980, 4837, -72},   {1912, 25919, 5004, -76},
        {1813, 25852, 5174, -81},   {1716, 25780, 5347, -87},   {1622, 25704, 5522, -92},   {1531, 25621, 5701, -98},
        {1442, 25533, 5882, -103},  {1357, 25440, 6066, -109},  {1274, 25342, 6253, -115},  {1193, 25239, 6442, -121},
        {1115, 25131, 6635, -127},  {1040, 25018, 6830, -133},  {967, 24899, 7027, -140},   {897, 24776, 7227, -146},
        {829, 24648, 7430, -153},   {764, 24516, 7635, -159},   {701, 24379, 7842, -166},   {641, 24237, 8052, -174},
        {583, 24091, 8264, -181},   {526, 23940, 8478, -187},   {472, 23785, 8695, -194},   {420, 23626, 8914, -202},
        {371, 23462, 9135, -209},   {324, 23295, 9358, -215},   {279, 23123, 9583, -222},   {236, 22948, 9809, -230},
        {194, 22769, 10038, -237},  {154, 22586, 10269, -243},  {117, 22399, 10501, -250},  {81, 22208, 10735, -258},
        {47, 22015, 10970, -265},   {15, 21818, 11206, -271},   {-16, 21618, 11444, -277},  {-44, 21415, 11684, -283},
        {-71, 21208, 11924, -290},  {-97, 20999, 12166, -296},  {-121, 20786, 12409, -302}, {-143, 20571, 12653, -306},
        {-163, 20354, 12898, -311}, {-183, 20134, 13143, -316}, {-201, 19911, 13389, -321}, {-218, 19686, 13635, -325},
        {-234, 19459, 13882, -328}, {-248, 19230, 14130, -332}, {-261, 18998, 14377, -335}, {-273, 18765, 14625, -337},
        {-284, 18531, 14873, -339}, {-294, 18295, 15121, -341}, {-302, 18057, 15369, -341}, {-310, 17817, 15617, -341},
        {-317, 17577, 15864, -340}, {-323, 17335, 16111, -340}, {-328, 17092, 16357, -338}, {-332, 16848, 16603, -336},
        {-336, 16603, 16848, -332}, {-338, 16357, 17092, -328}, {-340, 16111, 17335, -323}, {-340, 15864, 17577, -317},
        {-341, 15617, 17817, -310}, {-341, 15369, 18057, -302}, {-341, 15121, 18295, -294}, {-339, 14873, 18531, -284},
        {-337, 14625, 18765, -273}, {-335, 14377, 18998, -261}, {-332, 14130, 19230, -248}, {-328, 13882, 19459, -234},
        {-325, 13635, 19686, -218}, {-321, 13389, 19911, -201}, {-316, 13143, 20134, -183}, {-311, 12898, 20354, -163},
        {-306, 12653, 20571, -143}, {-302, 12409, 20786, -121}, {-296, 12166, 20999, -97},  {-290, 11924, 21208, -71},
        {-283, 11684, 21415, -44},  {-277, 11444, 21618, -16},  {-271, 11206, 21818, 15},   {-265, 10970, 22015, 47},
        {-258, 10735, 22208, 81},   {-250, 10501, 22399, 117},  {-243, 10269, 22586, 154},  {-237, 10038, 22769, 194},
        {-230, 9809, 22948, 236},   {-222, 9583, 23123, 279},   {-215, 9358, 23295, 324},   {-209, 9135, 23462, 371},
        {-202, 8914, 23626, 420},   {-194, 8695, 23785, 472},   {-187, 8478, 23940, 526},   {-181, 8264, 24091, 583},
        {-174, 8052, 24237, 641},   {-166, 7842, 24379, 701},   {-159, 7635, 24516, 764},   {-153, 7430, 24648, 829},
        {-146, 7227, 24776, 897},   {-140, 7027, 24899, 967},   {-133, 6830, 25018, 1040},  {-127, 6635, 25131, 1115},
        {-121, 6442, 25239, 1193},  {-115, 6253, 25342, 1274},  {-109, 6066, 25440, 1357},  {-103, 5882, 25533, 1442},
        {-98, 5701, 25621, 1531},   {-92, 5522, 25704, 1622},   {-87, 5347, 25780, 1716},   {-81, 5174, 25852, 1813},
        {-76, 5004, 25919, 1912},   {-72, 4837, 25980, 2015},   {-67, 4673, 26035, 2120},   {-63, 4512, 26085, 2227},
        {-58, 4354, 26130, 2338},   {-54, 4199, 26169, 2451},   {-50, 4046, 26202, 2568},   {-46, 3897, 26230, 2688},
        {-42, 3751, 26253, 2811},   {-38, 3608, 26270, 2936},   {-34, 3467, 26281, 3064},   {-32, 3329, 26287, 3195}}};
    // @fmt:on

    std::vector<i16> Resampler::ResampleBuffer(const std::vector<i16> &inputBuffer, double ratio, u8 channelCount) {
        auto step = static_cast<u32>(ratio * 0x8000);
        auto outputSize = static_cast<size_t>(inputBuffer.size() / ratio);
        std::vector<i16> outputBuffer(outputSize);

        const std::array<skyline::audio::LutEntry, 128> &lut = [step] {
            if (step > 0xaaaa)
                return CurveLut0;
            else if (step <= 0x8000)
                return CurveLut1;
            else
                return CurveLut2;
        }();

        for (size_t outIndex = 0, inIndex = 0; outIndex < outputSize; outIndex += channelCount) {
            auto lutIndex = (fraction >> 8) << 2;

            for (u8 channel = 0; channel < channelCount; channel++) {
                i32 data = inputBuffer[(inIndex + 0) * channelCount + channel] * lut[lutIndex].a +
                    inputBuffer[(inIndex + 1) * channelCount + channel] * lut[lutIndex].b +
                    inputBuffer[(inIndex + 2) * channelCount + channel] * lut[lutIndex].c +
                    inputBuffer[(inIndex + 3) * channelCount + channel] * lut[lutIndex].d;

                outputBuffer[outIndex + channel] = static_cast<i16>(std::clamp(data >> 15, static_cast<i32>(std::numeric_limits<i16>::min()), static_cast<i32>(std::numeric_limits<i16>::max())));
            }

            auto newOffset = fraction + step;
            inIndex += newOffset >> 15;
            fraction = newOffset & 0x7fff;
        }

        return outputBuffer;
    }
}
