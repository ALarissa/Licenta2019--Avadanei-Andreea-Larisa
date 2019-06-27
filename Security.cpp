#include "Security.h"
#include "LogTrace.h"

// PRIVATE FUNCTIONS
// read pin from Flash
void Security::ReadSecurityPin ( uint8_t pinType ){
  logtrace_print(T_ALL,__func__);
  PIN *tmpPin;
  uint32_t pinAddress;
  switch( pinType ){
    case PIN_TYPE_MASTER:
      tmpPin = &masterPin;
      pinAddress = PIN_ADDRESS1;
      break;
    case PIN_TYPE_GATE:
      tmpPin = &gatePin;
      pinAddress = PIN_ADDRESS2;
      break;
  }
  
  myStorage.storage_read( pinAddress, tmpPin, sizeof(PIN) );
  if( 0 != strcmp(tmpPin->sign, PIN_SIGN) ){
   logtrace_print(T_DEBUG,"Invalid PIN Entry found");
  }
  else{
     logtrace_print(T_ALL,"Valid PIN Found: %s", tmpPin->pin);
  }
  
}

// erase pin
void Security::ClearSecurityPin(uint8_t pinType){
  logtrace_print(T_ALL,__func__);
  uint32_t pinAddress;
  switch( pinType ){
    case PIN_TYPE_MASTER:
      pinAddress = PIN_ADDRESS1;
      break;
    case PIN_TYPE_GATE:
      pinAddress = PIN_ADDRESS2;
      break;
  }
  myStorage.storage_erase(pinAddress,sizeof(PIN));
  ReadSecurityPin(pinType);
}

void Security::StoreSecurityPin( char* buf, uint8_t len, uint8_t pinType ){
  logtrace_print(T_ALL,__func__);
  PIN *tmpPin;
  uint32_t pinAddress;
  switch( pinType ){
    case PIN_TYPE_MASTER:
      tmpPin = &masterPin;
      pinAddress = PIN_ADDRESS1;
      break;
    case PIN_TYPE_GATE:
      tmpPin = &gatePin;
      pinAddress = PIN_ADDRESS2;
      break;
  }

  logtrace_print(T_DEBUG,"[Security::StoreSecurityPin]: %s, %d, %d",buf,len,pinType);
  
  memset( tmpPin->pin,0,MAX_PIN);
  tmpPin->u8PinSize = len;
  memcpy( tmpPin->pin,buf,len);
  memcpy( tmpPin->sign,PIN_SIGN,MAX_SIGN);
  Serial.print(tmpPin->pin);
  myStorage.storage_write( pinAddress, tmpPin, sizeof( PIN ));
}

void Security::PrintCurrentPin(uint8_t pinType){
  logtrace_print(T_ALL,__func__);
  PIN *tmpPin;
  switch( pinType ){
    case PIN_TYPE_MASTER:
      tmpPin = &masterPin;
      break;
    case PIN_TYPE_GATE:
      tmpPin = &gatePin;
      break;
  }
  logtrace_print(T_INFO,"Current PIN: %s\n",tmpPin->pin);
}

char* Security::GetCurrentPin(uint8_t pinType){
  logtrace_print(T_ALL,__func__);
  PIN *tmpPin;
  switch( pinType ){
    case PIN_TYPE_MASTER:
      tmpPin = &masterPin;
      break;
    case PIN_TYPE_GATE:
      tmpPin = &gatePin;
      break;
  }
  return tmpPin->pin;
}

bool Security::ValidatePin(char* buf, uint8_t len,uint8_t pinType){
  logtrace_print(T_ALL,__func__);
  PIN *tmpPin;
  switch( pinType ){
    case PIN_TYPE_MASTER:
      tmpPin = &masterPin;
      break;
    case PIN_TYPE_GATE:
      tmpPin = &gatePin;
      break;
  }
  logtrace_print(T_DEBUG,"[Security::ValidatePin]: Received %s, %d, %d",buf,len,pinType);
  logtrace_print(T_DEBUG,"[Security::ValidatePin]: Current %s, %d",tmpPin->pin,tmpPin->u8PinSize);
  
  if( (0 != strcmp(tmpPin->pin,buf)) || ( len != tmpPin->u8PinSize ) )
    return false;
  return true;
}

//PUBLIC FUNCTIONS
// Init Class
void Security::Init(){
  logtrace_print(T_ALL,__func__);
  gatePin.u8PinSize = 0;
  memset( gatePin.pin,0,MAX_PIN );
  memset( gatePin.sign,0,MAX_SIGN );

  masterPin.u8PinSize = 0;
  memset( masterPin.pin,0,MAX_PIN );
  memset( masterPin.sign,0,MAX_SIGN );

  memset( &emailSetup,0,sizeof(EMAIL_SETUP));
  
  myStorage.storage_init();
  ReadSecurityPin(PIN_TYPE_MASTER);
  ReadSecurityPin(PIN_TYPE_GATE);
  ReadEmailSetup();
}

