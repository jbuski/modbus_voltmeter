/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "semphr.h"
#include <stdio.h>
#include "menu.h"
#include "Cmd.h"
#include "analogclock.h"
#include "version.h"
#include "save_variable.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

#define lowByte(w) ((w) & 0xff)
#define highByte(w) ((w) >> 8)

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern TIM_HandleTypeDef htim4;

extern analog_clock_t analog_clock;

extern ee_t ee;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes =
  { .name = "defaultTask", .stack_size = 128 * 4, .priority =
      (osPriority_t) osPriorityNormal, };
/* Definitions for myTaskSlave */
osThreadId_t myTaskSlaveHandle;
const osThreadAttr_t myTaskSlave_attributes =
  { .name = "myTaskSlave", .stack_size = 128 * 4, .priority =
      (osPriority_t) osPriorityLow, };

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask (void *argument);
void StartTaskSlave (void *argument);

extern void MX_USB_DEVICE_Init (void);
void MX_FREERTOS_Init (void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init (void)
{
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew (StartDefaultTask, NULL,
				   &defaultTask_attributes);

  /* creation of myTaskSlave */
  myTaskSlaveHandle = osThreadNew (StartTaskSlave, NULL,
				   &myTaskSlave_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask (void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init ();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for (;;)
    {
      osDelay (1);
    }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTaskSlave */
/**
 * @brief Function implementing the myTaskSlave thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskSlave */
void StartTaskSlave (void *argument)
{
  /* USER CODE BEGIN StartTaskSlave */
  HAL_TIM_PWM_Start (&htim4, TIM_CHANNEL_1);

  ModbusH.u16regs[0] = template[ee.address].value; //TIM1->CCR1
  ModbusH.u16regs[1] = ee.cal_min;
  ModbusH.u16regs[2] = ee.cal_max;
  ModbusH.u16regs[3] = template[ee.address].in_min;
  ModbusH.u16regs[4] = template[ee.address].in_max;

  ModbusH.u16regs[6] = 0;
  ModbusH.u16regs[7] = 0;
  ModbusH.u16regs[8] = 0;
  ModbusH.u16regs[9] = 0;
  ModbusH.u16regs[10] = 255;
  ModbusH.u16regs[14] = 0;
  ModbusH.u16regs[15] = ee.address;
  /* Infinite loop */
  for (;;)
    {
      xSemaphoreTake(ModbusH.ModBusSphrHandle, 100);
      analog_clock.value = ModbusH.u16regs[0]; //TIM1->CCR1
      analog_clock.out_min = ModbusH.u16regs[1];
      analog_clock.out_max = ModbusH.u16regs[2];
      analog_clock.in_min = ModbusH.u16regs[3];
      analog_clock.in_max = ModbusH.u16regs[4];

      calulate ();

      ModbusH.u16regs[6] = analog_clock.pwm;

      analog_clock.pwm_r = ModbusH.u16regs[7];
      analog_clock.pwm_g = ModbusH.u16regs[8];
      analog_clock.pwm_b = ModbusH.u16regs[9];
      analog_clock.pwm_w = ModbusH.u16regs[10];

      if (bitRead(ModbusH.u16regs[14], 7))
	{
	  bitClear(ModbusH.u16regs[14], 7);
	  reset_mcu ();
	}
      if (bitRead(ModbusH.u16regs[14], 6))
	{
	  bitClear(ModbusH.u16regs[14], 6);
	  load_eeprom ();
	  ModbusH.u16regs[1] = ee.cal_min;
	  ModbusH.u16regs[2] = ee.cal_max;
	  ModbusH.u16regs[15] = ee.address;
	  //new_adr_modbus(ModbusH.u16regs[15]);
	}
      if (bitRead(ModbusH.u16regs[14], 5))
	{
	  bitClear(ModbusH.u16regs[14], 5);
	  ee.cal_min = ModbusH.u16regs[1];
	  ee.cal_max = ModbusH.u16regs[2];
	  ee.address = ModbusH.u16regs[15];
	  save_eeprom ();
	  reset_mcu ();
	}
      if (bitRead(ModbusH.u16regs[14], 4))
	{
	  bitClear(ModbusH.u16regs[14], 4);
	  default_eeprom ();
	  reset_mcu ();
	}
      ModbusH.u16regs[27] = BUILD_NR;
      ModbusH.u16regs[28] = DEVICE_TYPE;
      ModbusH.u16regs[29] = VERSION_MAJOR;
      ModbusH.u16regs[30] = VERSION_MINOR;
      ModbusH.u16regs[31] = VERSION_MICRO;

      TIM4->CCR1 = analog_clock.pwm;
      xSemaphoreGive(ModbusH.ModBusSphrHandle);
      osDelay (200);
    }
  /* USER CODE END StartTaskSlave */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

