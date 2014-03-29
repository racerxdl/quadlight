#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <util/twi.h> 	
#include <avr/interrupt.h> 
#include <stdint.h> 

#define I2C_BUFF_SIZE 16
#define TWCR_ACK TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);  
#define TWCR_NACK TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
#define TWCR_RESET TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC);  


class I2CSlave  {
    public:
        static uint8_t buffer_adr;
        static uint8_t i2cdata[];
        static void Start(uint8_t);
        static uint8_t Available();
        static uint8_t Read();
        static inline void onReceive(void (*fn)(uint8_t))   {    I2CSlave::_i2c_cb = fn;  };
        
        static void _defaultI2C_CB(uint8_t);
        static void (*_i2c_cb)(uint8_t);
        
};


