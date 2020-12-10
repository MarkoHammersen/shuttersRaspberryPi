# shutterControlRaspberryPi
HW: 
- 1 raspberryPi 4
- 1 IoPi Plus (https://www.abelectronics.co.uk/p/54/io-pi-plus)
- 1 I2C Switch (https://www.abelectronics.co.uk/p/84/i2c-switch)
- 2 * 8 Channel I2C Interface Electromagnetic Relay Module 10A PCF8574 Arduino SPDT 8https://www.ebay.de/itm/8-Channel-I2C-Interface-Electromagnetic-Relay-Module-10A-PCF8574-Arduino-SPDT/114237194670?hash=item1a991141ae:g:5PoAAOSwEmNe0VP9)
- 1 * 16 Channel I2C Electromagnetic Relay Module for Arduino Raspberry 3.3V 5.0V IoT (https://www.ebay.de/itm/16-Channel-I2C-Electromagnetic-Relay-Module-for-Arduino-Raspberry-3-3V-5-0V-IoT/114063136814?hash=item1a8eb1582e:g:m~4AAOSw5eNeGyif)

The code controlling the I2c-Relay Modules has been customized from the sample code provided by the manufacturer of those boards. Many thanks

Setup:
Stack I2C switch on IoPiPlus on RaspberryPi.
Connect I2C lines of rlay Module to I2CSwitch.
All button input (5v) lines are to be connected to the pins of the IoPi Plus.
