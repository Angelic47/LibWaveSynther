﻿#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dsound.h>
#include "synther.h"
#include "synthtable.h"
#include "notetable_touhou.h"
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")

#define MAX_AUDIO_BUF 4 
#define BUFFERNOTIFYSIZE 192000

int sample_rate = 44100;	//PCM sample rate
int channels = 1;			//PCM channel number
int bits_per_sample = 16;	//bits per sample

DEFINE_SYNTH_TABLE(g_table_piano_wavefile) { 702, 1144, 1552, 2278, 3140, 4168, 5417, 6664, 7749, 8628, 9120, 9054, 8626, 8164, 7679, 7182, 7033, 7281, 7602, 8026, 8697, 9426, 10183, 11185, 12372, 13553, 14837, 16304, 17754, 19133, 20592, 22117, 23607, 24991, 26063, 26751, 27164, 27158, 26613, 25876, 25220, 24619, 24120, 23865, 23834, 23947, 24152, 24419, 24771, 25315, 26046, 26749, 27281, 27766, 28202, 28346, 28213, 28007, 27723, 27318, 26833, 26330, 25727, 24937, 24025, 23039, 22049, 21395, 21136, 21073, 21398, 22316, 23527, 24733, 25945, 27103, 27975, 28446, 28520, 28120, 27325, 26269, 24930, 23460, 22162, 20973, 19775, 18657, 17551, 16189, 14553, 13003, 11813, 10908, 10392, 10451, 10823, 11231, 11724, 12218, 12596, 12842, 12840, 12536, 12045, 11296, 10052, 8346, 6571, 4912, 3244, 1841, 1133, 867, 805, 1169, 1867, 2644, 3514, 4407, 5263, 6099, 6787, 7361, 7925, 8328, 8484, 8479, 8390, 8299, 8140, 7918, 7702, 7372, 7068, 7169, 7523, 7915, 8589, 9480, 10128, 10560, 11083, 11531, 11734, 11841, 11798, 11488, 11006, 10293, 9290, 8343, 7690, 7192, 6884, 6994, 7353, 7564, 7743, 8131, 8472, 8730, 9402, 10482, 11523, 12621, 13916, 14897, 15202, 15048, 14591, 13671, 12355, 11013, 9874, 8857, 7999, 7566, 7618, 7872, 8113, 8354, 8580, 8571, 8218, 7722, 7197, 6532, 5807, 5160, 4491, 3737, 2975, 2236, 1521, 854, 241, -377, -1049, -1765, -2614, -3599, -4353, -4660, -4666, -4222, -3183, -2023, -1041, 10, 1038, 1675, 2100, 2614, 3044, 3237, 3488, 4016, 4598, 4976, 5189, 5293, 5086, 4502, 3798, 3126, 2475, 1973, 1793, 2029, 2582, 3217, 3893, 4455, 4514, 4175, 3775, 3147, 2306, 1775, 1672, 1706, 1828, 2155, 2618, 2878, 2718, 2442, 2283, 1991, 1557, 1356, 1401, 1500, 1682, 2044, 2529, 2886, 2760, 2149, 1203, -221, -1982, -3547, -4899, -6201, -7217, -7861, -8226, -8231, -7871, -7258, -6371, -5120, -3627, -2159, -790, 585, 1903, 2862, 3244, 3229, 2981, 2259, 997, -490, -1995, -3460, -4763, -5691, -5930, -5479, -4680, -3622, -2252, -898, -28, 406, 744, 797, 357, -272, -1083, -2291, -3685, -5171, -6786, -8246, -9372, -10165, -10583, -10565, -10110, -9441, -8731, -7867, -6922, -6025, -5147, -4353, -3680, -3042, -2513, -2073, -1603, -1110, -670, -326, -129, -134, -352, -592, -708, -726, -598, -285, 44, 331, 580, 647, 498, 298, 77, -293, -744, -1082, -1436, -1987, -2485, -2803, -3175, -3468, -3370, -3080, -2813, -2385, -1811, -1377, -1052, -652, -220, 190, 638, 1064, 1359, 1547, 1631, 1487, 1128, 698, 211, -395, -1075, -1765, -2416, -3006, -3465, -3731, -3708, -3301, -2645, -1911, -1087, -207, 520, 972, 1252, 1498, 1652, 1619, 1415, 1028, 505, -72, -705, -1259, -1751, -2563, -3680, -4753, -5956, -7382, -8610, -9456, -10092, -10511, -10713, -10918, -11244, -11619, -12116, -12893, -13780, -14632, -15625, -16766, -17856, -18980, -20129, -20980, -21460, -21757, -21790, -21475, -21003, -20500, -20005, -19561, -19118, -18659, -18292, -18076, -18094, -18387, -19051, -20259, -21881, -23563, -25235, -27000, -28706, -30036, -30865, -31250, -31222, -30719, -29757, -28621, -27577, -26610, -25730, -25053, -24737, -24881, -25301, -25884, -26754, -27715, -28595, -29495, -30383, -31208, -32008, -32578, -32768, -32671, -32206, -31144, -29554, -27808, -26133, -24512, -23089, -22073, -21465, -21331, -21763, -22588, -23783, -25328, -26753, -27715, -28352, -28734, -28747, -28556, -28402, -28121, -27602, -27105, -26584, -25930, -25269, -24620, -23940, -23181, -22250, -21103, -19705, -18149, -16533, -14786, -13167, -11900, -10800, -9754, -8817, -7866, -6646, -5107, -3542, -1616 };

