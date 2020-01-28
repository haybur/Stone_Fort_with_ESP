//// STONE FORT ANIMATIONS ////
elapsedMillis time, time1;

// Six zones of glitter reacting to only 3 frequencies. 
void sixFreqGlitter() {
	if (initiate) {
		//brightness = 255;
		//currentPalette = justWhite_p;
		//currentPalette = gGradientPalettes[random8(gGradientPaletteCount)];
		changingHue = random8(2);
		initiate = false;
	}

	if (changingHue)
		hue1++;

	addSparkles(cor9, cor13, highAveBin, .15, -50, 0, false);
	addSparkles(cor1, cor9, midAveBin, .15, -50, 64, false);
	addSparkles(cor13, cor15, lowAveBin, .2, -50, 128, false);
	addSparkles(cor27, cor28, lowAveBin, .2, -50, 128, false);
	addSparkles(cor0, cor1, lowAveBin, .2, -50, 128, false);
	addSparkles(cor19, cor27, midAveBin, .15, -50, 64, false);
	addSparkles(cor15, cor19, highAveBin, .15, -50, 0, false);

	//addSparkles(cor9, cor13, 12, .15, -50, 0, false);
	//addSparkles(cor1, cor9, 8, .15, -50, 64, false);
	//addSparkles(cor13, cor15, 2, .2, -50, 128, false);
	//addSparkles(cor27, cor28, 2, .2, -50, 128, false);
	//addSparkles(cor0, cor1, 2, .2, -50, 128, false);
	//addSparkles(cor19, cor27, 8, .15, -50, 64, false);
	//addSparkles(cor15, cor19, 12, .15, -50, 0, false);
	
	fadeValue = constrain(fadeValue - 2, 0, 10);
	fadeAll2(254 - fadeValue);
}

///////// SWEEPS ///////////
int var;
bool sweepStatus;
bool pingPong;
int coordModeMain, coordMode;
// Put in the direction as an input and this will return the slope and starting y-int of the line that is sweeping across design
void sweeps(int coordMode) {
	switch (coordMode) {
	case 1:                ////// SWEEPING UP TO THE RIGHT
		slope = -1;
		yint = ledy[cor0] + ledx[cor0] + var;
		delay(16);
		if (yint > (ledy[cor14] + ledx[cor14])) sweepStatus = false;
		else sweepStatus = true;
		break;
	case 2:              ////// SWEEPING DOWN TO THE LEFT
		slope = -1;
		yint = ledy[cor14] + ledx[cor14] - var;
		delay(16);
		if (yint < (ledy[cor0] + ledx[cor0])) sweepStatus = false;
		else sweepStatus = true;
		break;
	case 3:              ////// SWEEPING UP FROM THE BOTTOM
		slope = 0;
		yint = ledy[cor10] + var;
		delay(16);            /// Delay to make it take about the same time to sweep up/down as it takes to go diagonal
		if (yint > ledy[cor16]) sweepStatus = false;
		else sweepStatus = true;
		break;
	case 4:              ////// SWEEPING DOWN FROM THE TOP
		slope = 0;
		yint = ledy[cor16] - var;
		delay(16);  // OLD DELAY VALUE IS 8, FOR THE TEENSY 3.2
		if (yint < ledy[cor10]) sweepStatus = false;
		else sweepStatus = true;
		break;
	case 5:              ////// SWEEPING LEFT TO RIGHT
		slope = 0;
		yint = ledx[cor17] + var;
		delay(16);
		if (yint > ledx[cor11]) sweepStatus = false;
		else sweepStatus = true;
		break;
	case 6:       ////// SWEEPING RIGHT TO LEFT
		slope = 0;
		yint = ledx[cor11] - var;
		delay(16);
		if (yint < ledx[cor17]) sweepStatus = false;
		else sweepStatus = true;
		break;
	case 7:     ////// SWEEPING UP TO THE LEFT
		slope = 1;
		yint = ledy[cor10] - ledx[cor11] + var;
		if (yint > (ledy[cor17] - ledx[cor17])) sweepStatus = false;
		else sweepStatus = true;
		break;
	case 8:     ////// SWEEPING DOWN TO THE RIGHT
		slope = 1;
		yint = ledy[cor17 - 1] - ledx[cor17 - 1] - var;
		if (yint < (ledy[cor10] - ledx[cor11 - 1]) - 1) sweepStatus = false;
		else sweepStatus = true;
		break;
	}
}

