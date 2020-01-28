#pragma once

#define NUM_LEDS 183

static uint8_t size = 1;

// cor10 = corner 10. cor10 = 105 means the 105th LED on the strip is at corner 10. Refer to drawing for corner numbers.
const int cor0 = 0, cor1 = 11, cor2 = 17, cor3 = 26, cor4 = 31, cor5 = 38, cor6 = 42, cor7 = 46, cor8 = 50, cor9 = 55, cor10 = 58;
const int cor11 = 66, cor12 = 74, cor13 = 80, cor14 = 92, cor15 = 107, cor16 = 109, cor17 = 118, cor18 = 126, cor19 = 128;
const int cor20 = 134, cor21 = 143, cor22 = 148, cor23 = 155, cor24 = 158, cor25 = 162, cor26 = 166, cor27 = 172, cor28 = NUM_LEDS;

const int unusedPixels = 15;
const int turnOffThesePixels[unusedPixels] = { 22, 23, 24, 25, 37, 54, 77, 78, 104, 105, 139, 140, 153, 154, 171};