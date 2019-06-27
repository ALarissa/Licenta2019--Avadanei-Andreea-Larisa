#ifndef _GUI_
#define _GUI_

#include "Security.h"
#include "Keypad_mapping.h" // keypad pinout
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

class gUI{
  private:
    Security mySecurity;  // used to access pins
    Keypad *myKeypad;
    Adafruit_SSD1306 *myDisplay;
    bool  bEmailEnabled;
  public:
    void DrawMainMenu();
    void DrawWelcomeMenu();
    void DrawRequestPinMenu();
    
    void DrawSetupMainMenu();
    
    void DrawPinMainMenu(uint8_t pinType );
    void DrawSetPinMenu(uint8_t pinType);
    void DrawDisplayCurrentPinMenu(uint8_t pinType);

    void DrawGateOpen();
        
    void DrawFormatFlashMenu();
    void DrawRemoveCurrentPinMenu();

    void DrawEmailMainMenu();
    void DrawEmailSetupNew();
    void DrawEmailPrintCurrent();
    void DrawEmailSetStatus();

    bool DrawEmailRequestPin();

 public:
  gUI();
  ~gUI();
  void Reset(void);
  void DrawMenu( uint8_t menuID );
};

#endif
