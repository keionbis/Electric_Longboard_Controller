#include <SoftwareSerial.h>
#define Experience_Shifter 4
#define SpeedControl 3
#define Trigger 5

SoftwareSerial xbee(2, 3); // RX, TX

typedef enum ExperienceLevel{
  Beginer = 1,
  Intermediate = 2,
  Advanced = 3
};

ExperienceLevel Level;


int Analog_Speed, pwm_Speed;
void setup() {
 attachInterrupt(digitalPinToInterrupt(Experience_Shifter), ChangeLevel, RISING);
 pinMode(Trigger, INPUT);
 xbee.begin( 19200 );
  
}

void loop() {
  
  if(digitalRead(Trigger) == HIGH)
  {
    Analog_Speed = analogRead(SpeedControl);
  switch(Level)
  {
    case Beginer:
      pwm_Speed = map(Analog_Speed, 0, 1024, 0, 155);
      break;
    case Intermediate:
      pwm_Speed = map(Analog_Speed, 0, 1024, 0, 200);
      break;
    case Advanced:
      pwm_Speed = map(Analog_Speed, 0, 1024, 0, 255);
      break;
  }
  
 }
 else
 {
  pwm_Speed = 0;
 }
 xbee.println(pwm_Speed);
  
}

void ChangeLevel()
{
  switch(Level)
  {
    case Beginer:
      Level = Intermediate;
      break;
    case Intermediate:
      Level = Advanced;
      break;
    case Advanced:
      Level = Beginer;
      break;
  }
}

