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
  oled.setCursor(20, 3);  oled.print(" Setting ADMIN  ");
  while(Serial1.available())
  {
    String inputString;
    String Val;
    while(Serial1.available()){inputString = Serial1.readString();}
    delay(65);
    Serial.println(inputString); 
    if(splitString(inputString,"#",1)=="ADMIN")
    {
      for(int i = 9; i < 21; i++){Val += inputString.charAt(i);}
      for (int i = 0; i < Val.length(); ++i){EEPROM.write(i, Val[i]);}  EEPROM.commit();
      delay(500);

      Serial1.println("AT+CMGF=1"); delay(500);
      Serial1.println("AT+CMGS=\"" + Val + "\"\r"); delay(500);
      Serial1.println("ADMIN OK! ");  
      Serial1.println((char)26); delay(500);
      
      oled.clear(); oled.setCursor(25,3); oled.print("ADMIN Completed"); 
      delay(2000);
      ESP.restart(); // reset
    }
  }
}

//---------------------------------------------------------------- Gửi SOS
void Send_SOS(String DATA)
{ 
  oled.clear();
  oled.setCursor(15, 3);  oled.print(DATA);
  Serial1.println("AT+CMGF=1"); delay(500);
  Serial1.println("AT+CMGS=\"" + ADMIN + "\"\r"); delay(500);
  Serial1.println(DATA);
  Serial1.print("http://maps.google.com/maps?z=18&q="); Serial1.print(myGPS);
  Serial1.println((char)26); delay(5000);

  Serial1.print (F("ATD"));
  Serial1.print (ADMIN);
  Serial1.print (F(";\r\n"));
  delay(20000);
  Serial1.print (F("ATH"));
  Serial1.print (F(";\r\n"));
  delay(500);
  flag2=0; fall=0;
  oled.clear();
}

//---------------------------------------------------------------- Nhắn tin lấy tọa độ
void Setup_MAP()
{
  while(Serial1.available())
  {
    String inputString;
    while(Serial1.available()){inputString = Serial1.readString(); delay(65);}  
    if(splitString(inputString,"#",1)=="MAP")
    {
      Serial.println("Send map");
      Serial1.println("AT+CMGF=1"); delay(500);
      Serial1.println("AT+CMGS=\"" + ADMIN + "\"\r"); delay(500);
      Serial1.print("http://maps.google.com/maps?z=18&q="); Serial1.println(myGPS);  
      Serial1.println((char)26); delay(500);
    }  
  } 
}
