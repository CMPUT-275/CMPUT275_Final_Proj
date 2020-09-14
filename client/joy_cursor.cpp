/*

*/

#define SD_CS 10
#define JOY_VERT  A9 // should connect A9 to pin VRx
#define JOY_HORIZ A8 // should connect A8 to pin VRy
#define JOY_SEL   53

#include <Arduino.h>

// core graphics library (written by Adafruit)
#include <Adafruit_GFX.h>

// Hardware-specific graphics library for MCU Friend 3.5" TFT LCD shield
#include <MCUFRIEND_kbv.h>

// needed for reading/writing to SD card
#include <SD.h>
#include "shared.h"




#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 320
#define JOY_CENTER   512
#define JOY_DEADZONE 64

#define CURSOR_SIZE 9
extern Shared sharedVals;
// the cursor position on the display

// I made the yegMiddleX and yegMiddleY variables gloal as they are now being used in two different
// functions in the program. I felt this would clean up both functions and since these variables 
// remain constant, I figured it would be okay to change their scope.

// forward declaration for redrawing the cursor

 
// redrawCursor has been altered so that if the colour black is inputted into it, instead
// of replacing the previous cursor position with black, it will replace it with the small
// part of the map that belongs at that position, preventing a obstructive trail.
void redrawCursor(uint16_t fill, MCUFRIEND_kbv &tft) {
	int startX = sharedVals.currentEdge.startCoordX;
	int endX = sharedVals.currentEdge.endCoordX;
	int startY = sharedVals.currentEdge.startCoordY;
	int endY = sharedVals.currentEdge.endCoordY;
  // The cursor values are called and then constrained so that the cursor cannot be drawn off the edge.
 	//sharedVals.cursorX = constrain(sharedVals.cursorX, startX+4, endX-5);
  	//sharedVals.cursorY = constrain(sharedVals.cursorY, startY+4, endY-5);
	/*sharedVals.cursorX = constrain(sharedVals.cursorX, 80, 400);
  	sharedVals.cursorY = constrain(sharedVals.cursorY, 4, 315);
  */
  if(fill == 0) {
    tft.fillRect(sharedVals.cursorX - CURSOR_SIZE/2, sharedVals.cursorY - CURSOR_SIZE/2,
               CURSOR_SIZE, CURSOR_SIZE, TFT_RED);
  } else if(fill == 1) {
    // a small part of the map the size of the cursor is drawn in the position of the cursor. 
    tft.fillRect(sharedVals.cursorX - CURSOR_SIZE/2, sharedVals.cursorY - CURSOR_SIZE/2,
               CURSOR_SIZE, CURSOR_SIZE, TFT_WHITE);
  }
}

// scaledVal function takes an analog input and normalizes that value from its current scale
// ([(JOY_CENTER+JOY_DEADZONE) to 1023] or [(JOY_CENTER-JOY_DEADZONE) to 0]) to a 1-10 scale.
int scaledVal(int val) {
  int newval;
  if(val > 576) {
    newval = (((float)(val-577)/446)*5)+1;

    return newval;
  } else if(val < 448) {
    newval = (((float)(val - 447)/(-447))*5)+1;
    return newval;
  }

}
bool mazeConstrain() {

	int cursX, cursY;
	Edge checkedEdge;
	int numEdge = sharedVals.numEdges;

	for(int i = 0; i < numEdge; i++) {
		checkedEdge = sharedVals.edges[i];
		cursX = sharedVals.cursorX;
		cursY = sharedVals.cursorY;

		if((cursX >= checkedEdge.startCoordX+6)&&(cursX <= checkedEdge.endCoordX-5)&&(cursY >= checkedEdge.startCoordY+6)&&(cursY <= checkedEdge.endCoordY-5)) {
			sharedVals.currentEdge = checkedEdge;//Serial.println("X val valid");
			return false;
		}
	}
	//Serial.println("Boundary breached!");
	return true;
}

void processJoystick(MCUFRIEND_kbv &tft) {
  int xVal = analogRead(JOY_HORIZ);
  int yVal = analogRead(JOY_VERT);
  int buttonVal = digitalRead(JOY_SEL);
  if (yVal < JOY_CENTER - JOY_DEADZONE) {
    redrawCursor(1, tft);
    sharedVals.vely = scaledVal(yVal);
    sharedVals.cursorY -= sharedVals.vely;// decrease the y coordinate of the cursor
    if(mazeConstrain()){
    	sharedVals.cursorY += sharedVals.vely;
    }
  }
  else if (yVal > JOY_CENTER + JOY_DEADZONE) {
    redrawCursor(1, tft);
    sharedVals.vely = scaledVal(yVal);
    sharedVals.cursorY += sharedVals.vely;
    if(mazeConstrain()){
    	sharedVals.cursorY -= sharedVals.vely;
    }
  }

  if (xVal > JOY_CENTER + JOY_DEADZONE) {
    redrawCursor(1,tft);
    sharedVals.velx = scaledVal(xVal);
    sharedVals.cursorX -= sharedVals.velx;
    if(mazeConstrain()){
    	sharedVals.cursorX += sharedVals.velx;
    }
  } else if (xVal < JOY_CENTER - JOY_DEADZONE) {
    sharedVals.velx = scaledVal(xVal);
    redrawCursor(1,tft);
    sharedVals.cursorX += sharedVals.velx;
    if(mazeConstrain()){
    	sharedVals.cursorX -= sharedVals.velx;
    }
  }
  redrawCursor(0,tft);

  delay(20);
}

