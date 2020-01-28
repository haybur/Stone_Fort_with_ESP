#pragma once

#include <Audio.h>
// GUItool: begin automatically generated code
AudioInputI2S i2s1; //xy=219.1999969482422,183
AudioMixer4 mixer1; //xy=403.1999969482422,203.1999969482422
AudioAnalyzeFFT1024 fft1024; //xy=547.1999969482422,239
AudioConnection patchCord1(i2s1, 0, mixer1, 0);
AudioConnection patchCord2(i2s1, 1, mixer1, 1);
AudioConnection patchCord3(mixer1, fft1024);
AudioControlSGTL5000 audioShield; //xy=446.1999969482422,317
// GUItool: end automatically generated code
const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

// AUDIO DETECTION VARIABLES
const int numFFTBins = 14;
int spectrumValue[numFFTBins];
int spectrumValueOld[numFFTBins];
double average[14];
double stDev[14];
long variance[14];
int volume, volumeAverage;
int fadeVals[14];
Statistic freq[14];
Statistic vol;
uint8_t beatDetected[14] = {};
elapsedMillis beatTimer[14] = {};
int beatInterval[14];
int beatIntervalOld[14];
int constantBeatCounter[14];
int beatPeak[14];
int beatPeakOld[14];
int binScore[14];
int lowBeatBin, midBeatBin, highBeatBin;
int lowAveBin, midAveBin, highAveBin;
elapsedMillis silenceDuration;
elapsedMillis songDuration;
bool silence = false;
// Music Automation Variables
bool lowBeats = false;
bool highBeats = false;
bool midBeats = false;
bool constBeat = false;
int maxConstBeat = 0;
uint8_t constBeatBin;
uint8_t lowBeatCounter, midBeatCounter, highBeatCounter;
int8_t musicState, musicStateOld;
int mixAmount;


void audioSetup() {
	// THIS CODE IS FOR A SWITCH TO CHOOSE BETWEEN MIC AND AUX INPUTS
	//if (digitalRead(switchPin) == 1) {
	//Serial.print("Brightness Pot is at ");
	//Serial.println(analogRead(brightnessPot));
	//if (analogRead(brightnessPot) > 500) {
	//	myInput = AUDIO_INPUT_LINEIN;
	//	Serial.println("Input is Aux");
	//}
	//else {
	//	myInput = AUDIO_INPUT_MIC;
	//	Serial.println("Input is Mic");
	//}

	if (myInput == AUDIO_INPUT_MIC)
		Serial.println("input is mic");

	// AUDIO STUFF
	AudioMemory(12);
	audioShield.enable();
	audioShield.inputSelect(myInput);
	//audioShield.lineInLevel(10); // This should normalize the volume levels of different audio inputs, but it also makes the quieter parts of songs louder. No Bueno.

	// configure the mixer to equally add left & right
	mixer1.gain(0, 0.5);
	mixer1.gain(1, 0.5);
}

// Clears the running mean and st dev. Used mostly between songs when silence is detected.
void clearStats() {
	for (int i = 0; i < 14; i++) {
		freq[i].clear();
		beatPeak[i] = 0;
		binScore[i] = 0;
		lowBeatBin = 2;
		midBeatBin = 8;
		highBeatBin = 12;
	}
}

void detectSilence() {
	uint8_t silenceCounter = 0;
	// If over 13 of the 15 frequencies are silent, 
	// reset the statistics because it's likely the song ended
	for (int i = 0; i < 14; i++) {
		if (spectrumValue[i] < 10) { // 10 is an arbitrary number but it means it is basically silent
			silenceCounter++;
		}
		else {
			break;
		}
	}
	// If at least 12 bins are under a value of 6, or if the song is 5 minutes long, reset stats.
	if (silenceCounter > 11 || songDuration > 300000) {
		//Serial.println("stats have been cleared");
		songDuration = 0;
		clearStats();
	}
	else
		silenceDuration = 0;

	if (silenceDuration > 5000) {
		silence = true;
		lowBeats = false;
		midBeats = false;
		highBeats = false;
		songDuration = 0;
	}
	else
		silence = false;
}

