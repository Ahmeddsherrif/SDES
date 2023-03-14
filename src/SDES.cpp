/*
 * SDES.cpp
 *
 *  Created on: Mar 13, 2023
 *      Author: Master
 */

#include <iostream>
#include <stdint.h>
#include <string>
#include <bitset>
#include <iomanip>

using namespace std;

#include "debug.h"
#include "standard_parameters.h"
#include "utilities.h"

#include "SDES.h"

uint16_t SDES::permutate(const uint8_t *permutationSequance, const uint16_t &inputNumber, const uint8_t &inputSize, const uint8_t &outputSize) {
	LOG_LABEL_WITH_NEWLINE("BEFORE PERMUTATION:", bitset<10>(inputNumber).to_string().substr(10-inputSize, inputSize));

	uint16_t outputNumber = 0;
	uint16_t currentBit = 0;

	LOG_ALLIGN("PERMUTATION SEQUANCE :");
	LOG("[");

	uint8_t i;
	for (i = 0; i < outputSize; i++) {
		LOG(static_cast<int>(permutationSequance[i]));

		if (i != outputSize - 1) {
			LOG(", ");
		}

		currentBit = GET_BIT(inputNumber, inputSize - permutationSequance[i]);
		SET_BIT(outputNumber, currentBit, outputSize - 1 - i);
	}

	LOG("]");
	LOG_NEWLINE();


	LOG_LABEL_WITH_NEWLINE("AFTER PERMUTATION:", bitset<10>(outputNumber).to_string().substr(10-outputSize, outputSize));

	return outputNumber;
}

uint8_t SDES::sbox(const uint8_t sboxArray[][sbox_size], const uint8_t &inputNumber) {

	LOG_LABEL_WITH_NEWLINE("SBOX INPUT:", bitset<4>(inputNumber));

	uint8_t rowNumber = GET_SBOX_ROW(inputNumber);
	uint8_t columnNumber = GET_SBOX_COLUMN(inputNumber);

	uint8_t outputNumber = sboxArray[rowNumber][columnNumber];

	LOG_LABEL_WITH_NEWLINE("SBOX ROW:", bitset<2>(rowNumber));
	LOG_LABEL_WITH_NEWLINE("SBOX COLUMN: ", bitset<2>(columnNumber));
	LOG_LABEL_WITH_NEWLINE("SBOX OUTPUT: ", bitset<2>(outputNumber));

	return outputNumber;
}

void SDES::generateKeys(const uint16_t &inputSecretKey, uint8_t &k1, uint8_t &k2) {

	LOG_LABEL_WITH_NEWLINE("INPUT SECRET KEY: ", bitset<10>(inputSecretKey));

	uint16_t permutationOutput = permutate(p10, inputSecretKey, size_10_bit, p10_size);

	uint16_t rightSide = GET_FIRST_5_BITS(permutationOutput);
	uint16_t leftSide = GET_SECOND_5_BITS(permutationOutput);

	LOG_LABEL_WITH_NEWLINE("PERMUTATION OUTPUT RIGHT SIDE: ", bitset<5>(rightSide));
	LOG_LABEL_WITH_NEWLINE("PERMUTATION OUTPUT LEFT SIDE:", bitset<5>(leftSide));

	rightSide = LEFT_SHIFT_5_BIT(rightSide);
	leftSide = LEFT_SHIFT_5_BIT(leftSide);

	LOG_LABEL_WITH_NEWLINE("LEFT SHIFT OUTPUT RIGHT SIDE: ", bitset<5>(rightSide));
	LOG_LABEL_WITH_NEWLINE("LEFT SHIFT OUTPUT LEFT SIDE:", bitset<5>(leftSide));

	uint16_t concatenated = CONCATINATE_5_BITS_TO_10(leftSide, rightSide);

	LOG_LABEL_WITH_NEWLINE("CONCATENATED OUTPUT:", bitset<10>(concatenated));

	k1 = static_cast<uint8_t>(permutate(p8, concatenated, size_10_bit, p8_size));

	LOG_LABEL_WITH_NEWLINE("K1:", bitset<8>(k1));

	int i;
	for (i = 0; i < 2; i++) {
		rightSide = LEFT_SHIFT_5_BIT(rightSide);
	}

	for (i = 0; i < 2; i++) {
		leftSide = LEFT_SHIFT_5_BIT(leftSide);
	}

	LOG_LABEL_WITH_NEWLINE("LEFT SHIFT 2 OUTPUT RIGHT SIDE: ", bitset<5>(rightSide));
	LOG_LABEL_WITH_NEWLINE("LEFT SHIFT 2 OUTPUT LEFT SIDE:", bitset<5>(leftSide));

	concatenated = CONCATINATE_5_BITS_TO_10(leftSide, rightSide);

	LOG_LABEL_WITH_NEWLINE("CONCATENATED OUTPUT:", bitset<10>(concatenated));

	k2 = static_cast<uint8_t>(permutate(p8, concatenated, size_10_bit, p8_size));

	LOG_LABEL_WITH_NEWLINE("K2:" , bitset<8>(k2));
}

