#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#define USE_OCTOWS2811
#include <OctoWS2811.h>
#include <FastLED.h>
#include <Statistic.h>
#include <Button.h>
#define buttonPin1 15
#define buttonPin2 16
Button button1(buttonPin1);
Button button2(buttonPin2);
double mult = 1.00;
#include "DDAudio.h"

#define NUM_STRIPS 3

#include "cornersSerial2.h"
//#include "cornersSerial.1big.h"

//CRGB leds[NUM_LEDS];
//CRGB leds2[NUM_LEDS];
//CRGB leds3[NUM_LEDS];
CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
CRGB leds2[NUM_STRIPS * NUM_LEDS_PER_STRIP];
CRGB leds3[NUM_STRIPS * NUM_LEDS_PER_STRIP];
int ledx[NUM_LEDS], ledy[NUM_LEDS];  // Used for giving every LED an x and y coordinate

#include "DDGradients.h"
// COLOR PALLETTE STUFF
CRGBPalette16 currentPalette = redBlueRed_p;
uint8_t currentPaletteNumber;
TBlendType currentBlending;

uint8_t brightness = 255;

// Mode Initiation Variables
bool initiate = true;
bool changingHue = false;
double fadeValue;
bool autoSwitch = 1;  // 0 = not switching, 1 = autoswitch mode
int SFmode = 1;
int SFmodeOld;
int audioMultiplier;

uint8_t hue, hue1, hue2;
CHSV color;
uint8_t masterBrightness = 150;
int MVPattern;
int ambPattern = 1;

double speed;

//elapsedMillis time2;

#include "SFDrawing.h" // Must be above SFanimatinos
#include "SFAnimations.h"
#include "SFESPcomm.h"
#include "SFPatternLists.h"

void setup()
{
	button1.begin();
	button2.begin();

	audioSetup();

	Serial.begin(115200);
	Serial1.begin(115200); // for ESP communication
	LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS);
	//LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS_PER_STRIP);

	FastLED.clear(true);
	delay(1000);
	// limit my draw to 1A at 5v of power draw
	FastLED.setMaxPowerInVoltsAndMilliamps(5, 10000);

	// Set x and y coordinates for each LED pixel
	setCoordinates();

	//STATS
	clearStats();

	// COLOR PALETTE STUFF
	currentBlending = LINEARBLEND;
	//currentBlending = NOBLEND;

	LEDS.setBrightness(masterBrightness);

	Serial.println("Setup Complete!");
}

void loop()
{
	LEDS.setBrightness(masterBrightness);

	talkToESP();

	fillStats();
	musicAnalytics(autoSwitch);

	normalOperation();

	//Serial.print(lowAveBin);
	//Serial.print("\t");
	//Serial.print(lowBeatBin);
	//Serial.print("\t");
	//Serial.print(midAveBin);
	//Serial.print("\t");
	//Serial.print(midBeatBin);
	//Serial.print("\t");
	//Serial.print(highAveBin);
	//Serial.print("\t");
	//Serial.println(highBeatBin);

	//////// DIAGNOSTIC FUNCTIONS
	//forTestingInputs();
	//displayCorners();
	//printSpectrum();
	//Turn entire strip Green
	//for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGB::Green;
	//Serial.println("is this working?");

	// Turn off pixels in the corners that can't be seen
	//for (int i = 0; i < ARRAY_SIZE(turnOffThesePixels); i++)  // FOR SOME REASON, THIS CAUSES THE LEDS TO FLICKER
	//	leds[turnOffThesePixels[i]] = 0;
	bruteForceTurnOffThesePixels();

	// Copy leds to output #3
	for (int i = 0; i < NUM_LEDS; i++) 	leds[i + NUM_LEDS * 2] = leds[i];

	LEDS.show();
}

void normalOperation() {
	if (changingHue) {
		EVERY_N_MILLISECONDS(200) hue++;
	}

	switch (SFmode) {
	case 0:
		LEDS.clear(true);
		break;
	case 1:
		//if (silence)  ambientPattern();
		 autoMusicVisualizing();
		break;
	case 2:
		ambientPattern();
		break;
	case 3:
		solidColor();
		break;
	case 4:
		staticMusicVizPattern();
		break;
	}

}

void autoMusicVisualizing() {
	static int mixAmountOld;
	// While mixAmount is showing a beat mode, change the color palette used by the non-beat mode
	if (mixAmount == 255) currentPaletteNumber = random8(gGradientPaletteCount);
	currentPalette = gGradientPalettes[currentPaletteNumber];

	if (mixAmount == 0 && mixAmountOld != 0) {
		nextLowBeatPattern();
		if (currentLowBeatPatternNumber == 2)
			coordModeMain = random8(1, 6);
	}
	if (mixAmount == 255 && mixAmountOld != 255) {
		changingHue = random8(2);
	}
	mixAmountOld = mixAmount;

	if (constBeat) circleDouble();
	else lowBeatPatterns[currentLowBeatPatternNumber]();	
	sixFreqGlitter();

	blend3(mixAmount);
	blend2((mixAmount - 255) * -1);

	//for (int i = 0; i < NUM_LEDS; i++) leds[i] = leds3[i];
	//for (int i = 0; i < NUM_LEDS; i++) leds[i] = leds2[i];
}

