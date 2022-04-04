/*
 * File:   app.c
 * Author: joemd
 *
 * Created on December 26, 2021, 12:17 PM
 */

/* Dependencies */
#include <xc.h>
#include <stdio.h>

/* App Header */
#include "app.h"
#include "my_spi.h"
#include "mcc_generated_files/spi1.h"

void myLEDArray(uint8_t r, uint8_t g, uint8_t b) 
{
    SPI1TXB = g;
    while (!SPI1STATUSbits.TXBE);
    SPI1TXB = r;
    while (!SPI1STATUSbits.TXBE);
    SPI1TXB = b;
    while (!SPI1STATUSbits.TXBE);
}

void myLEDReset(void) {
    
    __delay_us(300);
    
}

void myLEDClear(uint16_t led_count) 
{ 
    
    for (int i = 0; i < led_count; i++) {
        myLEDArray(0,0,0);
    };

    myLEDReset();
}

void myLEDSnake(uint16_t led_count) 
{
    int i = 0, j = 0;
    
    for (i = 0; i < led_count; i++) 
    {
        for(j = 0; j <= i; j++) 
        {
            if(j == i)
            {
                myLEDArray(0,0,255);
            }
            else
            {
                myLEDArray(0,0,0);
            }
        }
        myLEDReset();
        __delay_ms(100);
    }
}
/*
 */
void my_app(void) 
{
     
    int i = 0, j = 0;

    myLEDClear(1024);
    
    //LED Test
    printf("LED Boot\r\n");
    for(i = 0; i < 10; i++)
    {
        myLEDArray(255,0,0);
    }
    __delay_ms(2000);
    
    myLEDClear(1024);
    
    while(1) {
        
        //Insert LED Commands Here       
        myLEDClear(256);
        printf("loop\r\n");
    }
    return;
}
