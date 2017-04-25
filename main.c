/*Enviromental Sensor
 Junior Design 2017
 By Stefan Tomovic & Drew Ross 
 
-----references-----
 * https://electrosome.com/lcd-pic-mplab-xc8/
 * https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * https://ajwrs.wordpress.com/2016/11/26/sensors-bme280/
 * https://learn.sparkfun.com/tutorials/sparkfun-bme280-breakout-hookup-guide
 * http://www.microchip.com/forums/m964883.aspx
 * http://www.ermicro.com/blog/?p=2157
-------------------
 */



//---Used for True Altitude---//          //29.92 * 3386.375258 =
#define seaLvlPressure 1003.156  //default (1013.25)

#include "mcc_generated_files/mcc.h"
#include "lcd.h"
#include "i2csource.h"
#include <math.h>
#include "BMEsf.h"
 
void main()
{
    SYSTEM_Initialize();      // Initialize the PIC
    I2C_Master_Init(250000);
    lcd_Init();               // Initialize the LCD
    lcd_Clear();
    lcd_Set_Cursor(1,1);
    startSequence();
    construct();  
    BMEbegin();
    lcd_Clear();
    if (readRegister(0xD0) != 0x60)     //Chip_iD 
    {
        lcd_Set_Cursor(1,2);
        lcd_Write_String("BME ERROR");
        while(1);
    }        
        
    while(1)
    {
    //---------TEMP WINDOW--------//
        lcd_Clear();
        tempWindowC();
        __delay_ms(250);
        tempWindowC();
        __delay_ms(250);
        while(1)
        {
            if(RA0 == 1)
            {
                
                tempWindowC();
                for(int i = 0 ; i < 250 ; i++)
                {
                    __delay_ms(1);
                    if(RA1 == 0)
                    {
                        break ;
                    }
                    if(RA0 == 0)
                    {
                        lcd_Clear();
                        break ;
                    }
                }
            }
            
            else
            {
                lcd_Clear();
                tempWindowF();
                __delay_ms(250);
                tempWindowF();
                __delay_ms(250);
                while(1)
                {
                    tempWindowF();
                    for(int i = 0 ; i < 250 ; i++)
                    {   
                        __delay_ms(1);
                        if(RA0 == 0)
                        {
                            lcd_Clear();
                            break;
                        }
                        if(RA1 == 0)
                        {
                            break;
                        }
                    }
                    if(RA0 == 0)
                    {
                        lcd_Clear();
                        tempWindowC();
                        __delay_ms(250);
                        tempWindowC();
                        __delay_ms(250);
                        break;
                    }
                    if(RA1 == 0)
                    {
                        break;
                    }
                }
            }
            if(RA1 == 0)
            {
            break ;
            }
        }
    //------------------------------------//    
    //---------Baro WINDOW--------//
        lcd_Clear();
        baroWindowC();
        __delay_ms(250);
        baroWindowC();
        __delay_ms(250);
        while(1)
        {
            if(RA0 ==1)
            {
                baroWindowC();
                for(int i = 0 ; i < 250 ; i++)
                {
                    __delay_ms(1);
                    if(RA1 == 0)
                    {
                        break ;
                    }
                    if(RA0 == 0)
                    {
                        lcd_Clear();
                        break ;
                    }
                }
            }
            
            else
            {
                baroWindowF();
                __delay_ms(250);
                baroWindowF();
                __delay_ms(250);
                while(1)
                {
                    baroWindowF();
                    for(int i = 0 ; i < 250 ; i++)
                    {   
                        __delay_ms(1);
                        if(RA0 == 0)
                        {
                            lcd_Clear();
                            break;
                        }
                        if(RA1 == 0)
                        {
                            break;
                        }
                    }
                    if(RA0 == 0)
                    {
                        lcd_Clear();
                        baroWindowC();
                        __delay_ms(250);
                        baroWindowC();
                        __delay_ms(250);
                        break;
                    }
                    if(RA1 == 0)
                    {
                        break;
                    }
                }
            }
            if(RA1 == 0)
            {
            break ;
            }
        }
    //------------------------------------//
    //---------Humid WINDOW--------//
        lcd_Clear();
        humidWindowC();
        __delay_ms(250);
        humidWindowC();
        __delay_ms(250);
        while(1)
        {
            if(RA0 ==1)
            {
                humidWindowC();
                for(int i = 0 ; i < 250 ; i++)
                {
                    __delay_ms(1);
                    if(RA1 == 0)
                    {
                        break ;
                    }
                    if(RA0 == 0)
                    {
                        lcd_Clear();
                        break ;
                    }
                }
            }
            
            else
            {
                lcd_Clear();
                humidWindowF();
                __delay_ms(250);
                humidWindowF();
                __delay_ms(250);
                while(1)
                {
                    humidWindowF();
                    for(int i = 0 ; i < 250 ; i++)
                    {   
                        __delay_ms(1);
                        if(RA0 == 0)
                        {
                            lcd_Clear();
                            break;
                        }
                        if(RA1 == 0)
                        {
                            break;
                        }
                    }
                    if(RA0 == 0)
                    {
                        lcd_Clear();
                        humidWindowC();
                        __delay_ms(250);
                        humidWindowC();
                        __delay_ms(250);
                        break;
                    }
                    if(RA1 == 0)
                    {
                        break;
                    }
                }
            }
            if(RA1 == 0)
            {
            break ;
            }
        }
    //------------------------------------//    
    }   
}
          


