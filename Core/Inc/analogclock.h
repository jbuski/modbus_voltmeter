/*
 * analogclock.h
 *
 *  Created on: 5 Mar 2023
 *      Author: jacekbuski
 */

#ifndef SRC_ANALOGCLOCK_H_
#define SRC_ANALOGCLOCK_H_

#include <stdint.h>
#include "usart.h"

extern uint8_t template_default;

typedef struct analog_clock_d{
	uint16_t value;
	uint16_t out_min;
	uint16_t out_max;
	uint16_t in_min;
	uint16_t in_max;
	uint16_t pwm;
	uint16_t pwm_r;
	uint16_t pwm_g;
	uint16_t pwm_b;
	uint16_t pwm_w;
}analog_clock_t;

typedef struct analog_clock_template_d{
	uint16_t adr;
	uint16_t value;
	uint16_t out_min;
	uint16_t out_max;
	uint16_t in_min;
	uint16_t in_max;
}analog_clock_template_t;

extern analog_clock_template_t template[10];

extern analog_clock_t analog_clock;

void calulate(void);

void init_modbus(UART_HandleTypeDef *huart, uint8_t adr);

void new_adr_modbus(uint8_t adr);

uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

#endif /* SRC_ANALOGCLOCK_H_ */
