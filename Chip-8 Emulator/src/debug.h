/*
 * debug.h
 *
 *  Created on: 25 de jul de 2017
 *      Author: Zero
 */

#ifndef CHIP_8_EMULATOR_SRC_DEBUG_H_
#define CHIP_8_EMULATOR_SRC_DEBUG_H_
#include <stdbool.h>

void debug_init();
unsigned short debug_emulateCycle();
bool debug_loadApplication(const char * filename);

unsigned char V[16];
unsigned char buff[64 * 32];
unsigned char debugKey[16];

#endif /* CHIP_8_EMULATOR_SRC_DEBUG_H_ */
