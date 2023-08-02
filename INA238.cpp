#include <Arduino.h>
#include <Wire.h>
#include <INA238.h>

bool INA238::begin()
{
    lastOperateStatus = eIna238_InitError;
    Wire.begin();
    if (scan() == true)
    {       
        setBADC(eIna238_conversion_time_540);       
        setSADC(eIna238_conversion_time_540);       
        setSAMPLE(eIna238AdcSample_4);
        setADCRange(eIna238AdcRange_0);
        setMode(eIna238SBVolCon);       
        lastOperateStatus = eIna238_ok;
        return true;
    }
    else{
        return false;
    }
}

//Current_LSB = 10 / 2^15 = 305.176u

float INA238::getBusVoltage_V()
{
    return (float) (readInaRegUnsigned(INA238_VBUS)) * 3.125 * 0.001;
}

float INA238::getShuntVoltage_mV()
{
    return (float) readInaReg(INA238_VSHUNT) * 5 * 0.001;
}

float INA238::getCurrent_mA()
{
    return (float) readInaReg(INA238_CURRENT) *  305.176 *0.001;
}

float INA238::getPower_mW()
{
    return (float) readInaRegpower(INA238_POWER) * 61.035156 * 0.001 ;
}

/* Selects ADC sample averaging count  */
void INA238::setSAMPLE(eIna238AdcSample_t sample)
{
    int16_t    conf;      
    conf = readInaReg(INA238_ADC_CONFIG);
    conf &= ~((uint16_t) 0x07);
    conf |= (uint16_t) sample ;
    writeInaReg(INA238_ADC_CONFIG, conf);
}

/* Selects Shunt Voltage conversion time */
void INA238::setSADC(eIna238_conversion_time_t time)
{
    int16_t    conf;       
    conf = readInaReg(INA238_ADC_CONFIG);
    conf &= ~((uint16_t) 0x07 << 6);
    conf |= (uint16_t) time << 6;
    writeInaReg(INA238_ADC_CONFIG, conf);
}

/* Selects Bus Voltage conversion time */
void INA238::setBADC(eIna238_conversion_time_t time)
{
    int16_t    conf;        
    conf = readInaReg(INA238_ADC_CONFIG);
    conf &= ~((uint16_t) 0x07 << 9);    // 清空   ~:數值反向  &:and  原值 AND 0  = 0   
    conf |= (uint16_t) time << 9;       // 寫入   |:or              輸入 OR  0  = 輸入值
    writeInaReg(INA238_ADC_CONFIG, conf);
}

/*Selects continuous, triggered, or power-down mode of operation*/
void INA238::setMode(eIna238Mode_t mode)
{
    int16_t    conf;
    conf = readInaReg(INA238_ADC_CONFIG);
    conf &= ~((uint16_t) 0x0f << 12);
    conf |= (uint16_t) mode <<12;
    writeInaReg(INA238_ADC_CONFIG, conf);
}

/* Shunt full scale range selection across IN+ and IN–.  */
void INA238::setADCRange(eIna238AdcRange_t range)
{
    int16_t    conf;      
    conf = readInaReg(INA238_CONFIG);
    conf &= ~((uint16_t) 0x01 <<4);
    conf |= (uint16_t) range <<4 ;
    writeInaReg(INA238_CONFIG, conf);
}

/* 回到預設值(default values) Mode=Fh  BADC=5h  SADC=5h SAMPLE=0h */
void INA238::reset()
{
    int16_t    conf;      
    conf = readInaReg(INA238_CONFIG);    
    conf |= (uint16_t) 0x01 <<15 ;
    writeInaReg(INA238_CONFIG, conf); 

}

void INA238_IIC::writeReg(uint8_t reg, uint8_t *pBuf, uint16_t len)
{
    lastOperateStatus = eIna238_WriteRegError;
    _pWire->begin();
    _pWire->beginTransmission(_addr);
    _pWire->write(reg);
    for(uint16_t i = 0; i < len; i ++)
        _pWire->write(pBuf[i]);
    _pWire->endTransmission();
    lastOperateStatus = eIna238_ok;
}

void INA238_IIC::readReg(uint8_t reg, uint8_t *pBuf, uint32_t len)
{
    lastOperateStatus = eIna238_ReadRegError;
    _pWire->begin();
    _pWire->beginTransmission(_addr);
    _pWire->write(reg);
    if(_pWire->endTransmission() != 0)
        return;
    _pWire->requestFrom(_addr, (uint8_t) len);
    for(uint16_t i = 0; i < len; i ++) {
        pBuf[i] = _pWire->read();
    }
    _pWire->endTransmission();
    lastOperateStatus = eIna238_ok;
}

int16_t INA238::readInaReg(uint8_t reg)
{
    uint8_t buf[2] = {0};
    
    readReg(reg, buf, sizeof(buf));
    return (buf[0] << 8) | buf[1];
}

uint16_t INA238::readInaRegUnsigned(uint8_t reg)
{
    uint8_t buf[2] = {0};

    readReg(reg, buf, sizeof(buf));
    return (buf[0] << 8) | buf[1];
}
int32_t INA238::readInaRegpower(uint8_t reg)
{
    uint8_t buf[3] = {0};
    
    readReg(reg, buf, sizeof(buf));
    return (buf[0] << 16) |(buf[1] << 8) | buf[2];
}

void INA238::writeInaReg(uint8_t reg, uint16_t value)
{
    uint8_t buffer[2] = {(value >> 8), value & 0xff};
    writeReg(reg, buffer, 2);
}

bool INA238_IIC::scan()
{
    _pWire->beginTransmission(_addr);
    if (_pWire->endTransmission() == 0){
        return true;
    }
    return false;
}
