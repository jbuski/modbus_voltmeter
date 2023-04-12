/*
 * eeprom.h
 *
 *  Created on: Feb 4, 2023
 *      Author: jacekbuski
 */

#ifndef INC_SAVE_VARIABLE_H_
#define INC_SAVE_VARIABLE_H_

#include "typedef.h"

flash_state_t init_save(void);
flash_state_t default_eeprom (void);
flash_state_t save_eeprom (void);
flash_state_t load_eeprom (void);
void print_new_values(void);


#endif /* INC_SAVE_VARIABLE_H_ */
