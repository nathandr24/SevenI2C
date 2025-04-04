#include "mcc_generated_files/system.h"
#include "mcc_generated_files/drivers/i2c_simple_master.h"

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
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
    
    i2c_address_t distanceSensor = 0x40;
    uint8_t rawDistance[2] = {0,0};
    rawDistance[1] = i2c_read1ByteRegister(distanceSensor, 0x5E); //bits 11:4
    rawDistance[0] = i2c_read1ByteRegister(distanceSensor, 0x5F); //bits  3:0
    
    float trueDistance = 0;
    trueDistance = ((uint16_t)rawDistance[1]*16 + rawDistance[0])/16/4;
    
    
    // Following is the address set for the distance sensor
//    i2c_address_t distanceSensor = 0x40;
//    i2c_write1ByteRegister(distanceSensor, 0xEC, 0xFF); //stage 1
//    i2c_write1ByteRegister(distanceSensor, 0xC8, 0x00); //stage 2
//    i2c_write1ByteRegister(distanceSensor, 0xC9, 0x45); //stage 3
//    i2c_write1ByteRegister(distanceSensor, 0xCD, 0x00); //stage 4
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
//    if((addressCheck & 0b00011111) == 0b00000000)
//    {
//        i2c_write1ByteRegister(distanceSensor, 0xEF, 0x00); //stage 9
//        i2c_write1ByteRegister(distanceSensor, 0xEC, 0x7F); //stage 9
//        Nop();
//    }
    
    
    while (1)
    {   

    }

    return 1;
}