DEFINE_SYNTH_TABLE(g_table_triangleTable) { 0, 255, 511, 767, 1023, 1279, 1535, 1791, 2047, 2303, 2559, 2815, 3071, 3327, 3583, 3839, 4095, 4351, 4607, 4863, 5119, 5375, 5631, 5887, 6143, 6399, 6655, 6911, 7167, 7423, 7679, 7935, 8191, 8447, 8703, 8959, 9215, 9471, 9727, 9983, 10239, 10495, 10751, 11007, 11263, 11519, 11775, 12031, 12287, 12543, 12799, 13055, 13311, 13567, 13823, 14079, 14335, 14591, 14847, 15103, 15359, 15615, 15871, 16127, 16383, 16639, 16895, 17151, 17407, 17663, 17919, 18175, 18431, 18687, 18943, 19199, 19455, 19711, 19967, 20223, 20479, 20735, 20991, 21247, 21503, 21759, 22015, 22271, 22527, 22783, 23039, 23295, 23551, 23807, 24063, 24319, 24575, 24831, 25087, 25343, 25599, 25855, 26111, 26367, 26623, 26879, 27135, 27391, 27647, 27903, 28159, 28415, 28671, 28927, 29183, 29439, 29695, 29951, 30207, 30463, 30719, 30975, 31231, 31487, 31743, 31999, 32255, 32511, 32767, 32512, 32256, 32000, 31744, 31488, 31232, 30976, 30720, 30464, 30208, 29952, 29696, 29440, 29184, 28928, 28672, 28416, 28160, 27904, 27648, 27392, 27136, 26880, 26624, 26368, 26112, 25856, 25600, 25344, 25088, 24832, 24576, 24320, 24064, 23808, 23552, 23296, 23040, 22784, 22528, 22272, 22016, 21760, 21504, 21248, 20992, 20736, 20480, 20224, 19968, 19712, 19456, 19200, 18944, 18688, 18432, 18176, 17920, 17664, 17408, 17152, 16896, 16640, 16384, 16128, 15872, 15616, 15360, 15104, 14848, 14592, 14336, 14080, 13824, 13568, 13312, 13056, 12800, 12544, 12288, 12032, 11776, 11520, 11264, 11008, 10752, 10496, 10240, 9984, 9728, 9472, 9216, 8960, 8704, 8448, 8192, 7936, 7680, 7424, 7168, 6912, 6656, 6400, 6144, 5888, 5632, 5376, 5120, 4864, 4608, 4352, 4096, 3840, 3584, 3328, 3072, 2816, 2560, 2304, 2048, 1792, 1536, 1280, 1024, 768, 512, 256, 0, -256, -512, -768, -1024, -1280, -1536, -1792, -2048, -2304, -2560, -2816, -3072, -3328, -3584, -3840, -4096, -4352, -4608, -4864, -5120, -5376, -5632, -5888, -6144, -6400, -6656, -6912, -7168, -7424, -7680, -7936, -8192, -8448, -8704, -8960, -9216, -9472, -9728, -9984, -10240, -10496, -10752, -11008, -11264, -11520, -11776, -12032, -12288, -12544, -12800, -13056, -13312, -13568, -13824, -14080, -14336, -14592, -14848, -15104, -15360, -15616, -15872, -16128, -16384, -16640, -16896, -17152, -17408, -17664, -17920, -18176, -18432, -18688, -18944, -19200, -19456, -19712, -19968, -20224, -20480, -20736, -20992, -21248, -21504, -21760, -22016, -22272, -22528, -22784, -23040, -23296, -23552, -23808, -24064, -24320, -24576, -24832, -25088, -25344, -25600, -25856, -26112, -26368, -26624, -26880, -27136, -27392, -27648, -27904, -28160, -28416, -28672, -28928, -29184, -29440, -29696, -29952, -30208, -30464, -30720, -30976, -31232, -31488, -31744, -32000, -32256, -32512, -32768, -32512, -32256, -32000, -31744, -31488, -31232, -30976, -30720, -30464, -30208, -29952, -29696, -29440, -29184, -28928, -28672, -28416, -28160, -27904, -27648, -27392, -27136, -26880, -26624, -26368, -26112, -25856, -25600, -25344, -25088, -24832, -24576, -24320, -24064, -23808, -23552, -23296, -23040, -22784, -22528, -22272, -22016, -21760, -21504, -21248, -20992, -20736, -20480, -20224, -19968, -19712, -19456, -19200, -18944, -18688, -18432, -18176, -17920, -17664, -17408, -17152, -16896, -16640, -16384, -16128, -15872, -15616, -15360, -15104, -14848, -14592, -14336, -14080, -13824, -13568, -13312, -13056, -12800, -12544, -12288, -12032, -11776, -11520, -11264, -11008, -10752, -10496, -10240, -9984, -9728, -9472, -9216, -8960, -8704, -8448, -8192, -7936, -7680, -7424, -7168, -6912, -6656, -6400, -6144, -5888, -5632, -5376, -5120, -4864, -4608, -4352, -4096, -3840, -3584, -3328, -3072, -2816, -2560, -2304, -2048, -1792, -1536, -1280, -1024, -768, -512, -256 };

