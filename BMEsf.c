#include "BMEsf.h"
#include <math.h>
#include "i2csource.h"

void construct()
{
	I2CAddress  = 0xEE; 
    I2CAddress2 = 0xEF;
	runMode = MODE_NORMAL;    //sleep mode
	tempOverSample = SAMPLING_X16;
	pressOverSample = SAMPLING_X16;
	humidOverSample = SAMPLING_X1;
    tStandby = STANDBY_MS_125;   // 1000ms standby for normal mode
    filter = FILTER_X4;
            
}

void BMEbegin(void)
{
    uint8_t dataToWrite = 0;
    	//Reading all compensation data, range 0x88:A1, 0xE1:E7
	resetBME();
    __delay_ms(500);
    
    dig_T1 = ((uint16_t)((readRegister(BME280_DIG_T1_MSB_REG) << 8) + readRegister(BME280_DIG_T1_LSB_REG)));
	dig_T2 = ((int16_t)((readRegister(BME280_DIG_T2_MSB_REG) << 8) + readRegister(BME280_DIG_T2_LSB_REG)));
	dig_T3 = ((int16_t)((readRegister(BME280_DIG_T3_MSB_REG) << 8) + readRegister(BME280_DIG_T3_LSB_REG)));

	dig_P1 = ((uint16_t)((readRegister(BME280_DIG_P1_MSB_REG) << 8) + readRegister(BME280_DIG_P1_LSB_REG)));
	dig_P2 = ((int16_t)((readRegister(BME280_DIG_P2_MSB_REG) << 8) + readRegister(BME280_DIG_P2_LSB_REG)));
	dig_P3 = ((int16_t)((readRegister(BME280_DIG_P3_MSB_REG) << 8) + readRegister(BME280_DIG_P3_LSB_REG)));
	dig_P4 = ((int16_t)((readRegister(BME280_DIG_P4_MSB_REG) << 8) + readRegister(BME280_DIG_P4_LSB_REG)));
	dig_P5 = ((int16_t)((readRegister(BME280_DIG_P5_MSB_REG) << 8) + readRegister(BME280_DIG_P5_LSB_REG)));
	dig_P6 = ((int16_t)((readRegister(BME280_DIG_P6_MSB_REG) << 8) + readRegister(BME280_DIG_P6_LSB_REG)));
	dig_P7 = ((int16_t)((readRegister(BME280_DIG_P7_MSB_REG) << 8) + readRegister(BME280_DIG_P7_LSB_REG)));
	dig_P8 = ((int16_t)((readRegister(BME280_DIG_P8_MSB_REG) << 8) + readRegister(BME280_DIG_P8_LSB_REG)));
	dig_P9 = ((int16_t)((readRegister(BME280_DIG_P9_MSB_REG) << 8) + readRegister(BME280_DIG_P9_LSB_REG)));

	dig_H1 = ((uint8_t)(readRegister(BME280_DIG_H1_REG)));
	dig_H2 = ((int16_t)((readRegister(BME280_DIG_H2_MSB_REG) << 8) + readRegister(BME280_DIG_H2_LSB_REG)));
	dig_H3 = ((uint8_t)(readRegister(BME280_DIG_H3_REG)));
	dig_H4 = ((int16_t)((readRegister(BME280_DIG_H4_MSB_REG) << 4) + (readRegister(BME280_DIG_H4_LSB_REG) & 0x0F)));
	dig_H5 = ((int16_t)((readRegister(BME280_DIG_H5_MSB_REG) << 4) + ((readRegister(BME280_DIG_H4_LSB_REG) >> 4) & 0x0F)));
	dig_H6 = ((uint8_t)readRegister(BME280_DIG_H6_REG));

	//Set the oversampling control words.
	//config will only be writeable in sleep mode, so first insure that.
	writeRegister(BME280_CTRL_MEAS_REG, 0x00);
	
	//Set the config word
	dataToWrite = (tStandby << 0x5) & 0xE0;
	dataToWrite |= (filter << 0x02) & 0x1C;
	writeRegister(BME280_CONFIG_REG, dataToWrite);
	
	//Set ctrl_hum first, then ctrl_meas to activate ctrl_hum
	dataToWrite = humidOverSample & 0x07; //all other bits can be ignored
	writeRegister(BME280_CTRL_HUMIDITY_REG, dataToWrite);
	
	//set ctrl_meas
	//First, set temp oversampling
	dataToWrite = (tempOverSample << 0x5) & 0xE0;
	//Next, pressure oversampling
	dataToWrite |= (pressOverSample << 0x02) & 0x1C;
	//Last, set mode
	dataToWrite |= (runMode) & 0x03;
	//Load the byte
	writeRegister(BME280_CTRL_MEAS_REG, dataToWrite);
	
	//return readRegister(0xD0);  //return chip_ip (hopefully 0x60)
}

