#define trigL  5  
#define echoL  4   
#define trigR  11   
#define echoR  10

#define LEDL   3
#define LEDR   2


void setup()
{
    Serial.begin(9600);
    pinMode(trigL,OUTPUT);   
    pinMode(echoL,INPUT);
    pinMode(LEDL, OUTPUT);
     
    pinMode(trigR,OUTPUT);   
    pinMode(echoR,INPUT); 
    pinMode(LEDR, OUTPUT); 
    
}
 
void loop()
{  
  LEFT();
  RIGHT();
}



void LEFT()
{
    digitalWrite(trigL,0);   
    delayMicroseconds(2);
    digitalWrite(trigL,1);   
    delayMicroseconds(5);  
    digitalWrite(trigL,0);  
    unsigned long durationL = pulseIn(echoL,HIGH);  
    int distanceL = int(durationL/2/29.412);
    Serial.print(distanceL); Serial.print(" | ");
    
    if(distanceL < 100) {digitalWrite(LEDL,1); }
    if(distanceL > 100) {digitalWrite(LEDL,0); }
}

void RIGHT()
{
    digitalWrite(trigR,0);   
    delayMicroseconds(2);
    digitalWrite(trigR,1);   
    delayMicroseconds(5);  
    digitalWrite(trigR,0);  
    unsigned long durationR = pulseIn(echoR,HIGH);  
    int distanceR = int(durationR/2/29.412);
    Serial.println(distanceR);
    
    if(distanceR < 100) {digitalWrite(LEDR,1); }
    if(distanceR > 100) {digitalWrite(LEDR,0); }
}
