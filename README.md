# Lab model for Sensor Technology course

Lab model consists of 7 temperature sensors with different working principles
(RTD, 2 implementations of NTCs, thermocouple and 3 implementations of IC sensors),
STM32F4 Discovery Board, Peltier heater/cooler, H-bridge, LP filter and voltage source.
Sensors are controlled by STM32F4 with I2C, SPI, 1-Wire or simple ADC, while H-bridge
is controlled with PWM. Purpose of project is to compare sensitivity of temperature sensors.

![alt text](model.jpg)

## Short project guide

Implementation of communication protocols of sensors are in matching filenames of sensors.

* RTD -> G-NICO001 -> ADC
* NTCs -> MAX6682 and NTC -> SPI and ADC
* thermocouple -> MAX6675 -> SPI
* IC sensors -> TMP101, DS18B20 and LM35DZ -> I2C, ADC and ADC

H-bridge is controlled by STM32F4 with PWM through serial communication. Characters sent from
serial Terminal are used to determine heating or cooling of Peltier and value of dutycycle of
PWM signal.

After sending start request, sensors start measuring temperature of Peltier and after every 1s
measurement results are sent through serial communication to computer using custom made
communication protocol.

### Communication protocol between STM32F4 and computer

Communication protocol used in project is in form of package which carries information about
temperature and time in s that passed after start request.

Communication protocol is explained with comments in packing.c file.
