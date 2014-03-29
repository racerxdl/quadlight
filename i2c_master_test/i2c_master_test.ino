#include <Wire.h>

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
    
    Part of this code has been copied from Multiwii Source Code.
    You can see Multiwii Source code and License here: https://code.google.com/p/multiwii/
*/

// This will turn on and off the landing light in 1sec intervals

void i2c_init(void) {
  TWSR = 0;                                    // no prescaler => prescaler = 1
  TWBR = ((F_CPU / 100000) - 16) / 2;       // change the I2C clock rate
  TWCR = 1<<TWEN;                              // enable twi module, no interrupt
}
void i2c_rep_start(uint8_t address) {
  TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) ; // send REPEAT START condition
  waitTransmissionI2C();                       // wait until transmission completed
  TWDR = address;                              // send device address
  TWCR = (1<<TWINT) | (1<<TWEN);
  waitTransmissionI2C();                       // wail until transmission completed
}
void i2c_stop(void) {
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
  //  while(TWCR & (1<<TWSTO));                // <- can produce a blocking state with some WMP clones
}
void i2c_write(uint8_t data ) {
  TWDR = data;                                 // send data to the previously addressed device
  TWCR = (1<<TWINT) | (1<<TWEN);
  waitTransmissionI2C();
}

uint8_t i2c_read(uint8_t ack) {
  TWCR = (1<<TWINT) | (1<<TWEN) | (ack? (1<<TWEA) : 0);
  waitTransmissionI2C();
  uint8_t r = TWDR;
  if (!ack) i2c_stop();
  return r;
}

uint8_t i2c_readAck() {
  return i2c_read(1);
}

uint8_t i2c_readNak(void) {
  return i2c_read(0);
}

void waitTransmissionI2C() {
  uint16_t count = 255;
  while (!(TWCR & (1<<TWINT))) {
    count--;
    if (count==0) {              //we are in a blocking state => we don't insist
      TWCR = 0;                  //and we force a reset on TWINT register
      break;
    }
  }
}

size_t i2c_read_to_buf(uint8_t add, void *buf, size_t size) {
  i2c_rep_start((add<<1) | 1);  // I2C read direction
  size_t bytes_read = 0;
  uint8_t *b = (uint8_t*)buf;
  while (size--) {
    /* acknowledge all but the final byte */
    *b++ = i2c_read(size > 0);
    /* TODO catch I2C errors here and abort */
    bytes_read++;
  }
  return bytes_read;
}

size_t i2c_read_reg_to_buf(uint8_t add, uint8_t reg, void *buf, size_t size) {
  i2c_rep_start(add<<1); // I2C write direction
  i2c_write(reg);        // register selection
  return i2c_read_to_buf(add, buf, size);
}
#define QUADLIGHT_ADDRESS 0x52 //   7 bits

#define QLP_INFO 0
#define QLP_UPDATEARM 1
#define QLP_UPDATELED 2
#define QLP_HEADLIGHT 3
#define QLP_LANDING_ON 4
#define QLP_LANDING_OFF 5
#define QLP_SOUND 6
#define QLP_NOSOUND 7
#define QLP_PLAYSONG 8
#define QLP_UNKNOWN 255  

#define QLP_ARM_LEDR (1<<0)     //  Red Led
#define QLP_ARM_LEDB (1<<1)     //  Blue Led
#define QLP_ARM_LEDG (1<<2)     //  Green Led

#define QLP_TONE_START      1
#define QLP_TONE_BATT_LOW   2
#define QLP_TONE_BATT_CRIT  3
#define QLP_TONE_UNSTABLE   4

void QuadLightWriteCMD(uint8_t cmd, uint8_t *data)    {
    if(data != NULL)    {
        switch(cmd) {
            case QLP_INFO:      
            case QLP_LANDING_ON:
            case QLP_LANDING_OFF:
            case QLP_NOSOUND: 
                i2c_rep_start(QUADLIGHT_ADDRESS);
                i2c_write(cmd);
                i2c_stop();   
                break;
            case QLP_UPDATEARM:
                i2c_rep_start(QUADLIGHT_ADDRESS);
                i2c_write(cmd);
                i2c_write(data[0]);
                i2c_write(data[1]);
                i2c_stop();
                break;
            case QLP_UPDATELED:
                i2c_rep_start(QUADLIGHT_ADDRESS);
                i2c_write(cmd);
                i2c_write(data[0]);
                i2c_write(data[1]);
                i2c_write(data[2]);
                i2c_stop();
                break;
            case QLP_HEADLIGHT:
                i2c_rep_start(QUADLIGHT_ADDRESS);
                i2c_write(cmd);
                i2c_write(data[0]);
                i2c_stop();
                break;
            case QLP_SOUND:
                i2c_rep_start(QUADLIGHT_ADDRESS);
                i2c_write(cmd);
                i2c_write(data[0]);
                i2c_write(data[1]);
                i2c_stop();
                break;
            case QLP_PLAYSONG:
                i2c_rep_start(QUADLIGHT_ADDRESS);
                i2c_write(cmd);
                i2c_write(data[0]);
                i2c_write(data[1]);
                i2c_stop();
                break;
        }
    }
}

uint8_t data[8];

void setup()
{
  i2c_init();
  delay(1000);
  data[0] = 0xFF;
}

byte x = 0;

void loop()
{
  data[0] = 0xFF;
  QuadLightWriteCMD(QLP_HEADLIGHT, data);
  uint8_t led = QLP_ARM_LEDR | QLP_ARM_LEDB | QLP_ARM_LEDG;
  data[1] = led;
  for(int i=0;i<4;i++)  {
    data[0] = i;
    QuadLightWriteCMD(QLP_UPDATEARM, data);
  }
  delay(1000);
  data[0] = 0x00;
  QuadLightWriteCMD(QLP_HEADLIGHT, data);
  data[1] = 0;
  for(int i=0;i<4;i++)  {
    data[0] = i;
    QuadLightWriteCMD(QLP_UPDATEARM, data);
  }
  delay(1000);
}
