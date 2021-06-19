#include <EEPROM.h>

/*
memory allocation
0 -> flag : 1 for credentials stored
1 -> SSID length
2-20 -> SSID
21 -> password length
22-40 -> password
**/

int flag;  
String data_eeprom; 

bool credentials_stored(){
  flag = EEPROM.read(0);
  Serial.print("EEPROM FLAG VALUE : ");
  Serial.print(flag);
  Serial.println();  
  if (flag == 1){
    return true;
  }
  return false;
}

void write_eeprom(String ssid, String password){
  Serial.println("WRITING CREDENTIALS TO THE EEPROM ... ");
  EEPROM.write(0,1);  //flag
  EEPROM.write(1, ssid.length());
  for(int i=0;i<ssid.length();i++){
    EEPROM.write(2+i, ssid[i]); 
  }
  EEPROM.write(21, password.length());
  for(int i=0;i<password.length();i++){
   EEPROM.write(22+i, password[i]); 
  }
  EEPROM.commit(); 
}

//param -> 0 for SSID & 1 for password
String read_eeprom(int param){
  data_eeprom = "";
  if (param == 0){
    for(int i=0;i<EEPROM.read(1);i++){
      data_eeprom += char(EEPROM.read(2+i)); 
    }
  }
  else{
    for(int i=0;i<EEPROM.read(21);i++){
      data_eeprom += char(EEPROM.read(22+i)); 
    }
  }
  return data_eeprom;
}

void clear_eeprom(){
  Serial.println("CLEARING THE EEPROM ... ");
  for(int i=0;i<41;i++){
    EEPROM.write(i, 0); 
  }
}
