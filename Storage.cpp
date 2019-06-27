#include "Storage.h"
#include "LogTrace.h"

//init clasa storage
byte Storage::storage_init(){
  logtrace_print(T_ALL,__func__);
  // pentru a evita inituri multiple
  if( flashInit == true )
    return ERROR_SUCCESS;
  
  // init flash
  flash = new SPIFlash(FLASH_SS, 0xEF30);
  if (flash->initialize())
  {
    logtrace_print(T_DEBUG,"Flash Init OK!");
    flashInit = true;
    return ERROR_SUCCESS;
  }
  else{
    logtrace_print(T_ERROR,"Init FAIL!");
    return ERROR_FLASH_NOT_INIT;
  }
}

//costructor
Storage::Storage(){
  flashInit = false;
}


//destructor
Storage::~Storage(){
  if( NULL != flash ){
    delete(flash);
    flashInit = false;
  }
}

//read data from a specific address 
byte Storage::storage_read( uint32_t des, void* buf, uint16_t len ){
  logtrace_print(T_ALL,__func__);
  // verifica parametrii
  if( NULL == buf )
    return  ERROR_INVALID_PARAM;
  if( flashInit == false )
    return ERROR_FLASH_NOT_INIT;
  
  // citeste de la destinatie
  flash->readBytes( des,buf,len );
  return ERROR_SUCCESS;
}

// writes data to a specific address
byte Storage::storage_write( uint32_t des, const void* buf, uint16_t len ){
  logtrace_print(T_ALL,__func__);
  // verifica parametrii
  if( NULL == buf || len > SIZE_4K )
    return  ERROR_INVALID_PARAM;

  if( flashInit == false )
    return ERROR_FLASH_NOT_INIT;
  
  byte temp_buffer[SIZE_4K];
  storage_read( des,temp_buffer,SIZE_4K);
  memcpy(temp_buffer,buf,len);
  flash->blockErase4K(des);
  flash->writeBytes(des,temp_buffer,SIZE_4K);

  return ERROR_SUCCESS;
}

// erases flash data from a specific des
byte Storage::storage_erase( uint32_t des, uint32_t len ){
  logtrace_print(T_ALL,__func__);
  uint32_t  save_len = 0;
  uint32_t  save_add = 0;
  
  if( flashInit == false )
    return ERROR_FLASH_NOT_INIT;
  
  if( len < SIZE_4K ){
      save_len = SIZE_4K - len;
      save_add = des + len;
      
      byte* temp_buffer = new byte( save_len );
      storage_read( save_add,temp_buffer,save_len);
      flash->blockErase4K(des);
      flash->writeBytes(save_add,temp_buffer,save_len);
      delete(temp_buffer);
  }
  return ERROR_SUCCESS;
}

byte Storage::storage_format(){
  logtrace_print(T_ALL,__func__);
  if( flashInit == false )
    return ERROR_FLASH_NOT_INIT;
  
  flash->chipErase();
  while(flash->busy());
  return ERROR_SUCCESS;
}