void startSequence()
{
    lcd_Clear();
    lcd_Set_Cursor(1,1);
    lcd_Write_String(" ");
    lcd_Write_Char(0xF6);   //E
    lcd_Write_String("nvironmental ");
    lcd_Set_Cursor(2,1);
    lcd_Write_Char(0x2D);   
    lcd_Write_Char(0x2D);
    lcd_Write_Char(0x2D);
    lcd_Write_Char(0x2D);
    lcd_Write_String(" ");
    lcd_Write_String("Sensor");
    lcd_Write_String(" ");   
    lcd_Write_Char(0x2D);
    lcd_Write_Char(0x2D);
    lcd_Write_Char(0x2D);
    lcd_Write_Char(0x2D);
    __delay_ms(2000); 
    lcd_Clear();
    lcd_Set_Cursor(1,2);
    lcd_Write_String("Stefan Tomovic");
    lcd_Set_Cursor(2,2);
    lcd_Write_String("&  Drew Ross");
    __delay_ms(1000);
}

void tempWindowC ()
{
//-------- Temperature Window--------//
        char temp_str[20];
        float temp_C = readTempC();
        
        sprintf(temp_str, "Temp :  %.0f", (temp_C + 273.15));   
        lcd_Set_Cursor(2,1);
        lcd_Write_String(temp_str);
        lcd_Write_Char(0xDF);   //degrees
        lcd_Write_String("   K");
        lcd_Set_Cursor(1,1);
    //---convert temp to string & print---//
        if ( temp_C >= 0)
        {
            if ( temp_C < 10)  //(0,10)
            {
                sprintf(temp_str, "Temp :  %.1f", temp_C);      
                lcd_Write_String(temp_str);
                lcd_Write_Char(0xDF);   //degrees
                lcd_Write_String("   C");
                    
            }
            else  //(10,INF)
            {
                sprintf(temp_str, "Temp :  %.1f", temp_C);     
                lcd_Write_String(temp_str);
                lcd_Write_Char(0xDF);   //degrees
                lcd_Write_String("  C");
            }
            
        }
        else if ( temp_C <= -10)  //(-INF,-10)
        {    
            sprintf(temp_str, "Temp :  %.1f", temp_C);     
            lcd_Write_String(temp_str);
            lcd_Write_Char(0xDF);   //degrees
            lcd_Write_String(" C");
        }
        else // (-10,0)
        {    
            sprintf(temp_str, "Temp :  %.1f", temp_C);     
            lcd_Write_String(temp_str);
            lcd_Write_Char(0xDF);   //degrees
            lcd_Write_String("  C");
        }
}

