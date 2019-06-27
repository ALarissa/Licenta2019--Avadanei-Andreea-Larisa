#include "LogTrace.h"
#include "MailSender.h"

MailSender* MailSender::instance = 0;

MailSender* MailSender::Instance(){
  if(0 == instance)
    instance = new MailSender;
  return instance;
}

bool MailSender::GetSMTPAnswer(WiFiClientSecure &SMTP_Client, const String &answer, uint16_t timeOut){
  logtrace_print(T_ALL,__func__);
  uint32_t ts = millis();
  while (!SMTP_Client.available())
  {
    if(millis() > (ts + timeOut)) {
      last_error = "SMTP Response TIMEOUT!";
      return false;
    }
  }
  serverAnswer = SMTP_Client.readStringUntil('\n');
  logtrace_print(T_DEBUG,serverAnswer.c_str());
  if (answer && serverAnswer.indexOf(answer) == -1) 
    return false;
  return true;
}

void MailSender::Init(){
  logtrace_print(T_ALL,__func__);
  bInitDone = true;
}

char* MailSender::error_translator( const char* answer_expected ){
  if( "220" == answer_expected )
    return "Connection error";
  if( "250" == answer_expected )  
    return "identification error - Sending message error";
  if( "235" == answer_expected )  
    return "SMTP AUTH error";
  if( "354" == answer_expected )  
    return "SMTP DATA error";
  if( "221" == answer_expected )  
    return "SMTP QUIT error";
}

bool MailSender::SendToSMTP(WiFiClientSecure &SMTP_Client,  const String message ){
  logtrace_print(T_ALL,__func__);
  logtrace_print(T_DEBUG,"sending: %s",message.c_str());
  SMTP_Client.println(message);
}


bool MailSender::SendToSMTP_W(WiFiClientSecure &SMTP_Client,  const String message, const String &answer_expected, uint16_t timeOut ){
  logtrace_print(T_ALL,__func__);
  
  logtrace_print(T_DEBUG,"sending: %s",message.c_str());
  SMTP_Client.println(message);

  logtrace_print(T_DEBUG,"expecting: %s",answer_expected.c_str());
  if(!GetSMTPAnswer(SMTP_Client, answer_expected)){
    last_error = error_translator(answer_expected.c_str());
    logtrace_print(T_DEBUG,last_error );
  }
}

bool MailSender::Send( const String &message ){
  logtrace_print(T_ALL,__func__);
  return Send(EMAIL_DEST,message);
}


bool MailSender::Send( const String &dest, const String &message ){
  logtrace_print(T_ALL,__func__);

  
  
  WiFiClientSecure SMTP_Client;
  SMTP_Client.setInsecure();
  logtrace_print(T_ALL,"Connecting to: %s",SMTP_ADDRESS);
  
  // try to connect to SMTP SERVER
  if(!SMTP_Client.connect(SMTP_ADDRESS,SMTP_PORT)){
    logtrace_print(T_DEBUG,"Could not connect to mail server");
    last_error = "Could not connect to mail server";
    return false;
  }
  
  //Await for SMTP Response
  if(!GetSMTPAnswer(SMTP_Client, "220")){
    logtrace_print(T_DEBUG,"Connection error");
    last_error = "Connection error";
  }

  //send HELO
  if(!SendToSMTP_W(SMTP_Client, "HELO Friend","250"))
    return false;

  //send AUTH LOGIN
  if(!SendToSMTP_W(SMTP_Client, "AUTH LOGIN"))
    return false;

  //send UserName
  if(!SendToSMTP_W(SMTP_Client, EMAIL_USER))
    return false;
  
  //send Password
  if(!SendToSMTP_W(SMTP_Client, EMAIL_PASS,"235"))
    return false;

  if(!SendToSMTP_W(SMTP_Client, "MAIL FROM: <" + String(EMAIL_FROM) + '>'))
    return false;

  if(!SendToSMTP_W(SMTP_Client, "RCPT TO: <" + dest + '>'))
    return false;

  if(!SendToSMTP_W(SMTP_Client, "DATA"))
    return false;

  SendToSMTP(SMTP_Client, "From: <" + String(EMAIL_FROM) + '>');
  SendToSMTP(SMTP_Client, "To: <" + dest + '>');
  
  SendToSMTP(SMTP_Client, "Subject: " + String(EMAIL_SBJ));
  
  
  SendToSMTP(SMTP_Client, "Mime-Version: 1.0");
  SendToSMTP(SMTP_Client, "Content-Type: text/html; charset=\"UTF-8\"");
  SendToSMTP(SMTP_Client, "Content-Transfer-Encoding: 7bit");
  SendToSMTP(SMTP_Client, "");
  String body = "<!DOCTYPE html><html lang=\"en\">" + message + "</html>";
  SendToSMTP(SMTP_Client, body);
  if(!SendToSMTP_W(SMTP_Client, ".","250"))
      return false;
      
  if(!SendToSMTP_W(SMTP_Client, "QUIT","221"));
     return false;
     
}

String MailSender::getLastAnswer(){
  logtrace_print(T_ALL,__func__);
  return serverAnswer;
}

char* MailSender::getError(){
  logtrace_print(T_ALL,__func__);
  return last_error;
}


void MailSender::Set_SMTP_PORT( uint16_t uPort){
  SMTP_PORT = uPort;
}
void MailSender::Set_SMTP_ADDRESS( const char* ccAddress ){
  SMTP_ADDRESS = new char[strlen(ccAddress)];
  memset(SMTP_ADDRESS,0,strlen(ccAddress));
  memcpy(SMTP_ADDRESS,ccAddress,strlen(ccAddress)-1);
}
void MailSender::Set_Email_User( const char* ccUser ){
  EMAIL_USER = new char[strlen(ccUser)];
  memset(EMAIL_USER,0,strlen(ccUser));
  memcpy(EMAIL_USER,ccUser,strlen(ccUser)-1);
}
void MailSender::Set_Email_Pass( const char* ccPass ){
  EMAIL_PASS = new char[strlen(ccPass)];
  memset(EMAIL_PASS,0,strlen(ccPass));
  memcpy(EMAIL_PASS,ccPass,strlen(ccPass)-1);
}
void MailSender::Set_Email_From( const char* ccEFrom ){
  EMAIL_FROM = new char[strlen(ccEFrom)];
  memset(EMAIL_FROM,0,strlen(ccEFrom));
  memcpy(EMAIL_FROM,ccEFrom,strlen(ccEFrom)-1);
}

void MailSender::Set_Email_Dest( const char* ccETo ){
  EMAIL_DEST = new char[strlen(ccETo)];
  memset(EMAIL_DEST,0,strlen(ccETo));
  memcpy(EMAIL_DEST,ccETo,strlen(ccETo)-1);
}

bool  MailSender::IsInit(){
  return bInitDone;
}

