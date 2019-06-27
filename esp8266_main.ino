#include "LogTrace.h"
#include "WiFiConn.h"
#include "MailSender.h"
#include "uart_inc.h"


MailSender  *myMailSender;
WiFiConn    myWiFi;

// Init WiFi Settings
// Blocking Point
bool  InitWiFi(){
  logtrace_print(T_ALL,__func__);
  // wait for wifi parameters
  char  msg_content[MAX_MSG_SIZE];
  memset(msg_content,0,MAX_MSG_SIZE);
  if( !wait_for_next_message( M_WIFI_INIT, 100000, msg_content) ){
    return false;
  }
  send_ack(M_ACK);
  
  char  mSID[100];
  char  mPass[100];
    
  memset(mSID,0,100);
  memset(mPass,0,100);

  logtrace_print(T_DEBUG,"waiting for SID");
  
  if( !wait_for_next_message( M_WIFI_SID, 100000, mSID) ){
    send_ack(M_NOK);  
    return false;
  }
  logtrace_print(T_ALL,"SID: %s", mSID);
  send_ack(M_ACK);

  logtrace_print(T_DEBUG,"waiting for Password");
  if( !wait_for_next_message( M_WIFI_PASS, 100000, mPass) ){
    send_ack(M_NOK);
    return false;
  }
  logtrace_print(T_ALL,"PASS: %s", mPass);
  send_ack(M_ACK);
  myWiFi.Init(mSID,mPass,10000);

  logtrace_print(T_DEBUG,"Connecting to WiFi");
  bool bConnected = false;
  uint8_t  uRetry = 0;
  while(!bConnected && uRetry < 5 ){
    bConnected = myWiFi.Connect();
    uRetry++;
  }

  if( !bConnected ){
    send_ack(M_NOK);
    return false;
  }
  
  return true;
}

// init Email Settings
// Blocking Point

bool InitEmailSettings(){
  logtrace_print(T_ALL,__func__);

  myMailSender = MailSender::Instance();

  // Init
  char  msg_content[MAX_MSG_SIZE];
  memset(msg_content,0,MAX_MSG_SIZE);
  if( !wait_for_next_message( M_SMTP_INIT, 100000, msg_content) ){
    send_ack(M_NOK);
    return false;
  }
  send_ack(M_ACK);

  // smtp_server
  logtrace_print(T_DEBUG,"waiting for STMP_SERVER");
  memset(msg_content,0,MAX_MSG_SIZE);
  if( !wait_for_next_message( M_SMTP_SERVER, 100000, msg_content) ){
    send_ack(M_NOK);
    return false;
  }
  logtrace_print(T_ALL,"SMTP_ADDRRESS: %s", msg_content);
  myMailSender->Set_SMTP_ADDRESS(msg_content);
  send_ack(M_ACK);

  // smtp_port
  logtrace_print(T_DEBUG,"waiting for STMP_PORT");
  memset(msg_content,0,MAX_MSG_SIZE);
  if( !wait_for_next_message( M_SMTP_PORT, 100000, msg_content) ){
    send_ack(M_NOK);
    return false;
  }
  myMailSender->Set_SMTP_PORT(atoi(msg_content));
  logtrace_print(T_ALL,"SMTP_PORT: %s", msg_content);
  send_ack(M_ACK);

  // user_name
  logtrace_print(T_DEBUG,"waiting for EMAIL USER");
  memset(msg_content,0,MAX_MSG_SIZE);
  if( !wait_for_next_message( M_USERNAME, 100000, msg_content) ){
    send_ack(M_NOK);
    return false;
  }
  myMailSender->Set_Email_User(msg_content);
  logtrace_print(T_ALL,"EMAIL_USER: %s", msg_content);
  send_ack(M_ACK);

  // password
  logtrace_print(T_DEBUG,"waiting for EMAIL PASS");
  memset(msg_content,0,MAX_MSG_SIZE);
  if( !wait_for_next_message( M_PASSWORD, 100000, msg_content) ){
    send_ack(M_NOK);
    return false;
  }
  myMailSender->Set_Email_Pass(msg_content);
  logtrace_print(T_ALL,"EMAIL_PASS: %s", msg_content);
  send_ack(M_ACK);

  // email from
  logtrace_print(T_DEBUG,"waiting for EMAIL FROM");
  memset(msg_content,0,MAX_MSG_SIZE);
  if( !wait_for_next_message( M_MAIL_SENDER, 100000, msg_content) ){
    send_ack(M_NOK);
    return false;
  }
  myMailSender->Set_Email_From(msg_content);
  logtrace_print(T_ALL,"EMAIL_FROM: %s", msg_content);
  send_ack(M_ACK);

  // email to
  logtrace_print(T_DEBUG,"waiting for EMAIL DEST");
  memset(msg_content,0,MAX_MSG_SIZE);
  if( !wait_for_next_message( M_MAIL_DEST, 100000, msg_content) ){
    send_ack(M_NOK);
    return false;
  }
  myMailSender->Set_Email_Dest(msg_content);
  logtrace_print(T_ALL,"EMAIL_TO: %s", msg_content);
  send_ack(M_ACK);

  myMailSender->Init();
  
  return true;
}


void setup() {

  if(!InitWiFi())
    InitWiFi();

  if(!InitEmailSettings())
    InitEmailSettings();
  
}

void loop() {

  bool bSetupDone = false;
  
  while(!bSetupDone){
    if( myWiFi.IsInit() && myMailSender->IsInit())
      bSetupDone= true;
  }

 
 while (Serial.available() > 0) {
        char   expected_message[5];
        String myString;
        char   message_header[5];
        char   message_content[250];  

        // get message
        get_message(&myString);
        logtrace_print(T_DEBUG,myString.c_str());

        // pars message
        pars_message( myString, message_header, message_content );
        if( strcmp(message_header, M_SEND_MAIL) ){
          send_ack(M_UNEXPECTED);
        }
        else{
          if( !wait_for_next_message( M_PIN, 10000, message_content) ){
            send_ack(M_TIMEOUT);
          }
          else{
            send_ack(M_ACK);
            myMailSender->Send(message_content);
          }
        }
        
        
        
   }

}
