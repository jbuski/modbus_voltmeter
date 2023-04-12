/*******************************************************************
 Copyright (C) 2009 FreakLabs
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 3. Neither the name of the the copyright holder nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.

 Originally written by Christopher Wang aka Akiba.
 Please post support questions to the FreakLabs forum.

 *******************************************************************/
/*!
 \file Cmd.c

 This implements a simple command line interface for the Arduino so that
 its possible to execute individual functions within the sketch.
 */
/**************************************************************************/

#include "Cmd.h"
#include "main.h"
#include <stdio.h>
#include "usbd_cdc_if.h"
#include "menu.h"
#include "version.h"

// command line message buffer and pointer
static uint8_t msg[MAX_MSG_SIZE];
static uint8_t *msg_ptr;
static uint8_t command_ready_flag = 0;
int8_t index_p = 0;

extern uint8_t clear;
extern uint8_t clearNow;

// linked list for command table
static cmd_t *cmd_tbl_list, *cmd_tbl;

// text strings for command prompt (stored in flash)
extern const char cmd_banner1[];
extern const char cmd_banner2[];
extern const char cmd_prompt[];
extern const char cmd_unrecog[];

UART_HandleTypeDef *gHuart;

/**************************************************************************/
/*!
 Generate the main command prompt
 */
/**************************************************************************/
void cmd_display (void)
{
  print_sw_version();
  CDC_printf ("\r\n");
  CDC_printf (cmd_banner1);
  CDC_printf (cmd_banner2);
  CDC_printf ("\r");
  //print_promt_time();
  CDC_printf (cmd_prompt);
  CDC_printf ("%s", msg);
}

/**************************************************************************/
/*!
 Parse the command line. This function tokenizes the command input, then
 searches for the command table entry associated with the commmand. Once found,
 it will jump to the corresponding function.
 */
/**************************************************************************/
void cmd_parse (char *cmd)
{
  uint8_t argc, i = 0;
  char *argv[30];
  //char buf[50];
  cmd_t *cmd_entry;

  //fflush (stdout);

  // parse the command line statement and break it up into space-delimited
  // strings. the array of strings will be saved in the argv array.
  argv[i] = strtok (cmd, " ");
  do
    {
      argv[++i] = strtok (NULL, " ");
    }
  while ((i < 30) && (argv[i] != NULL));

  // save off the number of arguments for the particular command.
  argc = i;

  // parse the command table for valid command. used argv[0] which is the
  // actual command name typed in at the prompt
  for (cmd_entry = cmd_tbl; cmd_entry != NULL; cmd_entry = cmd_entry->next)
    {
      if (!strcmp (argv[0], cmd_entry->cmd))
	{
	  cmd_entry->func (argc, argv);
	  //cmd_display();
	  return;
	}
    }

  // command not recognized. print message and re-generate prompt.
  CDC_printf (cmd_unrecog);
//    Serial.println(buf);

  cmd_display();
}

/**************************************************************************/
/*!
 This function processes the individual characters typed into the command
 prompt. It saves them off into the message buffer unless its a "backspace"
 or "enter" key.
 */
/**************************************************************************/
void cmd_handler (char c)
{
//char c = getchar();

  if (c == '\r')
    {
      command_ready_flag = 1;
    }
  else if (c == '\b' || c == 127)
    {
      if (msg_ptr > msg && index_p > 0)
	{
	  //putchar (c);
	  //printf("%i",index_p);
	  index_p--;
	  msg_ptr--;
	}
    }
  else
    {

      if (index_p < MAX_MSG_SIZE)
	{
	  index_p++;
	  *msg_ptr++ = c;
	  //putchar (c);
	  //printf("%i",index_p);
	}
    }
}

/**************************************************************************/
/*!
 This function should be set inside the main loop. It needs to be called
 constantly to check if there is any available input at the command prompt.
 */
/**************************************************************************/
void cmd_exec (void)
{
  if (command_ready_flag)
    {
      index_p = 0;
      *msg_ptr = '\0';
      CDC_printf ("\r\n");
      cmd_parse ((char*) msg);
      //clear = 100;
      //clearNow = 1;
      msg_ptr = msg;
      for (uint8_t i = 0; i < MAX_MSG_SIZE; i++)
	{
	  msg[i] = '\0';
	}
      command_ready_flag = 0;
    }
  else
    {

    }
}

void cmdPoll (void)
{
      CDC_printf ("\r");
      CDC_printf ("%s", cmd_prompt);
      CDC_printf ("%s", msg);
}

/**************************************************************************/
/*!
 Initialize the command line interface. This sets the terminal speed and
 and initializes things.
 */
/**************************************************************************/
void cmdInit (uint32_t speed, UART_HandleTypeDef *huart)
{

  // init the msg ptr
  msg_ptr = msg;

  //c_ptr = c;

  // init the command table
  cmd_tbl_list = NULL;

  // set the serial speed
  //gHuart->Init.BaudRate = speed;
  //HAL_UART_Receive_IT (gHuart, &c, 1);
  //CDC_Transmit_FS();
}

/**************************************************************************/
/*!
 Add a command to the command table. The commands should be added in
 at the setup() portion of the sketch.
 */
/**************************************************************************/
void cmdAdd (char *name, void (*func) (int argc, char **argv))
{
  // alloc memory for command struct
  cmd_tbl = (cmd_t*) malloc (sizeof(cmd_t));

  // alloc memory for command name
  char *cmd_name = (char*) malloc (strlen (name) + 1);

  // copy command name
  strcpy (cmd_name, name);

  // terminate the command name
  cmd_name[strlen (name)] = '\0';

  // fill out structure
  cmd_tbl->cmd = cmd_name;
  cmd_tbl->func = func;
  cmd_tbl->next = cmd_tbl_list;
  cmd_tbl_list = cmd_tbl;
}

/**************************************************************************/
/*!
 Convert a string to a number. The base must be specified, ie: "32" is a
 different value in base 10 (decimal) and base 16 (hexadecimal).
 */
/**************************************************************************/
uint32_t cmdStr2Num (char *str, uint8_t base)
{
  return strtol (str, NULL, base);
}

