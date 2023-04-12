/*
 * version.c
 *
 *  Created on: Nov 19, 2022
 *      Author: jacekbuski
 */

#include "version.h"
#include <stdio.h>
#include "menu.h"

#define SIZE_OF_FWINFO  8
/*
__attribute__((section(".myvars.FWInfo")))  static uint8_t nFirmwareInfoBlock[SIZE_OF_FWINFO] =
  { 0xff,   // reserved for future
      0xff,   // reserved for future
      DEVICE_TYPE,    // DeviceType
      BUILD_NR,       // BuildNr of Firmware
      VERSION_MINOR,  // VersionMinor of Firmware
      VERSION_MAJOR,  // VersionMajor of Firmware
      0xFF,           // Checksum
      0xFF            // Checksum
    };

__attribute__((section(".myvars.COMPILATION_DATE"))) static char COMPILATION_DATE[] =
    __DATE__;
__attribute__((section(".myvars.COMPILATION_TIME"))) static char COMPILATION_TIME[] =
    __TIME__;
*/

void print_sw_version (void)
{
  CDC_printf ("Version %s %s %s\n\r", VERSION, __DATE__, __TIME__);
}
