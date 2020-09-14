/*-------------------------------------------------------------------------
 CMPUT 275 | Winter 2020
 Names: Saif Husnain, Landyn David, Justin Javier, Darius Fang
 CCIDs: sahusnai, lpugh, jjavier, dfang1
 IDs: 1573497, 1586291, 1573754, 1570975

 Final Major Project

 Description: Maze Game
 server.cpp

-------------------------------------------------------------------------- */
#include <iostream>
#include "maze_generator.h"
#include "maze_solver.h"
#include "digraph.h"
#include "unionfind.h"
#include "serialport.h"

using namespace std;

enum Server {
	/* States of the server. 
	"WAITING_FOR_XY" Recieves the size of maze to be generated from client 1.
	"SEND_XY" Sends the size of maze to be generated to client 2.
	"GENERATE_MAZE" Generates mae using kruskal's algorithm. Also finds the quickest path.
	"SEND_MAZE" Sends both the maze and soluton to both clients
	"RECIEVE_USER" Recives the user's time to finish the maze
	"SEND_SCORE" Sends the score to the other client 
	*/
	WAITING_FOR_XY, SEND_XY, GENERATE_MAZE, SEND_MAZE, RECIEVE_USER, SEND_SCORE
};

bool ack(SerialPort &Serial) {
	/* Reads in "A\n" to for recieving acknowledgement from the client
	Parameters: 
		Serial: selected client

	Returns:
		true if ackowledged
	*/
	string line;
	do {
		line = Serial.readline();
		if (line == "") {
			return false;
		}
	} while (line != "A\n");
	return true;
}

void parseint(string line, int &first, int &second) {
	/* Recives the integer input of 2 values, and converting them to integers
	The line input should be "Char Input1 Input2"
	Parameters:
		line: Line of the input string
		first: The second input of the string
		second: The third input of the string

	Returns:
		first and second
	*/
	int start1, start2;
	int end = line.size()-1;
	start1 = line.find(" ");
	start2 = line.rfind(" ");
	if (start1 == start2) { //if only 1 int
		first = stoi(line.substr(start1, end));
		second = stoi(line.substr(start2, end));
	} else {
		first = stoi(line.substr(start1, start2-1));
		second = stoi(line.substr(start2, end));
	}
}

int read_xy(SerialPort &Serial1, int &x, int&y) {
	/* Reads in the size of the maze to be generated from client 1

	Parameters:
		Serial1: client 1
		x: x length of maze
		y: y length of maze

	Returns:
		x and y
	*/
	string line;
	cout << "READING XY" << endl;
	do {
		line = Serial1.readline();
		if (line == "") {
			cout << "Timeout" << endl;
			return 0;
		}
	} while (line.front() != 'X');
	cout << line;
	parseint(line, x, x);
	do {
		line = Serial1.readline();
		if (line == "") {
			cout << "Timeout" << endl;
			return 0;
		}
	} while (line.front() != 'Y');
	cout << line;
	parseint(line, y, y);
	return 1;
}

int send_xy(SerialPort &Serial2, int x, int y) {
	/* Sends the size of the maze to be generated to the client
	Parameters:
		Serial2: client to send
		x: length
		y: height

	Returns:
		1 if successful
	*/

	cout << "SENDING XY" << endl;
	string output_line;
	output_line = "X " + to_string(x) + "\n";

	output_line = "Y " + to_string(y) + "\n";

	if (ack(Serial2)) {
		return 1;
	} else {
		return 0;
	}
}

int send_solution(SerialPort &Serial, vector<int> answer) {
	/* Sends the solution to the arduino in vector form of the node
	Parameters:
		Serial: client to send
		answer: vector of the solution

	Returns:
		1 if successful
	*/
	cout << "SENDING SOLUTION" << endl;
	string output_line;
	output_line = "N " + to_string(answer.size()-1) + "\n";
	cout << output_line;
	Serial.writeline(output_line);
	if (!ack(Serial)) {
		return 0;
	}
	for (int i = 0; i < answer.size()-1; i++) {
		output_line = "C " + to_string(answer[i]) + " " + to_string(answer[i+1]) + "\n";  // nodes 
		cout << output_line;
		Serial.writeline(output_line);
		if (!ack(Serial)) {
			return 0;
		}
	}
	Serial.writeline("H\n");
	return 1;
}

