#include "mcc_generated_files/mcc.h"
#include "BME280.h"



void write8(uint8_t reg, uint8_t value)
{
   I2C_Master_Start(); 
   I2C_Master_Write(_i2caddr);
   I2C_Master_Write(reg);
   I2C_Master_Write(value);
}

uint8_t read8(uint8_t reg)
{
    uint8_t value;
    I2C_Master_Start();
    I2C_Master_Write((uint8_t)_i2caddr);
    I2C_Master_Write((uint8_t)reg);
    I2C_Master_RepeatedStart();
    I2C_Master_Write((uint8_t)_i2caddr);
    value = I2C_Master_Read(0);
    I2C_Master_Stop();
    return value;
}

uint16_t read16(uint8_t reg)
{
    uint16_t value;
    I2C_Master_Start();
    I2C_Master_Write(_i2caddr);
    I2C_Master_Write(reg);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(_i2caddr);
    value = (I2C_Master_Read() << 8) | I2C_Master_Read();
    I2C_Master_Stop();
    return value;
}    
    
uint16_t read16_LE(uint8_t reg) 
{
    uint16_t temp = read16(reg);
    return (temp >> 8) | (temp << 8);
}

int16_t readS16(uint8_t reg)
{
    return (int16_t)read16(reg);
}

int16_t readS16_LE(uint8_t reg)
{
    return (int16_t)read16_LE(reg);
}

void read24(uint8_t reg)
{
    uint32_t value;
    I2C_Master_Start();
    I2C_Master_Write(_i2caddr);
    I2C_Master_Write(reg);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(_i2caddr);
    value = I2C_Master_Read();
    value <<= 8;
    value = I2C_Master_Read();
    value <<= 8;
    value = I2C_Master_Read();
    I2C_Master_Stop();
    return value;
}

void takeForcedMeasurement()
{
    // set to forced mode, i.e. "take next measurement"
        write8(BME280_REGISTER_CONTROL, 0x6D);
    // wait until measurement has been completed, otherwise we would read
    // the values from the last measurement
        while (read8(BME280_REGISTER_STATUS) & 0x08)
		__delay_ms(1);
        
}






/**************************************************************************/
/*!
    @brief  Reads the factory-set coefficients
*/
/**************************************************************************/



//******************
//Sparkfun




