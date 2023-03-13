/*
 * standard_parameters.h
 *
 *  Created on: Mar 13, 2023
 *      Author: Master
 */

#ifndef STANDARD_PARAMETERS_H_
#define STANDARD_PARAMETERS_H_

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




#endif /* STANDARD_PARAMETERS_H_ */
