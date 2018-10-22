#include <packing.h>

//Function calculateCRC calculates CRC of data contained in packet
//Arguments:	time -					float time data converted to 4 byte char array
//						temp -					float temperature data converted to 4 byte char array
//						divider -				polynomial for CRC calculation
//						remainderBits -	calculation of remainderBits-CRC, max value is 8
//Return value: calculated CRC of given data

uint8_t calculateCRC (uint8_t *time, uint8_t *temp, int divider, int remainderBits)
{
	int data1, data2, i, j=0;
  int shift_bit;
  int remainder;
	int mask=0x1<<remainderBits;
//convert time and temp array to two 32 bit data for easier calculation
  data1=(int)(((int)time[0]<<24) | (int)(time[1]<<16) | (int)(time[2]<<8) | (int)(time[3]));
  data2=(int)(((int)temp[0]<<24) | (int)(temp[1]<<16) | (int)(temp[2]<<8) | (int)(temp[3]));
//first remainder is remainderBits highest bits od data1
	remainder=data1>>(31-remainderBits);
//loop in range of number of bits in both data1 and data2
  for(i=0; i<(2*8*sizeof(int)); ++i)
  {
	//if highest bit of remainder is 1 then operation is remainder XOR divider, else there is no operation
    if(mask & remainder)
    {
      remainder = remainder ^ divider;
    }
	//left shift remainder for 1 to get space for next bit of data
    remainder<<=1;
	//check if there are bits of data1 left to shift
    if(i<(8*sizeof(int)-(remainderBits+1)))
    {
      shift_bit=(uint8_t)(data1>>(31-(remainderBits+1+i)));
      shift_bit&=0x01;
      remainder=remainder|shift_bit;
    }
	//if all 32 bits of data1 are shifted move to data2
    else
    {
		//chek if there are bits of data2 left to shift
      if (i<(2*8*sizeof(int)-9))
      {
        shift_bit=(uint8_t)(data2>>(31-j));
        shift_bit&=0x01;
        remainder=remainder|shift_bit;
        ++j;
      }
    }
  }
//right shift of remainder to get final value of remainder
  remainder>>=1;
  return (uint8_t)remainder;
}


//Function getOpcode calculates operational code used for instructions to unpack data
//Arguments:	sensorID -		can be value 0x01..0x06
//						time_temp -		used for setting time/temp bit in Opcode (if set, time will be sent first)
//						MSB_LSB -			used for setting MSB/LSB bit in Opcode (if set, most significant byte will be sent first)
//						error_check -	used for setting CRC bit in Opcode (if set, CRC calculation will be sent)
//						END -					used for setting END bit in Opcode (if set, end sign will be sent)
//Return value:	calculated value of operational code

uint8_t getOpcode (uint8_t sensorID, uint8_t time_temp, uint8_t MSB_LSB, uint8_t error_check, uint8_t END)
{
	uint8_t Opcode=0;
	Opcode=((sensorID << 4) | (time_temp << 3) | (MSB_LSB << 2) | (error_check << 1) | END);
	return Opcode;
}


//Function returnPacked packs data
//Arguments:	time -			time data to be packed
//						temp -			temperature data to be packed
//						Opcode -		instructions how to pack data
//						counter -		ordinal of packet

int returnPacked (float time, float temp, uint8_t Opcode, int counter, uint8_t *packedData)
{
	union{
		char data[8];
		float data1;
		float data2;
	}pack_union;
	int i,j, byte=0;
	uint8_t time_char[sizeof(float)+1], temp_char[sizeof(float)+1], array[10];
	time_char[4]=0;
	temp_char[4]=0;
	
//	uint8_t packedData[PACKSIZE];
//	memset(packedData, 0, sizeof(packedData));
//	memset(pack, 0, sizeof(pack));

//first 2 bytes (A and B) are START sign of packet
	packedData[byte++] = 'A';
	packedData[byte++] = 'B';
//third byte of packet is Opcode
	packedData[byte++] = Opcode;
//fourth byte of packet is ordinal of packet
	packedData[byte++] = counter;
//other bytes of packet are formed by instructions of Opcode, following bytes can be: data, CRC byte and END sign of packet

//convert float data to 4 byte char array, index 0 of array is least significant byte
	memcpy(time_char, &time, sizeof(float));
	memcpy(temp_char, &temp, sizeof(float));

//check MSB/LSB bit in Opcode, if it is set change order of bytes
	if(Opcode & 0x04)
	{
		for(i=0, j=(sizeof(float)-1); i<j ; ++i, --j)
		{
			uint8_t tmp=time_char[i];
			time_char[i]=time_char[j];
			time_char[j]=tmp;

			tmp=temp_char[i];
			temp_char[i]=temp_char[j];
			temp_char[j]=tmp;
		}
	}

//check if time/temp bit in Opcode is set
	if(Opcode&0x08){
		for(i=0; i<sizeof(float); ++i)
		{
			packedData[byte++]=time_char[i];
		}

		for(i=0; i<(sizeof(float)); ++i)
		{
			packedData[byte++]=temp_char[i];
		}
	}
//if time/temp bit is not set temperature data comes first
	else{
		for(i=0; i<sizeof(float); ++i)
		{
			packedData[byte++]=temp_char[i];
		}

		for(i=0; i<(sizeof(float)); ++i)
		{
			packedData[byte++]=time_char[i];
		}
	}

//if CRC bit is set in Opcode, following byte is calculated CRC
	if(Opcode & 0x02)
	{
		uint8_t calculated_CRC;
		calculated_CRC=calculateCRC(time_char, temp_char, EIGHT_BIT_DIVIDER, EIGHT_BIT_CRC);
		packedData[byte++]=(calculated_CRC);
	}

//if END bit is set in Opcode, following bytes are END sign of packet (Y and Z)
	if(Opcode & 0x01)
	{
		packedData[byte++]='Y';
		packedData[byte++]='Z';
	}
//	sprintf(array, "%3.4f", time);
//	for(i=0;i<sizeof(array);i++){
//		USART6_SendChar(array[i]);
//	}
//	USART6_SendChar(0x0D); USART6_SendChar(0x0A);
//	sprintf(pack, "%s", packedData);
	return byte;
}


