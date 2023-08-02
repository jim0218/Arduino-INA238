# -*- coding: utf-8 -*-

import smbus
import time

class INA238:
  
  INA238_I2C_ADDRESS1            = (0x40)# I2C ADDRESS 1
  INA238_I2C_ADDRESS2            = (0x41)# I2C ADDRESS 2
  INA238_I2C_ADDRESS3            = (0x44)# I2C ADDRESS 3
  INA238_I2C_ADDRESS4            = (0x45)# I2C ADDRESS 4
  
  INA238_CONFIG                  = (0x00) #Configuration
  INA238_ADC_CONFIG              = (0x01) #ADC Configuration
  INA238_SHUNT_CAL               = (0x02) #Shunt Calibration
  INA238_VSHUNT                  = (0x04) #Shunt Voltage Measurement
  INA238_VBUS                    = (0x05) #Bus Voltage Measurement
  INA238_DIETEMP                 = (0x06) #Temperature Measurement
  INA238_CURRENT                 = (0x07) #Current Result
  INA238_POWER                   = (0x08) #Power Result
  INA238_DIAG_ALRT               = (0x0B) #Diagnostic Flags and Alert
  INA238_SOVL                    = (0x0C) #Shunt Overvoltage Threshold
  INA238_SUVL                    = (0x0D) #Shunt Undervoltage Threshold
  INA238_BOVL                    = (0x0E) #Bus Overvoltage Threshold
  INA238_BUVL                    = (0x0F) #Bus Undervoltage Threshold
  INA238_TEMP_LIMIT              = (0x10) #Temperature Over-Limit Threshold
  INA238_PWR_LIMIT               = (0x11) #Power Over-Limit Threshold
  INA238_MANUFACTURER_ID         = (0x3E) #Manufacturer ID
  INA238_DEVICE_ID               = (0x3F) #Device ID
  
  #Shunt full scale range selection across IN+ and IN–
  adc_range_0                    = 0  #0h = ±163.84 mV  #default
  adc_range_1                    = 1  #1h = ± 40.96 mV

  #Size of the sample collected by adc every time                          
  adc_sample_1                   = 0  #default
  adc_sample_4                   = 1
  adc_sample_16                  = 2
  adc_sample_64                  = 3
  adc_sample_128                 = 4
  adc_sample_256                 = 5
  adc_sample_512                 = 6  
  adc_sample_1024                = 7

  #conversion time
  conversion_time_50             = 0
  conversion_time_84             = 1 
  conversion_time_150            = 2 
  conversion_time_280            = 3 
  conversion_time_540            = 4 
  conversion_time_1052           = 5  #default
  conversion_time_2074           = 6
  conversion_time_4120           = 7  
  
  # Mode
  shutdown1                     = 0
  bus_vol_tirg                  = 1
  shunt_vol_tirg                = 2
  shunt_bus_vol_tirg            = 3                               
  temp_tirg                     = 4
  temp_bus_vol_tirg             = 5
  temp_shunt_vol_tirg           = 6
  temp_shunt_bus_vol_tirg       = 7
  shuntdown2                    = 8
  bus_vol_con                   = 9
  shunt_vol_con                 = 10
  shunt_bus_vol_con             = 11
  temp_con                      = 12
  temp_bus_vol_con              = 13
  temp_shunt_vol_con            = 14
  temp_shunt_bus_vol_con        = 15  #default
                                
  
  
  
  def __init__(self, bus, addr):
      self.i2cbus=smbus.SMBus(bus)
      self.i2c_addr = addr
  
  def begin(self):
    '''!
      @fn begin
      @brief Initialize I2C bus and configure INA238 config register before reading data
      @return True (succeeded)/False (failed)
    '''
    if not self.scan():
        return False
    else:        
        self.set_sample(adc_sample_4)
        self.set_shunt_ADC(conversion_time_540)
        self.set_bus_ADC(conversion_time_540)
        self.set_mode(shunt_bus_vol_con)
        self.set_ADC_range(adc_range_0)
        return True
  
  
  def reset(self):
      conf = 0
      conf = self.read_ina_reg(self.INA238_CONFIG)      
      conf |= adcrange <<15
      self._write_register(self.INA238_CONFIG, conf)
  
  def _write_register(self, register, value):
      self.i2cbus.write_i2c_block_data(self.i2c_addr, register, [value >> 8, value & 0xff])
  
  def _read_register(self, register):
      return self.i2cbus.read_i2c_block_data(self.i2c_addr, register) 
  
  def get_bus_voltage_V(self):
      '''!
        @fn get_bus_voltage_V
        @brief  get the BusVoltage （Voltage of IN- to GND)
        @return Voltage unit:V
      '''
      return float(self.read_ina_reg(self.INA238_VBUS) ) * 3.125 *0.001
  
  def get_shunt_voltage_mV(self):
      '''
        @fn get_shunt_voltage_mV
        @brief  get the ShuntVoltage （Voltage of the sampling resistor, IN+ to NI-)
        @return Voltage unit:mV
      '''
      return float(self.read_ina_reg(self.INA238_VSHUNT))
  
  def get_current_mA(self):
      '''
        @fn get_current_mA
        @brief get the Current(Current flows across IN+ and IN-.
        @n If the current flows from IN+ to IN-, the reading is positive. 
        @n If the current flows from IN- to IN+, the reading is negative)
        @return Current unit:mA
      '''
      return float(self.read_ina_reg(self.INA238_CURRENT))
  
  def get_power_mW(self):
     '''
       @fn get_power_mW
       @brief  get the ShuntVoltage （Voltage of the sampling resistor, IN+ to NI-)
       @return Voltage unit:mV
     '''
     return float(self.read_ina_reg_power(self.INA238_POWER)) * 20
  
  def set_sample(self, value):      
      conf = 0
      conf = self.read_ina_reg(self.INA238_ADC_CONFIG)
      conf &= ~(0x07)
      conf |= value
      self._write_register(self.INA238_ADC_CONFIG, conf)
  
  def set_shunt_ADC(self, time):      
      conf = 0
      conf = self.read_ina_reg(self.INA238_ADC_CONFIG)
      conf &= ~(0x07 << 6)
      conf |= time << 6
      self._write_register(self.INA238_ADC_CONFIG, conf)
  
  def set_bus_ADC(self, time):
      conf = 0      
      conf = self.read_ina_reg(self.INA238_ADC_CONFIG)
      conf &= ~(0x07 << 9)
      conf |= time << 9
      self._write_register(self.INA238_ADC_CONFIG, conf)
  
  def set_mode(self, mode):      
      conf = 0
      conf = self.read_ina_reg(self.INA238_ADC_CONFIG)
      conf &= ~(0x0f << 12)
      conf |= mode <<12
      self._write_register(self.INA238_ADC_CONFIG, conf)

  def set_ADC_range(self, adcrange):      
      conf = 0
      conf = self.read_ina_reg(self.INA238_CONFIG)
      conf &= ~(0x01 << 4)
      conf |= adcrange <<4
      self._write_register(self.INA238_CONFIG, conf)

  def read_ina_reg(self, reg):
      buf = []
      buf = self._read_register(reg)
      if (buf[0] & 0x80):
          return - 0x10000 + ((buf[0] << 8) | (buf[1]))
      else:
          return (buf[0] << 8) | (buf[1])

  def read_ina_reg_power(self, reg):
      buf = []
      buf = self._read_register(reg)
      if (buf[0] & 0x80):
          return - 0x1000000 + ((buf[0] << 16) | (buf[1] << 8) | buf[2])
      else:
          return ((buf[0] << 16) | (buf[1] << 8) | buf[2])

  def scan(self):
      try:
          self.i2cbus.read_byte(self.i2c_addr)
          return True
      except:
          print("I2C init fail")
          return False
  
  