void tempWindowF ()
{
    char temp_str[20];
    float temp_F = readTempF();
    float temp_C = readTempC();
    
    sprintf(temp_str, "Temp :  %.0f", (temp_C + 273.15));   
    lcd_Set_Cursor(2,1);
    lcd_Write_String(temp_str);
    lcd_Write_Char(0xDF);   //degrees
    lcd_Write_String("   K");
    lcd_Set_Cursor(1,1);
    if ( temp_F >= 0)
    {
        if (temp_F < 10)  //(0,10)
        {
            sprintf(temp_str, "Temp :  %.1f", temp_F);            
            lcd_Write_String(temp_str);
            lcd_Write_Char(0xDF);   //degrees
            lcd_Write_String("   F");
        }
        else if (temp_F >= 100)  //(100,INF)
        {
            sprintf(temp_str, "Temp :  %.1f", temp_F);   
            lcd_Write_String(temp_str);
            lcd_Write_Char(0xDF);   //degrees
            lcd_Write_String(" F");
        }
        else  //(10,100)
        {
            sprintf(temp_str, "Temp :  %.1f", temp_F);   
            lcd_Write_String(temp_str);
            lcd_Write_Char(0xDF);   //degrees
            lcd_Write_String("  F");
        }
            
    }
    else if (temp_F <= -10)  //(-INF,-10)
    {
        sprintf(temp_str, "Temp :  %.1f", temp_F);   
        lcd_Write_String(temp_str);
        lcd_Write_Char(0xDF);   //degrees
        lcd_Write_String(" F");
    }
    else  //(-10,0)
    {
        sprintf(temp_str, "Temp :  %.1f", temp_F);   
        lcd_Write_String(temp_str);
        lcd_Write_Char(0xDF);   //degrees
        lcd_Write_String("  F");
    }   
}

void baroWindowC()
{
//-------- Barometer Window--------//
        char baro_str[20];
        float t_meas = readTempC();
        float p_meas = readFloatPressure();
        //float a_meas = readTrueAltitude(seaLvlPressure);
        float a_meas = pressureToAltitude(seaLvlPressure, (p_meas/100), t_meas);
        
        //float alt = readTrueAltitude(seaLvlPressure);
        
        lcd_Set_Cursor(1,1);
//--Pres--------------------
        if((readFloatPressure() / 100) >= 1000)
        {    
            sprintf(baro_str, "Pres: %.1f", (p_meas / 100)); // hPa
            lcd_Write_String(baro_str);
            lcd_Write_String(" hPa");    //inMg, Pa, hPa
        }
        else 
        {    
            sprintf(baro_str, "Pres: %.2f", (p_meas /100 )); // hPa
            lcd_Write_String(baro_str);
            lcd_Write_String(" hPa");    //inMg, Pa, hPa        
        }
        
        lcd_Set_Cursor(2,1);
//--Alt----------------------- 
        if(a_meas >= 10000)
        {
        sprintf(baro_str, "Alt : %.0f", a_meas); 
        lcd_Write_String(baro_str);
        lcd_Write_String("  m ");
        }
        else if(a_meas >= 1000)
        {
        sprintf(baro_str, "Alt : %.0f", a_meas); 
        lcd_Write_String(baro_str);
        lcd_Write_String("    m ");
        }
        else if(a_meas >= 100)
        {
        sprintf(baro_str, "Alt : %.0f", a_meas); 
        lcd_Write_String(baro_str);
        lcd_Write_String("     m ");
        }
        else if(a_meas >= 10)
        {
        sprintf(baro_str, "Alt : %.0f", a_meas); 
        lcd_Write_String(baro_str);
        lcd_Write_String("      m ");
        }
        else if(a_meas <10)
        {    
        sprintf(baro_str, "Alt : %.0f", a_meas); 
        lcd_Write_String(baro_str);
        lcd_Write_String("       m ");
        }
}

void baroWindowF()
{
//-------- Barometer Window--------//
        char baro_str[20];
        float t_meas = readTempC();
        float p_meas = readFloatPressure();
        float a_meas = pressureToAltitude(seaLvlPressure, (p_meas/100), t_meas) * 3.28084;
        
        //float a_meas = readFloatAltitudeFeet();
       
    //---baro---
        sprintf(baro_str, "Pres: %.2f", (p_meas / 3386.375258)); //inMg
        //sprintf(baro_str, "Pres: %.2f", baro); //Pa
        //sprintf(baro_str, "Pres: %.2f", (baro / 100)); // hPa
         
        lcd_Set_Cursor(1,1);
        lcd_Write_String(baro_str);
        lcd_Write_String(" inMg");    //inMg, Pa, hPa
    //---alt--- 
        if(a_meas >= 10000)
        {
        sprintf(baro_str, "Alt : %.0f", a_meas); 
        lcd_Set_Cursor(2,1);
        lcd_Write_String(baro_str);
        lcd_Write_String("   ft");
        }
        else if(a_meas >= 1000)
        {
        sprintf(baro_str, "Alt : %.0f", a_meas); 
        lcd_Set_Cursor(2,1);
        lcd_Write_String(baro_str);
        lcd_Write_String("    ft");
        }
        else if(a_meas >= 100)
        {
        sprintf(baro_str, "Alt : %.0f", a_meas); 
        lcd_Set_Cursor(2,1);
        lcd_Write_String(baro_str);
        lcd_Write_String("     ft");
        }
        else if(a_meas >= 10)
        {
        sprintf(baro_str, "Alt : %.0f", a_meas); 
        lcd_Set_Cursor(2,1);
        lcd_Write_String(baro_str);
        lcd_Write_String("      ft");
        }
        else if(a_meas < 10)
        {
        sprintf(baro_str, "Alt : %.0f", a_meas); 
        lcd_Set_Cursor(2,1);
        lcd_Write_String(baro_str);
        lcd_Write_String("       ft");  
        }
}
   
