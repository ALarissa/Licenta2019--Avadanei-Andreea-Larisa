// include all libs here

#include "GUI.h"
#include "LogTrace.h"
#include "ESP8266_com.h"

// defines


gUI *myGui;

void setup(){
  Serial1.begin(SERIAL1_BAUD);
  myGui = new gUI();

  ResetWiFi();
  delay(100);
  
  InitWiFi();
  while( Serial1.available()){
    logtrace_print(T_DEBUG,Serial1.readStringUntil('\n').c_str());
  }
  //delay(5000);
  InitSMTP();
}


// main loop
void loop(){
    //SendEmail();
    myGui->DrawMainMenu();
}

/*
void Blink(byte PIN, int DELAY_MS, byte loops)
{
  pinMode(PIN, OUTPUT);
  while (loops--)
  {
    digitalWrite(PIN,HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN,LOW);
    delay(DELAY_MS);  
  }
}
*/
