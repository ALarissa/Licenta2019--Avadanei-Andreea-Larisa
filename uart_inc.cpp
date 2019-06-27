#include "uart_inc.h"
#include "LogTrace.h"

void get_message( String* message ){
  logtrace_print(T_ALL,__func__);
 *message = Serial1.readStringUntil('\n');
}

void send_ack( char* ack_type ){
  logtrace_print(T_ALL,__func__);
  Serial1.println(ack_type);
}

bool wait_for_next_message( char* answer, uint32_t timeout, char *message_content ){
  logtrace_print(T_ALL,__func__);
  bool bAnswer = false;
  char cAnswer[6];
  bool waiting = true;
  
  uint32_t ts = millis();
  String  tmp_String;

  do{
    //logtrace_print(T_ALL,"wait_for_next_message:1");
    while (!Serial1.available())
    {
      //logtrace_print(T_ALL,"wait_for_next_message:3");
      if(millis() > (ts + timeout)) {
        waiting = false;
        bAnswer = false;
      }
    }
  
    while( Serial1.available()){
      //logtrace_print(T_ALL,"wait_for_next_message:2");
      memset( cAnswer,0,6);
      tmp_String = Serial1.readStringUntil('\n');
      logtrace_print(T_DEBUG,tmp_String.c_str());
      strcpy(cAnswer,tmp_String.substring(0,5).c_str());

      if( strcmp(answer, cAnswer) == 0 ){
        bAnswer = true;
        waiting = false;
        strcpy(message_content,tmp_String.substring(5,tmp_String.length()).c_str());
        break;
      }
      else{
        if( cAnswer[4] == ']' ){
          bAnswer = false;
          waiting = false;
        }
          
      }
    }
  }while(waiting);
  return bAnswer;
}

bool wait_for_next_message( char* answer, uint32_t timeout ){
  logtrace_print(T_ALL,__func__);
   bool bAnswer = false;
  char cAnswer[6];
  bool waiting = true;
  
  uint32_t ts = millis();
  String  tmp_String;

  do{
    //logtrace_print(T_ALL,"wait_for_next_message:1");
    memset( cAnswer,0,6);
    while (!Serial1.available())
    {
     // logtrace_print(T_ALL,"wait_for_next_message:3");
      if(millis() > (ts + timeout)) {
        waiting = false;
        bAnswer = false;
      }
    }
  
    while( Serial1.available()){
      //logtrace_print(T_ALL,"wait_for_next_message:2");
      memset( cAnswer,0,6);
      tmp_String = Serial1.readString();
      logtrace_print(T_DEBUG,tmp_String.c_str());
      strcpy(cAnswer,tmp_String.substring(0,5).c_str());

      if( strcmp(answer, cAnswer) == 0 ){
        bAnswer = true;
        waiting = false;
        break;
      }
      else{
        if( cAnswer[4] == ']' ){
          bAnswer = false;
          waiting = false;
        }
          
      }
    }
  }while(waiting);
  return bAnswer;
}

void pars_message( String message, char* message_header, char* message_content ){
  logtrace_print(T_ALL,__func__);
  strcpy(message_header,message.substring(0,5).c_str());
  strcpy(message_content,message.substring(5,message.length()).c_str());
}

void send_message( char* header, char* message_content ){
  logtrace_print(T_ALL,__func__);
    char* tmp_buffer = new char( strlen(header) + strlen(message_content) + 1 );
    memset(tmp_buffer,0, strlen(header) + strlen(message_content) + 1 );
    memcpy(tmp_buffer,header,strlen(header));
    memcpy(tmp_buffer + strlen(header), message_content, strlen(message_content) );
    logtrace_print(T_DEBUG,"Sending message: %s",tmp_buffer);
    Serial1.println(tmp_buffer);

    delete tmp_buffer;
}

void send_message( char* header ){
  logtrace_print(T_ALL,__func__);
    Serial1.println(header);
}
