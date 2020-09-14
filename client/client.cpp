#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include "drawMaze.h"
#include "edge.h"
#include "mazeSelect.h"
#include "shared.h"
#include "joy_cursor.h"
#include "communication.h"
#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 320 

using namespace std;
Shared sharedVals;
MCUFRIEND_kbv tft;

void setup() {
  // initialize Arduino (Borrowed from Assignment2's client.cpp)
  	init();

	uint16_t ID = tft.readID();
	tft.begin(ID);
	tft.setRotation(1);
  	tft.fillScreen(TFT_BLACK);

	Serial.begin(9600);
	Serial.flush(); // get rid of any leftover bits
}


void placeEnds() {
	for(int i = 0; i < sharedVals.numEdges; i++) {
		if((sharedVals.edges[i].edgeStart == sharedVals.startVert) || (sharedVals.edges[i].edgeEnd == sharedVals.startVert)) {
			sharedVals.edges[i].isStart = true;
		} else if((sharedVals.edges[i].edgeEnd == sharedVals.endVert) || (sharedVals.edges[i].edgeStart == sharedVals.endVert)) {
			sharedVals.edges[i].isEnd = true;
		}
	}
}

int main() {

	setup();

	while(true){
	mazeSelect(tft);
	before_maze(tft);
	placeEnds();
	Serial.println(sharedVals.mazeDim);
	int size = sharedVals.mazeDim;
	drawMap(tft);				//Draws a maze using a 5x5 graph

	tft.setTextColor(TFT_WHITE);
	tft.setCursor(0, 0);
	tft.setTextSize(2);


	int startX;
	int endX;
	int startY;
	int endY;
	int i = 0;
	sharedVals.passedEdges[0] = sharedVals.currentEdge;
	uint32_t startTime = millis(); 						//Start timer
	while(!sharedVals.currentEdge.isEnd) { 
		startX = sharedVals.currentEdge.startCoordX;	//Assigns starting x coord of current edge to startX
		endX = sharedVals.currentEdge.endCoordX;		//Assigns ending x coord of current edge to endX
		startY = sharedVals.currentEdge.startCoordY;	//Assigns starting y coord of current edge to startY
		endY = sharedVals.currentEdge.endCoordY;		//Assigns ending y coord of current ede to endY
  		Serial.println(sharedVals.currentEdge.edgeStart);
		processJoystick(tft);
		if(!havePassed()) {
			sharedVals.passedEdges[sharedVals.numPassed] = sharedVals.currentEdge;
			sharedVals.numPassed++;
		}

	}

	uint32_t endTime = millis(); // End timer
	sharedVals.userTime = (double)abs(endTime - startTime)/1000; //Calculate total time in seconds
	//solnPath(tft); // Print fastest path (not properly tested)
	delay(500);
	after_maze(); // End screen showing time and score
	endScreen(tft);
	for(int i = 0; i < sharedVals.numPassed; i++) {
		Serial.print(sharedVals.passedEdges[i].edgeStart);
		Serial.print(" ");
		Serial.println(sharedVals.passedEdges[i].edgeEnd);
	}
	for(int i = 0; i < sharedVals.numPassed; i++) {
		sharedVals.passedEdges[i].edgeStart = 0;
		sharedVals.passedEdges[i].edgeEnd = 0;

	}
		sharedVals.numPassed = 0;
	}	
	return 0;
}
