#include "WiFiConn.h"
#include "LogTrace.h"

//Constructor
WiFiConn::WiFiConn(){
    logtrace_print(T_ALL,__func__);  
    ssID = NULL;
    password = NULL;
    conn_status = CONN_OFF;
    retry_time = 0;
    bInitDone = false;
}

//destructor
WiFiConn::~WiFiConn(){
  logtrace_print(T_ALL,__func__);
  if( NULL != ssID ){
    delete ssID;
    ssID = NULL;
  }
  if( NULL != password ){
    delete password;
    password = NULL;
  }
  conn_status = CONN_OFF;
  retry_time = 0;
  bInitDone = false;
}

void WiFiConn::Init(const char* ID, const char* pass, uint16_t timeout){
  logtrace_print(T_ALL,__func__);
  if( NULL == ID || NULL == pass )
    return;

  ssID = new char[strlen(ID)];
  memset(ssID,0,strlen(ID));
  logtrace_print(T_ALL,"%s, %d",ID,strlen(ID));
  //strcpy(ssID, ID);
  memcpy(ssID,ID,strlen(ID)-1);
  
  password = new char[strlen(pass)];
  memset(password,0,strlen(pass));
  logtrace_print(T_ALL,"%s, %d",pass,strlen(pass));
  //strcpy(password,pass);
  memcpy(password,pass,strlen(pass)-1);
  
  retry_time = timeout;
  bInitDone = true;
}

bool WiFiConn::Connect(){
   logtrace_print(T_ALL,__func__);
   uint8_t counter = 0;
   WiFi.begin(ssID,password);
   logtrace_print(T_ALL,"ssID: %s\nPass: %s",ssID,password );
   
   logtrace_print(T_DEBUG,"Trying to connect to: %s",ssID );
   while(WiFi.status() != WL_CONNECTED && counter++ < WiFi_MAX_WAITING_COUNTS ){
    delay(WiFi_CONN_WAIT_DELAY);
    logtrace_print(T_DEBUG," * ");
   }

   if( counter > WiFi_MAX_WAITING_COUNTS ){
    logtrace_print(T_INFO,"Unable to connect to %s\n",ssID);
    conn_status = CONN_TIMEOOUT;
    return false;
   }
   logtrace_print(T_INFO,"Connected to %s\n",ssID);
   conn_status = CONN_ON;

   IPAddress ip = WiFi.localIP();
   logtrace_print(T_ALL,ip.toString().c_str());
   
   return true;
}

void WiFiConn::Disconnect(){
  logtrace_print(T_ALL,__func__);
  WiFi.disconnect();
  conn_status = CONN_OFF;
}

uint8_t WiFiConn::GetStatus(){
  logtrace_print(T_ALL,__func__);
  return conn_status;
}

const char* WiFiConn::GetIP(){
  logtrace_print(T_ALL,__func__);
  IPAddress ip = WiFi.localIP();
  return ip.toString().c_str();
}

bool  WiFiConn::IsInit(){
  return bInitDone;
}

