//---------------------------------------------------------------- Xử lý chuỗi
String splitString(String inputString, String delim, uint16_t pos){
String tmp = inputString;
  for(int i=0; i<pos; i++){
    tmp = tmp.substring(tmp.indexOf(delim)+1);
    if(tmp.indexOf(delim)== -1 
    && i != pos -1 )
      return "";
  }
  return tmp.substring(0,tmp.indexOf(delim));
}


//---------------------------------------------------------------- Cài ADMIN
void Setup_ADMIN()
{
  display.clearDisplay(); display.setCursor(16, 16); display.print("Setting ADMIN"); display.display();
  while(Serial2.available())
  {
    String inputString;
    String Val;
    while(Serial2.available()){inputString = Serial2.readString();} delay(65);
    if(splitString(inputString,"#",1)=="ADMIN")
    {
      for(int i = 9; i < 21; i++){Val += inputString.charAt(i);}
      for (int i = 0; i < Val.length(); ++i){EEPROM.write(i, Val[i]);}  EEPROM.commit();
      delay(500);

      Serial2.println("AT+CMGF=1"); delay(500);
      Serial2.println("AT+CMGS=\"" + Val + "\"\r"); delay(500);
      Serial2.println("ADMIN OK! ");  
      Serial2.println((char)26); delay(500);
      
      display.clearDisplay(); display.setCursor(25, 16); display.print("ADMIN Completed"); display.display();
      delay(2000);
      ESP.restart();
    }
  }
}

//---------------------------------------------------------------- Gửi SOS
void Send_SOS(String DATA)
{ 
  display.clearDisplay(); 
  display.setCursor(10,16);
  display.print(DATA); 
  display.display(); 

  SerialBT.print('S');
  Serial2.println("AT+CMGF=1"); delay(500);
  Serial2.println("AT+CMGS=\"" + ADMIN + "\"\r"); delay(500);
  Serial2.println(DATA);
  Serial2.print("http://maps.google.com/maps?z=18&q="); Serial2.print(GPS);
  Serial2.println((char)26);
  delay(5000);

  Serial2.print (F("ATD"));
  Serial2.print (ADMIN);
  Serial2.print (F(";\r\n"));
  delay(20000);
  Serial2.print (F("ATH"));
  Serial2.print (F(";\r\n"));
  delay(500);
  display.clearDisplay();
}

//---------------------------------------------------------------- Nhắn tin lấy tọa độ
void Setup_MAP()
{
  while(Serial2.available())
  {
    String inputString;
    while(Serial2.available()){inputString = Serial2.readString(); delay(65);}  
  
     if(splitString(inputString,"#",1)=="MAP")
      {
      Serial2.println("AT+CMGF=1"); delay(500);
      Serial2.println("AT+CMGS=\"" + ADMIN + "\"\r"); delay(500);
      Serial2.print("http://maps.google.com/maps?z=18&q="); 
      Serial2.print(GPS);  
      Serial2.println((char)26); delay(500);
      }

     if(splitString(inputString,"#",1)=="GPS")
      {
      GPS = splitString(inputString,"#",2);

      for (int i = 15; i < 35; ++i)         {EEPROM.write(i, 0);}      
      for (int i = 0; i < GPS.length(); ++i){EEPROM.write(i+15, GPS[i]);}  EEPROM.commit();
      
      Serial2.println("AT+CMGF=1"); delay(500);
      Serial2.println("AT+CMGS=\"" + ADMIN + "\"\r"); delay(500);
      Serial2.print("Setup Map Success"); 
      Serial2.println((char)26); delay(500);
      
      display.clearDisplay(); display.setCursor(15,16); display.print("Setup Map Success"); display.display();
      delay(2000);
      ESP.restart();
      }      
  }
}


void drawBattery(int percentage) {
  display.clearDisplay();
  display.setCursor(25, 0); display.print("Charging"); 
  display.drawRect(105, 0, 20, 10, SSD1306_WHITE);  display.drawRect(124, 2, 3, 6, SSD1306_WHITE);
  int batteryLength = map(percentage, 0, 100, 0, 16);
  batteryLength = constrain(batteryLength, 0, 16);
  display.fillRect(107, 2, batteryLength, 6, SSD1306_WHITE);
  display.display();
}
