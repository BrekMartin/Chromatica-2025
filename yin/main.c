/*
                              CHROMATICA
                               Art 2025
*/

#include <pspkernel.h>
#include <pspiofilemgr.h>
#include <pspaudiolib.h>
#include <pspaudio.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "psp_audio_ext.h"
#include <png.h>
#include <pspgu.h>
#include "graphics.h"
#include "math.h"
#include "Art.h"

#define SAMPLE_RATE 44100

PSP_MODULE_INFO("Chromatica 2025", 0, 1, 1);

short yint_input[4096];				//
short yint_buffer[4096];			//
short yin_buffer[4096];				//
int yint_index = 0;				//
float YIN_THRESHOLD = 0.08;			// algorithm dicumentation suggests 0.10f
int YIN_BUFFER_SIZE = 2048;			//
u32 colouri = 0x000000;				// indicator colour
float fade = 1.0;				// colour fade value
int xPos = 0;					//
int debugfreq = 0;				// frequency display flag
int nux;					// delay variable
int largefreq = 0;				// large frequency display status
int rate = 44100;				// sample rate
unsigned char lastscaled = 128;			//
int xx = 0;
int yy = 0;
int x = 0;
int y = 0;
int bitcnt = 0;
int tindex = 0;
unsigned int tx = 0;
unsigned int ty = 0;
int txp = 0;
int typ = 0;
int sinvalue = 0;
unsigned int tcnt = 0;
unsigned char tchar = 0;
int sinindex = 0;
int sinstart = 0;
unsigned char sindir = 0;
unsigned char sinstartdir = 0;
unsigned char sinamp = 10;
int scroller = 0;
char texts[136];
char textn[64];
char textt[64];
int pxx = 0;
int pyy = 0;
u32 pixel;					//
u32 sample;					//
int imgbuf = 0;
//
sample_t* loop_buffer;
short mic_level;
Image *background;
Image *abackground;
Image *bbackground;
Image *tbackground;
Image * Art;
//
typedef struct {
    const char *name;
    double freq;
} Note;
//
Note notes[] = {
    {"C0", 16.3516}, {"C#0", 17.3239}, {"D0", 18.3540}, {"D#0", 19.4454},
    {"E0", 20.6017}, {"F0", 21.8268}, {"F#0", 23.1247}, {"G0", 24.4997},
    {"G#0", 25.9565}, {"A0", 27.5000}, {"A#0", 29.1352}, {"B0", 30.8677},
    {"C1", 32.7032}, {"C#1", 34.6478}, {"D1", 36.7081}, {"D#1", 38.8909},
    {"E1", 41.2034}, {"F1", 43.6535}, {"F#1", 46.2493}, {"G1", 48.9994},
    {"G#1", 51.9131}, {"A1", 55.0000}, {"A#1", 58.2705}, {"B1", 61.7354},
    {"C2", 65.4064}, {"C#2", 69.2957}, {"D2", 73.4162}, {"D#2", 77.7817},
    {"E2", 82.4069}, {"F2", 87.3071}, {"F#2", 92.4986}, {"G2", 97.9989},
    {"G#2", 103.826}, {"A2", 110.000}, {"A#2", 116.541}, {"B2", 123.471},
    {"C3", 130.813}, {"C#3", 138.591}, {"D3", 146.832}, {"D#3", 155.563},
    {"E3", 164.814}, {"F3", 174.614}, {"F#3", 184.997}, {"G3", 195.998},
    {"G#3", 207.652}, {"A3", 220.000}, {"A#3", 233.082}, {"B3", 246.942},
    {"C4", 261.626}, {"C#4", 277.183}, {"D4", 293.665}, {"D#4", 311.127},
    {"E4", 329.628}, {"F4", 349.228}, {"F#4", 369.994}, {"G4", 391.995},
    {"G#4", 415.305}, {"A4", 440.000}, {"A#4", 466.164}, {"B4", 493.883},
    {"C5", 523.251}, {"C#5", 554.365}, {"D5", 587.330}, {"D#5", 622.254},
    {"E5", 659.255}, {"F5", 698.456}, {"F#5", 739.989}, {"G5", 783.991},
    {"G#5", 830.609}, {"A5", 880.000}, {"A#5", 932.328}, {"B5", 987.767},
    {"C6", 1046.50}, {"C#6", 1108.73}, {"D6", 1174.66}, {"D#6", 1244.51},
    {"E6", 1318.51}, {"F6", 1396.91}, {"F#6", 1479.98}, {"G6", 1567.98},
    {"G#6", 1661.22}, {"A6", 1760.00}, {"A#6", 1864.66}, {"B6", 1975.53},
    {"C7", 2093.00}, {"C#7", 2217.46}, {"D7", 2349.32}, {"D#7", 2489.02},
    {"E7", 2637.02}, {"F7", 2793.83}, {"F#7", 2959.96}, {"G7", 3135.96},
    {"G#7", 3322.44}, {"A7", 3520.00}, {"A#7", 3729.31}, {"B7", 3951.07},
    {"C8", 4186.01}
};
//
// half wave sine table precalculated
const unsigned short sin_table[181] = {
0,1,4,11,19,31,44,61,79,100,124,150,179,209,243,279,317,357,400,446,494,544,
596,651,708,767,829,892,958,1027,1097,1170,1244,1321,1400,1481,1564,1649,1736,1825,
1916,2009,2104,2200,2299,2399,2501,2605,2710,2817,2926,3036,3148,3261,3376,3493,3611,
3730,3850,3972,4096,4220,4346,4472,4600,4729,4860,4991,5123,5256,5390,5524,5660,5796,
5933,6071,6210,6349,6488,6628,6769,6910,7051,7193,7335,7478,7620,7763,7906,8049,8192,
8334,8477,8620,8763,8905,9048,9190,9332,9473,9614,9755,9895,10034,10173,10312,10450,
10587,10723,10859,10993,11127,11260,11392,11523,11654,11783,11911,12037,12163,12288,
12411,12533,12653,12772,12890,13007,13122,13235,13347,13457,13566,13673,13778,13882,
13984,14084,14183,14279,14374,14467,14558,14647,14734,14819,14902,14983,15062,15139,
15213,15286,15356,15425,15491,15554,15616,15675,15732,15787,15839,15889,15937,15983,
16026,16066,16104,16140,16174,16204,16233,16259,16283,16304,16322,16339,16352,16364,
16372,16379,16382,16384
};
//
// wave table for spring physics scrolling
const unsigned char spmessage[27] = {
1,1,2,4,6,9,12,16,22,30,39,48,57,66,74,82,
90,98,106,112,116,119,122,124,126,127,127
};
//
// bold variable width character map
const unsigned char charmapb[420] = {
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // space
0b00000000, 0b00000000, 0b00000110, 0b01011111, 0b01011111, 0b00000110, 0b00000000, // !
0b00000000, 0b00000011, 0b00000011, 0b00000000, 0b00000011, 0b00000011, 0b00000000, // "
0b00010100, 0b01111111, 0b01111111, 0b00010100, 0b01111111, 0b01111111, 0b00010100, // #
0b00100100, 0b00101110, 0b01101011, 0b01101011, 0b00111010, 0b00010010, 0b00000000, // $
0b01000110, 0b01100110, 0b00110000, 0b00011000, 0b00001100, 0b01100110, 0b01100010, // %
0b00110000, 0b01111010, 0b01001111, 0b01011101, 0b00110111, 0b01111010, 0b01001000, // &
0b00000000, 0b00000100, 0b00000111, 0b00000011, 0b00000000, 0b00000000, 0b00000000, // '
0b00000000, 0b00011100, 0b00111110, 0b01100011, 0b01000001, 0b00000000, 0b00000000, // (
0b00000000, 0b01000001, 0b01100011, 0b00111110, 0b00011100, 0b00000000, 0b00000000, // )
0b00010000, 0b01010100, 0b01111100, 0b00111000, 0b01111100, 0b01010100, 0b00010000, // *
0b00000000, 0b00010000, 0b00010000, 0b01111100, 0b00010000, 0b00010000, 0b00000000, // +
0b00000000, 0b01011000, 0b00111000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // ,
0b00000000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00000000, 0b00000000, // -
0b00000000, 0b01100000, 0b01100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // .
0b00000000, 0b00000000, 0b00110000, 0b00011000, 0b00001100, 0b00000000, 0b00000000, // /
0b00111110, 0b01111111, 0b01000001, 0b01000001, 0b01111111, 0b00111110, 0b00000000, // 0
0b01000000, 0b01000010, 0b01111111, 0b01111111, 0b01000000, 0b01000000, 0b00000000, // 1
0b01100010, 0b01110011, 0b01011001, 0b01001001, 0b01101111, 0b01100110, 0b00000000, // 2
0b00100010, 0b01100011, 0b01001001, 0b01001001, 0b01111111, 0b00110110, 0b00000000, // 3
0b00011000, 0b00011100, 0b00010110, 0b01010011, 0b01111111, 0b01010000, 0b00000000, // 4
0b00100111, 0b01100101, 0b01000101, 0b01000101, 0b01111101, 0b00111001, 0b00000000, // 5
0b00111100, 0b01111110, 0b01001011, 0b01001001, 0b01111001, 0b00110000, 0b00000000, // 6
0b00000011, 0b00000011, 0b01110001, 0b01111001, 0b00001111, 0b00000111, 0b00000000, // 7
0b00110110, 0b01111111, 0b01001001, 0b01001001, 0b01111111, 0b00110110, 0b00000000, // 8
0b00000110, 0b01001111, 0b01001001, 0b01101001, 0b00111111, 0b00011110, 0b00000000, // 9
0b00000000, 0b00000000, 0b01100110, 0b01100110, 0b00000000, 0b00000000, 0b00000000, // :
0b00000000, 0b01000000, 0b01110011, 0b00110011, 0b00000000, 0b00000000, 0b00000000, // ;
0b00000000, 0b00001000, 0b00011100, 0b00110110, 0b01100011, 0b01000001, 0b00000000, // <
0b01000110, 0b01100110, 0b00110000, 0b00011000, 0b00001100, 0b01100110, 0b01100010, // %
0b00000000, 0b01000001, 0b01100011, 0b00110110, 0b00011100, 0b00001000, 0b00000000, // >
0b00000010, 0b00000011, 0b01010001, 0b01011001, 0b00001111, 0b00000110, 0b00000000, // ?
0b00000000, 0b00010100, 0b00010100, 0b00010100, 0b00010100, 0b00000000, 0b00000000, // =
0b01111100, 0b01111110, 0b00010011, 0b00010011, 0b01111110, 0b01111100, 0b00000000, // A
0b01000001, 0b01111111, 0b01111111, 0b01001001, 0b01001001, 0b01111111, 0b00110110, // B
0b00011100, 0b00111110, 0b01100011, 0b01000001, 0b01000001, 0b01100011, 0b00100010, // C
0b01000001, 0b01111111, 0b01111111, 0b01000001, 0b01100011, 0b00111110, 0b00011100, // D
0b01000001, 0b01111111, 0b01111111, 0b01001001, 0b01011101, 0b01000001, 0b01100011, // E
0b01000001, 0b01111111, 0b01111111, 0b01001001, 0b00011101, 0b00000001, 0b00000011, // F
0b00011100, 0b00111110, 0b01100011, 0b01000001, 0b01010001, 0b01110011, 0b01110010, // G
0b01111111, 0b01111111, 0b00001000, 0b00001000, 0b01111111, 0b01111111, 0b00000000, // H
0b00000000, 0b01000001, 0b01111111, 0b01111111, 0b01000001, 0b00000000, 0b00000000, // I
0b00110000, 0b01110000, 0b01000000, 0b01000001, 0b01111111, 0b00111111, 0b00000001, // J
0b01000001, 0b01111111, 0b01111111, 0b00001000, 0b00011100, 0b01110111, 0b01100011, // K
0b01000001, 0b01111111, 0b01111111, 0b01000001, 0b01000000, 0b01100000, 0b01110000, // L
0b01111111, 0b01111111, 0b00001110, 0b00011100, 0b00001110, 0b01111111, 0b01111111, // M
0b01111111, 0b01111111, 0b00000110, 0b00001100, 0b00011000, 0b01111111, 0b01111111, // N
0b00011100, 0b00111110, 0b01100011, 0b01000001, 0b01100011, 0b00111110, 0b00011100, // O
0b01000001, 0b01111111, 0b01111111, 0b01001001, 0b00001001, 0b00001111, 0b00000110, // P
0b00011110, 0b00111111, 0b00100001, 0b01110001, 0b01111111, 0b01011110, 0b00000000, // Q
0b01000001, 0b01111111, 0b01111111, 0b00001001, 0b00011001, 0b01111111, 0b01100110, // R
0b00100010, 0b01100111, 0b01001101, 0b01011001, 0b01110011, 0b00100010, 0b00000000, // S
0b00000011, 0b01000001, 0b01111111, 0b01111111, 0b01000001, 0b00000011, 0b00000000, // T
0b00111111, 0b01111111, 0b01000000, 0b01000000, 0b01111111, 0b00111111, 0b00000000, // U
0b00011111, 0b00111111, 0b01100000, 0b01100000, 0b00111111, 0b00011111, 0b00000000, // V
0b01111111, 0b01111111, 0b00110000, 0b00011000, 0b00110000, 0b01111111, 0b01111111, // W
0b01100011, 0b00110110, 0b00011100, 0b00011100, 0b00110110, 0b01100011, 0b00000000, // X
0b00000111, 0b01001111, 0b01111000, 0b01111000, 0b01001111, 0b00000111, 0b00000000, // Y
0b01000111, 0b01100011, 0b01110001, 0b01011001, 0b01001101, 0b01100111, 0b01110011, // Z
0b01001100, 0b01100100, 0b01110100, 0b01011100, 0b01001100, 0b01100100, 0b00000000 // z
};
//
const unsigned int colors[480] = {
0x0000FF,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,
0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,
0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,
0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,
0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0001FD,0x0001FD,0x0001FD,0x0001FD,0x0001FD,0x0001FD,
0x0001FD,0x0001FD,0x0001FD,0x0001FD,0x0001FD,0x0001FD,0x0002FC,0x0002FC,0x0002FC,0x0002FC,0x0002FC,0x0002FC,
0x0002FC,0x0002FC,0x0003FB,0x0003FB,0x0003FB,0x0003FB,0x0003FB,0x0003FB,0x0004FA,0x0004FA,0x0004FA,0x0004FA,
0x0004FA,0x0005F9,0x0005F9,0x0005F9,0x0005F9,0x0005F9,0x0006F8,0x0006F8,0x0006F8,0x0007F7,0x0007F7,0x0007F7,
0x0007F7,0x0008F6,0x0008F6,0x0008F6,0x0009F5,0x0009F5,0x0009F5,0x000AF4,0x000AF4,0x000BF3,0x000BF3,0x000BF3,
0x000CF2,0x000CF2,0x000DF1,0x000DF1,0x000EF0,0x000EF0,0x000FEF,0x000FEF,0x0010EE,0x0010EE,0x0011ED,0x0011ED,
0x0012EC,0x0012EC,0x0013EB,0x0014EA,0x0014EA,0x0015E9,0x0016E8,0x0016E8,0x0017E7,0x0018E6,0x0019E5,0x0019E5,
0x001AE4,0x001BE3,0x001CE2,0x001DE1,0x001DE1,0x001EE0,0x001FDF,0x0020DE,0x0021DD,0x0022DC,0x0023DB,0x0024DA,
0x0025D9,0x0026D8,0x0027D7,0x0028D6,0x0029D5,0x002AD4,0x002CD2,0x002DD1,0x002ED0,0x002FCF,0x0031CD,0x0032CC,
0x0033CB,0x0034CA,0x0036C8,0x0037C7,0x0039C5,0x003AC4,0x003CC2,0x003DC1,0x003FBF,0x0040BE,0x0042BC,0x0043BB,
0x0045B9,0x0047B7,0x0048B6,0x004AB4,0x004CB2,0x004EB0,0x004FAF,0x0051AD,0x0053AB,0x0055A9,0x0057A7,0x0059A5,
0x005BA3,0x005DA1,0x005F9F,0x00619D,0x00639B,0x006599,0x006797,0x006A94,0x006C92,0x006E90,0x00708E,0x00738B,
0x007589,0x007787,0x007A84,0x007C82,0x007F7F,0x00817D,0x00847A,0x008678,0x008975,0x008B73,0x008E70,0x00916D,
0x00936B,0x009668,0x009965,0x009C62,0x009E60,0x00A15D,0x00A45A,0x00A757,0x00AA54,0x00AD51,0x00AF4F,0x00B24C,
0x00B549,0x00B846,0x00BB43,0x00BE40,0x00C13D,0x00C43A,0x00C737,0x00CA34,0x00CE30,0x00D12D,0x00D42A,0x00D727,
0x00DA24,0x00DD21,0x00E01E,0x00E31B,0x00E717,0x00EA14,0x00ED11,0x00F00E,0x00F30B,0x00F707,0x00FA04,0x00FD01,
0x00FD01,0x00FA04,0x00F707,0x00F30B,0x00F00E,0x00ED11,0x00EA14,0x00E717,0x00E31B,0x00E01E,0x00DD21,0x00DA24,
0x00D727,0x00D42A,0x00D12D,0x00CE30,0x00CA34,0x00C737,0x00C43A,0x00C13D,0x00BE40,0x00BB43,0x00B846,0x00B549,
0x00B24C,0x00AF4F,0x00AD51,0x00AA54,0x00A757,0x00A45A,0x00A15D,0x009E60,0x009C62,0x009965,0x009668,0x00936B,
0x00916D,0x008E70,0x008B73,0x008975,0x008678,0x00847A,0x00817D,0x007F7F,0x007C82,0x007A84,0x007787,0x007589,
0x00738B,0x00708E,0x006E90,0x006C92,0x006A94,0x006797,0x006599,0x00639B,0x00619D,0x005F9F,0x005DA1,0x005BA3,
0x0059A5,0x0057A7,0x0055A9,0x0053AB,0x0051AD,0x004FAF,0x004EB0,0x004CB2,0x004AB4,0x0048B6,0x0047B7,0x0045B9,
0x0043BB,0x0042BC,0x0040BE,0x003FBF,0x003DC1,0x003CC2,0x003AC4,0x0039C5,0x0037C7,0x0036C8,0x0034CA,0x0033CB,
0x0032CC,0x0031CD,0x002FCF,0x002ED0,0x002DD1,0x002CD2,0x002AD4,0x0029D5,0x0028D6,0x0027D7,0x0026D8,0x0025D9,
0x0024DA,0x0023DB,0x0022DC,0x0021DD,0x0020DE,0x001FDF,0x001EE0,0x001DE1,0x001DE1,0x001CE2,0x001BE3,0x001AE4,
0x0019E5,0x0019E5,0x0018E6,0x0017E7,0x0016E8,0x0016E8,0x0015E9,0x0014EA,0x0014EA,0x0013EB,0x0012EC,0x0012EC,
0x0011ED,0x0011ED,0x0010EE,0x0010EE,0x000FEF,0x000FEF,0x000EF0,0x000EF0,0x000DF1,0x000DF1,0x000CF2,0x000CF2,
0x000BF3,0x000BF3,0x000BF3,0x000AF4,0x000AF4,0x0009F5,0x0009F5,0x0009F5,0x0008F6,0x0008F6,0x0008F6,0x0007F7,
0x0007F7,0x0007F7,0x0007F7,0x0006F8,0x0006F8,0x0006F8,0x0005F9,0x0005F9,0x0005F9,0x0005F9,0x0005F9,0x0004FA,
0x0004FA,0x0004FA,0x0004FA,0x0004FA,0x0003FB,0x0003FB,0x0003FB,0x0003FB,0x0003FB,0x0003FB,0x0002FC,0x0002FC,
0x0002FC,0x0002FC,0x0002FC,0x0002FC,0x0002FC,0x0002FC,0x0001FD,0x0001FD,0x0001FD,0x0001FD,0x0001FD,0x0001FD,
0x0001FD,0x0001FD,0x0001FD,0x0001FD,0x0001FD,0x0001FD,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,
0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,
0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,
0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,
0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FE,0x0000FF
};
//
u32 fade_to_black(u32 color, float fade_factor) { // ride the lightning
	if (fade_factor < 0.0f) fade_factor = 0.0f; // extract colours
	if (fade_factor > 1.0f) fade_factor = 1.0f;
	uint8_t r = (color & 0x0000FF); // apply fade
	uint8_t g = (color & 0x00FF00) >> 8;
	uint8_t b = (color & 0xFF0000) >> 16;
	r = (uint8_t)(r * fade_factor);
	g = (uint8_t)(g * fade_factor);
	b = (uint8_t)(b * fade_factor);
	return (b << 16) | (g << 8) | r; // recombine colours
}
//
int hasEnoughSignal(const int16_t *buffer, size_t length, float threshold) { // check for enough audio energy to process
	long long sumSquares = 0;
	int i = 0;
	for (i = 0; i < 512; i++) { // for optimisation no need to check entire buffer
	int sample = buffer[i];
	sumSquares += (long long)sample * sample;
	}
	float rms = sqrtf((float)sumSquares / (float)length); // calculate RMS
	return (rms > threshold);
}
//
void convert_16bit_to_float(const short* input, float* output, int size) { // convert 16 bit signed audio buffer to float normalized [-1,1]
	int i = 0;
	for (i = 0; i < size; i++) {
	output[i] = (float)input[i] / 32768.0f;
	}
}
//
/*
void difference_function(const float* x, float* diff, int size, int max_tau) { // compute the difference function
	int tau = 0;
	for (tau = 0; tau < max_tau; tau++) {
	float sum = 0.0f;
	int i = 0;
	for (i = 0; i < size - tau; i++) {
	float delta = x[i] - x[i + tau];
	sum += delta * delta;
	}
	diff[tau] = sum;
	}
}
*/
//
void difference_function(const float* x, float* diff, int size, int max_tau) { // compute the difference function optimised
	int tau = 0;
	for (tau = 0; tau < max_tau; tau++) {
	float sum = 0.0f;
        int i = 0;
	for (i = 0; i + 3 < size - tau; i += 4) { // unrolled loop 4x
	float d0 = x[i]     - x[i + tau];
	float d1 = x[i + 1] - x[i + tau + 1];
	float d2 = x[i + 2] - x[i + tau + 2];
	float d3 = x[i + 3] - x[i + tau + 3];
	sum += d0 * d0 + d1 * d1 + d2 * d2 + d3 * d3;
	}
	for (i = 0; i < size - tau; i++) { // leftover iterations
	float delta = x[i] - x[i + tau];
	sum += delta * delta;
	}
	diff[tau] = sum;
	}
}
//
void cumulative_mean_normalized_difference(float* diff, int size) { // compute the cumulative mean normalized difference function
	diff[0] = 1.0f;  // avoid division by zero
	float running_sum = 0.0f;
	int tau = 0;
	for (tau = 1; tau < size; tau++) {
	running_sum += diff[tau];
	diff[tau] *= tau / running_sum;
	}
}
//
int absolute_threshold(const float* diff, int size, float threshold) { // find absolute minimum of the normalized difference function below threshold
	int tau = 0;
	for (tau = 2; tau < size; tau++) {
	if (diff[tau] < threshold) {      
	while (tau + 1 < size && diff[tau + 1] < diff[tau]) { // find local minimum
	tau++;
	}
	return tau;
	}
	}
	return -1;  // No suitable tau found
}
//
float parabolic_interpolation(const float* diff, int tau, int size) { // parabolic interpolation to refine tau estimate
	if (tau <= 0 || tau >= size - 1) return (float)tau;
	float s0 = diff[tau - 1];
	float s1 = diff[tau];
	float s2 = diff[tau + 1];
	float denom = (s0 + s2) - 2.0f * s1;
	if (denom == 0.0f) return (float)tau;
	return tau + (s0 - s2) / (2.0f * denom);
}
//
float yin_pitch_detect(const short* input_buffer, int buffer_size, int sample_rate) { // pitch detection main function
	static float yin_buffer[4096];
	static float diff_buffer[2048];
	convert_16bit_to_float(input_buffer, yin_buffer, buffer_size); // convert input to float
	int max_tau = buffer_size / 2;
	difference_function(yin_buffer, diff_buffer, buffer_size, max_tau);
	cumulative_mean_normalized_difference(diff_buffer, max_tau);
	int tau = absolute_threshold(diff_buffer, max_tau, YIN_THRESHOLD);
	if (tau == -1) {return -1.0f;} // No pitch found
	float better_tau = parabolic_interpolation(diff_buffer, tau, max_tau);
	float frequency = (float)sample_rate / better_tau; // Convert lag (tau) to frequency
	return frequency;
}
//
// Bresenham's line drawing algorithm
void adrawline (int bx1,int by1,int bx2,int by2) {
	int bx,by,bdx,bdy,bdx1,bdy1,bpx,bpy,bxe,bye,bi;
	bdx=bx2-bx1; bdy=by2-by1;
	bdx1=fabs(bdx); bdy1=fabs(bdy);
	bpx=2*bdy1-bdx1; bpy=2*bdx1-bdy1;
	if(bdy1<=bdx1) {
	if(bdx>=0) {bx=bx1; by=by1; bxe=bx2;} else {bx=bx2; by=by2; bxe=bx1;}
	putPixelImage(0x00EFDDCA, bx, by, abackground);
	putPixelImage(0x00EFDDCA, bx, by+1, abackground);
	for(bi=0;bx<bxe;bi++) {
	bx=bx+1;
	if(bpx<0) {
	bpx=bpx+2*bdy1;
	} else {
	if((bdx<0 && bdy<0) || (bdx>0 && bdy>0)) {by++;} else {by--;}
	bpx=bpx+2*(bdy1-bdx1);
	}
	putPixelImage(0x00EFDDCA, bx, by, abackground);
	putPixelImage(0x00EFDDCA, bx, by+1, abackground);
	}
	} else {
	if(bdy>=0) {bx=bx1; by=by1; bye=by2;} else {bx=bx2; by=by2; bye=by1;}
	putPixelImage(0x00EFDDCA, bx, by, abackground);
	putPixelImage(0x00EFDDCA, bx, by+1, abackground);
	for(bi=0;by<bye;bi++) {
	by=by+1;
	if(bpy<=0) {
	bpy=bpy+2*bdx1;
	} else {
	if((bdx<0 && bdy<0) || (bdx>0 && bdy>0)) {bx++;} else {bx--;}
	bpy=bpy+2*(bdx1-bdy1);
	}
	putPixelImage(0x00EFDDCA, bx, by, abackground);
	putPixelImage(0x00EFDDCA, bx, by+1, abackground);
	}}
}
//
// Bresenham's line drawing algorithm
void bdrawline (int bx1,int by1,int bx2,int by2) {
	int bx,by,bdx,bdy,bdx1,bdy1,bpx,bpy,bxe,bye,bi;
	bdx=bx2-bx1; bdy=by2-by1;
	bdx1=fabs(bdx); bdy1=fabs(bdy);
	bpx=2*bdy1-bdx1; bpy=2*bdx1-bdy1;
	if(bdy1<=bdx1) {
	if(bdx>=0) {bx=bx1; by=by1; bxe=bx2;} else {bx=bx2; by=by2; bxe=bx1;}
	putPixelImage(0x00EFDDCA, bx, by, bbackground);
	putPixelImage(0x00EFDDCA, bx, by+1, bbackground);
	for(bi=0;bx<bxe;bi++) {
	bx=bx+1;
	if(bpx<0) {
	bpx=bpx+2*bdy1;
	} else {
	if((bdx<0 && bdy<0) || (bdx>0 && bdy>0)) {by++;} else {by--;}
	bpx=bpx+2*(bdy1-bdx1);
	}
	putPixelImage(0x00EFDDCA, bx, by, bbackground);
	putPixelImage(0x00EFDDCA, bx, by+1, bbackground);
	}
	} else {
	if(bdy>=0) {bx=bx1; by=by1; bye=by2;} else {bx=bx2; by=by2; bye=by1;}
	putPixelImage(0x00EFDDCA, bx, by, bbackground);
	putPixelImage(0x00EFDDCA, bx, by+1, bbackground);
	for(bi=0;by<bye;bi++) {
	by=by+1;
	if(bpy<=0) {
	bpy=bpy+2*bdx1;
	} else {
	if((bdx<0 && bdy<0) || (bdx>0 && bdy>0)) {bx++;} else {bx--;}
	bpy=bpy+2*(bdx1-bdy1);
	}
	putPixelImage(0x00EFDDCA, bx, by, bbackground);
	putPixelImage(0x00EFDDCA, bx, by+1, bbackground);
	}}
}
//

