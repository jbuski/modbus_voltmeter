/*
 * eeprom.c
 *
 *  Created on: Feb 4, 2023
 *      Author: jacekbuski
 */
#include "save_variable.h"
#include "typedef.h"
#include "eeprom.h"
#include "main.h"
#include <stdio.h>
#include "analogclock.h"

#define EE_DELAY 1000
#define eeAdd_lock 0x0000
#define eeAddress 0x0002
#define DEFAULT_ADR 0x01

extern ee_t ee;
extern flash_state_t flash_ready;
extern analog_clock_template_t template[10];

flash_state_t init_save(void)
{
  HAL_FLASH_Unlock();
  printf("Unlocking FLASH ... ");
  if( EE_Init() != EE_OK)
  {
    flash_ready = FLASH_NOT_READY;
    printf ("FLASH not ready ... ");
    Error_Handler();
  }
  else
    {
      printf ("FLASH Init OK ... ");
      flash_ready = FLASH_READY;
    }
  HAL_FLASH_Lock();
  printf("Locking FLASH ... ");
  printf("Done\n\r");
  return flash_ready;
}

flash_state_t default_eeprom (void)
{
  printf("Restoring defualt settings ... \n\r");

  ee.address = DEFAULT_ADR;
  ee.cal_min = template[DEFAULT_ADR].out_min;
  ee.cal_max = template[DEFAULT_ADR].out_max;

  save_eeprom ();
  printf("Done\n\r");
  HAL_Delay (EE_DELAY);
  print_new_values ();
  return flash_ready;
}

flash_state_t save_eeprom (void)
{
  printf("Saving to FLASH ...  ");
  if (flash_ready)
    {
    HAL_FLASH_Unlock();
    printf("Unlocking FLASH ... ");
    for (int i = 0; i <= sizeof(ee.raw); i++)
      {
	if (EE_WriteVariable (eeAddress + (i), ee.raw[i]) != 0)
	  {
	    printf("Error write variable %d \n\r",i);
	  }
      }
    HAL_FLASH_Lock();
    printf("Locking FLASH ... ");
    }
  else
    {
      printf("FLASH not ready... ");
    }
  printf("Done\n\r");
  HAL_Delay (EE_DELAY);
  print_new_values ();
  return flash_ready;
}

flash_state_t load_eeprom (void)
{
  printf("Loading from FLASH ... ");
  uint16_t value;
  if (flash_ready)
    {
    if ((EE_ReadVariable (eeAdd_lock, &value) != 0) || (value != 0xDEAD))
      {
	HAL_FLASH_Unlock();
	if (EE_WriteVariable (eeAdd_lock, 0xDEAD) != 0)
	  {
	    printf("Error write variable %d \n\r",0xDEAD);
	  }
	HAL_FLASH_Lock();
	default_eeprom();
	return flash_ready;
      }
    for (int i = 0; i <= sizeof(ee.raw); i++)
	{
	if (EE_ReadVariable (eeAddress + (i), &ee.raw[i]) !=0)
	    {
	      printf("Error read variable %d\n\r",i);
	    }
      }
    }
  else
    {
      printf ("FLASH not ready ... ");
    }
  printf("Done\n\r");
  HAL_Delay (EE_DELAY);
  print_new_values ();
  return flash_ready;
}

void print_new_values (void)
{
  printf ("Address: %d \n\r", ee.address);
  printf ("Calibration values min = %d max = %d \n\r ", ee.cal_min, ee.cal_max);
}
