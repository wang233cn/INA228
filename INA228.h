#ifndef __INA228_H
#define __INA228_H

#include "main.h"

#define INA228_ADDR 0x40 << 1


#define INA228_CONFIG             0x0     //配置
#define INA228_ADC_CONFIG         0x1     //ADC配置
#define INA228_SHUNT_CAL          0x2     //分流校准
#define INA228_SHUNT_TEMPCO       0x3     //分流温度系数
#define INA228_VSHUNT             0x4     //分流电压测量
#define INA228_VBUS               0x5     //总线电压测量
#define INA228_DIETEMP            0x6     //温度测量
#define INA228_CURRENT            0x7     //电流结果
#define INA228_POWER              0x8     //功率结果
#define INA228_ENERGY             0x9     //电能结果
#define INA228_CHARGE             0xA     //电荷结果
#define INA228_DIAG_ALRT          0xB     //诊断标志和警报
#define INA228_SOVL               0xC     //分流过压阈值
#define INA228_SUVL               0xD     //分流欠压阈值
#define INA228_BOVL               0xE     //总线过压阈值
#define INA228_BUVL               0xF     //总线欠压阈值
#define INA228_TEMP_LIMIT         0x10     //温度高于上限阈值
#define INA228_PWR_LIMIT          0x11     //功率高于上限阈值
#define INA228_MANUFACTURER_ID    0x3E     //制造商ID
#define INA228_DEVICE_ID          0x3F     //器件ID


typedef struct
{
	uint8_t RST;
	uint8_t RSTACC;
	uint8_t CONVDLY;
	uint8_t TEMPCOMP;
	uint8_t ADCRANGE;
	float MEC;//MaximumExpectedCurrent
}INA228_CONFIGTypeDef;

typedef struct
{
	uint8_t MODE;
	uint8_t VBUSCT;
	uint8_t VSHCT;
	uint8_t VTCT;
	uint8_t AVG;
}INA228_ADCTypeDef;


typedef struct
{
	uint8_t ALATCH;
	uint8_t CNVR;
	uint8_t SLOWALERT;
	uint8_t APOL;
	uint8_t ENERGYOF;  //Read Only
	uint8_t CHARGEOF;  //Read Only
	uint8_t MATHOF;    //Read Only
	uint8_t RESERVE;   //Read Only
	uint8_t TMPOL;
	uint8_t SHNTOL;
	uint8_t SHNTUL;
	uint8_t BUSOL;
	uint8_t BUSUL;
	uint8_t POL;
	uint8_t CNVRF;
	uint8_t MEMSTAT;
	
}INA228_ALRETTypeDef;


uint8_t INA228_Init(INA228_CONFIGTypeDef *INA228_Init);
void INA228_ADC_Config(INA228_ADCTypeDef *INA228_ADCInit);
void INA228_SET_SHUNT_CAL(uint16_t RSHUNT,uint16_t ppm);
float INA228_Get_VSHUNT(void);
float INA228_Get_VBUS(void);
float INA228_Get_DIETEMP(void);
float INA228_Get_CURRENT(void);
float INA228_Get_POWER(void);
float INA228_Get_ENERGY(void);
float INA228_Get_CHARGE(void);
void INA228_RESET_ACC(void);

#endif