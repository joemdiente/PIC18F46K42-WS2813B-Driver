/*
 * File:   my_spi.c
 * Author: joemd
 * Note: This code is made specific for PIC18F46K42
 * Created on December 28, 2021, 5:14 PM
 */

#include <xc.h>
#include <stdio.h>

#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/spi1.h"

void spi_send(uint8_t data, uint8_t count)
{
    SPI1TCNT = count;

    SPI1CON2bits.RXR = 0;
    SPI1CON2bits.TXR = 1;

    LATCbits.LATC2 = 0;

    SPI1_WriteByte(data);

    //Note 1 for BUSY bit
    Nop();
    Nop();
    while(SPI1CON2bits.BUSY == 1);
    SPI1STATUSbits.CLRBF = 1;
    while(SPI1INTFbits.SRMTIF != 1);

    LATCbits.LATC2 = 1;
}
