#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include "shared.h"
#define SD_CS 10
#define JOY_VERT  A9 // should connect A9 to pin VRx
#define JOY_HORIZ A8 // should connect A8 to pin VRy
#define JOY_SEL   53
#define JOY_CENTER   512
#define JOY_DEADZONE 64
extern Shared sharedVals;

void mazeSetup(MCUFRIEND_kbv &tft) {				// Prepare TFT display for display
	pinMode(53, INPUT_PULLUP);						
	tft.fillScreen(TFT_BLACK);				
	tft.setCursor(120, 20);
	tft.setTextColor(TFT_RED);
	tft.setTextSize(3);
	tft.println("275 MAZE GAME");
	tft.drawRect(50, 140, 120, 90, TFT_RED);
	tft.drawRect(180, 140, 120, 90, TFT_RED);
	tft.drawRect(310, 140, 120, 90, TFT_RED);
	tft.setTextColor(TFT_WHITE);
	tft.setCursor(85, 175);
	tft.print("5x5");
	tft.setCursor(215, 175);
	tft.print("6x6");
	tft.setCursor(345, 175);
	tft.print("7x7");

}

void setBoxes(int currentBox, MCUFRIEND_kbv &tft) {			// Set dimension option boxes
	if(currentBox == 0) {
			tft.fillRect(181, 141, 118, 88, TFT_BLACK);
			tft.setTextColor(TFT_WHITE);
			tft.setCursor(215, 175);
			tft.print("6x6");
			tft.fillRect(51, 141, 118, 88, TFT_WHITE);
			tft.setTextColor(TFT_BLACK);
			tft.setCursor(85, 175);
			tft.print("5x5");
		} else if(currentBox == 1) {
			tft.fillRect(51, 141, 118, 88, TFT_BLACK);
			tft.setTextColor(TFT_WHITE);
			tft.setCursor(85, 175);
			tft.print("5x5");
			tft.fillRect(311, 141, 118, 88, TFT_BLACK);
			tft.setTextColor(TFT_WHITE);
			tft.setCursor(345, 175);
			tft.print("7x7");
			tft.fillRect(181, 141, 118, 88, TFT_WHITE);
			tft.setTextColor(TFT_BLACK);
			tft.setCursor(215, 175);
			tft.print("6x6");
		} else if(currentBox == 2) {
			tft.fillRect(311, 141, 118, 88, TFT_WHITE);
			tft.setTextColor(TFT_BLACK);
			tft.setCursor(345, 175);
			tft.print("7x7");
			tft.fillRect(181, 141, 118, 88, TFT_BLACK);
			tft.setTextColor(TFT_WHITE);
			tft.setCursor(215, 175);
			tft.print("6x6");
		}
}

void mazeSelect(MCUFRIEND_kbv &tft) { //Select maze dimension
	mazeSetup(tft);
	int xVal;
  	int yVal;
  	int buttonVal;
  	int currentBox = 0;
	while(true) {
		xVal = analogRead(JOY_HORIZ);
  		yVal = analogRead(JOY_VERT);
  		buttonVal = digitalRead(JOY_SEL);
		if (xVal > JOY_CENTER + JOY_DEADZONE && currentBox != 0) {
			currentBox--;
			currentBox = constrain(currentBox, 0, 2);
			setBoxes(currentBox, tft);
			delay(100);
  		} else if (xVal < JOY_CENTER - JOY_DEADZONE && currentBox != 2) {
			currentBox++;
			currentBox = constrain(currentBox, 0, 2);
			setBoxes(currentBox, tft);
			delay(100);
  		}
  		if(buttonVal == LOW) {
  			if(currentBox == 0) {
  				sharedVals.mazeDim = 5;
  				break;
  			} else if(currentBox == 1) {
  				sharedVals.mazeDim = 6;
  				break;
  			} else if(currentBox == 2) {
  				sharedVals.mazeDim = 7;
  				break;
  			}
  		}

	}
}

void solnPath(MCUFRIEND_kbv &tft) {		// Draw fastst path (not tested properly)
	int startX;
	int startY;
	int endX;
	int endY;
	int width;
	int height;
	int lineY;
	int lineX;
	for(int i = 0; i < sharedVals.solutionSize; i++) {
		startX = sharedVals.solutionPath[i].startCoordX;
		startY = sharedVals.solutionPath[i].startCoordY;
		endX = sharedVals.solutionPath[i].endCoordX;
		endY = sharedVals.solutionPath[i].endCoordY;

		if(abs(sharedVals.edges[i].edgeStart - sharedVals.edges[i].edgeEnd) == 1) {
			lineY = startY + (abs(endY - startY)/2);
			tft.drawLine(startX+(abs(endY - startY)/2), lineY, endX-(abs(endY - startY)/2), lineY, TFT_RED);
		} else if(abs(sharedVals.edges[i].edgeStart - sharedVals.edges[i].edgeEnd) == sharedVals.mazeDim){
			lineX = startX + (abs(endX - startX)/2);
			tft.drawLine(lineX, startY+(abs(endX - startX)/2), lineX, endY-(abs(endX - startX)/2), TFT_RED);
		}
		

	}
}

void endScreen(MCUFRIEND_kbv &tft) {			//Shows the score and time
	double scaledTime = ((double)sharedVals.userTime/1000);

	tft.fillScreen(TFT_BLACK);
	tft.setTextColor(TFT_WHITE);
	tft.setTextSize(3);
	tft.setCursor(0, 150);
	tft.print("You solved in ");
	tft.print(sharedVals.userTime);
	tft.println(" sec!");
	tft.print("Your score is ");
	tft.print(sharedVals.score);
	delay(5000);
}