void staticMusicVizPattern() {
	switch (MVPattern) {
	case 1:
		sixFreqGlitter();
		EVERY_N_SECONDS(15) { currentPaletteNumber = random8(gGradientPaletteCount); }
		currentPalette = gGradientPalettes[currentPaletteNumber];
		blend2(255);
		break;
	case 2:
		sweepsToBeat();
		EVERY_N_SECONDS(15) { coordModeMain = random8(1, 6); }
		blend3(255);
		break;
	case 3:
		lowBeatWithFadeAndSparkles();
		blend3(255);
		break;
	case 4:
		lowBeatNoFadeAndSparkles();
		blend3(255);
		break;
	case 5:
		bouncingDotWithFade();
		blend3(255);
		break;
	case 6:
		bouncingDotWithoutFade();
		blend3(255);
		break;
	case 7:
		circle_midOut_alternatingBlack();
		blend3(255);
		break;
	case 8:
		circle_midOut_fading();
		blend3(255);
		break;
	case 9:
		circle_midOut_NoFade();
		blend3(255);
		break;
	case 10:
		circleDouble();
		blend3(255);
		break;
	}
}

void ambientPattern() {
	switch (ambPattern) {
	case 1:
		ambientRainbow();
		break;
	case 2:
		ambientSweeps();
		blend3(255);
		break;
	case 3:
		ambientBouncingBall();
		blend3(255);
		break;
	case 4:
		ambientSparkles();
		break;
	}
}

// AMBIENT MODES
void ambientRainbow() {
	if (initiate) {
		changingHue = false;
		initiate = false;
	}
	//int tempTime = silenceDuration;
	//tempTime = constrain(tempTime, 5000, 10000);
	//uint8_t ambientBrightness = map(tempTime, 5000, 10000, 10, 255);
	//ambientBrightness = constrain(ambientBrightness, 0, 255);

	for (int i = 0; i < NUM_LEDS; i++) {
		int temp = -1 * ledx[i] + ledy[i];
		leds[i] = CHSV(temp + hue, 255, 255);
	}
	EVERY_N_MILLIS(50)
		hue = hue + 2;

	leds[0] = leds[1];
}

void ambientBouncingBall() {
	speed = 40;
	bouncingDot(255, .02);

	fadeRange3(254, cor3, cor7);
	fadeRange3(254, cor19, cor21);
	fadeRange3(254, cor25, cor27);
}

void solidColor() {
	for (int i = 0; i < NUM_LEDS; i++)
		leds[i] = CHSV(solidHue, solidSat, solidVal);
}

void ambientSparkles() {
	addSparkles(cor0, cor28, 20, 0, 20, random8(), true);
	fadeAll(250);
}

void ambientSweeps() {
	if (initiate) {
		changingHue = true;
		initiate = false;
		coordMode = random8(1, 9);
	}

	//Serial.print(sweepStatus);
	//Serial.print("\t");
	//Serial.print(var);
	//Serial.print("\t");
	//Serial.print(coordMode);
	//Serial.print("\t");
	//Serial.println(time);

	if (!sweepStatus && time > 3000) {
		var = 0;
		coordMode = random8(1, 9);
		color = CHSV(random8(), 255, 255);
	}

	if (sweepStatus && time > var*2) {
		var++;
		time = 0;
	}

	sweeps(coordMode);

	linearSweepMath(coordMode);
	
	fadeAll(253);
}

