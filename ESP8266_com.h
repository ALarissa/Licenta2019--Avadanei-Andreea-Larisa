#ifndef ESP8266_COM
#define ESP8266_COM

#define SERIAL1_BAUD      115200 // Serial baud rate

#define WiFi_SID    "15jun2019"
#define WiFi_PASS   "223344asd"

#define    SMTP_PORT    "465"
#define    SMTP_ADDRESS "smtp.gmail.com"
#define    EMAIL_USER   "bGFyaXNhLnNmaXJuYWNpdWNAZ21haWwuY29t"//"YXZhZGFuZWlsYXJpc2FAZ21haWwuY29t" 
#define    EMAIL_PASS   "U3R5a3VsMjI3N0A="//"U3R5a3VsMDMyOA=="
#define    EMAIL_FROM   "larisa.sfirnaciuc@gmail.com"
#define    EMAIL_DEST   "larisa.sfirnaciuc@gmail.com"

void  ResetWiFi();

bool  InitWiFi();
bool  InitSMTP();

bool  SendEmail(char* pin);


#endif
