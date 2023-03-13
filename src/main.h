/*
 * main.h
 *
 *  Created on: Mar 13, 2023
 *      Author: Master
 */

#ifndef MAIN_H_
#define MAIN_H_


#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

#include "standard_parameters.h"
#include "utilities.h"
#include "SDES.h"


#define PRINT_SEPRATOR_WIDTH_CONFIG			100
#define PRINT_SEPRATOR_CHAR_CONFIG			'='
#define PRINT_SEPERATOR()					cout << setfill(PRINT_SEPRATOR_CHAR_CONFIG) << setw(PRINT_SEPRATOR_WIDTH_CONFIG) << "" << endl
#define PRINT(STRING)						cout << STRING << endl;


#endif /* MAIN_H_ */
