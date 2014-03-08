/*
    A Light Control for Multirotors
    Copyright (C) 2014  Lucas Teske

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "config.h"

char leds[4] = {0,0,0,0};
char buff[6];
char recv;

void UpdateLeds()  {
  digitalWrite(L1R, leds[0] & (1 << 0));
  digitalWrite(L1B, leds[0] & (1 << 1));
  digitalWrite(L1G, leds[0] & (1 << 2));
  
  digitalWrite(L2R, leds[1] & (1 << 0));
  digitalWrite(L2B, leds[1] & (1 << 1));
  digitalWrite(L2G, leds[1] & (1 << 2));
  
  digitalWrite(L3R, leds[2] & (1 << 0));
  digitalWrite(L3B, leds[2] & (1 << 1));
  digitalWrite(L3G, leds[2] & (1 << 2));
  
  digitalWrite(L4R, leds[3] & (1 << 0));
  digitalWrite(L4B, leds[3] & (1 << 1));
  digitalWrite(L4G, leds[3] & (1 << 2));
}

inline void UpdateHeadlight(uint8_t value)  {
  #ifdef HEADLIGHT_PWM
   analogWrite(HEADLIGHT, value);
  #else
    if(value >= HEADLIGHT_MID)
      digitalWrite(HEADLIGHT, HIGH);
    else
      digitalWrite(HEADLIGHT,LOW);
  #endif 
}

inline void LandingOn()  {  digitalWrite(LANDING, HIGH); }
inline void LandingOff() {  digitalWrite(LANDING, LOW);  }

inline void UpdateLed(uint8_t arm, uint8_t led, uint8_t val)  {
  if(arm >= 0 && arm < 4) 
    leds[arm] = (leds[arm] & ~(1<<led)) | ((!!val) << led);  
}

inline void UpdateARMLED(uint8_t arm, uint8_t data)  {
   if(arm >= 0 && arm < 4)  
      leds[arm] = data;
}

void PlayTone(int note, int duration)  {
  tone(SPEAKER,note,TIME_UNIT*duration);
  delay(TIME_UNIT*1);
}

void InitLeds()  {
  pinMode(SPEAKER, OUTPUT);
  pinMode(HEADLIGHT, OUTPUT);
  pinMode(LANDING, OUTPUT);
  
  LandingOff();
  UpdateHeadlight(0);
  
  pinMode(L1R, OUTPUT);
  pinMode(L1B, OUTPUT);
  pinMode(L1G, OUTPUT);
  
  pinMode(L2R, OUTPUT);
  pinMode(L2B, OUTPUT);
  pinMode(L2G, OUTPUT);
  
  pinMode(L3R, OUTPUT);
  pinMode(L3B, OUTPUT);
  pinMode(L3G, OUTPUT);
  
  pinMode(L4R, OUTPUT);
  pinMode(L4B, OUTPUT);
  pinMode(L4G, OUTPUT);
  
  UpdateLeds();
}
inline void ShiftLedLeft(uint8_t arm)  { if(arm >= 0 && arm < 4) leds[arm] <<= 1;  }
inline void ShiftLedRight(uint8_t arm) { if(arm >= 0 && arm < 4) leds[arm] >>= 1;  }

void ClearLeds()  {
  for(int i=0;i<4;i++)
    leds[i] = 0;
  UpdateLeds();
}

void setup() {
  Serial.begin(115200);
  InitLeds();
  recv = 0;
  UpdateARMLED(0, 1);
  UpdateARMLED(1, 1);
  UpdateARMLED(2, 1);
  UpdateARMLED(3, 1);
  UpdateLeds();
  
  PlayTone(NOTE_2,1);
  PlayTone(NOTE_4,2);
  for(int i=0;i<4;i++)  ShiftLedLeft(i); 
  UpdateLeds();
  
  PlayTone(NOTE_5,2);
  PlayTone(NOTE_7,1);
  for(int i=0;i<4;i++)  ShiftLedLeft(i); 
  UpdateLeds();
  
  PlayTone(NOTE_5,2);
  PlayTone(NOTE_4,2);
  for(int i=0;i<4;i++)  ShiftLedRight(i); 
  UpdateLeds();
  
  PlayTone(NOTE_2,2);
  PlayTone(NOTE_4,1);
  for(int i=0;i<4;i++)  ShiftLedRight(i); 
  UpdateLeds();
  LandingOff();
  
  delay(400);
  for(int i=0;i<7;i++)  {
    delay(400);
    UpdateARMLED(0, 7*(i%2));
    UpdateARMLED(1, 7*(i%2));
    UpdateARMLED(2, 7*(i%2));
    UpdateARMLED(3, 7*(i%2));
    UpdateLeds();
  }
}

void loop() {
  if(Serial.available() > 0)  {
    buff[recv] = Serial.read();
    recv++;  
  }  
  if(recv == 6)  {
    Serial.print(buff);
    recv = 0;
     switch(buff[0])  {
       case QLP_INFO:  //  INFO
         Serial.write(QLP_INFO);
         Serial.write(QUADLIGHT_VERSION);
         Serial.write(QUADLIGHT_PROTOCOL);
         Serial.write("\x00\x00\x00");
         break;
       case QLP_UPDATEARM:  UpdateARMLED(buff[1], buff[2]);UpdateLeds();        break;
       case QLP_UPDATELED:  UpdateLed(buff[1], buff[2], buff[3]);UpdateLeds();  break;
       case QLP_HEADLIGHT:  UpdateHeadlight(buff[1]);                           break;
       case QLP_LANDING_ON: LandingOn();                                        break;
       case QLP_LANDING_OFF:LandingOff();                                       break;
       case QLP_SOUND:      tone(SPEAKER,buff[1]+buff[2]*0xFF);                 break;
       case QLP_NOSOUND:    noTone(SPEAKER);                                    break;
       default:
         Serial.write("\xff\x00\x00\x00\x00\x00");
         break;
     } 
  }
}
