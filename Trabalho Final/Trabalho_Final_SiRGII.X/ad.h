#ifndef AD_H
#define	AD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <p18f4550.h>
#include <xc.h>
#include <delays.h>
#include <adc.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define FREQ = 20000000;

unsigned int ADCResult = 0;
float valor_tensao=0;
char temperatura_display[10];
int temperatura_minima=25;
int temperatura_maxima=30;

void init_ADC(void);
void convert(void);
void convert_float_to_char(float valor);
void ativa_atuadores(void);

#ifdef	__cplusplus
}
#endif

#endif	/* AD_H */