void humidWindowC()
{
//-------- Humidity Window--------//     
        char humid_str[20];
        float temp_C = readTempC();
        float h_meas = readFloatHumidity();
        float d_meas = (temp_C -((100 - h_meas)/5));
        
    //---hummid---\
        
        if(h_meas == 100)
        {
        sprintf(humid_str, "Rel Hum: %.1f", h_meas);     
        lcd_Set_Cursor(1,1);
        lcd_Write_String(humid_str);
        lcd_Write_String(" ");
        lcd_Write_Char(0x25);   //Percent            
        }
        else
        {    
        sprintf(humid_str, "Rel Hum: %.1f", h_meas);     
        lcd_Set_Cursor(1,1);
        lcd_Write_String(humid_str);
        lcd_Write_String("  ");
        lcd_Write_Char(0x25);   //Percent
        }
    //---Dew Point---      dewpoint = degreesC-((100-humidity)/5)
        if ( d_meas >= 10 )
        {        
            sprintf(humid_str, "Dew Pt : %.1f", d_meas);
            lcd_Set_Cursor(2,1);
            lcd_Write_String(humid_str);
            lcd_Write_Char(0xDF);   //degrees
            lcd_Write_String(" C");   
        }
        else if (d_meas < 0)
        {
            sprintf(humid_str, "Dew Pt : %.1f", d_meas);
            lcd_Set_Cursor(2,1);
            lcd_Write_String(humid_str);
            lcd_Write_Char(0xDF);   //degrees
            lcd_Write_String(" C");
        }
        else if (d_meas >= 0)
        {        
            sprintf(humid_str, "Dew Pt : %.1f", d_meas);
            lcd_Set_Cursor(2,1);
            lcd_Write_String(humid_str);
            lcd_Write_Char(0xDF);   //degrees
            lcd_Write_String("  C");
        }
        
}

void humidWindowF()
{
//-------- Humidity Window--------//     
        char humid_str[20];
        float h_meas = readFloatHumidity();
        float d_meas = ((readTempC()-((100 - h_meas)/5)) * (9 / 5) + 32);
        //float humid = readFloatHumidity();
    //---humnid---
        if( h_meas == 100 )
        {
        sprintf(humid_str, "Rel Hum: %.1f", h_meas);     
        lcd_Set_Cursor(1,1);
        lcd_Write_String(humid_str);
        lcd_Write_String(" ");
        lcd_Write_Char(0x25);   //Percent            
        }
        else
        {    
        sprintf(humid_str, "Rel Hum: %.1f", h_meas);     
        lcd_Set_Cursor(1,1);
        lcd_Write_String(humid_str);
        lcd_Write_String("  ");
        lcd_Write_Char(0x25);   //Percent
        }
    //---Dew Point---      dewpoint = degreesC-((100-humidity)/5) 
            if (d_meas >= 10)
            {
                sprintf(humid_str, "Dew Pt : %.1f", d_meas);
                lcd_Set_Cursor(2,1);
                lcd_Write_String(humid_str);
                lcd_Write_Char(0xDF);   //degrees
                lcd_Write_String(" F");
            }
            else if (d_meas < 0)
            {
                sprintf(humid_str, "Dew Pt : %.1f", d_meas);
                lcd_Set_Cursor(2,1);
                lcd_Write_String(humid_str);
                lcd_Write_Char(0xDF);   //degrees
                lcd_Write_String(" F");   
            }
            else if (d_meas >= 0)
            {
                sprintf(humid_str, "Dew Pt : %.1f", d_meas);
                lcd_Set_Cursor(2,1);
                lcd_Write_String(humid_str);
                lcd_Write_Char(0xDF);   //degrees
                lcd_Write_String("  F");
            }    
}



