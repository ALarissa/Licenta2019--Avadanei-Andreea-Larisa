#ifndef _STORAGE_
#define _STORAGE_

#include <SPIFlash.h>    //https://github.com/LowPowerLab/SPIFlash
#include <SPI.h>
#include <stdint.h>

#define LED           15 // Moteino MEGAs have LEDs on D15
#define FLASH_SS      23 // and FLASH SS on D23



#define FLASH_MID        0xEF30
#define SIZE_4K          4096

//error codes
#define ERROR_SUCCESS        0x00
#define ERROR_INVALID_PARAM  0x01
#define ERROR_FLASH_NOT_INIT 0x02
#define ERROR_READ           0x03
#define ERROR_WRITE          0x04

class Storage{
  public:
    SPIFlash *flash; // folosit pentru a stoca informatii in flash
    bool flashInit; // folosit pentru a vedea daca e initializata clasa
  public:
    Storage();
    ~Storage();
    byte storage_init();
    byte storage_read( uint32_t des, void* buf, uint16_t len );
    byte storage_write( uint32_t des, const void* buf, uint16_t len );
    byte storage_erase( uint32_t des, uint32_t len );
    byte storage_format();
};

#endif
