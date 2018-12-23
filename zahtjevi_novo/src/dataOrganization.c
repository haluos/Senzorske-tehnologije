#include "dataOrganization.h"

uint16_t tmp101_temp (float *temp, float *time){
	tmp101_init (0xE1);

	// uint16_t t = (float)tmp_temp(temp);
	return tmp_temp(temp);
//	*temp = 973.0;
	// *time = 321.09;
}

uint16_t max6675_temp (float *temp, float *time){
	// uint16_t t = (float)max6675temp(temp);
	return max6675temp(temp);
//	*temp = 973.0;
	// *time = 0.98;
//	timer2_wait_millisec(340);
}

uint16_t max6682_temp (float *temp, float *time){
	// uint16_t t = (float)max6682temp(temp);
	return max6682temp(temp);
//	*temp = 973.0;
	// *time = 45.233;
//	timer2_wait_millisec(340);
}

uint16_t lm35dz_temp (float *temp, float *time){
	// uint16_t t = (float)LM_temp(temp);
	return LM_temp(temp);
//	*temp = 973.0;
	// *time = 0.2;
//	timer2_wait_millisec(50);
}

uint16_t ds18b20_temp (float *temp, float *time){
	// uint16_t t = (float)DS_temp(temp);
	return DS_temp(temp);
//	*temp = 973.0;
	// *time = 1.5;
}

uint16_t gnico001_temp (float *temp, float *time){
	// uint16_t t = (float)NICO_temp (temp);
	return NICO_temp (temp);
//	*temp = 16.98;
	// *time = 99.56;
//	timer2_wait_millisec(340);
}

uint16_t ntc_temp (float *temp, float *time){
	// uint16_t t = (float)NTC(temp);
	return NTC(temp);
//	*temp = 16.98;
	// *time = 99.56;
//	timer2_wait_millisec(340);
}

void send_requested (uint8_t request){
	uint8_t pack[PACKSIZE], Opcode;
	float time=0, temp, t;
	int i, counter, bytes;

	counter=1;
	for(i=0; i<ELEMENTS; ++i){
		if((request != '0') && (request != '3'))
			timer2_wait_millisec(340);
		if(request == '1'){
			lm35dz_temp (&temp, &t);
			Opcode=getOpcode(0x01, TIME, LSB_FIRST, CRC_YES, END_YES);
			bytes = returnPacked(time, temp, Opcode, counter++, pack);
			fillBuffer(pack, bytes);
		}
		if(request == '2'){
			ds18b20_temp (&temp, &t);
			Opcode=getOpcode(0x02, TIME, LSB_FIRST, CRC_YES, END_YES);
			bytes = returnPacked(time, temp, Opcode, counter++, pack);
			fillBuffer(pack, bytes);
		}
		if(request == '4'){
			gnico001_temp (&temp, &t);
			Opcode=getOpcode(0x04, TIME, LSB_FIRST, CRC_YES, END_YES);
			bytes = returnPacked(time, temp, Opcode, counter++, pack);
			fillBuffer(pack, bytes);
		}
		if(request == '5'){
			max6675_temp (&temp, &t);
			Opcode=getOpcode(0x05, TIME, LSB_FIRST, CRC_YES, END_YES);
			bytes = returnPacked(time, temp, Opcode, counter++, pack);
			fillBuffer(pack, bytes);
		}
		if(request == '6'){
			max6682_temp (&temp, &t);
			Opcode=getOpcode(0x06, TIME, LSB_FIRST, CRC_YES, END_YES);
			bytes = returnPacked(time, temp, Opcode, counter++, pack);
			fillBuffer(pack, bytes);
		}
		if(request == '3'){
			tmp101_temp (&temp, &t);
			Opcode=getOpcode(0x03, TIME, LSB_FIRST, CRC_YES, END_YES);
			bytes = returnPacked(time, temp, Opcode, counter++, pack);
			fillBuffer(pack, bytes);
		}
		time += (float)0.340;
	}
	empty_buffer();
	buffer_initialization();
}

// void send_referent(void){
// 	uint8_t Opcode, pack[PACKSIZE], counter;
// 	int bytes;
// 	float temp=0, t=0, time;
// 	lm35dz_temp (&temp, &t);
// 	Opcode=getOpcode(0x01, TIME, LSB_FIRST, CRC_YES, END_YES);
// 	bytes = returnPacked(time, temp, Opcode, counter++, pack);
// //	fillBuffer(pack, bytes);
// 	sendPack(pack, bytes);
// }

void get_temp(uint32_t tim, float *referent_temp){
	uint8_t Opcode, pack[PACKSIZE], counter=0;
	int bytes;
	float temp=0, t=0;
	uint16_t temperature;

//	timer2_wait_millisec(320);

	temperature = ds18b20_temp (&temp, &t);
	Opcode=getOpcode(0x02, TIME, LSB_FIRST, CRC_YES, END_YES);
	bytes = returnPacked(tim, temperature, Opcode, counter++, pack);
	fillBuffer(pack, bytes);
//	Unpack(pack, &t,&t);
//	memset(pack, 0, sizeof(pack));

	temperature = lm35dz_temp (&temp, &t);
	*referent_temp = temp;
	Opcode=getOpcode(0x01, TIME, LSB_FIRST, CRC_YES, END_YES);
	bytes = returnPacked(tim, temperature, Opcode, counter++, pack);
	fillBuffer(pack, bytes);
//	Unpack(pack, &t,&t);
//	memset(pack, 0, sizeof(pack));

	temperature = gnico001_temp (&temp, &t);
	Opcode=getOpcode(0x04, TIME, LSB_FIRST, CRC_YES, END_YES);
	bytes = returnPacked(tim, temperature, Opcode, counter++, pack);
	fillBuffer(pack, bytes);
//	Unpack(pack, &t,&t);
//	memset(pack, 0, sizeof(pack));

	temperature = max6675_temp (&temp, &t);
	Opcode=getOpcode(0x05, TIME, LSB_FIRST, CRC_YES, END_YES);
	bytes = returnPacked(tim, temperature, Opcode, counter++, pack);
	fillBuffer(pack, bytes);
//	Unpack(pack, &t,&t);
//	memset(pack, 0, sizeof(pack));

	temperature = max6682_temp (&temp, &t);
	Opcode=getOpcode(0x06, TIME, LSB_FIRST, CRC_YES, END_YES);
	bytes = returnPacked(tim, temperature, Opcode, counter++, pack);
	fillBuffer(pack, bytes);
//	Unpack(pack, &t,&t);
//	memset(pack, 0, sizeof(pack));

	temperature = ntc_temp (&temp, &t);
	Opcode=getOpcode(0x07, TIME, LSB_FIRST, CRC_YES, END_YES);
	bytes = returnPacked(tim, temperature, Opcode, counter++, pack);
	fillBuffer(pack, bytes);
//	Unpack(pack, &t,&t);
//	memset(pack, 0, sizeof(pack));

	temperature = tmp101_temp (&temp, &t);

	Opcode=getOpcode(0x03, TIME, LSB_FIRST, CRC_YES, END_YES);
	bytes = returnPacked(tim, temperature, Opcode, counter++, pack);
	fillBuffer(pack, bytes);
//	Unpack(pack, &t,&t);
//	memset(pack, 0, sizeof(pack));
}