void displayCorners() {
	// Use this code to display a red LED at an interval of 10 to figure out the corner numbers
	//for (int i = 0; i < 200; i = i + 10) {
	//  leds[i] = CRGB::Red;
	//}

	leds[cor0] = CHSV(0, 255, 255);
	leds[cor1] = CHSV(0, 255, 255);
	leds[cor2] = CHSV(0, 255, 255);
	leds[cor3] = CHSV(0, 255, 255);
	leds[cor4] = CHSV(0, 255, 255);
	leds[cor5] = CHSV(0, 255, 255);
	leds[cor6] = CHSV(0, 255, 255);
	leds[cor7] = CHSV(0, 255, 255);
	leds[cor8] = CHSV(0, 255, 255);
	leds[cor9] = CHSV(0, 255, 255);
	leds[cor10] = CHSV(0, 255, 255);
	leds[cor11] = CHSV(0, 255, 255);
	leds[cor12] = CHSV(0, 255, 255);
	leds[cor13] = CHSV(0, 255, 255);
	leds[cor14] = CHSV(0, 255, 255);
	leds[cor15] = CHSV(0, 255, 255);
	leds[cor16] = CHSV(0, 255, 255);
	leds[cor17] = CHSV(0, 255, 255);
	leds[cor18] = CHSV(0, 255, 255);
	leds[cor19] = CHSV(0, 255, 255);
	leds[cor20] = CHSV(0, 255, 255);
	leds[cor21] = CHSV(0, 255, 255);
	leds[cor22] = CHSV(0, 255, 255);
	leds[cor23] = CHSV(0, 255, 255);
	leds[cor24] = CHSV(0, 255, 255);
	leds[cor25] = CHSV(0, 255, 255);
	leds[cor26] = CHSV(0, 255, 255);
	leds[cor27] = CHSV(0, 255, 255);
	leds[cor28 - 1] = CHSV(0, 255, 255); // -1 is for WS2813 LEDs throwing out the last LED when the 2 data signals are connected before leds[0]

	for (int i = 0; i < (sizeof(turnOffThesePixels) / sizeof(turnOffThesePixels[0])); i++) {
		leds[turnOffThesePixels[i]] = CHSV(96, 255, 255);
	}
}

void forTestingInputs() {
	//input testing
	/*if (button1.released()) {
	Serial.println("released");
	}*/

	Serial.print(button1.read());
	Serial.print("\t");
	Serial.println(button2.read());
}