// Depending on the coordModeMain variable set in initiate(), this function switches coordMode between 2 values to make it go back and fourth
void pingPongingSweeps() {
	pingPong = !pingPong;

	switch (coordModeMain) {
	default:
		coordMode = random8(1, 9);
		break;
	case 1:
		coordMode = (pingPong) ? 1 : 2;
		break;
	case 2:
		coordMode = (pingPong) ? 3 : 4;
		break;
	case 3:
		coordMode = (pingPong) ? 5 : 6;
		break;
	case 4:
		coordMode = (pingPong) ? 7 : 8;
		break;
	case 5:
		coordMode = random8(1, 9);
		break;
	}
}

void sweepsToBeat() {
	if (initiate) {
		coordModeMain = random8(1, 6); // random8(n,m) goes from n to m-1
		changingHue = false;
		initiate = false;
	}

	if (beatDetected[lowBeatBin] == 2) {
		// Sweep from the opposite side. "coordModeMain" sets which direction it's pingponging
		pingPongingSweeps();
		// Decrease the speed variable, which increases the speed of the sweep. (less time passing before it continues)
		speed -= .85*speed;
		// Reset the sweep variable
		var = 0;
		brightness = 255;
	}

	// Sweeps() sets the slope, y intercept, and increases "var" until it completes the sweep
	sweeps(coordMode);

	// This function calculates which LEDs should light up as the line sweeps across the mirror
	color = CHSV(hue, 255, 255);
	linearSweepMath(coordMode);

	//// slow down the sweep by a little bit
	speed = constrain(speed + .1 + .02*speed, 0, 80);
	fadeAll3(250);

	// When sweepStatus = true, the sweep is done. If done, display glitter on small squares
	if (!sweepStatus) {

		addSparkles3(cor0, cor28, 12, .1, -50, 0, true);
		addSparkles3(cor0, cor28, 10, .1, -50, 0, true);

		//sixFreqGlitter2();

		//// 0 keeps any color from being displayed in the big middle square
		//fadeRange2(0, cor0, cor1);
		//fadeRange2(0, cor27, cor28);
		//fadeRange2(0, cor13, cor15);
	}

	//// If enough time has passed, increase the sweep by 1
	if (time > speed) {
		var++;
		hue += 3;
		time = 0;
	}
}

//// CIRCLES  ////
// This mode shows a circle expanding from the middle
int length;
CHSV circleColors[5];
uint8_t circleHues[5];
void circle_midOut(int mode) {   
	static int radIndex;

	length = 1.1 * sqrt(pow(ledx[cor11], 2) + pow(ledy[cor11], 2)); // I know this is bad

	if (beatDetected[lowBeatBin] == 2) {
		radIndex++;
		if (radIndex == 5) radIndex = 0;
		radius[radIndex] = sqrt(pow(ledx[cor1], 2) + pow(ledy[cor2], 2)); // Works on both size mirrors
		time = 0;
		hue = hue + random8(50, 150);
		circleHues[radIndex] = hue;
		pingPong = !pingPong;
		speed -= .50*speed;
	}

	//if (time > pow(radius - sqrt(pow(ledx[cor1], 1.3) + pow(ledy[cor2], 2)), 1)) { // 1.3 for small, 
	if (time > speed) {
		for (int i = 0; i < 5; i++) { // 5 is the size of the radius[] array
			if (radius[i] < length)
				radius[i] = radius[i] + .5;
		}
		time = 0;
	}

	switch (mode) {
	case 0: // Switching between random color and black
		if (pingPong) circleColors[radIndex] = CHSV(circleHues[radIndex], random8(200, 255), brightness); // random color
		else circleColors[radIndex] = CHSV(hue, 0, 0); // black
		break;
	case 1: // Random color every time
		circleColors[radIndex] = CHSV(circleHues[radIndex], random8(130, 255), 255); // random color, fading
		fadeAll3(map(speed, 0, 60, 247, 253));
		break;
	case 2:
		for (int i = 0; i < 5; i++) 
			circleColors[i] = CHSV(circleHues[i], 255, 255 - 175 * (radius[i] / length)); // random color, no fdae
		break;
	}

	for (int i = 0; i < 5; i++) {
		//Serial.print(radius[i]);
		//Serial.print("\t");
		if (radius[i] < length + 2)
			propagateCircle(circleColors[i], 0, 0, radius[i]);
	}

	speed = constrain(speed + .12, 10, 50);  // old is .1 and adding .15
}

