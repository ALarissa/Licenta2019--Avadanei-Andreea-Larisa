#include "GUI.h"
#include "LogTrace.h"
#include "ESP8266_com.h"

// constructor
gUI::gUI(){
  myKeypad = new Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); // folosit petru a citi ce se apasa pe keypad
  mySecurity.Init();
  myDisplay = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  myDisplay->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  myDisplay->display();
  myDisplay->cp437(true);
  myDisplay->setTextColor(WHITE);
  myDisplay->setTextSize(1);      // Normal 1:1 pixel scale
}

//destructor
gUI::~gUI(){
//  if( NULL != myKeypad )
//    delete myKeypad;
  if( NULL != myDisplay );
    //delete myDisplay;
}

void gUI::DrawMainMenu(){
  logtrace_print(T_ALL,__func__);
  myDisplay->clearDisplay();
  myDisplay->setCursor(0, 0);     // Start at top-left corner
  myDisplay->println(F("Please enter Pin:"));
  myDisplay->display();
  
  char key = '0';
  uint8_t index = 0;
  char pin[MAX_PIN];
  memset(pin,0,MAX_PIN);
  
  while(key!='#'){
    key = myKeypad->getKey();
    if(key){
      if(index >= 9){
        pin[index]='#';
        break;
      }
      else{
        pin[index] = key;
        index++;
        myDisplay->print(key);  
        myDisplay->display();
      }
    }
  }

  if( mySecurity.ValidateMasterPin(pin,index) )
    DrawSetupMainMenu();
  else{
     if( mySecurity.ValidateGatePin(pin,index) ){
      DrawGateOpen();
      DrawMainMenu();
     }
     else{
      DrawMainMenu();
     }
 }
}


void gUI::DrawSetupMainMenu(){
  logtrace_print(T_ALL,__func__);
  myDisplay->clearDisplay();
  myDisplay->setCursor(0, 0);     // Start at top-left corner
  myDisplay->println(F("1. Master Pin Setup")); 
  myDisplay->println(F("2. Gate Pin Setup")); 
  myDisplay->println(F("3. Email Setup")); 
  myDisplay->println(F("4. Print Current Config"));  
  myDisplay->display();

  char key = '0';
  while(1){
    while( key == '0' ){
      key = myKeypad->getKey();
      if( key == '1' )
        DrawPinMainMenu(PIN_TYPE_MASTER);
      if( key == '2' )
        DrawPinMainMenu(PIN_TYPE_GATE);
      if( key == '3' )
        DrawEmailMainMenu();
      //if( key == '4' )
        //DrawCurrentConfig()
      key = '0';
    }
  }
  
}


// draw SetupMainMenu
void gUI::DrawPinMainMenu(uint8_t pinType ){
  logtrace_print(T_ALL,__func__);
  myDisplay->clearDisplay();
  myDisplay->setCursor(0, 0);     // Start at top-left corner
  myDisplay->println(F("1. Setup a new Pin")); 
  myDisplay->println(F("2. Print current Pin")); 
  myDisplay->println(F("3. Remove current Pin")); 
  myDisplay->println(F("9. Main menu"));  
  myDisplay->display();

  char key = '0';
  while(1){
    while( key == '0' ){
      key = myKeypad->getKey();
      if( key == '1' )
        DrawSetPinMenu(pinType);
      if( key == '2' )
        DrawDisplayCurrentPinMenu(pinType);
      if( key == '9' )
        DrawMainMenu();
      key = '0';
    }
  }
}

// draw DisplayCurrentPinMenu()
void gUI::DrawDisplayCurrentPinMenu(uint8_t pinType){
  logtrace_print(T_ALL,__func__);
  myDisplay->clearDisplay();
  myDisplay->setCursor(0, 0);     // Start at top-left corner
  myDisplay->println(F("Current Pin:"));
  if( PIN_TYPE_MASTER == pinType )
    myDisplay->println(mySecurity.GetCurrentMasterPin());
  else
    myDisplay->println(mySecurity.GetCurrentGatePin());
  myDisplay->display();
  delay(2000);
  DrawPinMainMenu(pinType);
}

// draw setPinMenu
void gUI::DrawSetPinMenu(uint8_t pinType){
  logtrace_print(T_ALL,__func__);
  myDisplay->clearDisplay();
  myDisplay->setCursor(0, 0);     // Start at top-left corner
  myDisplay->println(F("9 chars followed by #"));
  myDisplay->println(F("New Pin:"));
  myDisplay->display();

  char key = '0';
  uint8_t index = 0;
  char pin[MAX_PIN];
  memset(pin,0,MAX_PIN);
  
  while(key!='#'){
    key = myKeypad->getKey();
    if(key){
      if(index >= 9){
        pin[index]='#';
        break;
      }
      else{
        pin[index] = key;
        index++;
        myDisplay->print(key);  
        myDisplay->display();
      }
    }
  }
  if( PIN_TYPE_MASTER == pinType )
    mySecurity.StoreMasterPin(pin,index);
  else
    mySecurity.StoreGatePin(pin,index);
  delay(1000);
  DrawPinMainMenu(pinType);
}

