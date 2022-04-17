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

/* App.h */
#define RGB_DATA_SIZE_IN_BYTES 3
#define MAX_NUMBER_OF_LEDS 20
#define LED_COUNT (MAX_NUMBER_OF_LEDS + 1) //Dummy LED
#define LED_STRIP
#define LED_TEST_DATA {r,g,b} (printf("r: %u, g: %u b: %u \r\n", r, g, b);)

/**
 * 2D Matrix for Storing LED
 * Pros: storing to memory. Can be addressed using uint8_t Pointers. Already packed(?). Graphics
 * 
 * Usage:
 *  char matrix_name[RGB LED INDEX][24-bit RGB].
 * 
 * Example:
 * [      INDEX 1     ]   ;RGB LED INDEX
 *   |      |       |
 * [RED] [GREEN] [BLUE]   ;24-bit RGB
 */
char led_strip_matrix[MAX_NUMBER_OF_LEDS][RGB_DATA_SIZE_IN_BYTES];

//OR

/**
 * typedef struct for led strip
 * Pros: Individual LED are directly addressable in code. Not sure if struct matrix is packed. Only for strip(?)
 * 
 * Usage:
 *  for single LED
 *      rgb_data <variable name>;
 *  for LED strip
 *      rgb_data <variable name>[number of LED];
 * 
 * Example:
 *  Passing pointer to member:
 *     <variable name>[x]->red_data, etc
 *  Assigning value:
 *     <variable name>[x]->red_data, etc  
 */
typedef struct __attribute__((packed)) {
    uint8_t red_data;
    uint8_t green_data;
    uint8_t blue_data;
} rgb_data;

rgb_data led_strip_struct[MAX_NUMBER_OF_LEDS];

void WS2813_Transmit_Data(uint8_t r, uint8_t g, uint8_t b) 
{

#ifdef TEST
    printf("r: %u, g: %u b: %u \r\n", r, g, b);
#endif 
    
    SPI1TXB = (uint8_t)g;
    while (!SPI1STATUSbits.TXBE);
    SPI1TXB = (uint8_t)r;
    while (!SPI1STATUSbits.TXBE);
    SPI1TXB = (uint8_t)b;
    while (!SPI1STATUSbits.TXBE);
}

void myLEDTransfer(rgb_data* led_strip)
{
     WS2813_Transmit_Data(led_strip->red_data, led_strip->green_data, led_strip->blue_data);
}
void myLEDReset(void) {
    
    //Reset Data Transfer 300us or more (page 5))
    __delay_us(301);   
}

void myLEDClear(uint16_t led_count) 
{     
    for (int i = 0; i < led_count; i++) {
        WS2813_Transmit_Data(0,0,0);
    };

    myLEDReset();
}
/** 
 * myLEDSnake Function
 * Demo Function
 */
void myLEDSnake(uint16_t led_count) 
{
    int i = 0, j = 0;
    
    for (i = 0; i <= led_count; i++) 
    {
        for(j = 0; j <= i; j++) 
        {
            if(j == i)
            {
                WS2813_Transmit_Data(0,0,255);
            }
            else
            {
                WS2813_Transmit_Data(0,0,0);
            }
        }
        myLEDReset();
        __delay_ms(50);
    }

}
/**
 * my_app
 */
void my_app(void) 
{
     
    int i = 0, j = 0;

    printf("LED Clear\r\n");
    myLEDClear(LED_COUNT);

    //LED Test
    printf("LED Boot\r\n");
    for(i = 0; i < LED_COUNT; i++)
    {
        WS2813_Transmit_Data(0,255,0);
    }
    
    //One Second Delay Start
    __delay_ms(1000);
    
    /* Populate led strip data */
    for (i = 0; i < LED_COUNT; i++)
    {
        led_strip_struct[i].red_data = (uint8_t)(255 - i*(255/LED_COUNT));
        led_strip_struct[i].green_data = 0;
        led_strip_struct[i].blue_data = 0;
    }
    
    while(1) {
        
        
        //LED Breath        
        for (j = 255; j > 0; j--)
        {   
            for (i = 0; i < LED_COUNT; i++)
            {
                led_strip_struct[i].red_data = j;     
                led_strip_struct[i].green_data = 0;
                led_strip_struct[i].blue_data = 0;
                myLEDTransfer(&led_strip_struct[i]);
            }
            __delay_ms(5);
        }
        
        for (j = 0; j < 255; j++)
        {   
            for (i = 0; i < LED_COUNT; i++)
            {
                led_strip_struct[i].red_data = j;     
                led_strip_struct[i].green_data = 0;
                led_strip_struct[i].blue_data = 0;
                myLEDTransfer(&led_strip_struct[i]);
            }
            __delay_ms(5);
        }
        
            
        //Insert LED Commands Here  
//        for (i = 0; i < LED_COUNT; i++)
//        {
//            myLEDTransfer(&myLEDStrip[i]);
//        }
             
        //Demo App
        myLEDSnake(LED_COUNT);
        
        //Loop
        printf("loop\r\n");
        myLEDClear(LED_COUNT);

    }
    return;
}
