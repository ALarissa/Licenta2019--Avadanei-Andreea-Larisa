#include "uart_inc.h"

void get_message( String* message ){
 *message = Serial.readStringUntil('\n');
}

void send_ack( char* ack_type ){
  Serial.println(ack_type);
}


bool wait_for_next_message( char* answer, uint32_t timeout, char *message_content ){
  uint32_t ts = millis();
  String  tmp_String;
  while (!Serial.available())
  {
    if(millis() > (ts + timeout)) {
      return false;
    }
  }
  tmp_String = Serial.readStringUntil('\n');
  if (answer && tmp_String.indexOf(answer) == -1) 
    return false;

  strcpy(message_content,tmp_String.substring(5,tmp_String.length()).c_str());
  return true;
}

void pars_message( String message, char* message_header, char* message_content ){
  strcpy(message_header,message.substring(0,5).c_str());
  strcpy(message_content,message.substring(5,message.length()).c_str());
}