void circle_midOut_alternatingBlack() {
	circle_midOut(0);
}

void circle_midOut_fading() {
	circle_midOut(1);
}

void circle_midOut_NoFade() {
	circle_midOut(2);
}

uint8_t tempHue, tempSat;
double radiusDubs;
void circleDouble() {   // This mode shows 2 circles expanding from the middle of the smallest squares

	if (beatDetected[lowBeatBin] == 2) {
		brightness = 254;
		tempHue = random8(255);
		tempSat = random8(150, 255);
		radiusDubs = 0;
		speed -= .50*speed;
		pingPong = !pingPong;
	}

	color = CHSV(tempHue, tempSat, brightness);

	// Circle on right 
	if (pingPong) {
		propagateCircle(color, ((ledx[cor4] + ledx[cor1]) / 2), ((ledy[cor4] + ledy[cor2]) / 2), radiusDubs);
		//  for loop keeps lights on the opposite small square from lighting
		for (int i = 0; i < 3; i++) {
			leds3[cor22 - i] = 0;
			leds3[cor22 + i] = 0;
		}
	}
	// Circle on left
	else {
		propagateCircle(color, ((ledx[cor21] + ledx[cor19]) / 2), ((ledy[cor22] + ledy[cor20]) / 2), radiusDubs);
		// This for loop keeps lights on the opposite small square from lighting
		for (int i = 0; i < 3; i++) {
			leds3[cor2 - i] = 0;
			leds3[cor2 + i] = 0;
		}
	}

	if (time > speed && radiusDubs < 16) {
		radiusDubs = radiusDubs + .5;
		time = 0;
	}

	speed = constrain(speed + .12, 10, 50);  // old is .1 and adding .15
	fadeAll3(248);
	if (brightness > 1)
		brightness = brightness - 1;
}

void lowBeatNoFadeAndSparkles() {
	EVERY_N_MILLIS(20) {
		fadeRange3(254, cor0, cor3);
		fadeRange3(254, cor13, cor15);
		fadeRange3(254, cor27, cor28);
		fadeRange3(254, cor21, cor23);
	}

	if (beatDetected[lowBeatBin] == 2) {
		if (var < 180) {
			lightSide(cor0, cor3, 0, color);
			lightSide(cor13, cor15 - 2, 0, color);
			lightSide(cor27, cor28, 0, color);
			lightSide(cor21, cor23, 0, color);
			color = CHSV(color.h + random8(50, 200), 255, 255);
		}

		var = 255;
	}
	var--;

	//currentPalette = justWhite_p;

	addSparkles3(cor3, cor5, 12, .15, -50, 0, true);
	addSparkles3(cor9, cor13, 12, .15, -50, 0, true);
	addSparkles3(cor5, cor9, 8, .15, -50, 64, true);
	addSparkles3(cor23, cor27, 8, .15, -50, 64, true);
	addSparkles3(cor15, cor21, 12, .15, -50, 0, true);

	fadeRange3(254 - fadeValue, cor3, cor13);
	fadeRange3(254 - fadeValue, cor15, cor21);
	fadeRange3(254 - fadeValue, cor23, cor27);
	fadeValue = constrain(fadeValue - 1.5, 0, 15);
}

