/*
 * SDES.h
 *
 *  Created on: Mar 13, 2023
 *      Author: Master
 */

#ifndef SDES_H_
#define SDES_H_

class SDES {
	public:
		static bool logFlag;
		static uint8_t encryptByte(const uint8_t &plainText, const uint16_t &ks);
		static uint8_t decryptByte(const uint8_t &cypherText, const uint16_t &ks);
	private:
		static uint16_t permutate(const uint8_t *permutationSequance, const uint16_t &inputNumber, const uint8_t &inputSize, const uint8_t &outputSize);
		static uint8_t sbox(const uint8_t sboxArray[][sbox_size], const uint8_t &inputNumber);
		static void generateKeys(const uint16_t &inputSecretKey, uint8_t &k1, uint8_t &k2);
		static uint8_t complexFunction(const uint8_t &input, uint8_t &key);
};

#endif /* SDES_H_ */
