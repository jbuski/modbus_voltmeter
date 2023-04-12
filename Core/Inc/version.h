/*
 * variable.h
 *
 *  Created on: Feb 27, 2022
 *      Author: jacekbuski
 */

#ifndef INC_VERSION_H_
#define INC_VERSION_H_

#include "stm32f4xx_hal.h"

#define BUILD_NR 0
#define DEVICE_TYPE 1
#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_MICRO 1
#define VERSION_SPECIAL "/beta"

#define STRINGIFY0(s) # s
#define STRINGIFY(s) STRINGIFY0(s)

#define SIZE_OF_FWINFO  8

#define VERSION STRINGIFY(VERSION_MAJOR)"."STRINGIFY(VERSION_MINOR)"."STRINGIFY(VERSION_MICRO)""VERSION_SPECIAL

void print_sw_version(void);

#endif /* INC_VERSION_H_ */