void fillStats() {
	for (int i = 0; i < 14; i++)
		spectrumValueOld[i] = spectrumValue[i];

	//Serial.println(mult);
	spectrumValue[0] = mult * fft1024.read(0) * 1000;
	spectrumValue[1] = mult * fft1024.read(1) * 1000;
	spectrumValue[2] = mult * fft1024.read(2, 3) * 1000;
	spectrumValue[3] = mult * fft1024.read(4, 6) * 1000;
	spectrumValue[4] = mult * fft1024.read(7, 10) * 1000;
	spectrumValue[5] = mult * fft1024.read(11, 15) * 1000;
	spectrumValue[6] = mult * fft1024.read(16, 22) * 1000;
	spectrumValue[7] = mult * fft1024.read(23, 32) * 1000;
	spectrumValue[8] = mult * fft1024.read(33, 46) * 1000;
	spectrumValue[9] = mult * fft1024.read(47, 66) * 1000;
	spectrumValue[10] = mult * fft1024.read(67, 93) * 1000;
	spectrumValue[11] = mult * fft1024.read(94, 131) * 1000;
	spectrumValue[12] = mult * fft1024.read(132, 184) * 1000;
	spectrumValue[13] = mult * fft1024.read(185, 257) * 1000;

	volume = 0;
	// Adjust the standard deviation and average of each array value as the song goes
	for (int i = 0; i < 14; i++) {
		freq[i].add(spectrumValue[i]);
		average[i] = freq[i].average();
		stDev[i] = freq[i].pop_stdev();
		//variance[i] = pow(stDev[i], 2);
		volume = volume + spectrumValue[i];
	}

	detectSilence();
}

void beatTiming(int i) {
	if (beatDetected[i] == 2) { // if beat has been detected
		if (beatTimer[i] < 200) // and it isn't 200 ms from the last one
			beatDetected[i] = 1; // but if it is, cancel the beat
		else {
			beatInterval[i] = beatTimer[i];  // a beat has been detected. Get the time since last beat.
			// could use absolute value below
			if ((beatIntervalOld[i] - beatInterval[i]) < 30 && (beatIntervalOld[i] - beatInterval[i]) > -30) // If the time between the new interval and old interval is less than 30 ms 
				constantBeatCounter[i]++;

			beatTimer[i] = 0;
			beatIntervalOld[i] = beatInterval[i];
		}
	}
	if (constantBeatCounter[i] > 0 && beatTimer[i] > beatInterval[i] + 50) constantBeatCounter[i] = 0; // clears beat counter when more time than the beat has passed

	// These print statements will print the constant beat counter of each bin
	//if (i == numFFTBins - 1) Serial.println(constantBeatCounter[i]);
	//else {
	//	Serial.print(constantBeatCounter[i]);
	//	Serial.print("\t");
	//}

	if ( i < 4) {
		if (constantBeatCounter[i] > maxConstBeat) {
			maxConstBeat = constantBeatCounter[i];
			constBeatBin = i;
		}
	}
	if (maxConstBeat > 10) 
		constBeat = true;
	else 
		constBeat = false;
}

void beatDetection(int i) {
	// 0 = no beat detected
	// 1 = beat hasn't dropped / reset yet
	// 2 = beat detected
	// Beat is detected here. Must be greater than the average+(2.3*st.dev) and greater than 100 which is pretty quiet
		//if (spectrumValue[i] > average[i] + 2.3 * stDev[i] && spectrumValue[i] > 100) {
	if (spectrumValue[i] > average[i] + 2.3 * stDev[i] && spectrumValue[i] > 100) {
		if (beatDetected[i] == 2)
			beatDetected[i] = 1;
		if (beatDetected[i] == 0)
			beatDetected[i] = 2;
		// if it's == 1, it stays 1.
	}
	// This is where 1's get reset to 0. If beat is not detected in that frequency, set it's status to 0.
	else {
		if (beatDetected[i] == 1) {
			// else, don't be ready for the next beat until the frequency has hit the running average.
			// This prevents multiple beats being triggered when they aren't well defined
			if (spectrumValue[i] <= average[i])
				beatDetected[i] = 0;
		}
	}
	// Fill Fade Values
	if (spectrumValue[i] > average[i])
		fadeVals[i] += (spectrumValue[i] - average[i]) / 2;
	else
		fadeVals[i] -= (average[i] - spectrumValue[i]) / 2;

	fadeVals[i] = constrain(fadeVals[i], 0, 255);
}

void printDetectedBeats(int i) {
	// These print statements will print the beat status of each bin
	if (i == numFFTBins - 1) Serial.println(beatDetected[i]);
	else {
		Serial.print(beatDetected[i]);
		Serial.print("\t");
	}
}

