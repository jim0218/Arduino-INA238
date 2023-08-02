# -*- coding: utf-8 -*-
import sys
import time
sys.path.append("../..")
from INA238_Setting import INA238

'''
  @param i2caddr  I2C address
  @n INA219_I2C_ADDRESS1  0x40   A0 = 0  A1 = 0
  @n INA219_I2C_ADDRESS2  0x41   A0 = 1  A1 = 0
  @n INA219_I2C_ADDRESS3  0x44   A0 = 0  A1 = 1
  @n INA219_I2C_ADDRESS4  0x45   A0 = 1  A1 = 1	 
'''
ina = INA38(1, INA238.INA238_I2C_ADDRESS4)                                #Change I2C address by dialing DIP switch

#begin return True if succeed, otherwise return False
while not ina.begin():
    time.sleep(2)
'''
Revise the following two paramters according to actula reading of the INA219 and the multimeter
for linearly calibration
'''
ina.set_sample(adc_sample_4)
ina.set_shunt_ADC(conversion_time_540)
ina.set_bus_ADC(conversion_time_540)
ina.set_mode(shunt_bus_vol_con)
ina.set_ADC_range(adc_range_0)

#ina.reset()                                     #Resets all registers to default values

def main():
    while True:
        time.sleep(1)
        print ("Shunt Voltage : %.2f mV" % ina.get_shunt_voltage_mV())
        print ("Bus Voltage   : %.3f V" % ina.get_bus_voltage_V())
        print ("Current       : %.f mA" % ina.get_current_mA())
        print ("Power         : %.f mW" % ina.get_power_mW())
        print (" ")

if __name__ == "__main__":
    main()