void forcedMode(void)
{
    writeRegister(BME280_CTRL_MEAS_REG, 0x01);
    while (readRegister(BME280_STAT_REG) & 0x08)
    __delay_ms(1);
}

void resetBME( void )
{
	writeRegister(BME280_RST_REG, 0xB6);    //reset code
}

float readTempC( void )
{
	// Returns temperature in DegC, resolution is 0.01 DegC. Output value of ?5123? equals 51.23 DegC.
	// t_fine carries fine temperature as global value

	//get the reading (adc_T);
	int32_t adc_T = ((uint32_t)readRegister(BME280_TEMPERATURE_MSB_REG) << 12) | ((uint32_t)readRegister(BME280_TEMPERATURE_LSB_REG) << 4) | ((readRegister(BME280_TEMPERATURE_XLSB_REG) >> 4) & 0x0F);
	//By datasheet, calibrate
    
	int32_t var1, var2;

	var1 = ((((adc_T>>3) - ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((int32_t)dig_T1)) * ((adc_T>>4) - ((int32_t)dig_T1))) >> 12) *
	((int32_t)dig_T3)) >> 14;
	t_fine = var1 + var2;
	float output = (t_fine * 5 + 128) >> 8;

	output = output / 100;
	
	return output;
}

float readTempF( void )
{
	float output = readTempC();
	output = (output * 9) / 5 + 32;

	return output;
}

float readFloatPressure( void )
{
    //---Must read temp first to establish t_fine---//
	// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
	// Output value of ?24674867? represents 24674867/256 = 96386.2 Pa = 963.862 hPa
    int32_t adc_P = ((uint32_t)readRegister(BME280_PRESSURE_MSB_REG) << 12) | ((uint32_t)readRegister(BME280_PRESSURE_LSB_REG) << 4) | ((readRegister(BME280_PRESSURE_XLSB_REG) >> 4) & 0x0F);
	
    int32_t var1, var2;
    uint32_t p;
    var1 = (((int32_t)t_fine)>>1) - (int32_t)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)dig_P6);
    var2 = var2 + ((var1*((int32_t)dig_P5))<<1);
    var2 = (var2>>2)+(((int32_t)dig_P4)<<16);
    var1 = (((dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)dig_P2) * var1)>>1))>>18;
    var1 =((((32768+var1))*((int32_t)dig_P1))>>15);
    if (var1 == 0)
    {
        return 0; // avoid exception caused by division by zero
    }
    p = (((uint32_t)(((int32_t)1048576)-adc_P)-(var2>>12)))*3125;
    if (p < 0x80000000)
    {
        p = (p << 1) / ((uint32_t)var1);
    }
    else
    {
        p = (p / (uint32_t)var1) * 2;
    }
    var1 = (((int32_t)dig_P9) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
    var2 = (((int32_t)(p>>2)) * ((int32_t)dig_P8))>>13;
    p = (uint32_t)((int32_t)p + ((var1 + var2 + dig_P7) >> 4));
    
    float output = p;
    return output;
}