bool fadeSpeed;
void lowBeatWithFadeAndSparkles() {
	if (beatDetected[lowBeatBin] == 2) {
		fadeSpeed -= .85*fadeSpeed; // In this case, lowSpeed is the speed of the fade
		if (time > 200) {
			lightSide(cor0, cor1, 1, color);
			lightSide(cor28, cor27, 1, color);
			lightSide(cor14, cor15 - 2, 1, color);
			lightSide(cor14, cor13 - 1, 1, color);
			lightSide(cor2, cor3 - 2, 1, color);
			lightSide(cor2, cor1 - 1, 1, color);
			lightSide(cor22, cor21 - 1, 1, color);
			lightSide(cor22, cor23 - 1, 1, color);
			color = CHSV(color.h + random8(50, 200), 255, 255);
		}
		time = 0;
	}

	fadeRange3(254 - fadeSpeed, cor0, cor3);
	fadeRange3(254 - fadeSpeed, cor13, cor15);
	fadeRange3(254 - fadeSpeed, cor27, cor28);
	fadeRange3(254 - fadeSpeed, cor21, cor23);

	addSparkles3(cor3, cor5, 12, .07, -50, 0, true);
	addSparkles3(cor9, cor13, 12, .07, -50, 0, true);
	addSparkles3(cor5, cor9, 8, .07, -50, 64, true);
	addSparkles3(cor23, cor27, 8, .07, -50, 64, true);
	addSparkles3(cor15, cor21, 12, .07, -50, 0, true);

	fadeRange3(254 - fadeValue, cor3, cor13);
	fadeRange3(254 - fadeValue, cor15, cor21);
	fadeRange3(254 - fadeValue, cor23, cor27);
	fadeValue = constrain(fadeValue - 1.5, 0, 15);

	fadeSpeed = constrain(fadeSpeed + .2 + .01*fadeSpeed, 240, 254);
}

int zone = 1;
void bouncingDot(int fade, double friction) {
	if (beatDetected[lowBeatBin] == 2) {
		speed -= .85*speed; // old is .85
	}

	if (time > speed) { // If enough time has passed (time1), then move the light
		movingLightPosition++;
		topSmallSqVar++;
		botSmallSqVar++;
		hue = hue + 4;
		time = 0;
	}
	switch (zone) {
	case 1: // Move left on bottom and up on right
			// dot on side without beginning and end of strip
		leds3[cor13 + movingLightPosition] = CHSV(hue, 255, 255);

		// dot on side with beginning and end of strip
		if (movingLightPosition < (cor1 - cor0 + 1))
			leds3[cor1 - movingLightPosition] = CHSV(hue, 255, 255);  
		else
			leds3[cor28 - movingLightPosition + (cor1 - cor0 + 1)] = CHSV(hue, 255, 255);

		if (cor28 - movingLightPosition + (cor1 - cor0 + 1) == cor27) {
			//if (cor13 + movingLightPosition == cor15 - 5) {
			topSmallSqVar = 0;
			movingLightPosition = 0;
			topSquareColor = hue;
			zone = 2;
		}
		break;
	case 2: // Move right on top and down on left
			// dot on side without beginning and end of strip
		leds3[cor15 - movingLightPosition - 4] = CHSV(hue, 255, 255);

		// // dot on side with beginning and end of strip
		if (cor27 + movingLightPosition < NUM_LEDS) leds3[cor27 + movingLightPosition] = CHSV(hue, 255, 255);
		else leds3[cor0 + movingLightPosition - (cor28 - cor27)] = CHSV(hue, 255, 255);
		//(leds[cor0 + movingLightPosition - 10] = CHSV(hue, 255, 255);

		if (cor15 - movingLightPosition - 4 == cor13) {
			botSmallSqVar = 0;
			movingLightPosition = 0;
			botSquareColor = hue;
			zone = 1;
		}
		break;
	}

	bouncingDotSmallSquares(botSquareColor, topSquareColor);

	speed = constrain(speed + friction + .01*speed, 15, 200);  // old is .01
															
	fadeInsideBigSquare(250);
	fadeRange3(fade, cor1, cor13); // Bottom right square fade
	fadeRange3(fade, cor15, cor27); // Top left sqaure fade
}

void bouncingDotWithFade() {
	bouncingDot(254, .5);
	fadeRange3(254, cor1, cor5);
	fadeRange3(254, cor19, cor23);
}

void bouncingDotWithoutFade() {
	bouncingDot(255, .2);

	fadeRange3(254, cor3, cor7);
	fadeRange3(254, cor19, cor21);
	fadeRange3(254, cor25, cor27);
}
