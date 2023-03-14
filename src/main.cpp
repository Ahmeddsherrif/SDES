//============================================================================
// Name        : SDES.cpp
// Author      : Ahmed
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "main.h"





int main(int argc, char *argv[]) {

	uint8_t outputByte;

	string operationType_string = string(argv[1]);
	string ks_string = string(argv[2]);
	string data_string = string(argv[3]);



	char operationType = operationType_string[0];
	uint16_t ks = static_cast<uint16_t>(bitset<10>(ks_string).to_ulong());
	uint8_t data = static_cast<uint8_t>(bitset<8>(data_string).to_ulong());

	switch (operationType) {
		case 'e':
		case 'E': {
			outputByte = SDES::encryptByte(data, ks);
			PRINT_LABEL_WITH_NEWLINE("Secret Key: ", bitset<10>(ks_string));
			PRINT_LABEL_WITH_NEWLINE("Plain Text: ", bitset<8>(data_string));
			PRINT_LABEL_WITH_NEWLINE("Encrypted Text: ", bitset<8>(outputByte));
			PRINT_SEPERATOR();
			break;
		}

		case 'd':
		case 'D': {
			outputByte = SDES::decryptByte(data, ks);
			PRINT_LABEL_WITH_NEWLINE("Secret Key: ", bitset<10>(ks_string));
			PRINT_LABEL_WITH_NEWLINE("Cypher Text: ", bitset<8>(data_string));
			PRINT_LABEL_WITH_NEWLINE("Decrypted Text: ", bitset<8>(outputByte));
			PRINT_SEPERATOR();
			break;
		}

		default: {

		}
	}

	return 0;
}