void setCoordinates() {

	if (size == 2) {
		for (int i = cor0; i < cor1; i++) { ledx[i] = i - 17;            ledy[i] = -17; }
		for (int i = cor1; i < cor2; i++) { ledx[i] = 8;                 ledy[i] = i - cor1 - 16; }
		for (int i = cor2; i < cor3; i++) { ledx[i] = i - cor2 + 9;      ledy[i] = -8; }
		for (int i = cor3; i < cor4; i++) { ledx[i] = 18;                ledy[i] = -i + cor3 - 10; }
		for (int i = cor4; i < cor5; i++) { ledx[i] = -i + cor4 + 17;    ledy[i] = -18; }
		for (int i = cor5; i < cor6; i++) { ledx[i] = i - cor5 + 9;      ledy[i] = -16; }
		for (int i = cor6; i < cor7; i++) { ledx[i] = 16;                ledy[i] = i - cor6 - 16; }
		for (int i = cor7; i < cor8; i++) { ledx[i] = -i + cor7 + 16;    ledy[i] = -10; }
		for (int i = cor8; i < cor9; i++) { ledx[i] = 10;                ledy[i] = -i + cor8 - 10; }
		for (int i = cor9; i < cor10; i++) { ledx[i] = 10;               ledy[i] = -i + cor9 - 18; }
		for (int i = cor10; i < cor11; i++) { ledx[i] = i - cor10 + 10;  ledy[i] = -25; }
		for (int i = cor11; i < cor12; i++) { ledx[i] = 25;              ledy[i] = i - cor11 - 24; }
		for (int i = cor12; i < cor13; i++) { ledx[i] = -i + cor12 + 24;  ledy[i] = -8; }
		for (int i = cor13; i < cor14; i++) { ledx[i] = 17;              ledy[i] = i - cor13 - 8; }
		for (int i = cor14; i < cor15; i++) { ledx[i] = -i + cor14 + 16; ledy[i] = 17; }
		for (int i = cor15; i < cor16; i++) { ledx[i] = -10;             ledy[i] = i - cor15 + 18; }
		for (int i = cor16; i < cor17; i++) { ledx[i] = -i + cor16 - 10; ledy[i] = 25; }
		for (int i = cor17; i < cor18; i++) { ledx[i] = -25;             ledy[i] = -i + cor17 + 24; }
		for (int i = cor18; i < cor19; i++) { ledx[i] = i - cor18 - 25;   ledy[i] = 10; }
		for (int i = cor19; i < cor20; i++) { ledx[i] = -18;             ledy[i] = i - cor19 + 10; }
		for (int i = cor20; i < cor21; i++) { ledx[i] = i - cor20 - 16;  ledy[i] = 18; }
		for (int i = cor21; i < cor22; i++) { ledx[i] = -8;              ledy[i] = -i + cor21 + 16; }
		for (int i = cor22; i < cor23; i++) { ledx[i] = -i + cor22 - 10; ledy[i] = 8; }
		for (int i = cor23; i < cor24; i++) { ledx[i] = i - cor23 - 16;  ledy[i] = 10; }
		for (int i = cor24; i < cor25; i++) { ledx[i] = -10;             ledy[i] = i - cor24 + 10; }
		for (int i = cor25; i < cor26; i++) { ledx[i] = -i + cor25 - 10; ledy[i] = 16; }
		for (int i = cor26; i < cor27; i++) { ledx[i] = -16;             ledy[i] = -i + cor26 + 16; }
		for (int i = cor27; i < cor28; i++) { ledx[i] = -17;             ledy[i] = -i + cor27 + 8; }
	}
	//  These are the coordinates set for the small mirror
	if (size == 1) {
		for (int i = cor0; i < cor1; i++) { ledx[i] = i - 8;             ledy[i] = -10; }
		for (int i = cor1; i < cor2; i++) { ledx[i] = 3;                 ledy[i] = i - cor1 - 9; }
		for (int i = cor2; i < cor3; i++) { ledx[i] = i - cor2 + 4;      ledy[i] = -3; }
		for (int i = cor3; i < cor4; i++) { ledx[i] = 12;                ledy[i] = -i + cor3 - 7; }
		for (int i = cor4; i < cor5; i++) { ledx[i] = -i + cor4 + 11;    ledy[i] = -12; }
		for (int i = cor5; i < cor6; i++) { ledx[i] = i - cor5 + 6;      ledy[i] = -10; }
		for (int i = cor6; i < cor7; i++) { ledx[i] = 10;                ledy[i] = i - cor6 - 9; }
		for (int i = cor7; i < cor8; i++) { ledx[i] = -i + cor7 + 9;    ledy[i] = -5; }
		for (int i = cor8; i < cor9; i++) { ledx[i] = 5;                ledy[i] = -i + cor8 - 6; }
		for (int i = cor9; i < cor10; i++) { ledx[i] = 5;               ledy[i] = -i + cor9 - 12; }
		for (int i = cor10; i < cor11; i++) { ledx[i] = i - cor10 + 6;  ledy[i] = -15; }
		for (int i = cor11; i < cor12; i++) { ledx[i] = 15;              ledy[i] = i - cor11 - 14; }
		for (int i = cor12; i < cor13; i++) { ledx[i] = -i + cor12 + 14;  ledy[i] = -5; }
		for (int i = cor13; i < cor14; i++) { ledx[i] = 10;              ledy[i] = i - cor13 - 3; }

		for (int i = cor14; i < cor15; i++) { ledx[i] = -i + cor14 + 9; ledy[i] = 10; }
		for (int i = cor15; i < cor16; i++) { ledx[i] = -5;             ledy[i] = i - cor15 + 12; }
		for (int i = cor16; i < cor17; i++) { ledx[i] = -i + cor16 - 7; ledy[i] = 15; }
		for (int i = cor17; i < cor18; i++) { ledx[i] = -15;             ledy[i] = -i + cor17 + 14; }
		for (int i = cor18; i < cor19; i++) { ledx[i] = i - cor18 - 14;  ledy[i] = 5; }
		for (int i = cor19; i < cor20; i++) { ledx[i] = -12;             ledy[i] = i - cor19 + 7; }
		for (int i = cor20; i < cor21; i++) { ledx[i] = i - cor20 - 11;  ledy[i] = 12; }
		for (int i = cor21; i < cor22; i++) { ledx[i] = -3;              ledy[i] = -i + cor21 + 8; }
		for (int i = cor22; i < cor23; i++) { ledx[i] = -i + cor22 - 5; ledy[i] = 4; }
		for (int i = cor23; i < cor24; i++) { ledx[i] = i - cor23 - 10;  ledy[i] = 5; }
		for (int i = cor24; i < cor25; i++) { ledx[i] = -5;             ledy[i] = i - cor24 + 5; }
		for (int i = cor25; i < cor26; i++) { ledx[i] = -i + cor25 - 7; ledy[i] = 10; }
		for (int i = cor26; i < cor27; i++) { ledx[i] = -11;             ledy[i] = -i + cor26 + 9; }
		for (int i = cor27; i <= cor28; i++) { ledx[i] = -11;             ledy[i] = -i + cor27 + 3; }
	}
}

void bruteForceTurnOffThesePixels() {
	leds[23] = CHSV(0, 0, 0);
	leds[24] = CHSV(0, 0, 0);
	leds[25] = CHSV(0, 0, 0);
	leds[37] = CHSV(0, 0, 0);
	leds[54] = CHSV(0, 0, 0);
	leds[77] = CHSV(0, 0, 0);
	leds[78] = CHSV(0, 0, 0);
	leds[103] = CHSV(0, 0, 0);
	leds[104] = CHSV(0, 0, 0);
	leds[105] = CHSV(0, 0, 0);
	leds[138] = CHSV(0, 0, 0);
	leds[139] = CHSV(0, 0, 0);
	leds[140] = CHSV(0, 0, 0);
	leds[141] = CHSV(0, 0, 0);
	leds[152] = CHSV(0, 0, 0);
	leds[153] = CHSV(0, 0, 0);
	leds[169] = CHSV(0, 0, 0);
}