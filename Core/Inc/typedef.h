/*
 * typedef.h
 *
 *  Created on: Feb 4, 2023
 *      Author: jacekbuski
 */

#ifndef INC_TYPEDEF_H_
#define INC_TYPEDEF_H_

#include <stdint.h>

typedef union
{
  uint16_t raw[3];
  struct
  {
    int16_t address;
    int16_t cal_min;
    int16_t cal_max;
  };
} ee_t;

typedef enum flash_state_e
{
  FLASH_NOT_READY,
  FLASH_READY,
}flash_state_t;


#endif /* INC_TYPEDEF_H_ */
