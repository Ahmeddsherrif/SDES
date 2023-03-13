//============================================================================
// Name        : SDES.cpp
// Author      : Ahmed
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "main.h"

enum class Error_t {
	NO_ERROR, INVALID_ARGUMENT_COUNT, INVAILD_PROCESS_TYPE, INVALID_SECRET_KEY, CANT_OPEN_INPUT_FILE
};

int main(int argc, char *argv[]) {

	uint16_t ks;

	Error_t currentErrorMessage = Error_t::NO_ERROR;

	//Check for Argument Count Error
	if (argc != 5) {
		currentErrorMessage = Error_t::INVALID_ARGUMENT_COUNT;
		cout << "INVALIED ARGUMENT COUNT" << endl;
	}

	//Check for SECRET KEY ERROR
	if (currentErrorMessage == Error_t::NO_ERROR) {
		string ks_string = string(argv[2]);
		ks = static_cast<uint16_t>(stoi(ks_string));

		if (ks > 1023) {
			currentErrorMessage = Error_t::INVALID_SECRET_KEY;
			cout << "INVALIED SECRET KEY RANGE" << endl;
		}
	}

	vector<string> data;
	if (currentErrorMessage == Error_t::NO_ERROR) {

		string inputFileName = string(argv[3]);
		ifstream inputFile(inputFileName);

		if (inputFile.is_open()) {

			string line;

			while (getline(inputFile, line)) {
				data.push_back(line);
			}

			inputFile.close();
		} else {
			cout << "CAN'T OPEN INPUT FILE" << endl;
			currentErrorMessage = Error_t::CANT_OPEN_INPUT_FILE;
		}
	}

	if (currentErrorMessage == Error_t::NO_ERROR) {

		string outputFileName = string(argv[4]);
		ofstream outputFile(outputFileName, ios::app);

		string processType = string(argv[1]);
		switch (processType[0]) {
			case 'E': {

				for (size_t i = 0; i < data.size(); i++) {
					string cypherString = SDES::encryptString(data.at(i), ks);
					outputFile << cypherString << endl;
				}

				break;
			}
			case 'D': {

				for (size_t i = 0; i < data.size(); i++) {
					string plainText = SDES::decryptString(data.at(i), ks);
					outputFile << plainText << endl;
				}

				break;
			}

			default: {
				currentErrorMessage = Error_t::INVAILD_PROCESS_TYPE;
				cout << "INVALID PROCESS TYPE" << endl;
			}
		}

		outputFile.close();

	}

	return 0;
}
