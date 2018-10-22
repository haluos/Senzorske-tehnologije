#include "dataOrganization.h"

void tmp101_temp (float *temp, float *time){
	tmp101_init (0xE1);
	*temp = tmp_temp();
	*time = 321.09;
}

void max6675_temp (float *temp, float *time){
	*temp = max6675temp();
	*time = 0.98;
//	timer2_wait_millisec(340);
}

void max6682_temp (float *temp, float *time){
//	*temp = max6682temp();
	*temp = 973.0;
	*time = 45.233;
//	timer2_wait_millisec(340);
}

void lm35dz_temp (float *temp, float *time){
	*temp = LM_temp();
	*time = 0.2;
//	timer2_wait_millisec(50);
}

void ds18b20_temp (float *temp, float *time){
	*temp= DS_temp();
	*time = 1.5;
}

void gnico001_temp (float *temp, float *time){
	*temp = 16.98;
	*time = 99.56;
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

void send_referent(void){
	uint8_t Opcode, pack[PACKSIZE], counter;
	int bytes;
	float temp=0, t=0, time;
	lm35dz_temp (&temp, &t);
	Opcode=getOpcode(0x01, TIME, LSB_FIRST, CRC_YES, END_YES);
	bytes = returnPacked(time, temp, Opcode, counter++, pack);
//	fillBuffer(pack, bytes);
	sendPack(pack, bytes);
}

void get_temp(float time){
	uint8_t Opcode, pack[PACKSIZE], counter;
	int bytes;
	float temp=0, t=0;
	
	ds18b20_temp (&temp, &t);
	Opcode=getOpcode(0x02, TIME, LSB_FIRST, CRC_YES, END_YES);
	bytes = returnPacked(time, temp, Opcode, counter++, pack);
	fillBuffer(pack, bytes);
	
	lm35dz_temp (&temp, &t);
	Opcode=getOpcode(0x01, TIME, LSB_FIRST, CRC_YES, END_YES);
	bytes = returnPacked(time, temp, Opcode, counter++, pack);
	fillBuffer(pack, bytes);
	
	gnico001_temp (&temp, &t);
	Opcode=getOpcode(0x04, TIME, LSB_FIRST, CRC_YES, END_YES);
	bytes = returnPacked(time, temp, Opcode, counter++, pack);
	fillBuffer(pack, bytes);
	
	max6675_temp (&temp, &t);
	Opcode=getOpcode(0x05, TIME, LSB_FIRST, CRC_YES, END_YES);
	bytes = returnPacked(time, temp, Opcode, counter++, pack);
	fillBuffer(pack, bytes);
	
	max6682_temp (&temp, &t);
	Opcode=getOpcode(0x06, TIME, LSB_FIRST, CRC_YES, END_YES);
	bytes = returnPacked(time, temp, Opcode, counter++, pack);
	fillBuffer(pack, bytes);
	
	tmp101_temp (&temp, &t);
	Opcode=getOpcode(0x03, TIME, LSB_FIRST, CRC_YES, END_YES);
	bytes = returnPacked(time, temp, Opcode, counter++, pack);
	fillBuffer(pack, bytes);
}
