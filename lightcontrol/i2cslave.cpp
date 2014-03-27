#include "i2cslave.h"

uint8_t I2CSlave::buffer_adr;
uint8_t I2CSlave::i2cdata[I2C_BUFF_SIZE];

void I2CSlave::Start(uint8_t addr)    {
    TWAR= addr;
    TWCR &= ~(1<<TWSTA)|(1<<TWSTO);
    TWCR|= (1<<TWEA) | (1<<TWEN)|(1<<TWIE);     
    buffer_adr=0xFF;  
    sei();
}

uint8_t I2CSlave::Available()   {
    if(I2CSlave::buffer_adr > 0 && I2CSlave::buffer_adr != 0xFF)
        return I2CSlave::buffer_adr;
    else
        return 0;
}

uint8_t I2CSlave::Read()    {
    uint8_t count = I2CSlave::buffer_adr;
    if(count > 0 && count != 0xFF)  {
        uint8_t data = I2CSlave::i2cdata[0];
        for(uint8_t i=1;i<count;i++)    
            I2CSlave::i2cdata[i-1] = I2CSlave::i2cdata[i];
        I2CSlave::buffer_adr--;
        return data;
    }else
        return 0x00;
}

ISR (TWI_vect)  {
    uint8_t data=0;

    switch (TW_STATUS)   {
    // Slave Receiver 
    case TW_SR_SLA_ACK:
            TWCR_ACK;
            I2CSlave::buffer_adr=0xFF;
            break;
    case TW_SR_DATA_ACK:
        data=TWDR;
        if(I2CSlave::buffer_adr == 0xFF)
            I2CSlave::buffer_adr = 0x00;
        /*
        if (buffer_adr == 0xFF)     {
            if(data<i2c_buffer_size+1)    
                buffer_adr= data;
            else
                buffer_adr=0;    
            TWCR_ACK;
        }else{*/
            if(I2CSlave::buffer_adr<I2C_BUFF_SIZE+1)    
                I2CSlave::i2cdata[I2CSlave::buffer_adr]=data;    
            I2CSlave::buffer_adr++;
            TWCR_ACK;    
        //}
        break;
        
    //Slave transmitter
    case TW_ST_SLA_ACK: 
    case TW_ST_DATA_ACK: 
        if (I2CSlave::buffer_adr == 0xFF) 
            I2CSlave::buffer_adr=0;
        if(I2CSlave::buffer_adr<I2C_BUFF_SIZE+1)    {
                TWDR = I2CSlave::i2cdata[I2CSlave::buffer_adr];
                I2CSlave::buffer_adr++;
        }else
                TWDR=0;
        TWCR_ACK;
        break;
    case TW_SR_STOP:
        TWCR_ACK;
        break;
    case TW_ST_DATA_NACK: 
    case TW_SR_DATA_NACK:
    case TW_ST_LAST_DATA:
    default:     
        TWCR_RESET;
    break;
    
    }
}

