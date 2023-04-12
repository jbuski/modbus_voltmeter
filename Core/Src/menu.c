/*
 * menu.c
 *
 *  Created on: 15 Feb 2022
 *      Author: jacekbuski
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "menu.h"
#include "main.h"
#include "Cmd.h"
#include "typedef.h"

#include "version.h"
#include "save_variable.h"
#include "usbd_cdc_if.h"

#include "FreeRTOS.h"
#include "task.h"

#define  CDC_printf(...)\
         printf(__VA_ARGS__); \

char ptrTaskList[250];

const char cmd_banner1[] = "********** CMD *********\r\n";
const char cmd_banner2[] = "Type 'help' to get help.\r\n";
const char cmd_prompt[] = "CMD >> ";
const char cmd_unrecog[] = "CMD: SYNTAX ERROR\r\n";

extern ee_t ee;

void init_cmd (void)
{
  //cmdInit (115200, huart1);
  cmdAdd ("help", help_cmd);
  cmdAdd ("reset", reset_cmd);
  cmdAdd ("top", top_cmd);
  cmdAdd ("version", version_cmd);
  cmd_display();
}

void help_cmd (int arg_cnt, char **args)
{
  if (arg_cnt == 1)
    {
      CDC_printf ("help \r\n");
      CDC_printf ("reset \r\n");
      CDC_printf ("top \r\n");
    }
  else if (arg_cnt == 2)
    {
      if (strcmp (args[1], "reset") == 0)
	{
	  reset_cmd_help_msg ();
	}
      else if (strcmp (args[1], "top") == 0)
	{
	  top_cmd_help_msg ();
	}
      else if (strcmp (args[1], "help") == 0)
	{
	  help_cmd_help_msg ();
	}
      else
	{
	  help_cmd_error_msg ();
	}
    }
  else
    {
      help_cmd_error_msg ();
    }
}

void help_cmd_help_msg (void)
{
  CDC_printf ("help \r\n");
}

void help_cmd_error_msg (void)
{
  CDC_printf ("Type 'help' to get help.\n\r");
}

void reset_cmd (int arg_cnt, char **args)
{
  if (arg_cnt == 1)
    {
      reset_mcu ();
    }
  else
    {
      reset_cmd_error_msg ();
    }
}

void clearScreen (void)
{
  CDC_printf ("\033[2J");
}

void homeScreen (void)
{
  CDC_printf ("\033[0;0H");
}

void moveUp (uint8_t a)
{
  CDC_printf ("\033[%iA", a);
}

void moveDown (uint8_t b)
{
  CDC_printf ("\033[%iB", b);
}

void moveTo_Pos (uint8_t x, uint8_t y)
{
  CDC_printf ("\033[%i;%iH", x, y);
}

void top_cmd_error_msg (void)
{
  CDC_printf ("Type 'help top' to get help.\n\r");
}

void cmd_error_msg (void)
{
  CDC_printf ("SYNTAX ERROR!\n\r");
  CDC_printf ("Type 'help' to get help.\n\r");
}

void reset_cmd_error_msg (void)
{
  CDC_printf ("Type 'help reset' to get help.\n\r");
}

void reset_cmd_help_msg (void)
{
  CDC_printf ("reset \r\n");
}

void print_cmd_help_msg (void)
{
  CDC_printf ("print \r\n");
}

void top_cmd_help_msg (void)
{
  CDC_printf ("top \r\n");
}

void reset_mcu (void)
{
  CDC_printf ("Reseting...\n\r");
  HAL_Delay(1000);
  HAL_NVIC_SystemReset ();
}

void version_cmd (int arg_cnt, char **args)
{
  if (arg_cnt == 1)
    {
      print_sw_version();
    }
  else
    {
      version_cmd_error_msg ();
    }
}

void version_cmd_help_msg (void)
{
  CDC_printf ("version \r\n");
}

void version_cmd_error_msg (void)
{
  CDC_printf ("Type 'help reset' to get help.\n\r");
}


void vprint(const char *fmt, va_list argp)
{
    char string[200];
    if(0 < vsprintf(string,fmt,argp)) // build string
    {
	while( CDC_Transmit_FS((uint8_t*)string, strlen(string)) != USBD_OK);
        //HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 0xffffff); // send message via UART
    }
}

//void CDC_printf(const char *fmt, ...) // custom printf() function
//    va_list argp;
//    va_start(argp, fmt);
//    vprint( fmt, argp);
 //   va_end(argp);
//}

void top_cmd (int arg_cnt, char **args)
{
  vTaskList(ptrTaskList);
  CDC_printf("******************************************\r\n");
  CDC_printf("Task          State   Prio    Stack    Num\r\n");
  CDC_printf("******************************************\r\n");
  CDC_printf(ptrTaskList);
  CDC_printf("******************************************\r\n");
  CDC_printf("'B' - Blocked  'R' - Ready  'D' - Deleted (waiting clean up)\r\n");
  CDC_printf("'S' - Suspended, or Blocked without a timeout\r\n");
}
