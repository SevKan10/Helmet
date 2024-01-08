void NRF()
{
  while(radio.available())
  {
    digitalWrite(Buzze, 0); digitalWrite(Power, 1);
    lcd.setCursor(0,0); lcd.print("Device Connected");
    radio.read(&data_receive, sizeof(package_receive)); 
    ping = 0; flag1=0;
  }
  
  while(!radio.available())
  {
    ping++; 
    if(ping/1000>50 && flag1==0)
    {
      digitalWrite(Buzze, 0); digitalWrite(Power, 0);
      lcd.setCursor(0,0); lcd.print("Disconnected    ");

      char customKey = customKeypad.getKey();
      if (customKey && flag1==0 ){
        lcd.clear();
        if(customKey!='A'){sdata+=customKey;}
        if(customKey=='C'){sdata="";}
        if(customKey=='A')
        {
          data = sdata; Serial.println(data);
          if(data!=PW){flag2++; Serial.println(flag2);}
        }
        while(flag2>=3){ lcd.setCursor(0,0); lcd.print("Wrong password"); digitalWrite(Buzze, 1); }
        lcd.setCursor(0,1); lcd.print(sdata);  
      }

      if(data==PW)
      {
        lcd.clear(); lcd.setCursor(0,0); lcd.print("Correct password"); flag1=1;
        while(data==PW)
        {
          char Key = customKeypad.getKey(); 
          while(Key=='A')  { lcd.setCursor(0,1); lcd.print("Unlocked"); digitalWrite(Power, 1); }  
          while(Key=='D')  
          { 
            lcd.setCursor(0,0); lcd.print("Reset password  ");
            char pass = customKeypad.getKey(); 
            if(pass)
            {
              lcd.clear(); 
              if(pass!='A'){datas+=pass; }
              if(pass=='C'){datas="";}
              lcd.setCursor(0,1); lcd.print(datas);
              if(pass=='A'){
                for (int i = 0; i < 99; ++i){EEPROM.write(i, 0);} delay(500); 
                for (int i = 0; i < datas.length(); ++i){EEPROM.write(i, datas[i]);} delay(500); 
                resetFunc();
                }
            }  
          }
        }
      }  
    }  
  }
}
