#pragma once



// The purpose of this function is to:
// - pick a stretch of leds that you want the sparkles to show up in. between "sparkStart" and "sparkEnd"
// - pick a FFT bin that you want the sparkles to react to, "freq"
// - the "multiplierAdjustment" and "additionAdjustment" variables change how many sparkles appear
//   - "additionAdjustment" gets rid of low volume noise creating sparkles. If the FFT bin value is 20, AA is -50, then no sparkles show
// - "hue" chooses the color of the sparkles
// - if you want sparkles to be white, as in a color that cannot be choosen with a hue, put true for that argument
int maxNumOfSparkles = 25;

void addSparkles(int sparkStart, int sparkEnd, uint8_t freq, double multiplierAdjustment, int additionAdjustment, int hue, bool white) {
	// If sparkles are being added to react to music, do this:
	static int numOfSparkles;
	static int tempVar;
	int sparkleBrightness;

	// If the freq argument is a valid FFT bin, 0 - 15
	if (freq > 0 && freq < 15) {
		numOfSparkles = spectrumValue[freq] + additionAdjustment;
		numOfSparkles = constrain(numOfSparkles, 0, 500);
		sparkleBrightness = map(numOfSparkles, 0, 500, 0, 255);
		numOfSparkles = map(numOfSparkles, 0, 500, 0, maxNumOfSparkles);
		numOfSparkles = numOfSparkles * multiplierAdjustment;

		for (int i = 0; i < numOfSparkles; i++) {
			//leds[getRandomPixel(sparkStart, sparkEnd)] = sparkleColor;
			int pos = random16(sparkStart, sparkEnd);
			tempVar = constrain(map(ledx[pos] - ledy[pos], -31, 31, 0, 255), 0, 255);
			//CRGBPalette16 tempPalette;
			//tempPalette = currentPalette;
			//CRGB tempColor;
			//tempColor = ColorFromPalette(tempPalette, tempVar + hue1, sparkleBrightness);
			//leds2[pos] = tempColor;
			leds2[pos] = ColorFromPalette(currentPalette, tempVar + hue1, sparkleBrightness);
			//leds2[pos] = ColorFromPalette(orangeRed_p, 10, 200, LINEARBLEND);
			//leds2[pos] = CHSV(0, 255, sparkleBrightness);
			//if (white) leds2[pos] = CHSV(0, 0, sparkleBrightness);
			fadeValue = fadeValue + numOfSparkles * .5;
		}
	}
	// Else, if sparkles are being added as an ambient effect, do this:
	else {
		EVERY_N_MILLISECONDS(additionAdjustment) {
			int pos = random16(sparkStart, sparkEnd);
			leds[pos] = CHSV(hue, 255, brightness);
			if (white) leds[pos] = CHSV(0, 0, brightness);
		}
	}
}

// Adds sparkles to leds2[ ], not leds3[ ]
void addSparkles3(int sparkStart, int sparkEnd, uint8_t freq, double multiplierAdjustment, int additionAdjustment, int hue, bool white) {
	// If sparkles are being added to react to music, do this:
	static int numOfSparkles;
	static int tempVar;
	int sparkleBrightness;

	// If the freq argument is a valid FFT bin, 0 - 15
	if (freq > 0 && freq < 15) {
		numOfSparkles = spectrumValue[freq] + additionAdjustment;
		numOfSparkles = constrain(numOfSparkles, 0, 500);
		sparkleBrightness = map(numOfSparkles, 0, 500, 0, 255);
		numOfSparkles = map(numOfSparkles, 0, 500, 0, maxNumOfSparkles);
		numOfSparkles = numOfSparkles * multiplierAdjustment;

		for (int i = 0; i < numOfSparkles; i++) {
			//leds[getRandomPixel(sparkStart, sparkEnd)] = sparkleColor;
			int pos = random16(sparkStart, sparkEnd);
			tempVar = constrain(map(ledx[pos] - ledy[pos], -31, 31, 0, 255), 0, 255);
			if (!white) leds3[pos] = ColorFromPalette(currentPalette, tempVar + hue1, sparkleBrightness, LINEARBLEND);
			if (white) leds3[pos] = CHSV(0, 0, sparkleBrightness);
			fadeValue = fadeValue + numOfSparkles * .5;
		}
	}
	// Else, if sparkles are being added as an ambient effect, do this:
	else {
		EVERY_N_MILLISECONDS(additionAdjustment) {
			int pos = random16(sparkStart, sparkEnd);
			leds[pos] = CHSV(hue, 255, brightness);
			if (white) leds[pos] = CHSV(0, 0, brightness);
		}
	}
}

void fadeRange(int fadeVar, int start, int stop) {
	for (int i = start; i < stop; i++) leds[i].nscale8(fadeVar);
}

void fadeAll(int fadeVar) {
	for (int i = 0; i < NUM_LEDS; i++) leds[i].nscale8(fadeVar);
}

