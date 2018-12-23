#include "usart.h"
#include "packing.h"
#include "stdlib.h"
#include "timer.h"
#include "defines.h"
#include "DS18B20.h"
#include "LM35DZ.h"
#include "TMP101.h"
#include "MAX6675.h"
#include "MAX6682.h"
#include "G-NICO.h"
#include "NTC.h"

uint16_t tmp101_temp (float *temp, float *time);
uint16_t max6675_temp (float *temp, float *time);
uint16_t max6682_temp (float *temp, float *time);
uint16_t lm35dz_temp (float *temp, float *time);
uint16_t ds18b20_temp (float *temp, float *time);
uint16_t gnico001_temp (float *temp, float *time);
uint16_t ntc_temp (float *temp, float *time);
void send_requested (uint8_t request);
void send_referent(void);
void get_temp(uint32_t tim, float *referent_temp);
