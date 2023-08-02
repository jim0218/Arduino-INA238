#ifndef INA238_H
#define INA238_H
#include <Wire.h>

#define INA238_I2C_ADDRESS1                      (0x40)///< I2C ADDRESS 1
#define INA238_I2C_ADDRESS2                      (0x41)///< I2C ADDRESS 2
#define INA238_I2C_ADDRESS3                      (0x44)///< I2C ADDRESS 3
#define INA238_I2C_ADDRESS4                      (0x45)///< I2C ADDRESS 4


#define INA238_CONFIG         (0x00) 
#define INA238_ADC_CONFIG     (0x01)
#define INA238_SHUNT_CAL      (0x02)
#define INA238_VSHUNT         (0x04)
#define INA238_VBUS           (0x05)
#define INA238_DIETEMP        (0x06)
#define INA238_CURRENT        (0x07)
#define INA238_POWER          (0x08)
#define INA238_DIAG_ALRTv     (0x0B)
#define INA238_SOVL           (0x0C)
#define INA238_SUVL           (0x0D)
#define INA238_BOVL           (0x0E)
#define INA238_BUVL           (0x0F)
#define INA238_TEMP_LIMIT     (0x10)
#define INA238_PWR_LIMIT      (0x11)
#define INA238_MANUFACTURER_ID (0x3E)
#define INA238_DEVICE_ID      (0x3F)


class INA238
{
public:
/**
 * @enum eIna238Status_t
 * @brief Error code
 * @details Explain errors that occur when running the program
 */
typedef enum {
    eIna238_ok,/**<No error */
    eIna238_InitError,/**<Init error */
    eIna238_WriteRegError,/**<Write register error */
    eIna238_ReadRegError,/**<Read register error */
}eIna238Status_t;


/**
 * @enum eIna238_conversion_time_t
 * @brief conversion-time
 */
typedef enum {
    eIna238_conversion_time_50,  //0h 0x00  50us
    eIna238_conversion_time_84,   //1h 0x01  84us
    eIna238_conversion_time_150,  //2h 0x02  150us
    eIna238_conversion_time_280,  //3h 0x03  280us
    eIna238_conversion_time_540,  //4h 0x04  540us
    eIna238_conversion_time_1052,  //5h 0x05  1052us
    eIna238_conversion_time_2074,  //6h 0x06  2074us
    eIna238_conversion_time_4120   //7h 0x07  4120us
} eIna238_conversion_time_t;


/**
 * @enum eIna238AdcSample_t
 * @brief Sample averaging count.
 */
typedef enum {
    eIna238AdcSample_1,    //0h 0x00
    eIna238AdcSample_4,    //1h 0x01
    eIna238AdcSample_16,   //2h 0x02
    eIna238AdcSample_64,   //3h 0x03
    eIna238AdcSample_128,  //4h 0x04
    eIna238AdcSample_256,  //5h 0x05
    eIna238AdcSample_512,  //6h 0x06
    eIna238AdcSample_1024  //7h 0x07
} eIna238AdcSample_t;


/**
 * @enum eIna238AdcRange_t
 * @brief  VSHUNT : ADCRANGE = 0 --> 5 μV/LSB   ADCRANGE = 1 --> 1.25 μV/LSB
 */
typedef enum {
    eIna238AdcRange_0,    //0h 0x00
    eIna238AdcRange_1,    //1h 0x01    
} eIna238AdcRange_t;


/**
 * @enum eInaMode_t
 * @brief Operating Mode
 */
typedef enum{
    eIna238Shutdown1,      //Shutdown                                               0h
    eIna238BVolTri,        //Triggered bus voltage                                  1h
    eIna238SVolTri,        //Triggered shunt voltage                                2h
    eIna238SBVolTri,       //Triggered shunt voltage and bus voltage                3h
    eIna238TempTri,        //Triggered temperature                                  4h
    eIna238TempBVolTri,    //Triggered temperature and bus voltage                  5h
    eIna238TempSVolTri,    //Triggered temperature and shunt voltage                6h
    eIna238TempSBVolTri,   //Triggered bus voltage, shunt voltage and temperature   7h
    eIna238Shutdown2,      //Shutdown                                               8h
    eIna238BVolCon,        //Continuous bus voltage                                 9h
    eIna238SVolCon,        //Continuous shunt voltage                               Ah 10
    eIna238SBVolCon,       //Continuous shunt and bus voltage                       Bh 11
    eIna238TempCon,        //Continuous temperature                                 Ch 12
    eIna238TempBVolCon,    //Continuous bus voltage and temperature                 Dh 13
    eIna238TempSVolCon,    //Continuous temperature and shunt voltage               Eh 14
    eIna238TempSBVolCon   //Continuous bus voltage shunt voltage and temperature    Fh 15
    
} eIna238Mode_t;


public:
    /**
     * @fn INA238
     * @brief Constructor
     * @return None
     */
    INA238(){};

