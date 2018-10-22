#include <TMP101.h>

void Init_I2C_GPIO(void){
	GPIO_InitTypeDef GPIOInit;
	I2C_InitTypeDef I2C2_Init;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE); //enable clock for I2C
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //enable clock for SCL

	GPIOInit.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12; 
	GPIOInit.GPIO_Mode = GPIO_Mode_AF;			
	GPIOInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInit.GPIO_OType = GPIO_OType_OD;
	GPIOInit.GPIO_PuPd = GPIO_PuPd_UP;			
	
	GPIO_Init(GPIOB, &GPIOInit);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_I2C2);
	
	I2C2_Init.I2C_ClockSpeed = 100000; 		
	I2C2_Init.I2C_Mode = I2C_Mode_I2C;		
	I2C2_Init.I2C_DutyCycle = I2C_DutyCycle_2;	
	I2C2_Init.I2C_OwnAddress1 = 0x00;			
	I2C2_Init.I2C_Ack = I2C_Ack_Enable;		
	I2C2_Init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	I2C_Init(I2C2, &I2C2_Init);
	I2C_Cmd(I2C2, ENABLE);
	
}

void start_trans(I2C_TypeDef* I2Cx, uint8_t address){

	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2Cx, ENABLE);
	
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2Cx, address, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//	USART6_SendChar('f');
//	USART1_SendChar('a');
	
}

void start_rec(I2C_TypeDef* I2Cx, uint8_t address){
	//while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2Cx, ENABLE);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2Cx, address, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
}

void i2c_write(I2C_TypeDef* I2Cx, uint8_t data){
	I2C_SendData(I2Cx, data);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//s	USART1_SendChar('a');
}

uint8_t read_ack(I2C_TypeDef* I2Cx){
	uint8_t received_data;
	// enable acknowledge of recieved data
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	// wait until one byte has been received
	while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
	
	// read data from I2C data register and return data byte
	received_data = I2C_ReceiveData(I2Cx);
	return received_data;
}

uint8_t read_nack(I2C_TypeDef* I2Cx){
	uint8_t received_data;
	// disabe acknowledge of received data
	// nack also generates stop condition after last byte received
	// see reference manual for more info
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	I2C_GenerateSTOP(I2Cx, ENABLE);
	// wait until one byte has been received
	while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
	// read data from I2C data register and return data byte
	received_data = I2C_ReceiveData(I2Cx);
	return received_data;
}

void tmp101_init (uint8_t conf){
	start_trans(I2C2, 0x90);
	
	i2c_write(I2C2, 0x01);
	i2c_write(I2C2, conf);
	I2C_GenerateSTOP (I2C2, ENABLE);
	timer2_wait_millisec(320);
}

float tmp_temp(void){
	uint8_t tempH, tempL;
	uint16_t t;
	float temp;
	
	start_trans(I2C2, 0x90);
	
	i2c_write(I2C2, 0x00);
	start_rec(I2C2, 0x90);
	tempH=read_ack(I2C2);
	tempL=read_nack(I2C2);
	
	t=(tempH<<4) | (tempL>>4);
	temp=(float)(t*0.0625);
	return temp;
}
