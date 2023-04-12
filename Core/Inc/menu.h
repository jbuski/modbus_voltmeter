/*
 * menu.h
 *
 *  Created on: 15 Feb 2022
 *      Author: jacekbuski
 */

#ifndef SRC_MENU_H_
#define SRC_MENU_H_

#include <stdio.h>
#include <string.h>

#include "stm32f4xx_hal.h"

void init_cmd(void);

void help_cmd(int arg_cnt, char **args);
void help_cmd_help_msg (void);
void help_cmd_error_msg (void);

void reset_cmd(int arg_cnt, char **args);
void reset_cmd_help_msg(void);
void reset_cmd_error_msg (void);
void reset_mcu(void);

void top_cmd (int arg_cnt, char **args);
void top_cmd_help_msg(void);
void top_cmd_error_msg(void);

void version_cmd (int arg_cnt, char **args);
void version_cmd_help_msg (void);
void version_cmd_error_msg (void);

void clearScreen(void);
void homeScreen(void);
void moveUp(uint8_t a);
void moveDown(uint8_t b);
void moveTo_Pos(uint8_t x,uint8_t y);

void CDC_printf(const char *fmt, ...);




#endif /* SRC_MENU_H_ */
