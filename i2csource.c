#include "mcc_generated_files/mcc.h"


//--------Master Functions--------/
//-----------------------------------
//Initialize I2C Module as Master

void I2C_Master_Init(const unsigned long c)
{
  SSPCON1 = 0b00101000;           //SSP Module as Master
  SSPCON2 = 0;
  SSPADD = (_XTAL_FREQ/(4*c))-1;  //Set Clock Speed (Hz)
  SSPSTAT = 0b00000000;            //Disable Slew
  TRISB4 = 1;                     //Setting as input as given in datasheet
  TRISB6 = 1;                     //Setting as input as given in datasheet
  LATB4 = 0;
  LATB6 = 0;
}

//For Waiting
void I2C_Master_Wait()
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); //Transmit is in progress
}

//Start Condition
void I2C_Master_Start()
{
  I2C_Master_Wait();
  SSPCON2bits.SEN = 1;             //Initiate start condition
  //while(SSPCON2bits.SEN);
}

//Repeated Start
void I2C_Master_RepeatedStart()
{
  I2C_Master_Wait();
  SSPCON2bits.RSEN = 1;           //Initiate repeated start condition
  //while(SSPCON2bits.RSEN);
}

//Stop Condition
void I2C_Master_Stop()
{
  I2C_Master_Wait();
  SSPCON2bits.PEN = 1;           //Initiate stop condition
}

//Write Data
void I2C_Master_Write(unsigned d)
{
  I2C_Master_Wait();
  SSPBUF = d;         //Write data to SSPBUF
}

//Read Data
unsigned short I2C_Master_Read(unsigned short a)
{
  unsigned short temp;
  I2C_Master_Wait();
  SSPCON2bits.RCEN = 1;
  I2C_Master_Wait();
  temp = SSPBUF;      //Read data from SSPBUF
  I2C_Master_Wait();
  SSPCON2bits.ACKDT = (a)?0:1;    //Acknowledge bit
  SSPCON2bits.ACKEN = 1;          //Acknowledge sequence
  return temp;
}