DEFINE_SYNTH_TABLE(g_table_piano)
{
	0, 362, 725, 1089, 1456, 1826, 2199, 2577, 2960, 3349, 3744, 4146, 4554, 4971, 5394, 5826, 6266, 6714, 7170, 7634, 8105, 8584, 9070, 9563, 10062, 10567, 11078, 11592, 12111, 12632, 13156, 13682, 14208, 14734, 15258, 15781, 16301, 16818, 17330, 17836, 18337, 18830, 19316, 19793, 20261, 20719, 21167, 21604, 22029, 22442, 22844, 23232, 23608, 23971, 24320, 24656, 24979, 25288, 25584, 25867, 26137, 26394, 26638, 26870, 27090, 27299, 27496, 27682, 27858, 28024, 28181, 28329, 28468, 28599, 28723, 28840, 28951, 29056, 29155, 29250, 29341, 29428, 29511, 29592, 29670, 29746, 29821, 29894, 29967, 30039, 30111, 30183, 30255, 30328, 30401, 30476, 30552, 30628, 30706, 30786, 30866, 30948, 31031, 31116, 31201, 31287, 31375, 31462, 31550, 31638, 31725, 31812, 31898, 31983, 32066, 32147, 32225, 32300, 32371, 32439, 32501, 32559, 32611, 32656, 32694, 32725, 32748, 32762, 32767, 32762, 32746, 32720, 32682, 32632, 32569, 32493, 32404, 32300, 32183, 32050, 31903, 31740, 31561, 31367, 31157, 30930, 30688, 30430, 30155, 29865, 29559, 29238, 28902, 28550, 28185, 27805, 27412, 27006, 26588, 26158, 25716, 25265, 24804, 24334, 23856, 23371, 22880, 22383, 21881, 21376, 20867, 20357, 19845, 19333, 18822, 18311, 17803, 17297, 16795, 16297, 15804, 15316, 14834, 14359, 13891, 13430, 12978, 12533, 12097, 11670, 11252, 10844, 10445, 10055, 9675, 9305, 8945, 8595, 8254, 7923, 7601, 7289, 6987, 6693, 6409, 6133, 5867, 5609, 5359, 5118, 4884, 4659, 4441, 4231, 4028, 3832, 3643, 3460, 3285, 3116, 2953, 2796, 2645, 2500, 2361, 2227, 2098, 1975, 1857, 1744, 1635, 1532, 1433, 1338, 1247, 1161, 1079, 1000, 925, 854, 786, 721, 659, 600, 543, 489, 437, 388, 340, 294, 249, 205, 163, 122, 81, 40, 0, -40, -81, -122, -163, -205, -249, -294, -340, -388, -437, -489, -543, -600, -659, -721, -786, -854, -925, -1000, -1079, -1161, -1247, -1338, -1433, -1532, -1635, -1744, -1857, -1975, -2098, -2227, -2361, -2500, -2645, -2796, -2953, -3116, -3285, -3460, -3643, -3832, -4028, -4231, -4441, -4659, -4884, -5118, -5359, -5609, -5867, -6133, -6409, -6693, -6987, -7289, -7601, -7923, -8254, -8595, -8945, -9305, -9675, -10055, -10445, -10844, -11252, -11670, -12097, -12533, -12978, -13430, -13891, -14359, -14834, -15316, -15804, -16297, -16795, -17297, -17803, -18311, -18822, -19333, -19845, -20357, -20867, -21376, -21881, -22383, -22880, -23371, -23856, -24334, -24804, -25265, -25716, -26158, -26588, -27006, -27412, -27805, -28185, -28550, -28902, -29238, -29559, -29865, -30155, -30430, -30688, -30930, -31157, -31367, -31561, -31740, -31903, -32050, -32183, -32300, -32404, -32493, -32569, -32632, -32682, -32720, -32746, -32762, -32767, -32762, -32748, -32725, -32694, -32656, -32611, -32559, -32501, -32439, -32371, -32300, -32225, -32147, -32066, -31983, -31898, -31812, -31725, -31638, -31550, -31462, -31375, -31287, -31201, -31116, -31031, -30948, -30866, -30786, -30706, -30628, -30552, -30476, -30401, -30328, -30255, -30183, -30111, -30039, -29967, -29894, -29821, -29746, -29670, -29592, -29511, -29428, -29341, -29250, -29155, -29056, -28951, -28840, -28723, -28599, -28468, -28329, -28181, -28024, -27858, -27682, -27496, -27299, -27090, -26870, -26638, -26394, -26137, -25867, -25584, -25288, -24979, -24656, -24320, -23971, -23608, -23232, -22844, -22442, -22029, -21604, -21167, -20719, -20261, -19793, -19316, -18830, -18337, -17836, -17330, -16818, -16301, -15781, -15258, -14734, -14208, -13682, -13156, -12632, -12111, -11592, -11078, -10567, -10062, -9563, -9070, -8584, -8105, -7634, -7170, -6714, -6266, -5826, -5394, -4971, -4554, -4146, -3744, -3349, -2960, -2577, -2199, -1826, -1456, -1089, -725, -362
};

