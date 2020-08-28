/* 
 * File:   display_LCD.h
 * Author: Windows
 *
 * Created on 25 de Abril de 2019, 14:59
 */

#ifndef DISPLAY_LCD_H
#define	DISPLAY_LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <delays.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "variaveis_globais.h"
#include "ad.h"
    
#define _XTAL_FREQ 20000000 // DEFINE FREQUENCIA. 
    
void DelayFor18TCY(void);
void DelayPORXLCD(void);
void DelayXLCD(void);

void init_XLCD(VOID); // INICIALIADOR DO DISPLAY LCD.
void exibir_lcd();
void checa_botao();
void pisca_cursor();

#ifdef	__cplusplus
}
#endif

#endif	/* DISPLAY_LCD_H */