uint8_t SDES::complexFunction(const uint8_t &input, uint8_t &key) {
	LOG_LABEL_WITH_NEWLINE("COMPLEX FUNCTION INPUT:", bitset<8>(input));

	uint8_t inputRightSide = GET_FIRST_4_BITS(input);
	uint8_t inputLeftSide = GET_SECOND_4_BITS(input);

	LOG_LABEL_WITH_NEWLINE("COMPLEX FUNCTION INPUT RIGHT SIDE:", bitset<4>(inputRightSide));
	LOG_LABEL_WITH_NEWLINE("COMPLEX FUNCTION INPUT LEFT SIDE:", bitset<4>(inputLeftSide));

	uint8_t inputRightSidePermutated = permutate(ep, inputRightSide, size_4_bit, ep_size);
	uint8_t xorOutput1 = XOR(inputRightSidePermutated, key);

	LOG_LABEL_WITH_NEWLINE("XOR OUTPUT 1:", bitset<8>(xorOutput1));

	uint8_t xorOutputRightSide = GET_FIRST_4_BITS(xorOutput1);
	uint8_t xorOutputLeftSide = GET_SECOND_4_BITS(xorOutput1);

	LOG_LABEL_WITH_NEWLINE("XOR OUTPUT 1 RIGHT SIDE:", bitset<4>(xorOutputRightSide));
	LOG_LABEL_WITH_NEWLINE("XOR OUTPUT 1 LEFT SIDE:", bitset<4>(xorOutputLeftSide));

	uint8_t sboxOutput1 = sbox(s1, xorOutputRightSide);
	uint8_t sboxOutput0 = sbox(s0, xorOutputLeftSide);

	uint8_t concatenated = CONCATINATE_2_BITS_TO_4(sboxOutput0, sboxOutput1);
	LOG_LABEL_WITH_NEWLINE("CONCATENATED OUTPUT:", bitset<4>(concatenated));

	uint8_t permutated = permutate(p4, concatenated, size_4_bit, p4_size);

	uint8_t xorOutput2 = XOR(permutated, inputLeftSide);
	LOG_LABEL_WITH_NEWLINE("XOR OUTPUT2:", bitset<4>(xorOutput2));

	uint8_t outputNumber = CONCATINATE_4_BITS_TO_8(xorOutput2, inputRightSide);
	LOG_LABEL_WITH_NEWLINE("CONCATENATED OUTPUT:", bitset<8>(outputNumber));

	LOG_LABEL_WITH_NEWLINE("COMPLEX FUNCTION OUTPUT:", bitset<8>(outputNumber));

	return outputNumber;
}

