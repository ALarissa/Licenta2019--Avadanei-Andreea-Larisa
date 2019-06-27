
#ifndef _MENU_
#define _MENU_

#include "Security.h";
#include "Keypad_mapping.h" // keypad pinout

#define MENU_SIZE 5

#define MMENU_ID  1

char *Main_Menu[] = { 
  "\n1. Setup a new Pin",
  "2. Print current Pin",
  "3. Remove current Pin",
  "9. Format Flash",
  "0. Exit"
};

Security mySecurity;

              
// print main menu
void menu_printMM(char **menu, uint8_t menusize ){
  mySecurity.Init();
  for( int i=0; i<menusize; i++){
    Serial.println(menu[i]);
  }
  
}

// print set new Pin
void menu_printSNP(){
  char pin[MAX_PIN];
  memset(pin,0,MAX_PIN);
  uint8_t index = 0;
  mySecurity.Init();
  Serial.println("Please enter a new pin ( max 9 number ) folowed by # key");
  Serial.print("New Pin: ");
  char key = '0';
  while(key!='#'){
    key = keypad.getKey();
    if(key){
      if(index >= 9){
        pin[index]='#';
        break;
      }
      else{
        pin[index] = key;
        index++;
        Serial.print(key);  
      }
    }
  }
  Serial.print("\nNew Pin is: ");
  Serial.println(pin);
  mySecurity.StoreSecurityPin(pin,sizeof(pin));
  
}

// next menu
void menu_printNextMenu( uint8_t menuID, uint8_t option ){
  mySecurity.Init();
  switch(menuID){
    case 1:
      if( option == 1 )
        menu_printSNP();
      if( option == 2 ){
        mySecurity.PrintCurrentPin();
      }
      if( option == 3 ){
        mySecurity.ClearSecurityPin();
      }
      if( option == 9){
        mySecurity.EraseFlash();
      }
      break;
    
  }
  menu_printMM(Main_Menu,MENU_SIZE);
}

#endif

