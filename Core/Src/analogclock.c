/*
 * analogclock.c
 *
 *  Created on: 5 Mar 2023
 *      Author: jacekbuski
 */

#include "analogclock.h"
#include <stdio.h>

uint8_t template_default = 1;

analog_clock_t analog_clock;

analog_clock_template_t template[10] =
    {
	{0,60,0,8192,0,60},//default
	{1,60,0,8192,0,60},//min
	{2,60,0,8192,0,60},//sec
	{3,12,0,7840,0,12},//h12
	{4,24,0,7840,0,24},//h24
	{5,52,0,8192,0,52},//wk
	{6,31,0,8192,0,31},//d
	{7,4,0,8192,0,4},//lun
	{8,30,0,8192,0,30},//lun
	{9,1,0,8192,0,4} //AM/PM

    };

uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  if (((in_max - in_min) + out_min) == 0) return 0;
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void calulate(void)
{
  analog_clock.pwm = map(analog_clock.value,
			 analog_clock.in_min,	analog_clock.in_max,
			 analog_clock.out_min,	analog_clock.out_max);
}

void init_modbus(UART_HandleTypeDef *huart, uint8_t adr)
{
  /* Modbus Slave initialization */
    //ModbusH.uModbusType = MB_SLAVE;
    ModbusH.uModbusType = MB_SLAVE;
    ModbusH.xTypeHW = USART_HW;
    ModbusH.port =  huart; // This is the UART port connected to STLINK in the NUCLEO F429
    ModbusH.u8id = adr; //slave ID, always different than zero
    //ModbusH.u8id = 0; //slave ID for master always 0
    ModbusH.u16timeOut = 1000;
    ModbusH.EN_Port = NULL; // No RS485
     //ModbusH2.EN_Port = LD2_GPIO_Port; // RS485 Enable
     //ModbusH2.EN_Pin = LD2_Pin; // RS485 Enable
    ModbusH.u16regs = ModbusDATA;
    ModbusH.u16regsize= sizeof(ModbusDATA)/sizeof(ModbusDATA[0]);
     //Initialize Modbus library
    ModbusInit(&ModbusH);
    //Start capturing traffic on serial Port
    ModbusStart(&ModbusH);

    printf("slave %d\n\r",adr);

}

void new_adr_modbus(uint8_t adr)
{
    ModbusH.u8id = adr; //slave ID, always different than zero

    printf("slave %d\n\r",adr);
}
