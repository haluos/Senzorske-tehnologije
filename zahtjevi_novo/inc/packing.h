#ifndef PACKING_H
#define PACKING_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <usart.h>
#include "defines.h"

uint8_t calculateCRC (uint8_t *time, uint8_t *temp, int divider, int remainderBits);
uint8_t getOpcode (uint8_t sensorID, uint8_t time_temp, uint8_t MSB_LSB, uint8_t error_check, uint8_t END);
int returnPacked (uint32_t tim, uint32_t temp, uint8_t Opcode, int counter, uint8_t *packedData);
int Unpack (uint8_t *pack, float *time, float *temp);

#endif
