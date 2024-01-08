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
  while(simSerial.available())
  {
    String inputString;
    String Val;
    while(simSerial.available()){inputString = simSerial.readString();}
    delay(65);
    Serial.println(inputString);
    if(splitString(inputString,"#",1)=="ADMIN")
    {
      for(int i = 9; i < 21; i++){Val += inputString.charAt(i);}
      Serial.print("ADMIN: "); Serial.println(Val);
      for (int i = 0; i < Val.length(); ++i){EEPROM.write(i, Val[i]);} 
      delay(500);

      simSerial.println("AT+CMGF=1"); delay(500);
      simSerial.println("AT+CMGS=\"" + Val + "\"\r"); delay(500);
      simSerial.println("ADMIN OK! ");  
      simSerial.println((char)26); delay(500);
      oled.clear(); oled.setCursor(0,0); oled.print("ADMIN Completed");
      resetFunc(); // reset
    }
  }
}

//---------------------------------------------------------------- Gửi SOS
void Send_SOS()
{ 
  simSerial.listen();
  digitalWrite(Buzz, 1);
  simSerial.println("AT+CMGF=1"); delay(500);
  simSerial.println("AT+CMGS=\"" + ADMIN + "\"\r"); delay(500);
  simSerial.println("Help me!");
  simSerial.print("http://maps.google.com/maps?z=18&q="); 
  simSerial.print(Lat, 6); simSerial.print(","); simSerial.println(Lng, 6); 
  simSerial.println((char)26); delay(5000);

  simSerial.print (F("ATD"));
  simSerial.print (ADMIN);
  simSerial.print (F(";\r\n"));
  delay(20000);
  simSerial.print (F("ATH"));
  simSerial.print (F(";\r\n"));
  delay(500);
  digitalWrite(Buzz, 0);
  flag2=0; fall = millis(); 
}
