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


#define LOGGING	1
#if LOGGING==1
#define LOG_LABEL_WITH_NEWLINE(LABEL, INPUT)		cout <<left <<setfill(' ')<<setw(PRINT_WIDTH)<<LABEL <<INPUT<< endl
#define LOG_WITH_NEWLINE(INPUT)						cout << INPUT << endl
#define LOG_ALLIGN(INPUT)							cout << left <<setfill(' ')<<setw(PRINT_WIDTH) <<INPUT
#define LOG_NEWLINE()								cout << endl
#define LOG(INPUT)									cout << INPUT
#define PRINT_SEPERATOR()							cout << setfill(PRINT_SEPRATOR_CHAR_CONFIG) << setw(PRINT_SEPRATOR_WIDTH_CONFIG) << "" << endl
#else
#define LOG_LABEL_WITH_NEWLINE(LABEL, INPUT)
#define LOG_WITH_NEWLINE(INPUT)
#define LOG_ALLIGN(INPUT)
#define LOG_NEWLINE()
#define LOG(INPUT)
#define PRINT_SEPERATOR()

#endif

#endif /* DEBUG_H_ */
