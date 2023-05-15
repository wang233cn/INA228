#include "INA228.h"
#include "i2c.h"

INA228_CONFIGTypeDef *INA228_Config;
INA228_ADCTypeDef *INA228_ADCConfig;

HAL_StatusTypeDef STATE;
float CURRENT_LSB;

//读取函数
void INA228_Write_Mem(uint8_t addr,uint8_t LENGTH,uint8_t *buff){
	HAL_I2C_Mem_Write(&hi2c1,INA228_ADDR,addr,I2C_MEMADD_SIZE_8BIT,buff,LENGTH,HAL_MAX_DELAY);
}

void INA228_Read_Mem(uint8_t addr,uint8_t LENGTH,uint8_t *buff){
	HAL_I2C_Mem_Read(&hi2c1,INA228_ADDR,addr,I2C_MEMADD_SIZE_8BIT,buff,LENGTH,HAL_MAX_DELAY);
}

//初始化
uint8_t INA228_Init(INA228_CONFIGTypeDef *INA228_Init){
//	uint32_t State;
//	INA228_Read_Mem(INA228_MANUFACTURER_ID,1,(uint8_t *)&State);
//	if(State != 0x4954) return 0;	
//	
	uint16_t data;
	data = 1<<15;
	uint8_t data0[2] = {data>>8,data};
	INA228_Write_Mem(INA228_CONFIG,2,data0);
	
	INA228_Config = INA228_Init;
	CURRENT_LSB = INA228_Config->MEC/524288.0;
	data = INA228_Config->RST<<15|INA228_Config->RSTACC<<14|INA228_Config->CONVDLY<<6|INA228_Config->TEMPCOMP<<5|INA228_Config->ADCRANGE<<4;
	
	uint8_t data1[2]={data>>8,data};
	INA228_Write_Mem(INA228_CONFIG,2,data1);

  return 1;
}
//ADC初始化
void INA228_ADC_Config(INA228_ADCTypeDef *INA228_ADCInit){
	INA228_ADCConfig = INA228_ADCInit;
  uint16_t data = INA228_ADCConfig->MODE<<12|INA228_ADCConfig->VBUSCT<<9|INA228_ADCConfig->VSHCT<<6|INA228_ADCConfig->VTCT<<3|INA228_ADCConfig->AVG;
	uint8_t data1[2]={data>>8,data};
	INA228_Write_Mem(INA228_ADC_CONFIG,2,data1);
}
	
//设置警报
void INA228_SET_Alert(){
}

//分流电阻校准，单位：毫欧,ppm/℃
void INA228_SET_SHUNT_CAL(uint16_t RSHUNT,uint16_t ppm){
	uint16_t SHUNT_CAL;
	if(INA228_Config->ADCRANGE == 1)
		SHUNT_CAL = 131072  * CURRENT_LSB * 100000 * (RSHUNT/1000.0) * 4;
	else
	  SHUNT_CAL = 131072  * CURRENT_LSB * 100000 * (RSHUNT/1000.0);
	
	uint8_t data1[2]={SHUNT_CAL>>8,SHUNT_CAL};
	INA228_Write_Mem(INA228_SHUNT_CAL,2,data1);
	uint8_t data2[2]={ppm>>8,ppm};
	INA228_Write_Mem(INA228_SHUNT_TEMPCO,2,data2);
}


//获取分流电阻电压
float INA228_Get_VSHUNT(void){
	float VSHUNT;
//	uint32_t data;
	uint8_t data1[3];
  INA228_Read_Mem(INA228_VSHUNT,3,data1);
	uint32_t data = (data1[0]<<16|data1[1]<<8|data1[2])>>4;
//	data = data>>4;
	if(INA228_Config->ADCRANGE == 1)
		VSHUNT = 0.0000003125*data;
	else
		VSHUNT = 0.000000078125*data;
	return VSHUNT;
}

//获取VBUS电压
float INA228_Get_VBUS(void){
	float VBUS;
	uint8_t data1[3];
	INA228_Read_Mem(INA228_VBUS,3,data1);
	//data = data>>4;
	uint32_t data = (data1[0]<<16|data1[1]<<8|data1[2])>>4;
	VBUS = 0.0001953125*data;
	return VBUS;
}

//获取芯片温度
float INA228_Get_DIETEMP(void){
	float TEMP;
  uint16_t data;
	uint8_t data1[2];
	INA228_Read_Mem(INA228_DIETEMP,2,data1);
	data = data1[0]<<8|data1[1];
	TEMP = 0.0078125*data;
	return TEMP;
}

//获取电流
float INA228_Get_CURRENT(void){
	float CURRENT;
	uint8_t data1[3];
	INA228_Read_Mem(INA228_CURRENT,3,data1);
	uint32_t data = (data1[0]<<16|data1[1]<<8|data1[2])>>4;
	CURRENT = data*CURRENT_LSB;
	return CURRENT;
}

//获取功率
float INA228_Get_POWER(void){
	float POWER;
	uint8_t data1[3];
	INA228_Read_Mem(INA228_POWER,3,data1);
	uint32_t data = data1[0]<<16|data1[1]<<8|data1[2];
	POWER = data*CURRENT_LSB*3.2;
	return POWER;
}

//获取能量
float INA228_Get_ENERGY(void){
	float ENERGY;
	uint64_t data;
	uint8_t data1[5];
	INA228_Read_Mem(INA228_ENERGY,5,data1);
	data = data1[0];
	data = data<<32;
  data = data1[1]<<24|data1[2]<<16|data1[1]<<8|data1[0];
	ENERGY = 16.0*3.2*CURRENT_LSB*data;
	return ENERGY;
}

//获取电荷量
float INA228_Get_CHARGE(void){
	float CHARGE;
	uint64_t data;
	uint8_t data1[5];
	INA228_Read_Mem(INA228_CHARGE,5,data1);
	data = data1[0];
	data = data<<32;
  data = data1[1]<<24|data1[2]<<16|data1[1]<<8|data1[0];
	CHARGE = CURRENT_LSB*data;
	return CHARGE;
}

//将累积寄存器 ENERGY 和 CHARGE 的内容复位为 0
void INA228_RESET_ACC(void){
	uint16_t data = INA228_Config->RST<<15|INA228_Config->RSTACC<<14|INA228_Config->CONVDLY<<6|INA228_Config->TEMPCOMP<<5|INA228_Config->ADCRANGE<<4;
	data |= 0x4000;
  uint8_t data1[2]={data>>8,data};
	INA228_Write_Mem(INA228_CONFIG,2,data1);
}

//复位
void INA228_RESET(void){
	uint16_t data = 0x8000;
  uint8_t data1[2]={data>>8,data};
	INA228_Write_Mem(INA228_CONFIG,2,data1);
}

void INA228_Set_SOVL(){
};
