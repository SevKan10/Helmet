void Check_BAT()
{
    while(digitalRead(CAG)==1)
    {
      for(int i=0; i<=100; i+=10){ drawBattery(i); delay(100); }
      if(digitalRead(CAG)==0){display.clearDisplay();} 
    }
}
