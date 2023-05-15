# INA228
INA228 driving library for stm32 HAL
Test ON STM32F411CEU6

```
使用示例：
Example:
void INA228_LOAD_Config(void){
	//初始化INA228配置   Initialize INA228 configuration
	INA228_MainConfig.ADCRANGE = 0;
	INA228_MainConfig.MEC = 3.2768;
	INA228_MainConfig.TEMPCOMP = 1;
	INA228_MainConfig.RST = 0;
	INA228_MainConfig.RSTACC = 1;
	INA228_Init(&INA228_MainConfig);
	//初始化INA228 ADC配置  Initialize INA228 ADC configuration
	INA228_ADC_MainConfig.MODE = 0xF;
	INA228_ADC_MainConfig.VBUSCT = 0x07;
	INA228_ADC_MainConfig.VSHCT = 0x07;
	INA228_ADC_MainConfig.VTCT = 0x07;
	INA228_ADC_MainConfig.AVG = 0x02;
	INA228_ADC_Config(&INA228_ADC_MainConfig);
	//设置Rshunt阻值和温漂  Set Rshunt resistance and temperature drift
	INA228_SET_SHUNT_CAL(50,20);
}
```
