/*
 * debug.h
 *
 *  Created on: Mar 14, 2023
 *      Author: Master
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#define PRINT_WIDTH							50
#define PRINT_SEPRATOR_WIDTH_CONFIG			100
#define PRINT_SEPRATOR_CHAR_CONFIG			'='



#define LOG_LABEL_WITH_NEWLINE(LABEL, INPUT)		(SDES::logFlag == true)? cout <<left <<setfill(' ')<<setw(PRINT_WIDTH)<<LABEL <<INPUT<< endl : cout << ""
#define LOG_WITH_NEWLINE(INPUT)						(SDES::logFlag == true)? cout << INPUT << endl :cout << ""
#define LOG_ALLIGN(INPUT)							(SDES::logFlag == true)? cout << left <<setfill(' ')<<setw(PRINT_WIDTH) <<INPUT : cout << ""
#define LOG_NEWLINE()								(SDES::logFlag == true)? cout << endl : cout << ""
#define LOG(INPUT)									(SDES::logFlag == true)? cout << INPUT : cout << ""
#define PRINT_SEPERATOR()							(SDES::logFlag == true)? cout << setfill(PRINT_SEPRATOR_CHAR_CONFIG) << setw(PRINT_SEPRATOR_WIDTH_CONFIG) << "" << endl : cout << ""

#endif /* DEBUG_H_ */
