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
#include <Wire.h>
#include "./config.h"
#include "SoundManager.h"
#include "LedManager.h"

static char i2cbuff[3];
static char buff[3];
static char recv;

SoundManager sndman(SPEAKER);
LedManager   ledman;


void I2C_Receive(int argc) {
  uint8_t count = 0;
  for(int i=0;i<3;i++)  i2cbuff[i] = 0;
  if(argc <= 3)  {
    while(count < argc)  {
      i2cbuff[count] = Wire.read();
      count++;
    }
    ProcessAction(i2cbuff, true);
  }
}
void I2C_Request()  {  /*Nothing for now*/ }

inline void StartI2C()  {
  Wire.onReceive(I2C_Receive);
  Wire.onRequest(I2C_Request);
  Wire.begin(I2C_ADDRESS);
}



void initResetTimer()  {
   TCCR1B = 0x00;
   TCNT1 = 0x00;
   TIFR1  = 0x00;
   TIMSK1 = 0x01;
   TCCR1A = 0x00;
   TCCR1B |= (1<< CS12) | (1<< CS10);
}
ISR(TIMER1_OVF_vect)
{
    if(recv < 6 && Serial.available() == 0)  {
       recv  =   0;    
       TCCR1B = 0x00;
       TCNT1 = 0x00;
       TIFR1  = 0x00;
    }
}

void setup() {
  Serial.begin(115200);
  recv = 0;
  ledman.InitLeds();
  ledman.UpdateARMLED(0, 1);
  ledman.UpdateARMLED(1, 1);
  ledman.UpdateARMLED(2, 1);
  ledman.UpdateARMLED(3, 1);
  ledman.UpdateLeds();
  sndman.StartSequence(1,false);
  uint16_t nextflash = millis() + 400;
  uint8_t c = 0;
  while(sndman.Playing())  {
    sndman.Update();
    if(millis() > nextflash)  {
      if(c>=2)
        for(int i=0;i<4;i++)  ledman.ShiftLedRight(i);
      else
        for(int i=0;i<4;i++)  ledman.ShiftLedLeft(i); 
      ledman.UpdateLeds();  
      c++;
      nextflash = millis() + 400;
    }
  }
  delay(200);
  for(int i=0;i<7;i++)  {
    delay(200);
    ledman.UpdateARMLED(0, 7*(i%2));
    ledman.UpdateARMLED(1, 7*(i%2));
    ledman.UpdateARMLED(2, 7*(i%2));
    ledman.UpdateARMLED(3, 7*(i%2));
    ledman.UpdateLeds();
  }

  StartI2C();
  initResetTimer();
}

void ProcessAction(char *data, boolean i2c)  {
    switch(data[0])  {
       case QLP_INFO:  //  INFO
         if(!i2c)  {
           Serial.write(QLP_INFO);
           Serial.write(QUADLIGHT_VERSION);
           Serial.write(QUADLIGHT_PROTOCOL);
         }else{
           Wire.write(QLP_INFO);
           Wire.write(QUADLIGHT_VERSION);
           Wire.write(QUADLIGHT_PROTOCOL); 
         }
         break;
       case QLP_UPDATEARM:  ledman.UpdateARMLED(data[1], data[2]);       ledman.UpdateLeds();  break;
       case QLP_UPDATELED:  ledman.UpdateLed(data[1], data[2], data[3]); ledman.UpdateLeds();  break;
       case QLP_HEADLIGHT:  ledman.UpdateHeadlight(data[1]);                                   break;
       case QLP_LANDING_ON: ledman.LandingOn();                                                break;
       case QLP_LANDING_OFF:ledman.LandingOff();                                               break;
       case QLP_SOUND:      sndman.SetTone(data[1]+data[2]*0xFF);                              break;
       case QLP_NOSOUND:    sndman.StopSequence(); sndman.ClearTone();                         break;
       case QLP_PLAYSONG:   sndman.StartSequence(data[1],data[2]>0);                           break;
       default:
         if(!i2c)
           Serial.write("\xff\x00\x00");
         else
           Wire.write("\xFF");
         break;
     }  
}

void loop() {
  
  if(Serial.available() > 0)  {
    buff[recv] = Serial.read();
    recv++;  
  }  
  if(recv == 3)  {
    recv = 0;
    ProcessAction(buff, false);
  }
  sndman.Update();
}
