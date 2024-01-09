//------------------------------------------------Thay đổi đơn vị
void set_num()
{
  if(digitalRead(SW3)==1)
  {
    while(digitalRead(SW3)==1);
    Ring(1,50);
    num1++; if(num1>5){num1=1;} 
    num2 = !num2;
  }

  if(digitalRead(SW4)==1)
  {
    while(digitalRead(SW4)==1);
    Ring(1,50);
    num1--; if(num1<1){num1=5;} 
    num2 = !num2; 
  }
}
//------------------------------------------------Thay đổi giá trị
void set_key()
{
  if(digitalRead(SW1)==1)
  {
    while(digitalRead(SW1)==1);
    Ring(1,50);
    key[num1]++; if(key[num1]>9){key[num1]=0;}
  }
  
  if(digitalRead(SW2)==1)
  {
    while(digitalRead(SW2)==1);
    Ring(1,50);
    key[num1]--; if(key[num1]<0){key[num1]=9;}
  }
}
