# Lab model for Sensor Technology course

Lab model consists of 7 temperature sensors with different working principles
(RTD, 2 implementations of NTCs, thermocouple and 3 implementations of IC sensors),
STM32F4 Discovery Board, Peliter heater/cooler, H-bridge, LP filter and voltage source.
Sensors are controlled by STM32F4 with I2C, SPI, 1-Wire or simple ADC, while H-bridge
is controlled with PWM. Purpose of project is to compare sensitivity of temperature sensors.

## Short project guide

Implementation of communication protocols of sensors are in matching filenames of sensors.

* RTD -> G-NICO001 -> ADC
* NTCs -> MAX6682 and NTC -> SPI and ADC
* thermocouple -> MAX6675 -> SPI
* IC sensors -> TMP101, DS18B20 and LM35DZ -> I2C, ADC and ADC
