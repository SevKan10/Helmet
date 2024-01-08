//-------------------------------------------------------------------Đọc mật khẩu
void load_pw()
{
  if(key[5]==1)
  {
    key[5]=0;
    DATA = DATA + key[1] + key[2] + key[3] + key[4]; delay(50); 
  if(DATA != PW){ DATA = ""; flag++; myDFPlayer.play(flag); if(flag>2){while(true);} }
  if(DATA == PW){ delay(500); Ring(1,500); lock = 0; }
  }
}


//-------------------------------------------------------------------Đổi mật khẩu
void set_pw()
{
  set_num();
  set_key(); 
  
  x = 7+(24*(num1-1)); 
  
  display.setTextSize(1);
  display.clearDisplay(); 
  display.setCursor(0,0); 
  display.print("Change Password Mode"); 

  display.setTextSize(2);
  display.setCursor(10,25);
  display.print(key[1]); display.print(" "); display.print(key[2]); display.print(" "); display.print(key[3]); display.print(" "); display.print(key[4]);

  display.drawCircle(110, 28, 3, SH110X_WHITE);
  display.drawLine(110, 31, 110, 38, SH110X_WHITE);
  display.drawLine(111, 36, 113, 36, SH110X_WHITE);
  display.drawLine(111, 34, 112, 34, SH110X_WHITE);
  
  display.drawRect(x, 22, 16, 20, SH110X_WHITE);
  display.display();

  if(key[5]==1)
  {
    DATA = DATA + key[1] + key[2] + key[3] + key[4]; delay(50); 
    
    for(int i=0; i<100; i++)
    {
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.clearDisplay();
      if(i>5) {display.setCursor(0,0);  display.print("Deleting data ");  }
      if(i>25){display.setCursor(0,15); display.print("Writing data");    }
      if(i>60){display.setCursor(0,30); display.print("Reseting device"); }  

      display.drawRect(15, 48, 100, 15, SH110X_WHITE);
      display.fillRect(15, 48, i,   15, SH110X_WHITE);
      display.display();
    }
    Ring(3,50);
    for (int i = 0; i < 9; ++i){EEPROM.write(i, 0);} delay(100); 
    for (int i = 0; i < DATA.length(); ++i){EEPROM.write(i, DATA[i]);} EEPROM.commit(); delay(100); 
    ESP.restart();

  }
}


void Ring(int repeat, int delays)
{
  for(int i=repeat; i>0; i--)
  {
    digitalWrite(BUZ,1); delay(delays); 
    digitalWrite(BUZ,0); delay(delays);
  }
}