DEFINE_SYNTH_TABLE(g_table_pulse25)
{
	32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768 
};

DEFINE_SYNTH_TABLE(g_table_pulse50) { 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768 };

DEFINE_SYNTH_TABLE(g_table_pulse75) { 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768 };

DEFINE_SYNTH_TABLE(g_table_random)
{
	-5913, 17729, -17705, 20413, -19262, -31887, -26275, -27241, 30880, -31890, -25503, -30713, -30460, 21731, -1528, 17805, 24186, 27812, -25357, -4430, 14358, -25296, -18586, 17159, 32497, -29887, 4602, -13276, 8181, -18118, -19439, -6162, 11659, 32604, 24277, -13825, -13514, -2253, 24095, 9639, 19539, -30564, 9794, -13993, 10069, -20861, 6026, -19338, -14532, 18842, 8478, -23767, -27308, 10676, 29177, 28826, -30602, 4384, -23767, 16711, -19739, 15694, 12965, 22033, -28694, -20912, 16883, -30704, 26254, -16132, -8895, -21555, -20002, -19258, 1567, -19568, -20785, -17533, -6050, -10506, 20615, -30834, 14852, -5670, 2908, 2539, -9939, 23124, -9332, 19379, 15000, -25510, -28275, -8101, 9281, 626, -27573, -1651, 3506, -29187, 17296, 2842, 2613, 877, -18036, 9908, 24776, 13438, -1959, 18916, -27236, -30119, -11006, 2002, -28735, 14586, -25544, -2192, 23712, 19296, -9787, 16006, -21160, 30918, 28724, -24100, -4435, 15803, -27122, -26875, 23078, -30336, -30230, 22695, -10399, 16046, 15403, 7912, 11847, 11273, 5912, 31293, -2420, 242, -19384, 32295, -6271, -12772, 25516, -12462, -30292, 14005, 28742, -14581, 27184, 18082, 759, -26228, 23250, -8041, -7809, 13585, 31425, -15828, 252, 16535, 31751, 28918, 6744, -12713, -25582, 21554, -3854, -23828, -29701, -8673, 11977, 11766, -16147, 27816, 3629, -7581, 124, 25460, -22734, 31403, -15508, 9219, 17833, 27939, 28722, -8306, -26600, 9651, 18775, -5013, 32295, 5611, 28763, -16998, -8208, -32597, -390, 23149, 28859, -17644, 15608, 14057, 11585, 16573, 21940, -1195, -7356, -23246, 29944, -9856, -14320, -21126, 2057, 3172, -12251, 6147, -16981, -27164, 2872, 11514, 23682, 22370, -1728, 32275, -3492, 4223, 1210, -22418, 27626, 5309, -24641, 17624, -4954, -4652, 10816, 19974, -21045, 1195, -32464, 3057, -24559, 24799, -13328, -2199, 5950, 14263, 25184, -31150, -30769, -19255, -4054, -11018, -16058, 12852, -21632, -15765, 9808, -3315, -26321, 28768, 3479, -31272, 27846, 17042, -22725, -30363, 19028, 2902, 19948, 21509, -16686, -14201, 1835, 22882, 7710, 5630, -11205, -1647, -15062, 31357, -26865, 23460, 17613, -17964, -5052, 28627, 6257, 28950, -20010, 27125, 7695, -11291, 25652, -18131, -29043, 13125, 30995, 11215, 14977, -29599, 11818, 23970, -20362, -1261, -28468, 5038, -31700, -3854, -16885, 2468, -28748, 26276, -11889, -21910, 20052, 10460, 29537, 8202, -18393, -32390, 27880, 16423, -10270, 16055, 16211, -7720, 9510, -3340, -25514, 29323, 1757, 7442, 16720, 30273, -20308, -30837, -1243, -5447, -7463, -1016, 447, 13332, -13912, 22432, 17718, -31448, -13992, -5033, -17451, -152, -19355, -31587, 8849, 9270, -15457, -23617, 26455, -12845, -6527, 4630, 5712, 32539, 28906, -6794, 10327, 15646, 25143, -29730, -5849, -2026, 28912, 2014, -1503, 22061, -1773, 19645, -20780, -3242, 22754, 3150, -10846, 5757, 4112, 27255, -4224, 8050, 32119, -26681, -11027, -24766, 8644, -3853, 5833, -18659, -16093, -11106, 27253, 9550, -24124, 22700, 19512, -24499, -11581, 554, 8345, -24408, -23808, -17287, -9402, -9392, 10367, -26172, 8530, 11302, -5201, -29700, 3091, -17911, -16826, 31801, 4753, 31801, -16050, -10344, 5615, 9479, -5982, 23812, 13092, 7639, -24843, -15585, 22675, 25016, 8796, 19391, -21155, -11446, 13075, -6063, -29714, -9464, 30181, -6771, 11318, -31299, -10604, -20509, -24801, 23356, 17081, -2635, -27936, -26192, 6645, 19431, -5333, -13084, 26343, -18766, 26558, -10574, -26747, 11671, 28051, -10503, -8336, -14401, -2686, -22072, -5187, 19819, 5521, 32143, -13420, -18499, -32728, -27406, -30662, -26449, 7002, 30095, 24048, -1594, -24496, 26149, -19162, -10999, 23004, -17933, 17504, -6634, 28127, 3649, 2436, -16175, -5693, 6847, -6141, 27518, -4459, 29568, 20039, 21071, 5396, -22070
};

