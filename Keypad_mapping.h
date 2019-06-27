#ifndef _KMAP_
#define _KMAP_

#include <Keypad.h>

static const byte ROWS = 4; //four rows
static const byte COLS = 3; //three columns
static char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
static byte rowPins[ROWS] = {21, 12, 13, 19}; 
static byte colPins[COLS] = {20, 22, 18};

static Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); // folosit petru a citi ce se apasa pe keypad

#endif
