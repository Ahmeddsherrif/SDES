/*
 * SDES.cpp
 *
 *  Created on: Mar 13, 2023
 *      Author: Master
 */

#include <stdint.h>
#include <string>

using namespace std;

#include "standard_parameters.h"
#include "utilities.h"

#include "SDES.h"


string SDES::encryptString(const string& plainTextString, const uint8_t &ks){
	char currentPlainTextCharchter;
	uint8_t cypherText;
	string cypherTextString;

	size_t i;
	for (i = 0; i < plainTextString.length(); i++) {
		currentPlainTextCharchter = plainTextString[i];
		cypherText = SDES::encryptByte(static_cast<uint8_t>(currentPlainTextCharchter), ks); //TODO: don't keep generating k1 and k2 each time
		cypherTextString.push_back(static_cast<char>(cypherText));
	}

	return cypherTextString;
}

string SDES::decryptString(const string& cypherTextString, const uint8_t &ks){
	char currentCypherTextCharchter;
	uint8_t plainText;
	string plainTextString;

	size_t i;
	for (i = 0; i < cypherTextString.length(); i++) {
		currentCypherTextCharchter = cypherTextString[i];
		plainText = SDES::decryptByte(static_cast<uint8_t>(currentCypherTextCharchter), ks); //TODO: don't keep generating k1 and k2 each time
		plainTextString.push_back(static_cast<char>(plainText));
	}

	return plainTextString;
}


uint16_t SDES::permutate(const uint8_t *permutationSequance, const uint16_t &inputNumber, const uint8_t &inputSize, const uint8_t &outputSize) {

	uint16_t outputNumber = 0;
	uint16_t currentBit = 0;

	uint8_t i;
	for (i = 0; i < outputSize; i++) {
		currentBit = GET_BIT(inputNumber, inputSize - permutationSequance[i]);
		SET_BIT(outputNumber, currentBit, outputSize - 1 - i);
	}

	return outputNumber;
}

uint8_t SDES::sbox(const uint8_t sboxArray[][sbox_size], const uint8_t &inputNumber) {
	uint8_t rowNumber = GET_SBOX_ROW(inputNumber);
	uint8_t columnNumber = GET_SBOX_COLUMN(inputNumber);

	uint8_t outputNumber = sboxArray[rowNumber][columnNumber];

	return outputNumber;
}

void SDES::generateKeys(const uint16_t &inputSecretKey, uint8_t &k1, uint8_t &k2) {
	uint16_t permutationOutput = permutate(p10, inputSecretKey, size_10_bit, p10_size);

	uint16_t rightSide = GET_FIRST_5_BITS(permutationOutput);
	uint16_t leftSide = GET_SECOND_5_BITS(permutationOutput);

	rightSide = LEFT_SHIFT_5_BIT(rightSide);
	leftSide = LEFT_SHIFT_5_BIT(leftSide);

	uint16_t concatenated = CONCATINATE_5_BITS_TO_10(leftSide, rightSide);

	k1 = static_cast<uint8_t>(permutate(p8, concatenated, size_10_bit, p8_size));

	int i;
	for (i = 0; i < 2; i++) {
		rightSide = LEFT_SHIFT_5_BIT(rightSide);
	}

	for (i = 0; i < 2; i++) {
		leftSide = LEFT_SHIFT_5_BIT(leftSide);
	}

	concatenated = CONCATINATE_5_BITS_TO_10(leftSide, rightSide);

	k2 = static_cast<uint8_t>(permutate(p8, concatenated, size_10_bit, p8_size));
}

uint8_t SDES::complexFunction(const uint8_t &input, uint8_t &key) {

	uint8_t inputRightSide = GET_FIRST_4_BITS(input);
	uint8_t inputLeftSide = GET_SECOND_4_BITS(input);

	uint8_t inputRightSidePermutated = permutate(ep, inputRightSide, size_4_bit, ep_size);
	uint8_t xorOutput1 = XOR(inputRightSidePermutated, key);

	uint8_t xorOutputRightSide = GET_FIRST_4_BITS(xorOutput1);
	uint8_t xorOutputLeftSide = GET_SECOND_4_BITS(xorOutput1);

	uint8_t sboxOutput1 = sbox(s1, xorOutputRightSide);
	uint8_t sboxOutput0 = sbox(s0, xorOutputLeftSide);

	uint8_t concatenated = CONCATINATE_2_BITS_TO_4(sboxOutput0, sboxOutput1);

	uint8_t permutated = permutate(p4, concatenated, size_4_bit, p4_size);

	uint8_t xorOutput2 = XOR(permutated, inputLeftSide);

	uint8_t outputNumber = CONCATINATE_4_BITS_TO_8(xorOutput2, inputRightSide);

	return outputNumber;
}

uint8_t SDES::encryptByte(const uint8_t &plainText, const uint16_t &ks) {
	uint8_t k1;
	uint8_t k2;

	generateKeys(ks, k1, k2);

	uint8_t cypherText;
	cypherText = permutate(ip, plainText, size_8_bit, ip_size);
	cypherText = complexFunction(cypherText, k1);
	cypherText = SWITCH_8_BITS(cypherText);
	cypherText = complexFunction(cypherText, k2);
	cypherText = permutate(ip_inverse, cypherText, size_8_bit, ip_inverse_size);

	return cypherText;
}

uint8_t SDES::decryptByte(const uint8_t &cypherText, const uint16_t &ks) {
	uint8_t k1;
	uint8_t k2;

	generateKeys(ks, k1, k2);

	uint8_t plainText;
	plainText = permutate(ip, cypherText, size_8_bit, ip_size);
	plainText = complexFunction(plainText, k2);
	plainText = SWITCH_8_BITS(plainText);
	plainText = complexFunction(plainText, k1);
	plainText = permutate(ip_inverse, plainText, size_8_bit, ip_inverse_size);


	return plainText;
}
