//============================================================================
// Name        : SDES.cpp
// Author      : Ahmed
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip>
#include <conio.h>
using namespace std;

#define PRINT_SEPRATOR_WIDTH_CONFIG			100
#define PRINT_SEPRATOR_CHAR_CONFIG			'='
#define PRINT_SEPERATOR()					cout << setfill(PRINT_SEPRATOR_CHAR_CONFIG) << setw(PRINT_SEPRATOR_WIDTH_CONFIG) << "" << endl
#define PRINT(STRING)						cout << STRING << endl;

constexpr uint8_t size_4_bit = 4;
constexpr uint8_t size_8_bit = 8;
constexpr uint8_t size_10_bit = 10;

constexpr uint8_t p10_size = 10;
constexpr uint8_t p8_size = 8;
constexpr uint8_t ip_size = 8;
constexpr uint8_t ip_inverse_size = 8;
constexpr uint8_t ep_size = 8;
constexpr uint8_t p4_size = 4;
constexpr uint8_t sbox_size = 4;

constexpr uint8_t p10[p10_size] = { 3, 5, 2, 7, 4, 10, 1, 9, 8, 6 };
constexpr uint8_t p8[p8_size] = { 6, 3, 7, 4, 8, 5, 10, 9 };
constexpr uint8_t ip[ip_size] = { 2, 6, 3, 1, 4, 8, 5, 7 };
constexpr uint8_t ip_inverse[ip_inverse_size] = { 4, 1, 3, 5, 7, 2, 8, 6 };
constexpr uint8_t ep[ep_size] = { 4, 1, 2, 3, 2, 3, 4, 1 };
constexpr uint8_t p4[p4_size] = { 2, 4, 3, 1 };

constexpr uint8_t s0[sbox_size][sbox_size] = { { 1, 0, 3, 2 }, { 3, 2, 1, 0 }, { 0, 2, 1, 3 }, { 3, 1, 3, 2 } };
constexpr uint8_t s1[sbox_size][sbox_size] = { { 0, 1, 2, 3 }, { 2, 0, 1, 3 }, { 3, 0, 1, 0 }, { 2, 1, 0, 3 } };

#define GET_BIT(NUMBER, BIT_NUMBER) 						(((NUMBER)>>(BIT_NUMBER))&1)
#define SET_BIT(NUMBER, BIT_VALUE, BIT_NUMBER)				((NUMBER)|=((BIT_VALUE)<<(BIT_NUMBER)))

#define GET_FIRST_4_BITS(NUMBER)							((NUMBER) & 0x0F)
#define GET_SECOND_4_BITS(NUMBER)							(((NUMBER) & 0xF0) >> 4)

#define GET_FIRST_5_BITS(NUMBER)							((NUMBER) & 0x1F)
#define GET_SECOND_5_BITS(NUMBER)							((NUMBER & 0x3E0) >> 5)

#define GET_FIRST_8_BITS(NUMBER)							(NUMBER & 0xFF)

#define CONCATINATE_2_BITS_TO_4(HIGH_NUMBER, LOW_NUMBER)	(((HIGH_NUMBER) << 2) | (LOW_NUMBER))
#define CONCATINATE_4_BITS_TO_8(HIGH_NUMBER, LOW_NUMBER)	(((HIGH_NUMBER) << 4) | (LOW_NUMBER))
#define CONCATINATE_5_BITS_TO_10(HIGH_NUMBER, LOW_NUMBER)	(((HIGH_NUMBER) << 5) | (LOW_NUMBER))

#define SWITCH_8_BITS(NUMBER)								(CONCATINATE_4_BITS_TO_8(GET_FIRST_4_BITS(NUMBER), GET_SECOND_4_BITS(NUMBER)))

#define LEFT_SHIFT_4_BIT(NUMBER)							((GET_BIT(NUMBER, 3) << 0) | GET_FIRST_4_BITS((NUMBER << 1)))
#define RIGHT_SHIFT_4_BIT(NUMBER)							((GET_BIT(NUMBER, 0) << 3) | GET_FIRST_4_BITS((NUMBER >> 1)))

#define LEFT_SHIFT_5_BIT(NUMBER)							((GET_BIT(NUMBER, 4) << 0) | GET_FIRST_5_BITS((NUMBER << 1)))
#define RIGHT_SHIFT_5_BIT(NUMBER)							((GET_BIT(NUMBER, 0) << 4) | GET_FIRST_5_BITS((NUMBER >> 1)))

#define LEFT_SHIFT_8_BIT(NUMBER)							((GET_BIT(NUMBER, 7) << 0) | GET_FIRST_8_BITS((NUMBER << 1)))
#define RIGHT_SHIFT_8_BIT(NUMBER)							((GET_BIT(NUMBER, 0) << 7) | GET_FIRST_8_BITS((NUMBER >> 1)))

#define GET_SBOX_ROW(NUMBER)								((GET_BIT(NUMBER, 3) << 1) + GET_BIT(NUMBER, 0))
#define GET_SBOX_COLUMN(NUMBER)								((GET_BIT(NUMBER, 2) << 1) + GET_BIT(NUMBER, 1))

#define XOR(FIRST_NUMBER, SECOND_NUMBER)					((FIRST_NUMBER)^(SECOND_NUMBER))

uint16_t permutate(const uint8_t *permutationSequance, const uint16_t &inputNumber, const uint8_t &inputSize, const uint8_t &outputSize) {

	uint16_t outputNumber = 0;
	uint16_t currentBit = 0;

	uint8_t i;
	for (i = 0; i < outputSize; i++) {
		currentBit = GET_BIT(inputNumber, inputSize - permutationSequance[i]);
		SET_BIT(outputNumber, currentBit, outputSize - 1 - i);
	}

	return outputNumber;
}

uint8_t sbox(const uint8_t sboxArray[][sbox_size], const uint8_t &inputNumber) {
	uint8_t rowNumber = GET_SBOX_ROW(inputNumber);
	uint8_t columnNumber = GET_SBOX_COLUMN(inputNumber);

	uint8_t outputNumber = sboxArray[rowNumber][columnNumber];

	return outputNumber;
}

void generateKeys(const uint16_t &inputSecretKey, uint8_t &k1, uint8_t &k2) {
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

uint8_t complexFunction(const uint8_t &input, uint8_t &key) {

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

uint8_t encrypt(const uint8_t &plainText, const uint16_t &ks) {
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

uint8_t decrypt(const uint8_t &cypherText, const uint16_t &ks) {
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

enum class State {
	ENCRYPT, DECRYPT
};

int main() {

	uint16_t ks;
	ks = 642;

	uint8_t plainText = 151;
	uint8_t cypherText = encrypt(plainText, ks);
	uint8_t plainText_ = decrypt(cypherText, ks);

	return 0;
}