void gUI::DrawGateOpen(){
  logtrace_print(T_ALL,__func__);

  bool bOpenGate = true;
  
  if( mySecurity.GetEmailActiveState() )
    bOpenGate = DrawEmailRequestPin();
  
  myDisplay->clearDisplay();
  myDisplay->setCursor(0, 0);     // Start at top-left corner
  myDisplay->setTextSize(1);

  if( bOpenGate )
  {
    myDisplay->setTextSize(1);
    myDisplay->println(F("Gate Opened"));
    myDisplay->display();
    delay(4000);
    DrawMainMenu();
  }
  else{
    myDisplay->setTextSize(1);
    myDisplay->println(F("Pin Failed"));
    myDisplay->display();
    delay(4000);
    DrawMainMenu();
  }
}

void gUI::DrawEmailMainMenu(){
  logtrace_print(T_ALL,__func__);
  myDisplay->clearDisplay();
  myDisplay->setCursor(0, 0);     // Start at top-left corner
  myDisplay->setTextSize(1);
  myDisplay->println(F("1. Setup Email ")); 
  myDisplay->println(F("2. Print Email Info "));

  if( mySecurity.GetEmailActiveState() )
    myDisplay->println(F("3. Disable Email Auth")); 
  else
    myDisplay->println(F("3. Enable Email Auth"));
  myDisplay->println(F("9. Main menu"));  
  myDisplay->display();

  char key = '0';
  while(1){
    while( key == '0' ){
      key = myKeypad->getKey();
      if( key == '1' )
        DrawEmailSetupNew();
      if( key == '2' )
        DrawEmailPrintCurrent();
      if( key == '3' )
        DrawEmailSetStatus();  
      if( key == '9' )
        DrawMainMenu();
      key = '0';
    }
  }
}

void gUI::DrawEmailSetupNew(){
  logtrace_print(T_ALL,__func__);
  myDisplay->clearDisplay();
  myDisplay->setCursor(0, 0);     // Start at top-left corner
  myDisplay->setTextSize(1);
  myDisplay->println("Unsupported");
  myDisplay->display();
  
  delay(4000);
  DrawEmailMainMenu();
}

void gUI::DrawEmailPrintCurrent(){
  logtrace_print(T_ALL,__func__);

  myDisplay->clearDisplay();
  myDisplay->setCursor(0, 0);     // Start at top-left corner
  myDisplay->setTextSize(1);
  myDisplay->println("User: ");myDisplay->print(mySecurity.GetEmailUserName());
  myDisplay->println("Pass: ");myDisplay->print(mySecurity.GetEmailPassword());
  myDisplay->display();
  
  delay(4000);
  DrawEmailMainMenu();
  
}

void gUI::DrawEmailSetStatus(){
  logtrace_print(T_ALL,__func__);
  bool EmailState = mySecurity.GetEmailActiveState();
  myDisplay->clearDisplay();
  myDisplay->setCursor(0, 0);     // Start at top-left corner
  myDisplay->setTextSize(1);
  if( EmailState ){
    mySecurity.EnableEmailAuth(0);
    myDisplay->println(F("Email Auth Disabled "));
  }
  else{
    mySecurity.EnableEmailAuth(1);
    myDisplay->println(F("Email Auth Enabled "));
  }
  myDisplay->display();

  delay(1000);  
  DrawEmailMainMenu();
}

bool gUI::DrawEmailRequestPin(){
  logtrace_print(T_ALL,__func__);

  myDisplay->clearDisplay();
  myDisplay->setCursor(0, 0);     // Start at top-left corner
  myDisplay->println(F("Pin sent via Email"));
  myDisplay->println(F("Please enter Pin:"));
  myDisplay->display();
  
  char key = '0';
  uint8_t index = 0;
  char pin[MAX_PIN];
  char genPin[MAX_PIN];
  bool timeout  = false;
  uint32_t ts = millis();
  
  memset(pin,0,MAX_PIN);
  memset(genPin,0,MAX_PIN);

  mySecurity.GeneratePIN(genPin);
  SendEmail( genPin );

  
  while(key!='#'){
    if( millis() > (ts + 50000))
      return false;
    key = myKeypad->getKey();
    if(key){
      if(index >= 9){
        pin[index]='#';
        break;
      }
      else{
        pin[index] = key;
        index++;
        myDisplay->print(key);  
        myDisplay->display();
      }
    }
  }

  if( 0 != strcmp(pin,genPin) )
    return false;

  return true;
  
  
}




