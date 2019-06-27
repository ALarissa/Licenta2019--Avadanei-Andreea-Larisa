#include "ESP8266_com.h"
#include "uart_inc.h"
#include "LogTrace.h"

void ResetWiFi(){
  pinMode(A0,OUTPUT);
  digitalWrite(A0, LOW);
  delay(100);
  digitalWrite(A0, HIGH);
}

bool  InitWiFi(){
  logtrace_print(T_ALL,__func__);

  logtrace_print(T_DEBUG,"SEND WIFI_INIT");
  send_message(M_WIFI_INIT);
  if( !wait_for_next_message(M_ACK,10000)){
    logtrace_print(T_ERROR,"NO ACK FOR WIFI_INIT");
    return false;
  }

  logtrace_print(T_DEBUG,"SEND WIFI_SID");
  send_message(M_WIFI_SID,WiFi_SID);
  if( !wait_for_next_message(M_ACK,10000)){
    logtrace_print(T_ERROR,"NO ACK FOR WIFI_SID");
    return false;
  }

  logtrace_print(T_DEBUG,"SEND WIFI_PASS");
  send_message(M_WIFI_PASS,WiFi_PASS);
  if( !wait_for_next_message(M_ACK,10000)){
    logtrace_print(T_ERROR,"NO ACK FOR WIFI_PASS");
    return false;
  }

  //logtrace_print(T_DEBUG,"SEND WIFI_CONNECT");
  //send_message(M_WIFI_CONNECT);
  //if( !wait_for_next_message(M_ACK,10000)){
  //  logtrace_print(T_ERROR,"NO ACK FOR WIFI_CONNECT");
  //  return false;
  //}
}

bool  InitSMTP(){
  logtrace_print(T_ALL,__func__);

  logtrace_print(T_DEBUG,"SEND SMTP_INIT");
  send_message(M_SMTP_INIT);
  if( !wait_for_next_message(M_ACK,10000)){
    logtrace_print(T_ERROR,"NO ACK FOR SMTP_INIT");
    return false;
  }

  //delay(1000);
  logtrace_print(T_DEBUG,"SEND SMTP_ADDRESS");
  send_message(M_SMTP_SERVER,SMTP_ADDRESS);
  if( !wait_for_next_message(M_ACK,10000)){
    logtrace_print(T_ERROR,"NO ACK FOR SMTP_ADDRESS");
    return false;
  }

  //delay(1000);
  logtrace_print(T_DEBUG,"SEND SMTP_PORT");
  send_message(M_SMTP_PORT,SMTP_PORT);
  if( !wait_for_next_message(M_ACK,10000)){
    logtrace_print(T_ERROR,"NO ACK FOR SMTP_PORT");
    return false;
  }

  //delay(1000);
  logtrace_print(T_DEBUG,"SEND EMAIL_USER");
  send_message(M_USERNAME,EMAIL_USER);
  if( !wait_for_next_message(M_ACK,10000)){
    logtrace_print(T_ERROR,"NO ACK FOR EMAIL_USER");
    return false;
  }

  //delay(1000);
  logtrace_print(T_DEBUG,"SEND EMAIL_PASS");
  send_message(M_PASSWORD,EMAIL_PASS);
  if( !wait_for_next_message(M_ACK,10000)){
    logtrace_print(T_ERROR,"NO ACK FOR EMAIL_PASS");
    return false;
  }

  //delay(1000);
  logtrace_print(T_DEBUG,"SEND EMAIL_FROM");
  send_message(M_MAIL_SENDER,EMAIL_FROM);
  if( !wait_for_next_message(M_ACK,10000)){
    logtrace_print(T_ERROR,"NO ACK FOR EMAIL_FROM");
    return false;
  }

  //delay(1000);
  logtrace_print(T_DEBUG,"SEND EMAIL_DEST");
  send_message(M_MAIL_DEST,EMAIL_DEST);
  if( !wait_for_next_message(M_ACK,10000)){
    logtrace_print(T_ERROR,"NO ACK FOR EMAIL_DEST");
    return false;
  }
}


bool  SendEmail(char* pin){
  logtrace_print(T_ALL,__func__);

    //delay(1000);
  logtrace_print(T_DEBUG,"SEND EMAIL REQUEST");
  send_message(M_SEND_MAIL);
  logtrace_print(T_DEBUG,"SEND EMAIL CONTENT");
  send_message(M_PIN,pin);
  if( !wait_for_next_message(M_ACK,10000)){
    logtrace_print(T_ERROR,"NO ACK FOR SEND EMAIL CONTENT");
    return false;
  }
}