// Bresenham's line drawing algorithm
void cdrawline (int bx1,int by1,int bx2,int by2) {
	int bx,by,bdx,bdy,bdx1,bdy1,bpx,bpy,bxe,bye,bi;
	bdx=bx2-bx1; bdy=by2-by1;
	bdx1=fabs(bdx); bdy1=fabs(bdy);
	bpx=2*bdy1-bdx1; bpy=2*bdx1-bdy1;
	if(bdy1<=bdx1) {
	if(bdx>=0) {bx=bx1; by=by1; bxe=bx2;} else {bx=bx2; by=by2; bxe=bx1;}
	putPixelImage(colouri, bx, by, abackground);
	putPixelImage(colouri, bx, by+1, abackground);
	for(bi=0;bx<bxe;bi++) {
	bx=bx+1;
	if(bpx<0) {
	bpx=bpx+2*bdy1;
	} else {
	if((bdx<0 && bdy<0) || (bdx>0 && bdy>0)) {by++;} else {by--;}
	bpx=bpx+2*(bdy1-bdx1);
	}
	putPixelImage(colouri, bx, by, abackground);
	putPixelImage(colouri, bx, by+1, abackground);
	}
	} else {
	if(bdy>=0) {bx=bx1; by=by1; bye=by2;} else {bx=bx2; by=by2; bye=by1;}
	putPixelImage(colouri, bx, by, abackground);
	putPixelImage(colouri, bx, by+1, abackground);
	for(bi=0;by<bye;bi++) {
	by=by+1;
	if(bpy<=0) {
	bpy=bpy+2*bdx1;
	} else {
	if((bdx<0 && bdy<0) || (bdx>0 && bdy>0)) {bx++;} else {bx--;}
	bpy=bpy+2*(bdx1-bdy1);
	}
	putPixelImage(colouri, bx, by, abackground);
	putPixelImage(colouri, bx, by+1, abackground);
	}}
}
//
// Bresenham's line drawing algorithm
void ddrawline (int bx1,int by1,int bx2,int by2) {
	int bx,by,bdx,bdy,bdx1,bdy1,bpx,bpy,bxe,bye,bi;
	bdx=bx2-bx1; bdy=by2-by1;
	bdx1=fabs(bdx); bdy1=fabs(bdy);
	bpx=2*bdy1-bdx1; bpy=2*bdx1-bdy1;
	if(bdy1<=bdx1) {
	if(bdx>=0) {bx=bx1; by=by1; bxe=bx2;} else {bx=bx2; by=by2; bxe=bx1;}
	putPixelImage(colouri, bx, by, bbackground);
	putPixelImage(colouri, bx, by+1, bbackground);
	for(bi=0;bx<bxe;bi++) {
	bx=bx+1;
	if(bpx<0) {
	bpx=bpx+2*bdy1;
	} else {
	if((bdx<0 && bdy<0) || (bdx>0 && bdy>0)) {by++;} else {by--;}
	bpx=bpx+2*(bdy1-bdx1);
	}
	putPixelImage(colouri, bx, by, bbackground);
	putPixelImage(colouri, bx, by+1, bbackground);
	}
	} else {
	if(bdy>=0) {bx=bx1; by=by1; bye=by2;} else {bx=bx2; by=by2; bye=by1;}
	putPixelImage(colouri, bx, by, bbackground);
	putPixelImage(colouri, bx, by+1, bbackground);
	for(bi=0;by<bye;bi++) {
	by=by+1;
	if(bpy<=0) {
	bpy=bpy+2*bdx1;
	} else {
	if((bdx<0 && bdy<0) || (bdx>0 && bdy>0)) {bx++;} else {bx--;}
	bpy=bpy+2*(bdx1-bdy1);
	}
	putPixelImage(colouri, bx, by, bbackground);
	putPixelImage(colouri, bx, by+1, bbackground);
	}}
}
//
void detectNote(double frequency) {
	int bestIndex = -1;
	double bestDiff = 1e9;
	int i = 0;
	for (i = 0; i < sizeof(notes)/sizeof(notes[0]); i++) { // find closest note
	double diff = fabs(frequency - notes[i].freq);
	if (diff < bestDiff) {
	bestDiff = diff;
	bestIndex = i;
	}
	}
	double noteFreq = notes[bestIndex].freq;
	double cents = 1200.0 * log2(frequency / noteFreq);
	if (fabs(cents) <= 50) { // within tuning window
	sprintf(textt, "%s", notes[bestIndex].name);
	// assuming cents is between -50 and +50
	double clampedCents = cents;
	if (clampedCents < -50.0) clampedCents = -50.0;
	if (clampedCents >  50.0) clampedCents =  50.0;
	xPos = (int)((clampedCents + 50.0) / 100.0 * 479.0); // map -50 to +50 cents to 0-479 pixels
	}

	if (debugfreq == 0) {
	if (cents > 0.0) {
	sprintf(texts,"+%.2f CENTS", cents);
	} else {
	sprintf(texts,"%.2f CENTS", cents);
	} // cents
	} else {
	if (cents > 0.0) {
	sprintf(texts,"%.2f H[      +%.2f CENTS", frequency, cents);
	} else {
	sprintf(texts,"%.2f H[      %.2f CENTS", frequency, cents);
	} // cents
	} // debugfreq

	fade = 1.0; // set solid colour indicator to begin with
}
//
void audioLoopStop() {
	pspAudioSetChannelCallback(0, NULL, NULL);
	pspAudioInputEnd();	
	if(loop_buffer != NULL) {
	free(loop_buffer);
	loop_buffer = NULL;
	}
}
//
int exit_callback(int arg1, int arg2, void *common) {
	audioLoopStop();
	sceKernelExitGame();
	return 0;
}
//
int CallbackThread(SceSize args, void *argp) {
	int cbid;
	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}
