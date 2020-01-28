//// Stone Fort ESP Communication ////

// Variables for solid color
uint8_t solidHue = 0;
uint8_t solidSat = 255;
uint8_t solidVal = 255;

const byte numChars = 10;
char receivedChars[10];
char tempChars[3];

int mode;

// variables to hold parsed data
char category[1] = { 0 };
int instruction = 0;

bool newCommand = false;

void doThingsWithCommands() {
	if (receivedChars[0] == 'o') {
		Serial.println(SFmodeOld);
		SFmode = SFmodeOld;
	}
	else if (receivedChars[0] == 's') {
		SFmode = 3;
		for (int i = 0; i < 3; i++)
			tempChars[i] = receivedChars[i + 1];
		solidHue = atoi(tempChars);
	}
	else if (receivedChars[0] == 't') {
		SFmode = 3;
		for (int i = 0; i < 3; i++)
			tempChars[i] = receivedChars[i + 1];
		solidSat = atoi(tempChars);
	}
	else if (receivedChars[0] == 'b') {
		SFmode = 3;
		for (int i = 0; i < 3; i++)
			tempChars[i] = receivedChars[i + 1];
		solidVal = atoi(tempChars);
	}
	else if (receivedChars[0] == 'f') {
		SFmodeOld = SFmode;
		SFmode = 0;
	}
	else if (receivedChars[0] == 'x') {
		SFmode = 1;
	}
	else if (receivedChars[0] == 'z') {
		for (int i = 0; i < 3; i++)
			tempChars[i] = receivedChars[i + 1];
		masterBrightness = atoi(tempChars);
	}
	else if (receivedChars[0] == 'y') {
		for (int i = 0; i < 3; i++)
			tempChars[i] = receivedChars[i + 1];
		audioMultiplier = atoi(tempChars);
		mult = (double)audioMultiplier / 100;
	}
	else if (receivedChars[0] == 'm') {
		for (int i = 0; i < 3; i++)
			tempChars[i] = receivedChars[i + 1];
		MVPattern = atoi(tempChars);
		SFmode = 4;
		Serial.println(MVPattern);
	}
	else if (receivedChars[0] == 'a') {
		for (int i = 0; i < 3; i++)
			tempChars[i] = receivedChars[i + 1];
		ambPattern = atoi(tempChars);
		SFmode = 2;
		Serial.println(ambPattern);
	}
	else {}
}

void recWithStartEndMarkers() {
	static boolean recInProgress = false;
	static byte ndx = 0;
	char startMarker = '<';
	char endMarker = '>';
	char rc;

	while (Serial1.available() && newCommand == false) {
		rc = Serial1.read();
		//Serial.println("data being read");
		if (recInProgress == true) {
			if (rc != endMarker) {
				receivedChars[ndx] = rc;
				ndx++;
				//if (ndx >= numChars) // THESE 2 LINES FUCK UP THE LEDS
				//	ndx = numChars - 1;
			}
			else {
				receivedChars[ndx] = '\0';
				recInProgress = false;
				ndx = 0;
				newCommand = true;
			}
		}
		else if (rc == startMarker)
			recInProgress = true;
		else {}
	}
}

void parseData() {
	if (newCommand == true) {
		for (int i = 0; i < sizeof(receivedChars); i++) {
			//    for (int i = 0; i < 5; i++) {
			Serial.print(receivedChars[i]);
			//Serial.print("  ");
		}
		Serial.println();
		//    Serial.print(receivedChars[0]);
		//    Serial.print("  ");
		//    Serial.print(receivedChars[1]);
		//    Serial.print("  ");  
		//    Serial.print(receivedChars[2]);
		//    Serial.print("  ");
		//    Serial.print(receivedChars[3]);
		//    Serial.print("  ");  
		//    Serial.print(receivedChars[4]);
		//    Serial.println("  ");
		   // Serial.println(receivedChars);

		doThingsWithCommands();

		newCommand = false;
	}
}

void talkToESP() {
	recWithStartEndMarkers();
	parseData();
}