void fadeRange2(int fadeVar, int start, int stop) {
	for (int i = start; i < stop; i++) leds2[i].nscale8(fadeVar);
}

void fadeAll2(int fadeVar) {
	for (int i = 0; i < NUM_LEDS; i++) leds2[i].nscale8(fadeVar);
}

void fadeRange3(int fadeVar, int start, int stop) {
	for (int i = start; i < stop; i++) leds3[i].nscale8(fadeVar);
}

void fadeAll3(int fadeVar) {
	for (int i = 0; i < NUM_LEDS; i++) leds3[i].nscale8(fadeVar);
}

// Uses coordMode in sweeps() in Animations.h and does the math to propogate the line 
int yint, slope;
void linearSweepMath(int coordMode) {
	if (coordMode == 3 || coordMode == 4) { // This one is rotated 90 degrees so the math works.
		for (int i = 0; i < NUM_LEDS; i++) {
			if (slope * ledy[i] + yint - ledx[i] < 1 && slope *ledy[i] + yint - ledx[i] > -1) leds3[i] = color;
		}
	}
	else {
		for (int i = 0; i < NUM_LEDS; i++) {
			if (slope * ledx[i] + yint - ledy[i] < 1 && slope *ledx[i] + yint - ledy[i] > -1) leds3[i] = color;
		}
	}
}

double radius[5];
void propagateCircle(CHSV color, int startx, int starty, double radius) {
	for (int i = 0; i < NUM_LEDS; i++) {
		if (pow(ledx[i] - startx, 2) + pow(ledy[i] - starty, 2) - pow(radius, 2) > -10 && pow(ledx[i] - startx, 2) + pow(ledy[i] - starty, 2) - pow(radius, 2) < 10) leds3[i] = color;
	}
}

void lightSide(int start, int stop, bool fade, CHSV color) {
	if (stop > start) {
		for (int i = start; i < stop; i++) {
			if (fade)
				leds3[i] = CHSV(color.h, color.s, 255 - ((i - start) * 200 / (stop - start)));
			else
				leds3[i] = color;
		}
	}
	else { // start > stop, if you want it to fade backwards
		for (int i = start; i > stop; i--) {
			if (fade)
				leds3[i] = CHSV(color.h, color.s, 255 + ((i - start) * 200 / (start - stop)));
			else
				leds3[i] = color;
		}
	}
}

// Bouncing Dot mode variables and functions below

int topSmallSqVar, botSmallSqVar;
int topSquareColor, botSquareColor;
int movingLightPosition;

void fadeInsideBigSquare(int fadeVar) {
	for (int i = cor0; i < cor1; i++) leds3[i].nscale8(fadeVar);
	for (int i = cor5; i < cor7; i++) leds3[i].nscale8(fadeVar);
	for (int i = cor13; i < cor15; i++) leds3[i].nscale8(fadeVar);
	for (int i = cor25; i < cor28; i++) leds3[i].nscale8(fadeVar);
}

void bouncingDotSmallSquares(int botSquareColor, int topSquareColor) {
	if (topSmallSqVar < (cor24 - cor23 + 3)) {
		leds3[cor23 + topSmallSqVar - 1] = CHSV(topSquareColor, 255, 255);
		leds3[cor25 - topSmallSqVar - 1] = CHSV(topSquareColor, 255, 255);
		leds3[cor21 + topSmallSqVar] = CHSV(topSquareColor, 255, 255);
		leds3[cor23 - topSmallSqVar - 3] = CHSV(topSquareColor, 255, 255); // "-3" is because the bend around the wood wastes a few LEDS
	}
	if (topSmallSqVar < (cor19 - cor17 + 1)) {
		leds3[cor19 - topSmallSqVar - 1] = CHSV(topSquareColor, 255, 255);
		leds3[cor15 + topSmallSqVar] = CHSV(topSquareColor, 255, 255);
	}
	if (botSmallSqVar < (cor2 - cor1)) {
		leds3[cor9 - botSmallSqVar - 2] = CHSV(botSquareColor, 255, 255);
		leds3[cor7 + botSmallSqVar] = CHSV(botSquareColor, 255, 255);
		leds3[cor1 + botSmallSqVar] = CHSV(botSquareColor, 255, 255);
		leds3[cor3 - botSmallSqVar - 5] = CHSV(botSquareColor, 255, 255);
	}
	if (botSmallSqVar < (cor11 - cor9 + 1)) {
		leds3[cor9 + botSmallSqVar] = CHSV(botSquareColor, 255, 255);
		leds3[cor13 - botSmallSqVar - 4] = CHSV(botSquareColor, 255, 255);
	}
}

void blend2(int mixAmount) {
	for (int i = 0; i < NUM_LEDS; i++) 
		nblend(leds[i], leds2[i], mixAmount);
}

void blend3(int mixAmount) {
	for (int i = 0; i < NUM_LEDS; i++)
		nblend(leds[i], leds3[i], mixAmount);
}