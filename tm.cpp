// TuringMachine.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <String>
#include <vector>
#include <sstream>
using namespace std;


class Transition {
public:
	char current_state = '\0';
	char input_symbol = '\0';
	char next_state = '\0';
	char write_symbol = '\0';
	char head_mov_dir = '\0';

	Transition() { }

	Transition(string line, size_t pos) { //Note: position 0 is the "t"
		current_state = line[pos + 1]; //position 1 is current state
		input_symbol = line[pos + 2]; //position 2 is input symbol
		next_state = line[pos + 3]; //position 3 is next state
		write_symbol = line[pos + 4]; //position 4 is symbol to write
		head_mov_dir = line[pos + 5]; //position 5 is which direction to move the head
	}

	//process the transition
	bool process(string &input, char &currentState, size_t &headPosition, size_t &tapeLength) {

		//if Transition can process the current state and input symbol
		if (currentState == current_state && input[headPosition] == input_symbol) {

			input[headPosition] = write_symbol; //write to input's current position with Transition's write symbol
			currentState = next_state; //transition the state to the Transition's next state

			if (head_mov_dir == 'R') {
				headPosition++; //move head to the right
			}
			else if (head_mov_dir == 'L') {
				headPosition--; //move head to the left
			}
			else if (head_mov_dir == 'H') {
				headPosition = tapeLength; //move head to the end since halted (don't process any more symbols)
			}

			return true;
		}
		//if Transition does not process the current state and input symbol
		return false;
	}
};

int main(int argc, const char * argv[]) {

	Transition *oneTransition;

	vector<char> acceptingStates; //vector to store all the accepting states
	vector<string> inputs; //vector to store all the input tapes
	vector<Transition *> transitions; //vector to store all the transitions

	string input; //used to store one line from the file 
	string line; //used to store formatted input line
	string lineFromFile;

	char currentState;
	bool hasTransition;
	bool acceptInput;

	ifstream file("TwosComplement.txt");

	//read file from standard input
	while(getline(cin, lineFromFile)){
		line = "";
		input = lineFromFile; //save one line from standard input

		// parse input and save to line (skip blank spaces)
		for (size_t i = 0, length = input.length(); i < length; i++) {
			if (input[i] != ' ') {
				line += input[i];
			}
		}

		size_t pos = 0;

		// store the final states into the final states vector (starts with "f")
		pos = line.find("f");
		if (pos != string::npos) {
			for (size_t i = (pos + 1), length = line.length(); i < length; i++) {
				acceptingStates.push_back(line[i]);
			}
			continue;
		}

		// store the transitions into the transitions vector (starts with "t")
		pos = line.find("t");
		if (pos != string::npos) {
			oneTransition = new Transition(line, pos);
			transitions.push_back(oneTransition);
			continue;
		}

		// store the inputs into the inputs vector (starts with "i")
		pos = line.find("i");
		if (pos != string::npos) {
			input = "Z";
			input = input + line.substr(pos + 1); //take the "i" out
			input = input + "Z";
			inputs.push_back(input);
			continue;
		}
	}

	//process each input tape
	for (size_t i = 0, inputsLength = inputs.size(); i < inputsLength; i++) {
		currentState = '0';
		hasTransition = false;
		acceptInput = false;

		string tempString = inputs[i];

		size_t headPosition = 1;
		size_t tapeLength = tempString.size();

		//process each symbol in the tape
		while (headPosition < tapeLength) {
			//check if there is a transition that can process the input symbol
			for (size_t k = 0, transitions_length = transitions.size(); k < transitions_length; k++) {
				hasTransition = transitions[k]->process(inputs[i], currentState, headPosition, tapeLength);

				if (hasTransition == true) {
					break;
				}
			}
			if (hasTransition == false) {
				break;
			}
		}

		//check if current state equals to any of the final/accepting states
		for (size_t k = 0, acceptingStatesLength = acceptingStates.size(); k < acceptingStatesLength; k++) {
			if (currentState == acceptingStates[k]) {
				acceptInput = true;
				break;
			}
		}

		//inputs[i] = inputs[i].substr(1, inputs[i].length()-2); //remove the Z's from the input tape

		//output the final input tape and whether the turing machine ACCEPTs or REJECTs it
		if (acceptInput == true) {
			cout << inputs[i] << endl << "ACCEPT" << endl << endl;
		}
		else {
			cout << inputs[i] << endl << "REJECT" << endl << endl;
		}
	}

	string exit;
	cout << "Type exit to exit:" << endl;
	getline(cin, exit);
	return 0;
}