// gate Pin
void Security::ClearGatePin(){
  logtrace_print(T_ALL,__func__);
  ClearSecurityPin(PIN_TYPE_GATE);
}
char* Security::GetCurrentGatePin(){
  logtrace_print(T_ALL,__func__);
  return GetCurrentPin(PIN_TYPE_GATE);
}
void Security::StoreGatePin(char* buf, uint8_t len){
  logtrace_print(T_ALL,__func__);
  StoreSecurityPin(buf,len,PIN_TYPE_GATE);
}
bool Security::ValidateGatePin(char* buf, uint8_t len){
  logtrace_print(T_ALL,__func__);
  return ValidatePin( buf,len,PIN_TYPE_GATE);
}

// Master Pin
void Security::ClearMasterPin(){
  logtrace_print(T_ALL,__func__);
  ClearSecurityPin(PIN_TYPE_MASTER);
}
char* Security::GetCurrentMasterPin(){
  logtrace_print(T_ALL,__func__);
  return GetCurrentPin(PIN_TYPE_MASTER);
}
void Security::StoreMasterPin(char* buf, uint8_t len){
  logtrace_print(T_ALL,__func__);
  StoreSecurityPin(buf,len,PIN_TYPE_MASTER);
}
bool Security::ValidateMasterPin(char* buf, uint8_t len){
  logtrace_print(T_ALL,__func__);
  return ValidatePin( buf,len,PIN_TYPE_MASTER);
}


 /*
 * Email Functions
 */

void Security::ReadEmailSetup ( ){
  logtrace_print(T_ALL,__func__);
  
  myStorage.storage_read( EMAIL_ADDRESS, &emailSetup, sizeof(EMAIL_SETUP) );
  if( 0 != strcmp(emailSetup.sign, EMAIL_SIGN) ){
   logtrace_print(T_DEBUG,"Invalid EmailSetup Entry found");
  }
  else{
     logtrace_print(T_ALL,"Valid EmailSetup Found: %s / %s / %d", emailSetup.pEmail, emailSetup.pPass, emailSetup.bIsActive);
  }
  
}

// erase Email
void Security::ClearEmailSetup(){
  logtrace_print(T_ALL,__func__);
  myStorage.storage_erase(EMAIL_ADDRESS,sizeof(EMAIL_SETUP));
  ReadEmailSetup();
}

// store Email
void Security::StoreEmailSetup(  uint8_t bActive, char* user, char* pass ){
  logtrace_print(T_ALL,__func__);

  if( NULL != user && NULL != pass )
    logtrace_print(T_DEBUG,"[Security::StoreEmailSetup]: %d, %s, %s",bActive, user,pass);
  else
    logtrace_print(T_DEBUG,"[Security::StoreEmailSetup]: %d",bActive );
 
  
  memset(&emailSetup,0,sizeof(EMAIL_SETUP));
  emailSetup.bIsActive = bActive;
  if( NULL != user && NULL != pass ){
    strcpy( emailSetup.pEmail, user );
    strcpy( emailSetup.pPass, pass );
  }
  memcpy( emailSetup.sign,EMAIL_SIGN,MAX_SIGN);
  myStorage.storage_write( EMAIL_ADDRESS, &emailSetup, sizeof( EMAIL_SETUP ));
}

// get UserName
char* Security::GetEmailUserName(){
  logtrace_print(T_ALL,__func__);
  return emailSetup.pEmail;
}

// get Password
char* Security::GetEmailPassword(){
  logtrace_print(T_ALL,__func__);
  return emailSetup.pPass;
}

// get ActiveState
uint8_t  Security::GetEmailActiveState(){
  logtrace_print(T_ALL,__func__);
  return emailSetup.bIsActive;
}

void  Security::EnableEmailAuth(uint8_t bActive ){
  logtrace_print(T_ALL,__func__);
  emailSetup.bIsActive = bActive;
  StoreEmailSetup(bActive);
}

void Security::GeneratePIN( char* pin ){
  logtrace_print(T_ALL,__func__);

  uint8_t pinSize = 0;
  uint8_t i = 0;
  memset(pin,0,MAX_PIN);

  randomSeed(millis());
  pinSize = random(4,10);
  for( i = 0; i< pinSize; i++)
    pin[i] = random(48,57);

  pin[i] = '#';
  logtrace_print(T_DEBUG,"Generated Pin: %s", pin);
}

void Security::EraseFlash(){
  logtrace_print(T_ALL,__func__);
  myStorage.storage_format();
  logtrace_print(T_INFO,"Flash Memory Formated\n");
}


