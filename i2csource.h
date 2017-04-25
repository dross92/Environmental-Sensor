//** i2c lib Headers **//

//--------Master Functions--------//
void I2C_Master_Init(const unsigned long c);
void I2C_Master_Wait();
void I2C_Master_Start();
void I2C_Master_RepeatedStart();
void I2C_Master_Stop();
void I2C_Master_Write(unsigned d);
unsigned short I2C_Master_Read(unsigned short a);