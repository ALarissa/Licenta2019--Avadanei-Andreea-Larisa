#include <WiFiClientSecure.h>


class MailSender{
  private:
    //WiFiClientSecure SMTP_Client;
    uint16_t   SMTP_PORT = 465;
    char*     SMTP_ADDRESS;// = "smtp.gmail.com";
    char*     EMAIL_SBJ    = "[GATE_ACCESS]";
    char*     EMAIL_USER;// 
    char*     EMAIL_PASS;// 
    char*     EMAIL_FROM;// 
    char*     EMAIL_DEST;
    String    serverAnswer;
    char*     last_error;
    static MailSender *instance;
    
    bool GetSMTPAnswer(WiFiClientSecure &SMTP_Client, const String &answer = "", uint16_t timeOut = 10000);
    bool SendToSMTP_W( WiFiClientSecure &SMTP_Client, const String message, const String &answer_expected = "", uint16_t timeOut = 10000 );
    bool SendToSMTP( WiFiClientSecure &SMTP_Client, const String message );
    char* error_translator( const char* answer_expected);

    bool bInitDone;  
  
  public:
  
    //MailSender();
    //~MailSender();
    void Init(  );
    bool Send( const String &dest, const String &message );
    bool Send( const String &message );
    String getLastAnswer();
    char* getError();
    static MailSender* Instance();

    void Set_SMTP_PORT( uint16_t uPort);
    void Set_SMTP_ADDRESS( const char* ccAddress );
    void Set_Email_User( const char* ccUser );
    void Set_Email_Pass( const char* ccPass );
    void Set_Email_From( const char* ccEFrom );
    void Set_Email_Dest( const char* ccETo );

    bool IsInit();
};

