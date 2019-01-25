#ifndef __SMT_ADC_H
#define __SMT_ADC_H

#ifdef __cplusplus
extern "C"{
#endif

void ADC_xPWMFOC_INTHandler(void);
void testADC_xPWMFOC(void);
void adc_normal_handler(void);  
void ADC_normalTest(void);
void por_test(void);
#define C_KEY_RELEASE  		0x80
#define C_KEY_HOLD       	0x40

#define KEY_NULL 			0xffffffff

#define KEY_SW1  			1
#define KEY_SW2  			2
#define KEY_SW3  			3
#define KEY_SW4  			4
#define KEY_SW5  			5
#define KEY_SW6  			6
#define KEY_SW7  			7
#define KEY_SW8  			8
#define KEY_SW9  			9
#ifdef __cplusplus
}
#endif

#endif /*__SMT_ADC_H*/