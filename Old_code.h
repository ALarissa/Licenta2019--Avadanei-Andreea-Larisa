 /*
  if(key == '1'){
    Serial.println(sizeof(test));
  }
  
  if(key == '2'){
    memset(memory,0xEE,SIZE_4K);
    myStorage.storage_read(0,memory,SIZE_4K);
    Serial.println("Flash content:");
    for( int i=0;i<SIZE_4K;i++){
      Serial.print(memory[i],HEX);
      Serial.print('.');
    }
  }
  if(key == '3'){
    myStorage.storage_write(0,test,sizeof(test));
  }
  
  if(key == '5'){
    Serial.print("Erasing Flash chip ... ");
    myStorage.flash->chipErase();
    while(myStorage.flash->busy());
    Serial.println("DONE");
  }
      
  if(key == '8'){
    Serial.println("Flash content:");
    int counter = 0;

    while(counter<=256){
      Serial.print(myStorage.flash->readByte(counter++), HEX);
      Serial.print('.');
    }
      
    Serial.println();
  }
  */
