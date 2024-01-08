void PASSWORD()
{
//------------------------------------------------------Nhập sai mật khẩu
      char customKey = customKeypad.getKey();
      if (customKey && flag1==0 )
      {
        if(customKey!='A')
        {
          sdata+=customKey; 
          lcd.setCursor(sdata.length(),1); lcd.print(customKey); delay(500);
          lcd.setCursor(sdata.length(),1); lcd.print("*"); 
        }
        if(customKey=='C'){sdata=""; lcd.clear();}
        if(customKey=='A')
        {
          data = sdata; 
          if(data!=PW){flag2++; myDFPlayer.play(flag2); sdata=""; lcd.clear(); }
        }
        while(flag2>=3){ lcd.setCursor(0,0); lcd.print("Wrong password");  myDFPlayer.play(3); delay (10000); }
        
      }
//------------------------------------------------------Nhập đúng mật khẩu      
      if(data==PW)
      {
        myDFPlayer.play(4); flag1=1;
        lcd.clear(); 
        lcd.setCursor(0,0); lcd.print("A in Unclock"); 
        lcd.setCursor(0,1); lcd.print("D in Change Pass"); 
        
        while(data==PW)
        {
          char Key = customKeypad.getKey(); 
          //------------------------------------------------------Mở khóa xe
          if(Key=='A')
          {
          lcd.clear(); delay(500);
          myDFPlayer.play(5); 
          while(Key=='A')  { lcd.setCursor(0,1); lcd.print("Unlocked"); digitalWrite(23, 1); } 
          } 
          
          //------------------------------------------------------Đổi mật khẩu
          
          if(Key=='D')
          {
          lcd.clear();  
          while(Key=='D')  
           { 
            lcd.setCursor(0,0); lcd.print("New password    ");
            char pass = customKeypad.getKey(); 
            if(pass)
            {
              if(pass!='A')
              {
                datas+=pass; 
                lcd.setCursor(datas.length(),1); lcd.print(pass); delay(500);
                lcd.setCursor(datas.length(),1); lcd.print("*"); 
              }
              if(pass=='C'){datas=""; lcd.clear();}
              if(pass=='A')
              {
                for (int i = 0; i < 99; ++i){EEPROM.write(i, 0);} delay(500); 
                for (int i = 0; i < datas.length(); ++i){EEPROM.write(i, datas[i]);} EEPROM.commit(); delay(500); 
                ESP.restart();
              }
             }
            }   
          }
        }
      }
      
}
