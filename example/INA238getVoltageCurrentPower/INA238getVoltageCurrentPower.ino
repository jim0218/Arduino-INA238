#include <INA238.h>
#include <Wire.h>

/**
 * @fn INA238_IIC
 * @brief pWire I2C controller pointer
 * @param i2caddr  I2C address
 * @n INA238_I2C_ADDRESS1  0x40   A0 = 0  A1 = 0
 * @n INA238_I2C_ADDRESS2  0x41   A0 = 1  A1 = 0
 * @n INA238_I2C_ADDRESS3  0x44   A0 = 0  A1 = 1
 * @n INA238_I2C_ADDRESS4  0x45   A0 = 1  A1 = 1   
  */
INA238_IIC     ina238(&Wire, INA238_I2C_ADDRESS1);

void setup(void) 
{
    Serial.begin(115200);
    //Open the serial port
    while(!Serial);
    
    Serial.println();
    //Initialize the sensor
    while(ina238.begin() != true) {
        Serial.println("INA238 begin faild");
        delay(1000);
    }
   Serial.println();
}

void loop(void)
{
    Serial.print("BusVoltage:   ");
    Serial.print(ina238.getBusVoltage_V(), 2);
    Serial.println("V");
    Serial.print("ShuntVoltage: ");
    Serial.print(ina238.getShuntVoltage_mV(), 3);
    Serial.println("mV");
    Serial.print("Current:      ");
    Serial.print(ina238.getCurrent_mA(), 1);
    Serial.println("mA");
    Serial.print("Power:        ");
    Serial.print(ina238.getPower_mW(), 1);
    Serial.println("mW");
    Serial.println("");
    delay(1000);
}
