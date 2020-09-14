#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <stdlib.h>
#include "shared.h"
#include "edge.h"
#define DISPLAY_WIDTH  320 // Due to the maze being a square
#define DISPLAY_HEIGHT 320 
using namespace std;
extern Shared sharedVals;

/*void randTest(int size, int verts[]) {
	 randTest: Creates a random array of values to test vertex spacing of array on TFT display. 
	NO LONGER USED  

//Point points;
 	int mapWidth = size;
 	int mapHeight = size;
 	int key;
 	int randW;
 	int randH;
 	srand(millis());
	for(int i = 0; i < 100; i++) {
		randW = abs(rand() % mapWidth);

		randH = abs(rand() % mapHeight);

		key = abs(((mapWidth * randH)) + ((randW%5) - 1)+1);
 		//if (verts.find(key) == verts.end()) {
			verts[i] = key;
			
 		//}


 	}
 }
*/


void drawMap(MCUFRIEND_kbv &tft) {
	/*drawMap: Recieves a series of edges from an array of "Edge" structs and prints a maze on the screen using them.

		Arguments: 	size -> Integer representing dimensions of maze (must be size x size)
					*edges -> Pointer pointing to an array containing a series of edges
					&tft -> A passed by reference instance of the TFT display

	*/
	int size = sharedVals.mazeDim;
	int mapWidth = sharedVals.mazeDim;
	int mapHeight = sharedVals.mazeDim; 			//Currently redundant. Only kept in incase mazes are not always square.
									// Easier to keep track of width and height values.

	int newWidth = 2*mapWidth+1; 	//New map dimensions
	int newHeight = 2*mapHeight+1;
	int vertX;						//Value of X for each vertex
	int vertY;						//Value of Y for each vertex
	int startVert;					//Where the edges start
	int endVert;					//Where the edges end
	int mapDivx;					//Width of each maze section
	int mapDivy;					//Height of each maze section
	int blockPlacex;				//Location of each maze section on the screen (x)
	int blockPlacey;				//Location of each maze section on the screen (y)
	int adj;						//Adjusted vertex value (for change from nxn size matrix to (2n+1)x(2n+1) size matrix)


	mapDivx = DISPLAY_WIDTH/newWidth;
	mapDivy = DISPLAY_HEIGHT/newHeight;
	int center = (480/2)-((mapDivx*newWidth)/2); 	// Used to center maze
	tft.fillScreen(TFT_BLACK);
	int dim = sharedVals.mazeDim;
	for(int i = 0; i < sharedVals.numEdges; i++) { 					//17 replaced with number of edges in edges[]
		startVert = sharedVals.edges[i].edgeStart;
		endVert = sharedVals.edges[i].edgeEnd;
		if(abs(startVert - endVert) == 1) { //If difference between the start and end vertices is 1, edge is parallel to x axis.
			adj = (2*newWidth*(((startVert)/mapWidth)) + (2*((startVert)%mapWidth)) + newWidth + 1);
			vertX = adj % newWidth;
			vertY = (adj-vertX)/newHeight;
			blockPlacex = (vertX)*mapDivx+1;
			blockPlacey = (vertY)*mapDivy+1;
			if(sharedVals.edges[i].isStart) {
				sharedVals.cursorX = blockPlacex+center+(mapDivx/2);
				sharedVals.cursorY = blockPlacey+(mapDivy/2);
				
			}

			if(startVert < endVert) {
					sharedVals.edges[i].startCoordX = blockPlacex+center;
					sharedVals.edges[i].endCoordX = blockPlacex+center+(3*(mapDivx));
					sharedVals.edges[i].startCoordY = blockPlacey;
					sharedVals.edges[i].endCoordY = blockPlacey+mapDivy;
					for(int j = 0; j < 3; j++) {

					tft.fillRect(blockPlacex+center+(j*mapDivx), blockPlacey, mapDivx, mapDivy, TFT_WHITE); //Maze path is placed at both vertices and in the section between them
				}
				if(sharedVals.edges[i].isStart) {

					tft.fillRect(blockPlacex+center, blockPlacey, mapDivx, mapDivy, TFT_GREEN); // place starting square

				} else if(sharedVals.edges[i].isEnd) {
					tft.fillRect(blockPlacex+center+(2*mapDivx), blockPlacey, mapDivx, mapDivy, TFT_RED); // place ending square
				}
				
			} else if(startVert > endVert) {
					sharedVals.edges[i].startCoordX = blockPlacex+center-(2*(mapDivx));
					sharedVals.edges[i].endCoordX = blockPlacex+center+mapDivx;
					sharedVals.edges[i].startCoordY = blockPlacey;
					sharedVals.edges[i].endCoordY = blockPlacey+mapDivy;
					for(int j = 0; j < 3; j++) {

					 //Maze path is placed at both vertices and in the section between them
						tft.fillRect(blockPlacex+center-(j*mapDivx), blockPlacey, mapDivx, mapDivy, TFT_WHITE);


					}
				if(sharedVals.edges[i].isStart) {
					tft.fillRect(blockPlacex+center, blockPlacey, mapDivx, mapDivy, TFT_GREEN);

					} else if(sharedVals.edges[i].isEnd) {
						tft.fillRect(blockPlacex+center-(2*mapDivx), blockPlacey, mapDivx, mapDivy, TFT_RED);
					}
					
			}

		} else if(abs(startVert-endVert) == size) { //If the difference between the start and end vertices is equal to size, edge is parallel to y axis.
			adj = (2*newWidth*(((startVert)/mapWidth)) + (2*((startVert)%mapWidth)) + newWidth + 1);
			vertX = adj % newWidth;
			vertY = (adj-vertX)/newHeight;
			blockPlacex = (vertX)*mapDivx+1;
			blockPlacey = (vertY)*mapDivy+1;
			if(sharedVals.edges[i].isStart) {
				sharedVals.cursorX = blockPlacex+center+(mapDivx/2);
				sharedVals.cursorY = blockPlacey+(mapDivy/2);
				
			}
			if(startVert < endVert) {
					sharedVals.edges[i].startCoordX = blockPlacex+center;
					sharedVals.edges[i].endCoordX = blockPlacex+center+mapDivx;
					sharedVals.edges[i].startCoordY = blockPlacey;
					sharedVals.edges[i].endCoordY = blockPlacey+(3*mapDivy);
					for(int j = 0; j < 3; j++) {
						tft.fillRect(blockPlacex+center, blockPlacey+(j*mapDivy), mapDivx, mapDivy, TFT_WHITE); 
					}
					if(sharedVals.edges[i].isStart) {
	
					tft.fillRect(blockPlacex+center, blockPlacey, mapDivx, mapDivy, TFT_GREEN); 
					} else if(sharedVals.edges[i].isEnd) {
						tft.fillRect(blockPlacex+center, blockPlacey+(2*mapDivy), mapDivx, mapDivy, TFT_RED);
					}
					
			
				

			} else if(startVert > endVert) {
					sharedVals.edges[i].startCoordX = blockPlacex+center;
					sharedVals.edges[i].endCoordX = blockPlacex+center+mapDivx;
					sharedVals.edges[i].startCoordY = blockPlacey-(2*mapDivy);
					sharedVals.edges[i].endCoordY = blockPlacey+mapDivy;
					for(int j = 0; j < 3; j++) {
						tft.fillRect(blockPlacex+center, blockPlacey-(j*mapDivy), mapDivx, mapDivy, TFT_WHITE);
					}
					if(sharedVals.edges[i].isStart) {
	
					tft.fillRect(blockPlacex+center, blockPlacey, mapDivx, mapDivy, TFT_GREEN); 
					} else if(sharedVals.edges[i].isEnd) {
						tft.fillRect(blockPlacex+center, blockPlacey-(2*mapDivy), mapDivx, mapDivy, TFT_RED);
					}
				

			}
		} 

		if(sharedVals.edges[i].isStart) {
			sharedVals.currentEdge = sharedVals.edges[i]; // Ensures the starting vertex is where the cursor begins.
		}
	}
}

bool havePassed() {		// Checking if the current edge has been passed
	for(int i = 0; i < sharedVals.numPassed; i++) {
		if(sharedVals.passedEdges[i].edgeStart == sharedVals.currentEdge.edgeStart && sharedVals.passedEdges[i].edgeEnd == sharedVals.currentEdge.edgeEnd) {
			return true;
		}
	}
	return false;
}


