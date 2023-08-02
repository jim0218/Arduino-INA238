# -*- coding: utf-8 -*-

import sys
import time
sys.path.append("../..")
from INA238_Setting import INA238

'''
INA238_I2C_ADDRESS1 = (0x40)  A1 = 0  A0 = 0
INA238_I2C_ADDRESS2 = (0x41)  A1 = 0  A0 = 1
INA238_I2C_ADDRESS3 = (0x44)  A1 = 1  A0 = 0
INA238_I2C_ADDRESS4 = (0x45)  A1 = 1  A0 = 1
'''

ina = INA238(1, INA238.INA238_I2C_ADDRESS1)                                #Change I2C address by dialing DIP switch

#begin return True if succeed, otherwise return False
while not ina.begin():
    time.sleep(1)
'''
Revise the following two paramters according to actual reading of the INA219 and the multimeter
for linearly calibration
'''

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