//
int SetupCallbacks(void) {
	int thid = 0;
	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0) {sceKernelStartThread(thid, 0, 0);}
	return thid;
}
//
// draw variable width text
void drawtextvarwidth(void) {
	tcnt = 0;
	tindex = 0;
	tchar = 0;
	xx = 0;
	yy = 0;
	bitcnt = 0;
	while (texts[tchar] != 0) {
	tindex = (texts[tchar] - 0x20) * 7;
	tcnt = 0;
	if (charmapb[tindex] == 0) {
	tcnt++;
	if (charmapb[tindex + 1] == 0) {
	tcnt++;
	if (charmapb[tindex + 2] == 0 && texts[tchar] != 0x20) {tcnt++;}
	}
	} // charmapb
	while (tcnt < 7) {
	xx = tx;
	bitcnt = 7;
	while (bitcnt > 0) {
	bitcnt--;
	if ((charmapb[tindex + tcnt] & (0b00000001 << bitcnt)) != 0) {
	yy = ty + bitcnt;
	if (xx > -1 && xx < 480) {
	if (yy > -1 && yy < 272) {
	putPixelImage(0x00EFDDCA, xx, yy, tbackground);
	}}
	}
	} // bitcnt
	tx++; tcnt++;
	} // tcnt
	if (charmapb[tindex + 6] == 0) {
	tx--;
	if (charmapb[tindex + 5] == 0) {
	tx--;
	if (charmapb[tindex + 4] == 0) {
	tx--;
	}
	}
	} // charmapb
	tchar++; tx++;
	}
}
//
// draw sin variable width text
void drawtextvarwidthsin(void) {
	unsigned short sinvalue;
	tcnt = 0;
	tindex = 0;
	tchar = 0;
	xx = 0;
	yy = 0;
	bitcnt = 0;
	sinindex = sinstart; sindir = sinstartdir;
	if (sindir == 0) {
	sinindex = sinindex + 2;
	if (sinindex > 358) {sindir = 1;}
	} else {
	sinindex = sinindex - 2;
	if (sinindex < 1) {sindir = 0;}
	} // sindir
	sinstart = sinindex; sinstartdir = sindir;
	while (textn[tchar] != 0) {
	tindex = (textn[tchar] - 0x20) * 7;
	tcnt = 0;
	if (charmapb[tindex] == 0) {
	tcnt++;
	if (charmapb[tindex + 1] == 0) {
	tcnt++;
	if (charmapb[tindex + 2] == 0 && textn[tchar] != 0x20) {
	tcnt++;
	}
	}
	} // charmapb
	while (tcnt < 7) {
	if (sinindex > 179) {
	sinvalue = sin_table[359 - sinindex];
	} else {
	sinvalue = sin_table[sinindex];
	}
	if (sindir == 0) {
	sinindex = sinindex + 2;
	if (sinindex > 358) { sindir = 1; }
	} else {
	sinindex = sinindex - 2;
	if (sinindex < 1) { sindir = 0; }
	} // sindir
	xx = tx;
	sinvalue = sinvalue >> sinamp;
	if (sinindex > 350) {sinvalue = 0;}
	bitcnt = 7;
	while (bitcnt > 0) {
	bitcnt--;
	if ((charmapb[tindex + tcnt] & (0b00000001 << bitcnt)) != 0) {
	yy = ty + bitcnt;
	if (xx > -1 && xx < 480) {
	if (yy+sinvalue > -1 && yy+sinvalue < 272) {
	putPixelImage(0x00EFDDCA, xx, yy+sinvalue, background);
	}}
	}
	} // bitcnt
	tx++; tcnt++;
	} // tcnt
	if (charmapb[tindex + 6] == 0) {
	tx--;
	if (charmapb[tindex + 5] == 0) {
	tx--;
	if (charmapb[tindex + 4] == 0) {
	tx--;
	}
	}
	} // charmapb
	tchar++; tx++;
	}
}
//
// draw large variable width sine text to framebuffer
	void drawtextvarwidthsinlarge(void) {
	int tcnt = 0;
	int tindex = 0;
	int tchar = 0;
	int xx = 0;
	int yy = 0;
	unsigned char looptwice;
	bitcnt = 0;
	sinindex = sinstart; sindir = sinstartdir;
	if (sindir == 0) {
	sinindex = sinindex + 2;
	if (sinindex > 358) {sindir = 1;}
	} else {
	sinindex = sinindex - 2;
	if (sinindex < 1) {sindir = 0;}
	} // sindir
	sinstart = sinindex; sinstartdir = sindir;
	while (textn[tchar] != 0) {
	txp++;
	tindex = (textn[tchar] - 0x20) * 7;
	tcnt = 0;
	if (charmapb[tindex] == 0) {
	tcnt++;
	if (charmapb[tindex+1] == 0) {
	tcnt++;
	if (charmapb[tindex+2] == 0) {
	tcnt++;
	if (charmapb[tindex+3] == 0 && textn[tchar] != 0x20) {
	tcnt++;
	}
	}
	}
	} // charmapb 
	while (tcnt < 7) {
	if (sinindex > 179) {
	sinvalue = sin_table[359-sinindex];
	} else {
	sinvalue = sin_table[sinindex];
	}
	if (sindir == 0) {
	sinindex = sinindex + 2;
	if (sinindex > 358) {sindir = 1;}
	} else {
	sinindex = sinindex - 2;
	if (sinindex < 1) {sindir = 0;}
	} // sindir
	looptwice = 0;
	sinvalue = sinvalue >> sinamp;
	if (sinindex > 350) {sinvalue = 0;}
	while (looptwice < 2) {
	xx = txp;
	if (txp > 0) { // skip left of screen for performance
	int bitcnt = 16; // support scaled 8 pixel high characters
	while (bitcnt > 0) {
	bitcnt = bitcnt - 2;
	if ((charmapb[tindex+tcnt] & (0b00000001 << (bitcnt / 2))) != 0) {
	yy = typ + bitcnt;
	if (xx > -1 && xx < 480) {
	if (yy+sinvalue > -1 && yy+sinvalue < 271) {
	putPixelImage(0x00EFDDCA, xx, yy+sinvalue, background);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy+sinvalue, background);
	}}
	} // charmap
	} // bitcnt
	} // tx
	txp++; looptwice++;
	} // looptwice
	tcnt++;
	} // tcnt
	if (charmapb[tindex+6] == 0) {
	txp--;
	if (charmapb[tindex+5] == 0) {
	txp--;
	if (charmapb[tindex+4] == 0) {
	txp--;
	}
	}
	} // charmapb
	tchar++; txp++;
	}
}
//
// draw large variable width text to framebuffer
	void drawtextvarwidthlarge(void) {
	int tcnt = 0;
	int tindex = 0;
	int tchar = 0;
	int xx = 0;
	int yy = 0;
	unsigned char looptwice;
	txp++; // shift string one pixel right
	while (texts[tchar] != 0) {
	txp++;
	tindex = (texts[tchar] - 0x20) * 7;
	tcnt = 0;
	if (charmapb[tindex] == 0) {
	tcnt++;
	if (charmapb[tindex+1] == 0) {
	tcnt++;
	if (charmapb[tindex+2] == 0) {
	tcnt++;
	if (charmapb[tindex+3] == 0 && texts[tchar] != 0x20) {
	tcnt++;
	}
	}
	}
	} // charmapb 
	while (tcnt < 7) {
	looptwice = 0;
	while (looptwice < 2) {
	xx = txp;
	if (txp > 0) { // skip left of screen for performance
	int bitcnt = 16; // support scaled 8 pixel high characters
	while (bitcnt > 0) {
	bitcnt = bitcnt - 2;
	if ((charmapb[tindex+tcnt] & (0b00000001 << (bitcnt / 2))) != 0) {
	yy = typ + bitcnt;
	if (xx > -1 && xx < 480) {
	if (yy > -1 && yy < 271) {
	putPixelImage(0x00EFDDCA, xx, yy, background);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, background);
	}}
	} // charmap
	} // bitcnt
	} // tx
	txp++; looptwice++;
	} // looptwice
	tcnt++;
	} // tcnt
	if (charmapb[tindex+6] == 0) {
	txp--;
	if (charmapb[tindex+5] == 0) {
	txp--;
	if (charmapb[tindex+4] == 0) {
	txp--;
	}
	}
	} // charmapb
	tchar++; txp++;
	}
}
//
// draw large variable width text to a buffer
	void drawtextbuf(void) {
	int tcnt = 0;
	int tindex = 0;
	int tchar = 0;
	int xx = 0;
	int yy = 0;
	unsigned char looptwice;
	txp++; // shift string one pixel right
	while (textt[tchar] != 0) {
	txp++;
	tindex = (textt[tchar] - 0x20) * 7;
	tcnt = 0;
	if (charmapb[tindex] == 0) {
	tcnt++;
	if (charmapb[tindex+1] == 0) {
	tcnt++;
	if (charmapb[tindex+2] == 0) {
	tcnt++;
	if (charmapb[tindex+3] == 0 && textt[tchar] != 0x20) {
	tcnt++;
	}
	}
	}
	} // charmapb 
	while (tcnt < 7) {
	looptwice = 0;
	while (looptwice < 2) {
	xx = txp;
	if (txp > 0) { // skip left of screen for performance
	int bitcnt = 16; // support scaled 8 pixel high characters
	while (bitcnt > 0) {
	bitcnt = bitcnt - 2;
	if ((charmapb[tindex+tcnt] & (0b00000001 << (bitcnt / 2))) != 0) {
	yy = typ + bitcnt;
	if (xx > -1 && xx < 480) {
	if (yy > -1 && yy < 271) {
	if (imgbuf == 0) {
	putPixelImage(0x00EFDDCA, xx, yy, abackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, abackground);
	} else {
	putPixelImage(0x00EFDDCA, xx, yy, bbackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, bbackground);
	}
	}}
	} // charmap
	} // bitcnt
	} // tx
	txp++; looptwice++;
	} // looptwice
	tcnt++;
	} // tcnt
	if (charmapb[tindex+6] == 0) {
	txp--;
	if (charmapb[tindex+5] == 0) {
	txp--;
	if (charmapb[tindex+4] == 0) {
	txp--;
	}
	}
	} // charmapb
	tchar++; txp++;
	}
}
//
// draw extra large variable width text to a buffer
	void drawextralargetextbuf(void) {
	int tcnt = 0;
	int tindex = 0;
	int tchar = 0;
	int xx = 0;
	int yy = 0;
	unsigned char loopfourtimes;
	txp++; // shift string one pixel right
	while (textt[tchar] != 0) {
	txp++;
	tindex = (textt[tchar] - 0x20) * 7;
	tcnt = 0;
	if (charmapb[tindex] == 0) {
	tcnt++;
	if (charmapb[tindex+1] == 0) {
	tcnt++;
	if (charmapb[tindex+2] == 0) {
	tcnt++;
	if (charmapb[tindex+3] == 0 && textt[tchar] != 0x20) {
	tcnt++;
	}
	}
	}
	} // charmapb 
	while (tcnt < 7) {
	loopfourtimes = 0;
	while (loopfourtimes < 4) {
	xx = txp;
	if (txp > 0) { // skip left of screen for performance
	int bitcnt = 32; // support scaled 8 pixel high characters
	while (bitcnt > 0) {
	bitcnt = bitcnt - 4;
	if ((charmapb[tindex+tcnt] & (0b00000001 << (bitcnt / 4))) != 0) {
	yy = typ + bitcnt;
	if (xx > -1 && xx < 480) {
	if (yy > -1 && yy < 271) {
	if (imgbuf == 0) {
	putPixelImage(0x00EFDDCA, xx, yy, abackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, abackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, abackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, abackground);
	} else {
	putPixelImage(0x00EFDDCA, xx, yy, bbackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, bbackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, bbackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, bbackground);
	}
	}}
	} // charmap
	} // bitcnt
	} // tx
	txp++; loopfourtimes++;
	} // loopfourtimes
	tcnt++;
	} // tcnt
	if (charmapb[tindex+6] == 0) {
	txp--;
	if (charmapb[tindex+5] == 0) {
	txp--;
	if (charmapb[tindex+4] == 0) {
	txp--;
	}
	}
	} // charmapb
	tchar++; txp++; txp++; // more space for space character
	}
}
//
// draw extra extra large variable width text to a buffer
	void drawextraextralargetextbuf(void) {
	int tcnt = 0;
	int tindex = 0;
	int tchar = 0;
	int xx = 0;
	int yy = 0;
	unsigned char loopsixtimes;
	txp++; // shift string one pixel right
	while (textt[tchar] != 0) {
	txp++;
	tindex = (textt[tchar] - 0x20) * 7;
	tcnt = 0;
	if (charmapb[tindex] == 0) {
	tcnt++;
	if (charmapb[tindex+1] == 0) {
	tcnt++;
	if (charmapb[tindex+2] == 0) {
	tcnt++;
	if (charmapb[tindex+3] == 0 && textt[tchar] != 0x20) {
	tcnt++;
	}
	}
	}
	} // charmapb 
	while (tcnt < 7) {
	loopsixtimes = 0;
	while (loopsixtimes < 6) {
	xx = txp;
	if (txp > 0) { // skip left of screen for performance
	int bitcnt = 48; // support scaled 8 pixel high characters
	while (bitcnt > 0) {
	bitcnt = bitcnt - 6;
	if ((charmapb[tindex+tcnt] & (0b00000001 << (bitcnt / 6))) != 0) {
	yy = typ + bitcnt;
	if (xx > -1 && xx < 480) {
	if (yy > -1 && yy < 271) {
	if (imgbuf == 0) {
	putPixelImage(0x00EFDDCA, xx, yy, abackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, abackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, abackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, abackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, abackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, abackground);
	} else {
	putPixelImage(0x00EFDDCA, xx, yy, bbackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, bbackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, bbackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, bbackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, bbackground);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, bbackground);
	}
	}}
	} // charmap
	} // bitcnt
	} // tx
	txp++; loopsixtimes++;
	} // loopsixtimes
	tcnt++;
	} // tcnt
	if (charmapb[tindex+6] == 0) {
	txp--;
	if (charmapb[tindex+5] == 0) {
	txp--;
	if (charmapb[tindex+4] == 0) {
	txp--;
	}
	}
	} // charmapb
	tchar++; txp++; txp++; txp++; txp++; // more space for space character
	}
}
//
// called by pspaudiolib each time the audio input buffer needs to be drained
void audioInputLoopCallback(void* buf, unsigned int length, void *userdata) {
	short* ubuf = (short*) buf;
	int i;
	for (i = 0; i < PSP_NUM_AUDIO_SAMPLES; i++) {
	loop_buffer[i].l = ubuf[i];
	yint_input[yint_index] = ubuf[i];
	yint_index++;
	if (yint_index > YIN_BUFFER_SIZE-1) {
	yint_index = 0;
	while (yint_index < YIN_BUFFER_SIZE) {
	yint_buffer[yint_index] = yint_input[yint_index];
	yint_index++;
	} // yint_index
	yint_index = 0;
	}
	}
}
//
void audioLoopStart() {
	loop_buffer = (sample_t*) malloc(PSP_NUM_AUDIO_SAMPLES * sizeof(sample_t));
	pspAudioInputInit(mic_level, 44100);
	sceKernelDelayThread(200000);	
	pspAudioSetInputCallback(audioInputLoopCallback, NULL);
}
//
//
//
int main(int argc, char* argv[]) {
	SetupCallbacks();
	initGraphics();
	background = createImage(480,272);
	tbackground = createImage(480,16);
	abackground = createImage(480,256);
	bbackground = createImage(480,256);

	clearImage(0,background);
	clearImage(0,bbackground);
	clearImage(0,abackground);
	clearImage(0,tbackground); // black

	SceCtrlData pad;
	sceCtrlSetSamplingCycle(0);
	sceCtrlReadBufferPositive(&pad, 1);
	if(pad.Buttons & PSP_CTRL_UP) {
	if(pad.Buttons & PSP_CTRL_TRIANGLE) {YIN_BUFFER_SIZE = 4096;} // use 4096 sample chunks
	if(pad.Buttons & PSP_CTRL_SQUARE) {YIN_THRESHOLD = 0.04;} // higher audio sensitivity
	if(pad.Buttons & PSP_CTRL_SELECT) {debugfreq = 1;} // debug frequency display
	}

	if(pad.Buttons & PSP_CTRL_CROSS) {
	if(pad.Buttons & PSP_CTRL_DOWN) {largefreq = 1;} // large frequency display
	}

	if(pad.Buttons & PSP_CTRL_CIRCLE) { // skip intro by default
	sprintf (texts, "     BREK MARTIN 2025");
	sprintf (textn, "     CHROMATICA");

	Artpic[5] = 0x64;
	Artpic[25] = 0x6D;
	Artpic[37] = 0x1D;
	Artpic[12] = 0x27;
	Artpic[7] = 0x64;
	Artpic[26] = 0x6E;
	Artpic[0] = 0xE7;
	Artpic[6] = 0x74;
	Artpic[1] = 0x3E;
	int xor;
	for(xor=0; xor<316; xor++) {
	Artpic[xor] = Artpic[xor] ^ 0x6E;	// simple xor decrypt Art logo graphic
	} // xor

	Art = loadImageMemory(Artpic, 316);

	pyy = 1;
	pxx = 1;
	pixel = getPixelImage(pxx, pyy, Art);
	sample = pixel;
	for (pyy=0; pyy<57; pyy++) { // image height
	for (pxx=0; pxx<100; pxx++) { // image width
	pixel = getPixelImage(pxx, pyy, Art);
	if (pixel == sample) {
	putPixelImage(0x00000000, pxx, pyy, Art);
	} else {
	putPixelImage(0xFFEFDDCA, pxx, pyy, Art);
	}
	} // width
	} // height

	scroller = 26; // reset scroll position
	for(nux=0; nux<26; nux++) { // scroll in and wait
	tx = 124-(spmessage[scroller]+spmessage[scroller]+spmessage[scroller]); ty = 70;
	txp = tx; typ = ty;
	drawtextvarwidthsinlarge();
	tx = 81+(spmessage[scroller]+spmessage[scroller]+spmessage[scroller]); ty = 160;
	txp = tx; typ = ty;
	drawtextvarwidthlarge();
	if (scroller > 0) {
	scroller--;
	} else {
	pyy = 1; pxx = 1;
	pixel = getPixelImage(pxx, pyy, Art);
	sample = pixel;
	for (pyy=0; pyy<57; pyy++) { // image height
	for (pxx=0; pxx<100; pxx++) { // image width
	pixel = getPixelImage(pxx, pyy, Art);
	if (pixel != sample) {
	putPixelImage(0xFF090907, pxx+378, pyy+220, background);
	}
	} // width
	} // height
	} // scroller

	blitImageToScreen(0, 0, 480, 272, background, 0, 0);
	sceDisplayWaitVblankStart();
	flipScreen();
	clearImage(0,background);
	} // nux

	for(nux=0; nux<26; nux++) { // scroll out and wait
	tx = 124+(spmessage[scroller]+spmessage[scroller]+spmessage[scroller]); ty = 70;
	txp = tx; typ = ty;
	drawtextvarwidthsinlarge();
	tx = 81-(spmessage[scroller]+spmessage[scroller]+spmessage[scroller]); ty = 160;
	txp = tx; typ = ty;
	drawtextvarwidthlarge();
	if (scroller < 26) {scroller++;}
	blitImageToScreen(0, 0, 480, 272, background, 0, 0);
	sceDisplayWaitVblankStart();
	flipScreen();
	clearImage(0,background);
	} // nux
	blitImageToScreen(0, 0, 480, 272, background, 0, 0);
	sceDisplayWaitVblankStart();
	flipScreen();
	} // square pressed

	clearImage(0,background);
	clearImage(0x333333,tbackground);

	mic_level = 20480;
	loop_buffer = NULL;

	rate = 44100;
	textt[0] = 0;
	pspAudioInit();
	audioLoopStart();

	sprintf(texts,"CHROMATICA                                       BREK MARTIN                                                25/08/25"); // biuld date

	for(;;) { // main program loop

	float frequency = 0.0;
	if (yint_buffer[900] != 0) { // guard against processing an empty buffer
	if (hasEnoughSignal(yint_buffer, YIN_BUFFER_SIZE, 200.0f)) { // ignore if RMS < 400
	for(nux=0; nux<YIN_BUFFER_SIZE; nux++) {yin_buffer[nux] = yint_buffer[nux];} // make a copy of buffer
	frequency = yin_pitch_detect(yin_buffer, YIN_BUFFER_SIZE, 44100);
	}}

	if (frequency > 0) {detectNote(frequency);}
	tx = 10; ty = 5;
	drawtextvarwidth(); // print info bar
	
	txp = 168; typ = 11; // extra large scaled font
	if (textt[2] == 0) {txp = 194;} // non sharp note offset

	if (largefreq != 0) { // large frequency display mode
	txp = 8;
	if (frequency > 0) {
	sprintf(textt,"%.2f H[", frequency);
	}}

	drawextraextralargetextbuf(); // print note name

	colouri = 0x00444444;
	if (imgbuf == 0) {
	cdrawline (239,251,239,249);
	cdrawline (240,251,240,249);
	cdrawline (239,210,239,211);
	cdrawline (240,210,240,211);
	} else { // imgbuf
	ddrawline (239,251,239,249);
	ddrawline (240,251,240,249);
	ddrawline (239,210,239,211);
	ddrawline (240,210,240,211);
	} // imgbuf

	colouri = colors[xPos];
	colouri = fade_to_black(colouri, fade);
	if (fade > 0.0) {fade = fade - 0.02;}

	if (xPos > 2 && xPos < 477) { // draw indicator
	if (imgbuf == 0) {
	cdrawline (xPos+3,243,xPos+3,217);
	cdrawline (xPos+2,246,xPos+2,214);
	cdrawline (xPos+1,247,xPos+1,213);
	cdrawline (xPos,248,xPos,212);
	cdrawline (xPos-1,247,xPos-1,213);
	cdrawline (xPos-2,246,xPos-2,214);
	cdrawline (xPos-3,243,xPos-3,217);
	} else { // imgbuf
	ddrawline (xPos+3,243,xPos+3,217);
	ddrawline (xPos+2,246,xPos+2,214);
	ddrawline (xPos+1,247,xPos+1,213);
	ddrawline (xPos,248,xPos,212);
	ddrawline (xPos-1,247,xPos-1,213);
	ddrawline (xPos-2,246,xPos-2,214);
	ddrawline (xPos-3,243,xPos-3,217);
	} // imgbuf
	} // xpos

	int i = 0; // draw audio wave
    	long sample = (yint_buffer[6] + 32768) >> 9;
    	lastscaled = sample + 64;
	if (imgbuf != 0) {
	for (i = 10; i < 470; i = i + 2) {
    	sample = (yint_buffer[i] + 32768) >> 9;
    	unsigned char scaled = sample + 64;
	adrawline (i-2,lastscaled,i,scaled);
	lastscaled = scaled;
	} // i
	} else { // imgbuf
	for (i = 10; i < 470; i = i + 2) {
    	sample = (yint_buffer[i] + 32768) >> 9;
    	unsigned char scaled = sample + 64;
	bdrawline (i-2,lastscaled,i,scaled);
	lastscaled = scaled;
	} // i
	} // imgbuf

	if (imgbuf == 0) {
	blitImageToScreen(0, 0, 480, 256, abackground, 0, 16);
	blitImageToScreen(0, 0, 480, 16, tbackground, 0, 0);
	sceDisplayWaitVblankStart();
	flipScreen();
	clearImage(0x000000,abackground);	
	imgbuf = 1;
	} else {
	blitImageToScreen(0, 0, 480, 256, bbackground, 0, 16);
	blitImageToScreen(0, 0, 480, 16, tbackground, 0, 0);
	sceDisplayWaitVblankStart();
	flipScreen();
	clearImage(0x000000,bbackground);	
	imgbuf = 0;
	} // imgbuf
	clearImage(0x444444,tbackground);
	}
}




