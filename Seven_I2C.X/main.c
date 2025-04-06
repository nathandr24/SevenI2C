#include "mcc_generated_files/system.h"
#include "mcc_generated_files/drivers/i2c_simple_master.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/delay.h"

uint16_t trueDistance;
uint8_t  outerSensorFlag = 0;
uint8_t  innerSensorFlag = 0;

uint8_t armFlag = 0;

uint8_t motionFlag = 0;

void readOuterDistance(void);
void outerTrip(void);
void readInnerDistance(void);
void innerTrip(void);


int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
//    for(long int k = 0; k < 10000000; k++)
//        Nop();
    
//    uint8_t clearMessage = 0x76;
//    i2c_writeNBytes(0x71, &clearMessage, 1);
//    
//    uint8_t messageData = 1;
//    i2c_writeNBytes(0x71, &messageData, 1);
//        
//    messageData = 2;
//    i2c_writeNBytes(0x71, &messageData, 1);       
//    
//    messageData = 3;
//    i2c_writeNBytes(0x71, &messageData, 1);    
//    
//    messageData = 4;
//    i2c_writeNBytes(0x71, &messageData, 1);
    

    
    
//     Following is the address set for the distance sensor
//     CONNECT VPP ON BACK OF SENSOR T0 3V3 to execute
//    i2c_address_t distanceSensor = 0x40;
//    i2c_write1ByteRegister(distanceSensor, 0xEC, 0xFF); //stage 1
//    i2c_write1ByteRegister(distanceSensor, 0xC8, 0x00); //stage 2
//    i2c_write1ByteRegister(distanceSensor, 0xC9, 0x45); //stage 3
//    i2c_write1ByteRegister(distanceSensor, 0xCD, 0x10); //stage 4
//    i2c_write1ByteRegister(distanceSensor, 0xCA, 0x01); //stage 5
//    i2c_write1ByteRegister(distanceSensor, 0xCA, 0x00); //stage 6
//    i2c_write1ByteRegister(distanceSensor, 0xEF, 0x00); //stage 7
//    i2c_write1ByteRegister(distanceSensor, 0xC8, 0x40); //stage 7
//    i2c_write1ByteRegister(distanceSensor, 0xC8, 0x00); //stage 7
//    i2c_write1ByteRegister(distanceSensor, 0xEE, 0x06); //stage 8
//    i2c_write1ByteRegister(distanceSensor, 0xEF, 0x00); //stage 9
//    i2c_write1ByteRegister(distanceSensor, 0xEC, 0xFF); //stage 9
//    i2c_write1ByteRegister(distanceSensor, 0xEF, 0x03); //stage 9
//    
//    uint8_t addressCheck = 0;
//    addressCheck = i2c_read1ByteRegister(distanceSensor, 0x27);
//    if((addressCheck & 0b00001111) == 0b00000001)
//    {
//        i2c_write1ByteRegister(distanceSensor, 0xEF, 0x00); //stage 9
//        i2c_write1ByteRegister(distanceSensor, 0xEC, 0x7F); //stage 9
//        Nop();
//    }

    while (1)
    {   
//        i2c_address_t distanceSensor = 0x40;
//        uint8_t rawDistance[2] = {0,0};
//        rawDistance[1] = i2c_read1ByteRegister(distanceSensor, 0x5E); //bits 11:4
//        rawDistance[0] = i2c_read1ByteRegister(distanceSensor, 0x5F); //bits  3:0
//        trueDistance = ((uint16_t)rawDistance[1]*16 + rawDistance[0])/16/4;        
//        
//        if(trueDistance < 50)
//        {
//            uint8_t motionFlag = 1;
//        }
        readOuterDistance();
//        readInnerDistance();
        
    }

    return 1;
}

void readOuterDistance(void)
{
    IO_OUT_SENSE_SetHigh();
    
    DELAY_milliseconds(10);
    
    i2c_address_t distanceSensor = 0x40;
    uint8_t rawDistance[2] = {0,0};
    rawDistance[1] = i2c_read1ByteRegister(distanceSensor, 0x5E); //bits 11:4
    rawDistance[0] = i2c_read1ByteRegister(distanceSensor, 0x5F); //bits  3:0
    trueDistance = ((uint16_t)rawDistance[1]*16 + rawDistance[0])/16/4;        
        
    if(trueDistance < 50)
    {
        outerTrip();
    }
    
    IO_OUT_SENSE_SetLow();
}

void outerTrip(void)
{
    if(outerSensorFlag == 1)
    {
        return;
    }
    else if(innerSensorFlag == 1)
    {
        armFlag = 1;
        innerSensorFlag = 0;
        return;
    }
    else
    {
        outerSensorFlag = 1;
        return;
    }
}

void readInnerDistance(void)
{
    IO_IN_SENSE_SetHigh();
    
    DELAY_milliseconds(10);
    
    i2c_address_t distanceSensor = 0x40;
    uint8_t rawDistance[2] = {0,0};
    rawDistance[1] = i2c_read1ByteRegister(distanceSensor, 0x5E); //bits 11:4
    rawDistance[0] = i2c_read1ByteRegister(distanceSensor, 0x5F); //bits  3:0
    trueDistance = ((uint16_t)rawDistance[1]*16 + rawDistance[0])/16/4;        
        
    if(trueDistance < 50)
    {
        innerTrip();
    }
    
    IO_IN_SENSE_SetLow();
}

void innerTrip(void)
{
    if(innerSensorFlag == 1)
    {
        return;
    }
    else if(outerSensorFlag == 1)
    {
        outerSensorFlag = 0;
        return;
    }
    else 
    {
        innerSensorFlag = 1;
        return;
    }
}