    /**
     * @fn begin
     * @brief Initialize I2C bus and configure INA238 config register before reading data
     * @return true (succeeded)/false (failed)
     */
    bool begin();
    	
    /**
     * @fn reset
     * @brief Reset config register
     */
    void reset();

    /**
     * @fn getBusVoltage_V
     * @brief  get the BusVoltage （Voltage of IN- to GND)
     * @return Voltage unit:V
     */
    float getBusVoltage_V();

    /**
     * @fn getShuntVoltage_mV
     * @brief  get the ShuntVoltage （Voltage of the sampling resistor, IN+ to NI-)
     * @return Voltage unit:mV
     */
    float getShuntVoltage_mV();
    
    /**
     * @fn getCurrent_mA
     * @brief get the Current(Current flows across IN+ and IN-.
     * @return Current unit:mA
     */
    float getCurrent_mA();
    
    /**
     * @fn getPower_mW
     * @brief Get power     
     * @return power unit：mW
     */
     float getPower_mW();
 
    /**
     * @fn setBADC default=eIna238AdcBits_5
     * @brief  Selects Bus Voltage conversion time
     * @param bits           Resolution     
     * @n                    eIna238AdcBits_0h, 50us
     * @n                    eIna238AdcBits_1h, 84us
     * @n                    eIna238AdcBits_2h, 150us
     * @n                    eIna238AdcBits_3h, 280us
     * @n                    eIna238AdcBits_4h, 540us
     * @n                    eIna238AdcBits_5h, 1052us
     * @n                    eIna238AdcBits_6h, 2047us
     * @n                    eIna238AdcBits_7h  4120us     
     */
    void  setBADC(eIna238_conversion_time_t time);

    /**
     * @fn setSADC default=eIna238AdcBits_5
     * @brief  Selects Shunt Voltage conversion time
     * @param bits           Resolution
     * @n                    eIna238AdcBits_0, 50us
     * @n                    eIna238AdcBits_1, 84us
     * @n                    eIna238AdcBits_2, 150us
     * @n                    eIna238AdcBits_3, 280us
     * @n                    eIna238AdcBits_4, 540us
     * @n                    eIna238AdcBits_5, 1052us
     * @n                    eIna238AdcBits_6, 2047us
     * @n                    eIna238AdcBits_7  4120us
     */
    void  setSADC(eIna238_conversion_time_t time);

    /**
     * @fn setSample default=eIna238AdcSample_1
     * @brief  ADC sample averaging count     
     * @param sample         Sample size
     * @n                    eIna238AdcSample_1,   0h
     * @n                    eIna238AdcSample_4,   1h
     * @n                    eIna238AdcSample_16,  2h
     * @n                    eIna238AdcSample_64,  3h
     * @n                    eIna238AdcSample_128, 4h
     * @n                    eIna238AdcSample_256, 5h
     * @n                    eIna238AdcSample_512, 5h
     * @n                    eIna238AdcSample_1024 7h
     */
    void  setSAMPLE(eIna238AdcSample_t sample);