uint8_t SDES::encryptByte(const uint8_t &plainText, const uint16_t &ks) {
	uint8_t k1;
	uint8_t k2;

	PRINT_SEPERATOR();

	LOG_WITH_NEWLINE("GENERATING KEYS K1 AND K2 ...");
	PRINT_SEPERATOR();

	generateKeys(ks, k1, k2);
	PRINT_SEPERATOR();

	LOG_LABEL_WITH_NEWLINE("K1:", bitset<8>(k1));
	LOG_LABEL_WITH_NEWLINE("K2:", bitset<8>(k2));
	PRINT_SEPERATOR();

	uint8_t cypherText;

	LOG_WITH_NEWLINE("INTIAL PERMUTATION OF PLAIN TEXT");
	PRINT_SEPERATOR();

	cypherText = permutate(ip, plainText, size_8_bit, ip_size);
	PRINT_SEPERATOR();

	LOG_WITH_NEWLINE("COMPLEX FUNCTION OF INTIAL PERMUTATION OUTPUT WITH K1");
	PRINT_SEPERATOR();

	cypherText = complexFunction(cypherText, k1);
	PRINT_SEPERATOR();

	LOG_WITH_NEWLINE("SWITCH COMPLEX FUNCTION OUTPUT");
	PRINT_SEPERATOR();

	LOG_LABEL_WITH_NEWLINE("BEFORE SWITCHING:", bitset<8>(cypherText));
	cypherText = SWITCH_8_BITS(cypherText);
	LOG_LABEL_WITH_NEWLINE("AFTER SWITCHING:", bitset<8>(cypherText));
	PRINT_SEPERATOR();

	LOG_WITH_NEWLINE("COMPLEX FUNCTION OF SWITCHING OUTPUT WITH K2");
	PRINT_SEPERATOR();

	cypherText = complexFunction(cypherText, k2);
	PRINT_SEPERATOR();

	LOG_WITH_NEWLINE("INVERSE PERMUTATION OF COMPLEX FUNCTION OUTPUT");
	PRINT_SEPERATOR();

	cypherText = permutate(ip_inverse, cypherText, size_8_bit, ip_inverse_size);
	PRINT_SEPERATOR();

	LOG_LABEL_WITH_NEWLINE("CYPHER TEXT:", bitset<8>(cypherText));
	PRINT_SEPERATOR();

	return cypherText;
}

uint8_t SDES::decryptByte(const uint8_t &cypherText, const uint16_t &ks) {
	uint8_t k1;
	uint8_t k2;

	PRINT_SEPERATOR();

	LOG_WITH_NEWLINE("GENERATING KEYS K1 AND K2 ...");
	PRINT_SEPERATOR();

	generateKeys(ks, k1, k2);
	PRINT_SEPERATOR();

	LOG_LABEL_WITH_NEWLINE("K1:", bitset<8>(k1));
	LOG_LABEL_WITH_NEWLINE("K2:", bitset<8>(k2));
	PRINT_SEPERATOR();

	uint8_t plainText;
	LOG_WITH_NEWLINE("INTIAL PERMUTATION OF CYPHER TEXT");
	PRINT_SEPERATOR();

	plainText = permutate(ip, cypherText, size_8_bit, ip_size);
	PRINT_SEPERATOR();


	LOG_WITH_NEWLINE("COMPLEX FUNCTION OF INTIAL PERMUTATION OUTPUT WITH K2");
	PRINT_SEPERATOR();

	plainText = complexFunction(plainText, k2);
	PRINT_SEPERATOR();

	LOG_WITH_NEWLINE("SWITCH COMPLEX FUNCTION OUTPUT");
	PRINT_SEPERATOR();

	LOG_LABEL_WITH_NEWLINE("BEFORE SWITCHING:", bitset<8>(plainText));
	plainText = SWITCH_8_BITS(plainText);
	LOG_LABEL_WITH_NEWLINE("AFTER SWITCHING:", bitset<8>(plainText));
	PRINT_SEPERATOR();

	LOG_WITH_NEWLINE("COMPLEX FUNCTION OF SWITCHING OUTPUT WITH K1");
	PRINT_SEPERATOR();

	plainText = complexFunction(plainText, k1);
	PRINT_SEPERATOR();

	LOG_WITH_NEWLINE("INVERSE PERMUTATION OF COMPLEX FUNCTION OUTPUT");
	PRINT_SEPERATOR();

	plainText = permutate(ip_inverse, plainText, size_8_bit, ip_inverse_size);
	PRINT_SEPERATOR();

	LOG_LABEL_WITH_NEWLINE("PLAIN TEXT:", bitset<8>(cypherText));
	PRINT_SEPERATOR();

	return plainText;
}