float pressureToAltitude(float seaLevel, float atmospheric, float temp)
{
  /* Hyposometric formula:                      */
  /*                                            */
  /*     ((P0/P)^(1/5.257) - 1) * (T + 273.15)  */
  /* h = -------------------------------------  */
  /*                   0.0065                   */
  /*                                            */
  /* where: h   = height (in meters)            */
  /*        P0  = sea-level pressure (in hPa)   */
  /*        P   = atmospheric pressure (in hPa) */
  /*        T   = temperature (in C)           */  

  return (((float)pow((seaLevel/atmospheric), 0.190223F) - 1.0F)
         * (temp + 273.15F)) / 0.0065F;
}

float readFloatHumidity( void )
{
	
	// Returns humidity in %RH as unsigned 32 bit integer in Q22. 10 format (22 integer and 10 fractional bits).
	// Output value of ?47445? represents 47445/1024 = 46. 333 %RH
	int32_t adc_H = ((uint32_t)readRegister(BME280_HUMIDITY_MSB_REG) << 8) | ((uint32_t)readRegister(BME280_HUMIDITY_LSB_REG));
	int32_t var1;
	var1 = (t_fine - ((int32_t)76800));
	var1 = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * var1)) +
	((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)dig_H6)) >> 10) * (((var1 * ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
	((int32_t)dig_H2) + 8192) >> 14));
	var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
	var1 = (var1 < 0 ? 0 : var1);
	var1 = (var1 > 419430400 ? 419430400 : var1);

	return (float)(var1>>12) / 1024.0;

}

uint8_t readRegister(uint8_t offset)
{
    uint8_t result;
    I2C_Master_Start();
    I2C_Master_Write(I2CAddress);
    I2C_Master_Write(offset);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(I2CAddress2);
    result = I2C_Master_Read(0);
    I2C_Master_Stop();
    return result;
}

void writeRegister(uint8_t offset, uint8_t dataToWrite)
{
    I2C_Master_Start();
    I2C_Master_Write(I2CAddress);
    I2C_Master_Write(offset);
    I2C_Master_Write(dataToWrite);
    I2C_Master_Stop();
}

/*
float readFloatAltitudeMeters( void )
{
	float heightOutput;
    float pressure = readFloatPressure();
	
	heightOutput = ((float)-45846.2)*(pow((pressure/(float)101325.0), 0.190263) - (float)1.0);
	return heightOutput;
	
}

float readFloatAltitudeFeet( void )
{
	float heightOutput;
	
	heightOutput = readFloatAltitudeMeters() * 3.28084;
	return heightOutput;
}
float readTrueAltitude(float seaLevelPressure)
{
    // Equation taken from BMP180 datasheet (page 16):
    //  http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf

    float atmospheric = readFloatPressure() ;
    return 44330.0 * (1.0 - pow(atmospheric / seaLevelPressure, 0.190263));
}
*/

/*
  // Pressure Conversion units courtesy of ( www.endmemo.com. )
  switch(unit){
    case 0x1: // hPa 
      final /= 100.0;
      break;
    case 0x2: // inHg 
      final /= 3386.3752577878;       ( final pa * 1inHg/3386.3752577878Pa )
      break;
    case 0x3: // atm 
      final /= 101324.99766353; ( final pa * 1 atm/101324.99766353Pa )
      break;
    case 0x4: // bar 
      final /= 100000.0;          ( final pa * 1 bar/100kPa )
      break;
    case 0x5: // torr 
      final /= 133.32236534674;        ( final pa * 1 torr/133.32236534674Pa ) 
      break;
    case 0x6: // N/m^2 
      break;                      ( 1Pa / N/m^2 )
    case 0x7: // psi 
      final /= 6894.744825494;  ( final pa * 1psi/6894.744825494Pa )
*/