//Function Unpack receives packed data and upacks it to return sent data
//Arguments:	pack -	array of data bytes formed in one packet
//						time -	float value of time after successful unpacking
//						temp -	float value of temp after successful unpacking
//Return value: if unpacking was successful returns ordinal of packet and 0 if unpacking failed

int Unpack (uint8_t *pack, float *time, float *temp)
{
	int i,j, ordinal=0, byte=0;
	uint8_t Opcode, start0= 'A', start1='B';
	uint8_t checkCRC, byteCRC;
	uint8_t time_char[sizeof(float)], temp_char[sizeof(float)];
	uint8_t array[10];

//recognition of START sign and fetching Opcode
	if(pack[byte++]==start0){
		if(pack[byte++]==start1)
			Opcode=pack[byte++];
	//if START sign is not recognized return 0
		else return 0;
	}
//next byte is ordinal
	ordinal=pack[byte++];

//check if next 4 bytes are time or temperature
	if(Opcode & 0x08){
		for (i=0; i<sizeof(float); ++i)
		{
			time_char[i]=pack[byte++];
//			USART6_SendChar(time_char[i]);
		}
		for (i=0; i<sizeof(float); ++i)
		{
			temp_char[i]=pack[byte++];
//			USART6_SendChar(temp_char[i]);
		}
	}
	else{
		for (i=0; i<sizeof(float); ++i)
		{
			temp_char[i]=pack[byte++];
		}
		for (i=0; i<sizeof(float); ++i)
		{
			time_char[i]=pack[byte++];
		}
	}
//check if MSB/LSB bit is set, if it is set order of bytes must be changed
	if(0x04 & Opcode)
	{
		for(i=0, j=(sizeof(float)-1); i<j ; ++i, --j)
		{
			uint8_t tmp=time_char[i];
			time_char[i]=time_char[j];
			time_char[j]=tmp;

			tmp=temp_char[i];
			temp_char[i]=temp_char[j];
			temp_char[j]=tmp;
		}
	}
//check if CRC bit is set in Opcode to fetch value of CRC and to calculate CRC of received data
	if(Opcode&0x02){
		byteCRC=pack[byte++];
		checkCRC=calculateCRC(time_char, temp_char, EIGHT_BIT_DIVIDER, EIGHT_BIT_CRC);
		if(byteCRC!=checkCRC){
			return 0;
		}
	}
//check if packet contains END sign
	if(Opcode&0x01){
		uint8_t end0='Y', end1='Z';
	//if END sign is not recognized return 0
		if(pack[byte++]==end0)
			if(pack[byte]!=end1)
				return 0;
	}
//if there was no error found convert char array of time and temperature to float value
	memcpy(time, time_char, sizeof(float));
	memcpy(temp, temp_char, sizeof(float));
//	sprintf(array, "%s", time_char);
//	for(i=0;i<sizeof(array);i++){
//		USART6_SendChar(array[i]);
//	}
//	USART6_SendChar(0x0D); USART6_SendChar(0x0A);
//	memset(array, 0, sizeof(array));
//	sprintf(array, "%3.4f", temp);
//	for(i=0;i<sizeof(array);i++){
//		USART6_SendChar(array[i]);
//	}
//	USART6_SendChar(0x0A); USART6_SendChar(0x0D);
	return ordinal;
}
