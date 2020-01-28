//#pragma once

uint8_t musicWithNoBeatPatternNumber = 0;
uint8_t currentLowBeatPatternNumber = 0;
uint8_t currentSpectrumPatternNumber = 0;
uint8_t currentPatternNumber = 0;
uint8_t currentAmbPatternNumber = 0;
uint8_t musicWithConstBeatPatternNumber = 0;
uint8_t allPatternsNumber = 0;

// These are lists of functions that will be displayed during different types of music / no music.
// No music is playing, so display an ambient function
typedef void(*ambientPatternList[])();
ambientPatternList ambientPatterns = {
	ambientRainbow, 
	ambientSweeps, 
	ambientBouncingBall 
};

typedef void(*musicWithNoBeatPatternsList[])();
musicWithNoBeatPatternsList musicWithNoBeatPatterns = {
	sixFreqGlitter  // leds 2
};
// NO LONGER USING SPECTRUMPATTERNLIST
// Both high frequency beats and low frequency beats are being detected
typedef void(*spectrumPatternList[])();
spectrumPatternList spectrumPatterns = { 
	sixFreqGlitter,  // leds 2
	//beatMeter,
	//lowBeatWithFadeAndSparkles,
	//lowBeatNoFadeAndSparkles
};
// Only beats in the low frequency are being detected
typedef void(*lowBeatPatternList[])();
lowBeatPatternList lowBeatPatterns = {
	bouncingDotWithFade, // leds 3
	bouncingDotWithoutFade,  // leds 3
	sweepsToBeat,  // leds 3
	circle_midOut_alternatingBlack,  // leds 3
	circle_midOut_fading,  // leds 3
	circle_midOut_NoFade,  // leds 3
	lowBeatWithFadeAndSparkles,  // leds 3
	lowBeatNoFadeAndSparkles,  // leds 3
};
// Not using this list, just oging with CircleDouble for constant beats
typedef void(*constBeatPatternList[])();
constBeatPatternList constBeatPatterns = {
	circle_midOut_alternatingBlack, // leds 3
	sweepsToBeat,  // leds 3
	circleDouble  // leds 3
};

typedef void(*allPatternsList[])();
allPatternsList allPatterns = {
	sixFreqGlitter,
	sweepsToBeat,
	lowBeatWithFadeAndSparkles,
	lowBeatNoFadeAndSparkles,
	bouncingDotWithFade,
	bouncingDotWithoutFade,
	circle_midOut_alternatingBlack,
	circle_midOut_fading,
	circle_midOut_NoFade,
	circleDouble
};

void nextAllPatterns() {
	allPatternsNumber++;
	if (allPatternsNumber >= ARRAY_SIZE(allPatterns))
		allPatternsNumber = 0;
}

// These functions choose a random pattern within the lists above
void nextMusicWithConstBeatPattern() {
	// add one to the current pattern number, and wrap around at the end
	//musicWithNoBeatPatternNumber = (musicWithNoBeatPatternNumber + 1) % ARRAY_SIZE(musicWithNoBeatPatterns);
	musicWithConstBeatPatternNumber = random8(ARRAY_SIZE(constBeatPatterns));
}

void nextMusicWithNoBeatPattern() {
	// add one to the current pattern number, and wrap around at the end
	//musicWithNoBeatPatternNumber = (musicWithNoBeatPatternNumber + 1) % ARRAY_SIZE(musicWithNoBeatPatterns);
	musicWithNoBeatPatternNumber = random8(ARRAY_SIZE(musicWithNoBeatPatterns));
}

void nextAmbPattern() {
	//currentAmbPatternNumber = (currentAmbPatternNumber + 1) % ARRAY_SIZE(ambientPatterns);
	currentAmbPatternNumber = random8(ARRAY_SIZE(ambientPatterns));
}

void nextLowBeatPattern() {
	//currentLowBeatPatternNumber = (currentLowBeatPatternNumber + 1) % ARRAY_SIZE(lowBeatPatterns);
	currentLowBeatPatternNumber = random8(ARRAY_SIZE(lowBeatPatterns));
}

void nextSpectrumPattern() {
	//currentSpectrumPatternNumber = (currentSpectrumPatternNumber + 1) % ARRAY_SIZE(spectrumPatterns);
	currentSpectrumPatternNumber = random8(ARRAY_SIZE(spectrumPatterns));
}