BOOL main(int argc, char * argv[])
{
	printf("FM Synther By Angelic47\n");
	printf("Using DirectSound for output\n");
	printf("Sound name: 恋娘 ～湖上ノ旋律～\n\n");
	printf("Allocating synther...\n");
	int i, j;
	unsigned short trackernum_all = g_touhou_notes.tracker_num;
	unsigned short tracks = 1;
	synther_t *synther;
	synther = (synther_t *)malloc(trackernum_all * sizeof(synther_t));
	synthnote_p *notes_array = (synthnote_p *)malloc(trackernum_all * sizeof(synthnote_p));
	synthnotetime_p *notes_time_array = (synthnotetime_p *)malloc(trackernum_all * sizeof(synthnotetime_p));
	synthnotenum_t *notes_num_array = (synthnotenum_t *)malloc(trackernum_all * sizeof(synthnotenum_t));
	synthnoteppm_t *notebpm_array = (synthnoteppm_t*)malloc(trackernum_all * sizeof(synthnoteppm_t));

	// 设置音符, 设置音色波形和adsr包络参数
	printf("Initalizing canon notes...\n");
	for (i = 0; i < g_touhou_notes.tracker_num; i++) {
		notes_array[i] = g_touhou_notes.notetables_array[i];
		notes_time_array[i] = g_touhou_notes.notetimes_array[i];
		notes_num_array[i] = g_touhou_notes.notenum_array[i];
		notebpm_array[i] = g_touhou_notes.notebpm;
		synth_initalize(&synther[i], g_table_piano);
		synth_adsr_set(
			&synther[i],
			25,
			65535,
			200,
			51000,
			36000,
			15000,
			200
		);
	}

	printf("Creating DirectSound Object...\n");
	// 创建DirectSound声音输出对象
	IDirectSound8 *m_pDS = NULL;
	IDirectSoundBuffer8 *m_pDSBuffer8 = NULL;	//used to manage sound buffers.
	IDirectSoundBuffer *m_pDSBuffer = NULL;
	IDirectSoundNotify8 *m_pDSNotify = NULL;
	DSBPOSITIONNOTIFY m_pDSPosNotify[MAX_AUDIO_BUF];
	HANDLE m_event[MAX_AUDIO_BUF];

	SetConsoleTitle(TEXT("FM Synther By Angelic47"));//Console Title
	//Init DirectSound
	if (FAILED(DirectSoundCreate8(NULL, &m_pDS, NULL)))
		return FALSE;
	if (FAILED(m_pDS->SetCooperativeLevel(FindWindow(NULL, TEXT("FM Synther By Angelic47")), DSSCL_NORMAL)))
		return FALSE;
	SetConsoleTitle(TEXT("FM Synther By Angelic47 - DirectSound - 恋娘 ～湖上ノ旋律～"));//Console Title


	DSBUFFERDESC dsbd;
	memset(&dsbd, 0, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2;
	dsbd.dwBufferBytes = MAX_AUDIO_BUF * BUFFERNOTIFYSIZE;
	//WAVE Header
	dsbd.lpwfxFormat = (WAVEFORMATEX*)malloc(sizeof(WAVEFORMATEX));
	dsbd.lpwfxFormat->wFormatTag = WAVE_FORMAT_PCM;
	/* format type */
	(dsbd.lpwfxFormat)->nChannels = channels;
	/* number of channels (i.e. mono, stereo...) */
	(dsbd.lpwfxFormat)->nSamplesPerSec = sample_rate;
	/* sample rate */
	(dsbd.lpwfxFormat)->nAvgBytesPerSec = sample_rate * (bits_per_sample / 8)*channels;
	/* for buffer estimation */
	(dsbd.lpwfxFormat)->nBlockAlign = (bits_per_sample / 8)*channels;
	/* block size of data */
	(dsbd.lpwfxFormat)->wBitsPerSample = bits_per_sample;
	/* number of bits per sample of mono data */
	(dsbd.lpwfxFormat)->cbSize = 0;

	//Creates a sound buffer object to manage audio samples. 
	if (FAILED(m_pDS->CreateSoundBuffer(&dsbd, &m_pDSBuffer, NULL)))
	{
		return FALSE;
	}
	if (FAILED(m_pDSBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_pDSBuffer8)))
	{
		return FALSE;
	}
	//Get IDirectSoundNotify8
	if (FAILED(m_pDSBuffer8->QueryInterface(IID_IDirectSoundNotify, (LPVOID*)&m_pDSNotify)))
	{
		return FALSE;
	}
	for (i = 0; i < MAX_AUDIO_BUF; i++)
	{
		m_pDSPosNotify[i].dwOffset = i * BUFFERNOTIFYSIZE;
		m_event[i] = ::CreateEvent(NULL, false, false, NULL);
		m_pDSPosNotify[i].hEventNotify = m_event[i];
	}
	m_pDSNotify->SetNotificationPositions(MAX_AUDIO_BUF, m_pDSPosNotify);
	m_pDSNotify->Release();

	//Start Playing
	BOOL isPlaying = TRUE;
	LPVOID buf = NULL;
	DWORD  buf_len = 0;
	DWORD res = WAIT_OBJECT_0;
	DWORD offset = BUFFERNOTIFYSIZE;

	m_pDSBuffer8->SetCurrentPosition(0);
	m_pDSBuffer8->Play(0, 0, DSBPLAY_LOOPING);

	unsigned long *process, *pointer;
	process = (unsigned long*)malloc(trackernum_all * sizeof(unsigned long));
	pointer = (unsigned long*)malloc(trackernum_all * sizeof(unsigned long));
	for (i = 0; i < trackernum_all; i++)
	{
		process[i] = 0;
		pointer[i] = 0;
		synth_key_on(&synther[i], notes_array[i][0]);
	}

	printf("Start rendering and play!\n\n");
	while (isPlaying)
	{
		if ((res >= WAIT_OBJECT_0) && (res <= WAIT_OBJECT_0 + 3))
		{;
			m_pDSBuffer8->Lock(offset, BUFFERNOTIFYSIZE, &buf, &buf_len, NULL, NULL, 0);
			printf("Rending %7d of buffer\n", offset);
			// 缓冲区渲染
			for (i = 0; i < buf_len / 2; i++)
			{
				signed long temp = 0;
				int j;
				for (j = 0; j < trackernum_all; j++)
				{
					if (process[j] > notes_time_array[j][pointer[j]] * notebpm_array[j])
					{
						synth_key_off(&synther[j]);
						process[j] = 0;
						pointer[j]++;
						if (pointer[j] >= notes_num_array[j])
							pointer[j] = 0;
						synth_key_on(&synther[j], notes_array[j][pointer[j]]);
					}
					signed long result = synth_get_next_data_adsr(&synther[j]);
					temp += result;
					process[j]++;
				}
				((synthdata_t *)buf)[i] = temp / 5;
			}
			m_pDSBuffer8->Unlock(buf, buf_len, NULL, 0);
			offset += buf_len;
			offset %= (BUFFERNOTIFYSIZE * MAX_AUDIO_BUF);
		}
		res = WaitForMultipleObjects(MAX_AUDIO_BUF, m_event, FALSE, INFINITE);
	}
	free(process); free(pointer);
	free(synther);

	return 0;
}