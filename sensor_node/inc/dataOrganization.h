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

void tmp101_temp (float *temp, float *time);
void max6675_temp (float *temp, float *time);
void max6682_temp (float *temp, float *time);
void lm35dz_temp (float *temp, float *time);
void ds18b20_temp (float *temp, float *time);
void gnico001_temp (float *temp, float *time);
void ntc_temp (float *temp, float *time);
void send_requested (uint8_t request);
// void send_referent(void);
void get_temp(float time, float *referent_temp);
