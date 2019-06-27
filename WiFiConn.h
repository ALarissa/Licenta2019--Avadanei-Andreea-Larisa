#ifndef _WiFiCONN_
#define _WiFiCONN_

#include <ESP8266WiFi.h>

#define WiFi_MAX_WAITING_COUNTS 30
#define WiFi_CONN_WAIT_DELAY 200

#define CONN_OFF 0
#define CONN_ON 1
#define CONN_TIMEOOUT 2

class WiFiConn{
  private:
    char* ssID;
    char* password;
    uint8_t conn_status;
    uint16_t retry_time;
    bool bInitDone;
  public:
    WiFiConn();
    ~WiFiConn();
    
    void Init( const char* ID, const char* pass, uint16_t timeout );
    bool Connect();
    void Disconnect();
    uint8_t GetStatus();
    const char* GetIP();
    bool IsInit(); 
};

#endif
