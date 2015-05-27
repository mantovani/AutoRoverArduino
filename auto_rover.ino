#include <Thermistor.h>

int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;    //M1 Direction Control
int M2 = 7;    //M1 Direction Control

int sensorR = 1;
int sensorRVal;
int sensorL = 0;
int sensorLVal;

int ledR = 13;
int ledL = 12;

int interval = 100;

Thermistor temp(5);
void stop(void)                    //Stop
{
  digitalWrite(E1,LOW);   
  digitalWrite(E2,LOW);      
}   
void advance(char a,char b)          //Move forward
{
  analogWrite (E1,a);      //PWM Speed Control
  digitalWrite(M1,LOW);    
  analogWrite (E2,b);    
  digitalWrite(M2,HIGH);
}  
void back_off (char a,char b)          //Move backward
{
  analogWrite (E1,a);      //PWM Speed Control
  digitalWrite(M1,HIGH);    
  analogWrite (E2,b);    
  digitalWrite(M2,LOW);
}
void turn_L (char a,char b)             //Turn Left
{
  analogWrite (E1,a);
  digitalWrite(M1,HIGH);    
  analogWrite (E2,b);    
  digitalWrite(M2,HIGH);
}
void turn_R (char a,char b)             //Turn Right
{
  analogWrite (E1,a);
  digitalWrite(M1,LOW);    
  analogWrite (E2,b);    
  digitalWrite(M2,LOW);
}
void setup(void) { 
  int i;
  for(i=4;i<=7;i++)
    pinMode(i, OUTPUT);
    
  pinMode(ledR,OUTPUT);
  pinMode(ledL,OUTPUT);
  
  Serial.begin(9600);      //Set Baud Rate
  Serial.println("Rover Read!");
  controlRover('w');
} 

int sensorVal(int sensor) {
  int sensorValue;
  for (int i=0; i< 64; i++) sensorValue += analogRead(sensor);
    sensorValue /= 64;
    return sensorValue;
}

void controlRover(char val) {
      switch(val)
      {
      case 'w'://Move Forward
        advance (200,200);   //move forward in max speed
        break;
      case 's'://Move Backward
        back_off (200,200);   //move back in max speed
        break;
      case 'a'://Turn Left
        turn_L (255,255);
        break;       
      case 'd'://Turn Right
        turn_R (255,255);
        break;
      case 'x':
        stop();
        break;
      }
}

void ledBlink(int led,int sensor, int cycles) {
  if (sensor >= 400 or sensor <= -300) {
    digitalWrite(led,HIGH);
      return;
  }
  
   if(sensor >= 300 && sensor < 400) {
    digitalWrite(led,LOW);
      if (cycles % 5 == 0)
        digitalWrite(led,HIGH);
      return;
  }
  
   if(sensor < 300 and sensor > 40) {
    digitalWrite(led,LOW);
      if (cycles % 16 == 0)
       digitalWrite(led,HIGH);   
  }
}

void loop(void) { 
  int active = 0;
  for(int i = 1;i<= 500000;i++) {
 
    sensorRVal = sensorVal(sensorR);
    sensorLVal = sensorVal(sensorL);
    
    int temperature = temp.getTemp();
    Serial.println(temperature);

    ledBlink(ledR,sensorRVal,i);
    ledBlink(ledL,sensorLVal,i);
    if (active == 0) {
      if (sensorRVal >= 400 or sensorRVal <= -300 && sensorLVal >= 400 or sensorRVal <= - 300) {
         controlRover('a');
         active++;
      }
      
      else if (sensorRVal >= 400 or sensorRVal <= -300 && sensorLVal >= 120) {
         controlRover('a');
         active++;
      }
      
      else if (sensorLVal >= 400 or sensorLVal <= -300 && sensorRVal >= 120) {
         controlRover('d');
         active++;
      }
    }
     else {
        active++;
        if(active == 5) {
          controlRover('w');
          active = 0;
        }
     }
     delay(interval);
  }
}
