#include <Arduino.h>
#include "shared.h"
#include <Adafruit_GFX.h>
#include "edge.h"
#include <MCUFRIEND_kbv.h>
extern Shared sharedVals;

bool check_mon() {
	if (Serial.available()) {
		return true; 
	}
	else {
		return false;
	}
}

enum Client {
	WAITING_FOR_START, RECIEVING_INFO, MAKE_CONNECTIONS, GET_SOLUTION, FASTEST_PATH, IN_GAME , USER_SOLN, GET_SCORE
};
Client Client_step = WAITING_FOR_START;


void before_maze(MCUFRIEND_kbv &tft) {
	char instruct;
	// Initially Pick options
	while (true) {
		if (Client_step == WAITING_FOR_START) {
			if (sharedVals.mazeDim == 5) {
				Serial.print("X "); 
				Serial.println(5);
				Serial.print("Y "); 
				Serial.println(5);  
				Client_step = RECIEVING_INFO; 
				int xcoord = 5; 
				int ycoord = 5; 
			}
			else if (sharedVals.mazeDim == 6) {
				Serial.print("X "); 
				Serial.println(6);
				Serial.print("Y "); 
				Serial.println(6); 
				Client_step = RECIEVING_INFO; 
				int xcoord = 6; 
				int ycoord = 6;
			}
			else if (sharedVals.mazeDim == 7) {
				Serial.print("X "); 
				Serial.println(7);
				Serial.print("Y "); 
				Serial.println(7);  
				Client_step = RECIEVING_INFO; 
				int xcoord = 7; 
				int ycoord = 7;
			}
			// Not sure we need this one
			else {
				continue; 
			}
		}
		else if (Client_step == RECIEVING_INFO) {
			if (Serial.available()) {
				instruct = Serial.read(); 
				// Get start
				if (instruct == 'S') {
					sharedVals.startVert = Serial.parseInt(); 
					Serial.flush(); 
					// Acknowledgement 
					Serial.println("A");
				}
				// Get number of edges after 
				else if (instruct == 'N') {
					sharedVals.numEdges = Serial.parseInt();
					Serial.flush();
					Serial.println("A");
					Client_step = MAKE_CONNECTIONS;
				}
				
			}
		}
		else if (Client_step == MAKE_CONNECTIONS) {
			Edge E;
			for (int i = 0; i <= sharedVals.numEdges; i++) {
				while (check_mon() == false) {}; 
				instruct = Serial.read();
				if (instruct == 'C') {
					int val1 = Serial.parseInt();
					int val2 = Serial.parseInt();
					E = {false, false, val1, val2, 0, 0, 0, 0};
					//if(!(val1 == 0 && val2 == 0)) {
						sharedVals.edges[i] = E; 

					//} //else {
					// 	i--;
					// }

					// Connect these I guess
					Serial.println("A"); 
					Serial.flush();  
					//delay(500);
				}
				else if (instruct == 'E') {
					sharedVals.endVert = Serial.parseInt(); 
					Serial.flush(); 
					Serial.println("A");
					Client_step = GET_SOLUTION; 
				}
			}
		}
		else if (Client_step == GET_SOLUTION) {
			if (Serial.available()) {
				instruct = Serial.read(); 
				if (instruct == 'N') {
					sharedVals.solutionSize = Serial.parseInt(); 
					Serial.flush(); 
					Serial.println("A");
					Client_step = FASTEST_PATH;
				}				
			}
		}
		else if (Client_step == FASTEST_PATH) {
			for (int i = 0; i <= sharedVals.solutionSize; i++) {
				while (check_mon() == false) {}; 
				instruct = Serial.read(); 
				if (instruct == 'C') {
					int val1 = Serial.parseInt();
					int val2 = Serial.parseInt();
					if(!(val1 == 0 && val2 == 0)) {
						sharedVals.solutionPath[i].edgeStart = val1; 
						sharedVals.solutionPath[i].edgeEnd = val2;
					} else {
						i--;
					}
					Serial.flush(); 
					Serial.println("A");
				}
				else if (instruct == 'H') {
					Client_step = USER_SOLN;
				}
			}
		}
		else {
			break;
		}
	}
}

void after_maze() {
	char instruct;
	while (true) {
		if (Client_step == USER_SOLN) {
			// Landyn my dude add the functions to get these values
			int Time = sharedVals.userTime;
			int soln_size  = sharedVals.numPassed;
			Serial.print("T ");
			Serial.println(Time); 
			Serial.print("N "); 
			Serial.println(soln_size); 
/*			for (int i = 0; i< soln_size; i++) {
				int current = sharedVals.passedEdges[i].edgeStart; 
				Serial.println(current); 
				intc
			}*/
			Client_step = GET_SCORE;
		}
		else if (Client_step == GET_SCORE) {
			if (Serial.available()) {
				instruct = Serial.read(); 
				if (instruct == 'O') {
					sharedVals.score = Serial.parseInt(); 
					Serial.flush(); 
					Serial.println("A");
					// Send to start again
					Client_step = WAITING_FOR_START; 
				}
			}
		} else {
			break;
		}
	}
}