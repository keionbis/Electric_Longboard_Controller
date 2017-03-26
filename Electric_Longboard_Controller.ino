#include <SoftwareSerial.h>
#define Experience_Shifter 4
#define SpeedControl 3
#define Trigger 5
int Analog_Speed, pwm_Speed, state, PowerMosfet = 6;
unsigned long long Btn_Stop, Btn_Start;

SoftwareSerial xbee(2, 3); // RX, TX

typedef enum ExperienceLevel{
  Beginer,
  Intermediate,
  Advanced, 
  Expert
};

typedef enum PowerStates{
  ON,
  OFF
};

ExperienceLevel Level;
PowerStates PowerState;


void setup() {
 pinMode(PowerMosfet, OUTPUT);
 PowerState = ON;
 attachInterrupt(digitalPinToInterrupt(Experience_Shifter), ChangeLevel, RISING);
 pinMode(Trigger, INPUT);
 xbee.begin(19200);
 Serial.begin(9600);
 

  
}

void loop() {
  switch(PowerState)
  {
    case ON:
      break;
    case OFF:
      digitalWrite(PowerMosfet, LOW);
  }
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
      pwm_Speed = map(Analog_Speed, 0, 1024, 0, 220);
      break;
    case Expert:
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
  if (digitalRead(2) == HIGH)
  {
    Btn_Start = millis;
    state = 1;
  }
  else if (digitalRead(2) == LOW)
  {
    Btn_Stop = millis;
    state = 0;
  }
  Process_Btn_Data();
}

void Process_Btn_Data()
{
  if (state == 0)
  {
    if((Btn_Stop - Btn_Start)< 500)
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
          Level = Expert;
          break;
        case Expert:
          Level = Beginer;
          break;
      }
    }
    else if((Btn_Stop - Btn_Start)>2000)
    {
      PowerState = OFF;
    }
  }
}