void scoreBins(int i) {
	// If there's a constant beat, increase score by 2
	if (constantBeatCounter[i] > 5)
		binScore[i] += 2;

	// If there's a beat detected, increase score by 1
	if (beatDetected[i] == 2)
		binScore[i] += 1;

	// If the highest scoring bin has it's constant beat stop, lower that score
	if (binScore[i] > 300 && constantBeatCounter[i] < 2)
		binScore[i] -= 2;

	// These print statements will print the score of each bin
	//if (i == numFFTBins - 1)
	//	Serial.println(binScores[i]);
	//else {
	//	Serial.print(binScores[i]);
	//	Serial.print("\t");
	//}

}

void musicAnalytics(bool autoSwitch) {
	maxConstBeat = 0;
	// All the deep analytics happen from the functions in this for loop
	for (int i = 0; i < 14; i++) {
		beatDetection(i);
		beatTiming(i);
		//printDetectedBeats(i);
		scoreBins(i);
	}

	// This for loop checks the lowest 4 bins, mid 4 bins, high 4 bins
	for (int i = 0; i < 4; i++) {
		// Find the beats in the low, mid, and high ranges
		if (binScore[i] > binScore[lowBeatBin])
			lowBeatBin = i;
		if (binScore[i + 5] > binScore[midBeatBin]) // binScore[4] is not measured
			midBeatBin = i + 5;
		if (binScore[i + 9] > binScore[highBeatBin])
			highBeatBin = i + 9;

		// Find highest averages in the low, mid, and high ranges
		if (average[i] > average[lowAveBin])
			lowAveBin = i;
		if (average[i + 5] > average[midAveBin])
			midAveBin = i + 5;
		if (average[i + 9] > average[highAveBin])
			highAveBin = i + 9;
	}

	// Prints all the beat bins
	//Serial.print(lowBeatBin);
	//Serial.print("\t");
	//Serial.print(binScore[lowBeatBin]);
	//Serial.print("\t");
	//Serial.print(midBeatBin);
	//Serial.print("\t");
	//Serial.print(binScore[midBeatBin]);
	//Serial.print("\t");
	//Serial.print(highBeatBin);
	//Serial.print("\t");
	//Serial.println(binScore[highBeatBin]);

	static int mixAmountInfluencer;

	//if (beatDetected[2] == 2 || beatDetected[12] == 2)
	if (beatDetected[lowBeatBin] == 2 || constantBeatCounter[lowBeatBin] > 8)
		mixAmountInfluencer += 5;

	EVERY_N_MILLIS(150)
		mixAmountInfluencer -= 1;

	mixAmountInfluencer = constrain(mixAmountInfluencer, -10, 10);
	mixAmount = constrain(mixAmount + mixAmountInfluencer, 0, 255);
	//Serial.print(mixAmountInfluencer);
	//Serial.print("\t");
	//Serial.println(mixAmount);
}

void printNumber(float n) {
  if (n >= 0.004) {
    Serial.print(n, 3);
    Serial.print(" ");
  }
  else {
    //  Serial.print("   -  "); // don't print "0.00"
    Serial.print("   0  "); // print 0 for excel purposes
  }
}

void printSpectrum() {
  ////// UNCOMMENT THEse LINES TO PRINT THE FFT WHILE PLAYING A SONG
	if (fft1024.available()) {
		// each time new FFT data is available
		// print to the Arduino Serial Monitor
		//Serial.print("FFT: ");
		printNumber(fft1024.read(0) * 1000);
		printNumber(fft1024.read(1) * 1000);
		printNumber(fft1024.read(2, 3) * 1000);
		printNumber(fft1024.read(4, 6) * 1000);
		printNumber(fft1024.read(7, 10) * 1000);
		printNumber(fft1024.read(11, 15) * 1000);
		printNumber(fft1024.read(16, 22) * 1000);
		printNumber(fft1024.read(23, 32) * 1000);
		printNumber(fft1024.read(33, 46) * 1000);
		printNumber(fft1024.read(47, 66) * 1000);
		printNumber(fft1024.read(67, 93) * 1000);
		printNumber(fft1024.read(94, 131) * 1000);
		printNumber(fft1024.read(132, 184) * 1000);
		printNumber(fft1024.read(185, 257) * 1000);
		printNumber(fft1024.read(258, 359) * 1000);
		printNumber(fft1024.read(360, 511) * 1000);
		Serial.println();
	}
	//else
		//Serial.println("No FFT data available");
}