#ifndef _SECURITY_
#define _SECURITY_

#include "Storage.h"
#include <stdint.h>

#define PIN_ADDRESS1      0x1000    //MasterPin
#define PIN_ADDRESS2      0x2000    //GatePin
#define EMAIL_ADDRESS     0x3000
#define PIN_SIGN         "0XBADD"
#define EMAIL_SIGN       "0XFADD"
#define MAX_PIN          11
#define MAX_SIGN          7
#define MAX_EMAIL        255

#define PIN_TYPE_MASTER   0
#define PIN_TYPE_GATE     1

typedef struct PIN_{
  int u8PinSize;
  char    pin[MAX_PIN];
  char    sign[MAX_SIGN]; 
} PIN;


typedef struct EMAIL_{
  uint8_t  bIsActive;
  char  pEmail[MAX_EMAIL];
  char  pPass[MAX_EMAIL];
  char  sign[MAX_SIGN];
}EMAIL_SETUP;


class Security{
  private:
    uint32_t pinAddress;
    PIN     gatePin;
    PIN     masterPin;
    EMAIL_SETUP emailSetup;
    
    Storage myStorage;
    void ReadSecurityPin ( uint8_t pinType );
    void ReadGatePin();
    void ReadMasterPin();

    void StoreSecurityPin( char* buf, uint8_t len, uint8_t pinType );
    void ClearSecurityPin( uint8_t pinType );
    bool ValidatePin( char* buf, uint8_t len, uint8_t pinType );
    void PrintCurrentPin(uint8_t pinType);
    char* GetCurrentPin(uint8_t pinType);
    
  public:
    void Init();
    
    void GetDataFromFlash( uint8_t address, char** buf, uint8_t len );
    void StoreDataInFlash( uint8_t address, char* buf, uint8_t len );
    void EraseFlash();
  public:
    void ClearGatePin();
    char* GetCurrentGatePin();
    void StoreGatePin(char* buf, uint8_t len);
    bool ValidateGatePin(char* buf, uint8_t len);
    
    void ClearMasterPin();
    char* GetCurrentMasterPin();
    void StoreMasterPin(char* buf, uint8_t len);
    bool ValidateMasterPin(char* buf, uint8_t len);

    void  ReadEmailSetup();
    void  StoreEmailSetup( uint8_t bActive, char* user = NULL, char* pass = NULL );
    void  EnableEmailAuth(uint8_t bActive );
    void  ClearEmailSetup();
    char* GetEmailUserName();
    char* GetEmailPassword();
    uint8_t  GetEmailActiveState();

    void GeneratePIN( char* pin );
    
};

#endif
