/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "mcc.h"
#include "i2c1_driver.h"

#ifndef FCY
#define FCY _XTAL_FREQ/2
#endif

#include <libpic30.h>

void (*i2c1_driver_busCollisionISR)(void);
void (*i2c1_driver_Masteri2cISR)(void);
void (*i2c1_driver_Slavei2cISR)(void);

void i2c1_driver_close(void)
{
    I2C1CONbits.I2CEN = 0;
}

/* Interrupt Control */
void i2c1_enableIRQ(void)
{
    IEC1bits.MI2C1IE = 1;
    IEC1bits.SI2C1IE = 1;
}

bool i2c1_IRQisEnabled(void)
{
    return IEC1bits.MI2C1IE || IEC1bits.SI2C1IE;
}

void i2c1_disableIRQ(void)
{
    IEC1bits.MI2C1IE = 0;
    IEC1bits.SI2C1IE = 0;
}

void i2c1_clearIRQ(void)
{
    IFS1bits.MI2C1IF = 0;
    IFS1bits.SI2C1IF = 0;
}

void i2c1_setIRQ(void)
{
    IFS1bits.MI2C1IF = 1;
    IFS1bits.SI2C1IF = 1;
}

void i2c1_waitForEvent(uint16_t *timeout)
{
    //uint16_t to = (timeout!=NULL)?*timeout:100;
    //to <<= 8;
    if((IFS1bits.MI2C1IF == 0) && (IFS1bits.SI2C1IF == 0))
    {
        while(1)// to--)
        {
            if(IFS1bits.MI2C1IF || IFS1bits.SI2C1IF) break;
            __delay_us(100);
        }
    }
}

bool i2c1_driver_driver_open(void)
{
    if(!I2C1CONbits.I2CEN)
    {
        // initialize the hardware
        // STAT Setting 
        I2C1STAT = 0x0;
        
        // CON Setting
        I2C1CON = 0x8000;
        
        // Baud Rate Generator Value: I2CBRG 100000;   
        I2C1BRG = 157;
        
        return true;
    }
    else
        return false;
}
bool i2c1_driver_initSlaveHardware(void)
{
    if(!I2C1CONbits.I2CEN)
    {


/* NOTE on SEN:
 * SEN will be set enabling clock stretching.  This is because we don't know how
 * long the user will take to process data bytes in their callbacks.  If they are fast,
 * we may not need to stretch the clock.  If they are slow, we need to stretch the clock.
 * If we ALWAYS stretch the clock, we will release the clock when the ISR is complete.
 */

/* NOTE on PCIE:
 * PCIE will be set to enable interrupts on STOP.  This will allow us know when
 * the master is finished
 */
        
/* NOTE on SCIE:
 * SCIE will be set to enable interrupts on START.  This will allow us to detect
 * both a START and a RESTART event and prepare to restart communications.
 */
        I2C1CONbits.STREN = 1;
        
        I2C1CONbits.I2CEN = 1;
        return true;
    }
    return false;
}

void i2c1_driver_resetBus(void)
{
    
}

void i2c1_driver_start(void)
{
    I2C1CONbits.SEN = 1;
}

void i2c1_driver_restart(void)
{
    I2C1CONbits.RSEN = 1;
}

void i2c1_driver_stop(void)
{
    I2C1CONbits.PEN = 1;
}

bool i2c1_driver_isNACK(void)
{
    return I2C1STATbits.ACKSTAT;
}

void i2c1_driver_startRX(void)
{
    I2C1CONbits.RCEN = 1;
}

char i2c1_driver_getRXData(void)
{
    return I2C1RCV;
}

void i2c1_driver_TXData(uint8_t d)
{
    I2C1TRN = d;
}

void i2c1_driver_sendACK(void)
{
    I2C1CONbits.ACKDT = 0;
    I2C1CONbits.ACKEN = 1; // start the ACK/NACK
}

void i2c1_driver_sendNACK(void)
{
    I2C1CONbits.ACKDT = 1;
    I2C1CONbits.ACKEN = 1; // start the ACK/NACK
}

void i2c1_driver_releaseClock(void)
{
    I2C1CONbits.SCLREL = 1;
}

bool i2c1_driver_isBuferFull(void)
{
    return I2C1STATbits.RBF || I2C1STATbits.TBF;
}

bool i2c1_driver_isStart(void)
{
    return I2C1STATbits.S;
}

bool i2c1_driver_isAddress(void)
{
    return !I2C1STATbits.D_A;
}

bool i2c1_driver_isStop(void)
{
    return I2C1STATbits.P;
}

bool i2c1_driver_isData(void)
{
    return I2C1STATbits.D_A;
}

bool i2c1_driver_isRead(void)
{
    return I2C1STATbits.R_W;
}

void i2c1_driver_clearBusCollision(void)
{
    I2C1STATbits.BCL = 0; // clear the bus collision.
}

void i2c1_driver_enableStartIRQ(void)
{
    
}

void i2c1_driver_disableStartIRQ(void)
{
    
}

void i2c1_driver_enableStopIRQ(void)
{
    
}

void i2c1_driver_disableStopIRQ(void)
{
    
}

void i2c1_driver_setBusCollisionISR(interruptHandler handler){
    i2c1_driver_busCollisionISR = handler;
}

void i2c1_driver_setMasterI2cISR(interruptHandler handler){
    i2c1_driver_Masteri2cISR = handler;
}

void i2c1_driver_setSlaveI2cISR(interruptHandler handler){
    i2c1_driver_Slavei2cISR = handler;
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _MI2C1Interrupt ( void )
{
    (*i2c1_driver_Masteri2cISR)();
}
void __attribute__ ( ( interrupt, no_auto_psv ) ) _SI2C1Interrupt ( void )
{
    (*i2c1_driver_Slavei2cISR)();
}
