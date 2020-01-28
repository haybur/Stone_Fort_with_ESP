#pragma once

#define NUM_LEDS 181
#define NUM_LEDS_PER_STRIP 181

const uint8_t size = 1;

// cor10 = corner 10. cor10 = 105 means the 105th LED on the strip is at corner 10. Refer to drawing for corner numbers
const int cor0 = 0;
const int cor1 = 12;
const int cor2 = 17;
const int cor3 = 26;
const int cor4 = 31;
const int cor5 = 38;
const int cor6 = 41;
const int cor7 = 45;
const int cor8 = 49;
const int cor9 = 55;
const int cor10 = 57;
const int cor11 = 65;
const int cor12 = 73;
const int cor13 = 79;
const int cor14 = 91;
const int cor15 = 106;
const int cor16 = 108;
const int cor17 = 116;
const int cor18 = 124;
const int cor19 = 127;
const int cor20 = 133;
const int cor21 = 141;
const int cor22 = 147;
const int cor23 = 154;
const int cor24 = 157;
const int cor25 = 161;
const int cor26 = 164;
const int cor27 = 170;
const int cor28 = NUM_LEDS;

const int turnOffThesePixels[] = { 22, 23, 24, 25, 36, 37, 54, 77, 78, 103, 104, 105, 138, 139, 139, 140, 152, 153, 169};