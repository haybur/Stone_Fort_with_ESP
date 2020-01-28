#pragma once

#define NUM_LEDS_PER_STRIP 343
#define NUM_LEDS 343

static uint8_t size = 2;

// cor10 = corner 10. cor10 = 105 means the 105th LED on the strip is at corner 10. Refer to drawing for corner numbers.
//const int cor0 = 0, cor1 = 11, cor2 = 17, cor3 = 26, cor4 = 31, cor5 = 38, cor6 = 42, cor7 = 46, cor8 = 50, cor9 = 55, cor10 = 58;
//const int cor11 = 66, cor12 = 74, cor13 = 80, cor14 = 92, cor15 = 107, cor16 = 109, cor17 = 118, cor18 = 126, cor19 = 128;
//const int cor20 = 134, cor21 = 143, cor22 = 148, cor23 = 155, cor24 = 158, cor25 = 162, cor26 = 166, cor27 = 172, cor28 = NUM_LEDS;

int cor0 = 0, cor1 = 26, cor2 = 35, cor3 = 45, cor4 = 54, cor5 = 63, cor6 = 71, cor7 = 79, cor8 = 87, cor9 = 97, cor10 = 105;
int cor11 = 121, cor12 = 138, cor13 = 147, cor14 = 172, cor15 = 197, cor16 = 206, cor17 = 222, cor18 = 240, cor19 = 248;
int cor20 = 257, cor21 = 267, cor22 = 276, cor23 = 286, cor24 = 294, cor25 = 302, cor26 = 310, cor27 = 319, cor28 = 343;

const int unusedPixels = 15;
const int turnOffThesePixels[unusedPixels] = {  };