    /**
     * @fn setADCRange
     * @brief  Set ADCRange   VSHUNT : ADCRANGE = 0 --> 5 μV/LSB   ADCRANGE = 1 --> 1.25 μV/LSB
     * @param range          range  0 or 1
     * @n                    eIna238AdcRange_0, 0h = ±163.84 mV  
     * @n                    eIna238AdcRange_1, 1h = ± 40.96 mV    
     */
    void  setADCRange(eIna238AdcRange_t range);

    /**
     * @fn setMode default=eIna238TempSBVolCon<Continuous bus voltage shunt voltage and temperature>
     * @brief  Set operation Mode
     * @param mode       
     * @n                eIna238Shutdown1,    <Shutdown>                                                                 0h
     * @n                eIna238BVolTri,      <Triggered bus voltage, single shot>                                       1h
     * @n                eIna238SVolTri,      <Triggered shunt voltage, single shot>                                     2h
     * @n                eIna238SBVolTri,     <Triggered shunt voltage and bus voltage, single shot>                     3h
     * @n                eIna238TempTri,      <Triggered temperature, single shot>                                       4h
     * @n                eIna238TempBVolTri,  <Triggered temperature and bus voltage, single shot>                       5h
     * @n                eIna238TempSVolTri,  <Triggered temperature and shunt voltage, single shot>                     6h
     * @n                eIna238TempSBVolTri, <Triggered bus voltage, shunt voltage and temperature, single shot>        7h
     * @n                eIna238Shutdown2,    <Shutdown>                                                                 8h
     * @n                eIna238BVolCon,      <Continuous bus voltage only>                                              9h
     * @n                eIna238SVolCon,      <Continuous shunt voltage only>                                            Ah
     * @n                eIna238SBVolCon,     <Continuous shunt and bus voltage>                                         Bh
     * @n                eIna238TempCon,      <Continuous temperature only>                                              Ch
     * @n                eIna238TempBVolCon,  <Continuous bus voltage and temperature>                                   Dh
     * @n                eIna238TempSVolCon,  <Continuous temperature and shunt voltage>                                 Eh
     * @n                eIna238TempSBVolCon  <Continuous bus voltage shunt voltage and temperature>                     Fh
     */
    void  setMode(eIna238Mode_t mode);
    
    eIna238Status_t lastOperateStatus;
protected:        
    virtual bool scan();
    virtual void writeReg(uint8_t reg, uint8_t *pBuf, uint16_t len);
    virtual void readReg(uint8_t reg, uint8_t *pBuf, uint32_t len);    
    int16_t readInaReg(uint8_t reg);
    uint16_t readInaRegUnsigned(uint8_t reg);
    int32_t readInaRegpower(uint8_t reg);
    void writeInaReg(uint8_t reg, uint16_t value);
    
};

class INA238_IIC : public INA238
{
public:
    /**
     * @fn INA238_IIC
     * @brief Constructor
     * @param pWire I2C controller pointer
     * @param i2caddr  I2C address
     * @n INA238_I2C_ADDRESS1  0x40   A0 = 0  A1 = 0
     * @n INA238_I2C_ADDRESS2  0x41   A0 = 1  A1 = 0
     * @n INA238_I2C_ADDRESS3  0x44   A0 = 0  A1 = 1
     * @n INA238_I2C_ADDRESS4  0x45   A0 = 1  A1 = 1
     */
    INA238_IIC(TwoWire *pWire, uint8_t i2caddr) : INA238() { _pWire = pWire; _addr = i2caddr; }

protected:
    void    writeReg(uint8_t reg, uint8_t *pBuf, uint16_t len);
    void    readReg(uint8_t reg, uint8_t *pBuf, uint32_t len);
    bool scan();
    uint8_t   _addr;
    TwoWire   *_pWire;
};

#endif