int send_maze(SerialPort &Serial, int start, int end, multimap<int, int> connected) {
	/* Send the maze with all the connections, the start of the maze, and end of the maze

	Parameters:
		Serial: client to send
		start: start of the maze
		end: end of the maze
		connect: the connect maze

	Returns:
		1 if sucessful
	*/
	cout << "SENDING MAZE" << endl;
	string output_line;

	output_line = "S " + to_string(start) + "\n";  // START node
	cout << output_line;
	Serial.writeline(output_line);
	if (!ack(Serial)) {
		return 0;
	}

	output_line = "N " + to_string(connected.size()) + "\n"; // Number of edges
	cout << output_line;
	Serial.writeline(output_line);
	if (!ack(Serial)) {
		return 0;
	}
	for (auto it = connected.begin(); it != connected.end(); it++) {

		output_line = "C " + to_string(it->first) + " " + to_string(it->second) + "\n"; // Connection of 2 nodes
		cout << output_line;
		Serial.writeline(output_line);
		if (!ack(Serial)) {
			return 0;
		}
	}

	output_line = "E " + to_string(end) + "\n";  // END node
	cout << output_line;
	Serial.writeline(output_line);
	if (!ack(Serial)) {
		return 0;
	}

	return 1;
}

int read_client(SerialPort &Serial, int &time, int &solution_len) {
	/* Reads in the input of the client. Specifically the time and the solution the client created

	Parameters:
		Serial: client selected
		time: time it took to complete the maze
		solution: solution given from the maze

	Returns:
		returns 1 if recieved

	*/
	cout << "READING CLIENT" << endl;
	string line;
	do {
		line = Serial.readline();
		if (line == "") {
			return 0;
		}
	} while (line.front() != 'T');  // Time
	cout << line;
	parseint(line, time, time);
	do {
		line = Serial.readline();
		if (line == "") {
			return 0;
		}
	} while (line.front() != 'N');  // Number of nodes
	cout << line;
	parseint(line, solution_len, solution_len);
	Serial.writeline("H\n");
	return 1;
}

int send_score(SerialPort &Serial, int score) {
	/* Sends the score of the client to the other client

	Parameters:
		Serial: Client selected
		score: score recieved

	Returns:
		1 if acknowledged
	*/
	cout << "SENDING SCORE" << endl;
	string output_line;
	output_line = "O " + to_string(score) + "\n";
	cout << output_line;
	Serial.writeline(output_line);
	if (!ack(Serial)) {
		return 0;
	}
	return 1;
}

int main() {
	//Initalize Variables
	Digraph graph;
	SerialPort Serial1("/dev/ttyACM0"); //Client 1
	// SerialPort Serial2("/dev/ttyACM1"); //Client 2
	unordered_map<int, int> shortest_path;
	multimap<int, int> connected;
	vector<int> answer;
	int len_answer, len_solution1, len_solution2;
	int x, y, time1 = 0, time2 = 0, score1 = 0, score2 = 0;
	int start, end;

	Server state = WAITING_FOR_XY;
	//read in x and y
	while (true) {
		if (state == WAITING_FOR_XY) {
			shortest_path.clear();
			connected.clear();
			answer.clear();
			graph.clear();

			while (read_xy(Serial1, x, y) == 0) {}; //keep trying to read
			Serial1.writeline("A\n"); //If gotten
			state = GENERATE_MAZE;

		} else if (state == SEND_XY) {
			// while (send_xy(Serial2, x, y) == 0) {};
			state = GENERATE_MAZE;
		} else if (state == GENERATE_MAZE) {
			srand(time(NULL)); //random seed each run
			//Different Types: dfs_maze, djk_maze, kruskal_maze
			kruskal_maze(x, y, graph, connected); //Create Maze
			//Generate start and end nodes
			start_end_node(x, y, start, end, graph);
			solve_maze(start, end, graph, answer);
			len_answer = answer.size();
			state = SEND_MAZE;

		} else if (state == SEND_MAZE) {
			while (send_maze(Serial1, start, end, connected) == 0) {};
			while (send_solution(Serial1, answer) == 0) {};
			// while(send_maze(Serial2, start, end, connected) == 0) {};
			// while (send_solution(Serial2, answer) == 0) {};
			state = RECIEVE_USER;
		} else if (state == RECIEVE_USER) {
			while (read_client(Serial1, time1, len_solution1) == 0) {} ;
			// while (read_client(Serial2, time2, len_solution2) == 0) {};
			score1 = calculate_score(time1, len_answer, len_solution1, graph);
			// score2 = calculate_score(time2, len_answer, len_solution2, graph);
			state = SEND_SCORE;
		} else if (state == SEND_SCORE) {
			while (send_score(Serial1, score1) == 0) {};
			// while (send_score(Serial2, score2) == 0 ) {};
			state = WAITING_FOR_XY;
		}
	}

	return 0;
}