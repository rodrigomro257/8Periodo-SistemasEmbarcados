#include "display_LCD.h"
#include "eeprom.h"

void DelayFor18TCY(void){
    Delay10TCYx(120);
}

void DelayPORXLCD(void){
    Delay1KTCYx(180);
    return;
}
 
void DelayXLCD(void){
    Delay1KTCYx(60);
    return;
}

// INICIALIADOR DO DISPLAY LCD.
void init_XLCD(VOID){
    OpenXLCD(FOUR_BIT&LINES_5X7); // CONFIGURA LCD EM 4bit DE DADOS E 5X7 characters. 
    while (BusyXLCD()); // VERIFICA SE O CONTROLADOR DO DISPLAY ESTÁ OCUPADO. 
    WriteCmdXLCD(0x06); // MOVE O CURSOR PARA A ESQUERA. 
    WriteCmdXLCD(0x0C); // LIGA O DISPLAY. 
}

void exibir_lcd(){
    // VARIÁVEIS PARA CONVERTER int EM char E EXIBIR NO DISPLAY.
    int a, b, c, d;
    // AD - TEMPERATURA.
    if(tela_exibida==0){
        convert_float_to_char(valor_tensao); // CONVERSÃO DO VALOR DO AD.
        sprintf(str,"TEMP: %s", temperatura_display); // CONCATENA LINHA 1.
        SetDDRamAddr(0x00); // POSICIONA O CURSOR NA LINHA 1.
        putrsXLCD(str); // ESCREVE.
        a=((temperatura_minima)/10)-(((temperatura_minima)%10)/10); // DEZENA.
        b=temperatura_minima%10; // UNIDADE.
        c=((temperatura_maxima)/10)-(((temperatura_maxima)%10)/10); // DEZENA.
        d=temperatura_maxima%10; // UNIDADE.
        sprintf(str, "MIN: %d%d MAX: %d%d", a, b, c, d); // CONCATENA LINHA 2.
        SetDDRamAddr(0x40); // POSICIONA O CURSOR NA LINHA 2. 
        putrsXLCD(str); // ESCREVE.
    }   
    // SERIAL.
    if(tela_exibida==1){
        gambiarra(); // NÃO PODE FALTAR!!!
    }
    // EEPROM
    if(tela_exibida==2){
        convert_float_to_char(valor_tensao); // CONVERSÃO DO VALOR DO AD.
        sprintf(str,"TEMP: %s", temperatura_display); // CONCATENA LINHA 1.
        SetDDRamAddr(0x00); // POSICIONA O CURSOR NA LINHA 1.
        putrsXLCD(str); // ESCREVE.
        sprintf(str, "HORA: %d          ", tempo_eeprom); // CONCATENA LINHA 2.
        SetDDRamAddr(0x40); // POSICIONA O CURSOR NA LINHA 2. 
        putrsXLCD(str); // ESCREVE.
    }
}

void checa_botao(){
    // TROCAR DE TELA. 
    if(PORTDbits.RD0==1){
        __delay_ms(180); 
        tela_exibida++;
        cursor=0; // SEMPRE QUE TROCO DE TELA O CURSOR VOLTA PRO INÍCIO.
        while(PORTDbits.RD0==1){}
    }
    if(tela_exibida>2) tela_exibida=0; // TENHO 2 TELAS PARA EXIBIR. 
    // INCREMENTAR VARIÁVEL.
    if(PORTDbits.RD1==1){
        __delay_ms(180);  
        if(tela_exibida==0){
            if(cursor==0) temperatura_minima=temperatura_minima+10;
            if(cursor==1) temperatura_minima++;
            if(cursor==2) temperatura_maxima=temperatura_maxima+10;
            if(cursor==3) temperatura_maxima++;            
        }
        while(PORTDbits.RD1==1){}
    }
    // DECREMENTAR VARIÁVEL.
    if(PORTDbits.RD2==1){
        __delay_ms(180);   
        if(tela_exibida==0){
            if(cursor==0) temperatura_minima=temperatura_minima-10;
            if(cursor==1) temperatura_minima--;
            if(cursor==2) temperatura_maxima=temperatura_maxima-10;
            if(cursor==3) temperatura_maxima--;
        }
        while(PORTDbits.RD2==1){}
    }
    // POSICIONAR CURSOR.
    if(PORTDbits.RD3==1){
        __delay_ms(180);
        cursor++;
        if(cursor>3 && tela_exibida==0) cursor=0; // 4 VARIÁVEIS DE TEMPERATURA.
        while(PORTDbits.RD3==1){}
    }
}

void pisca_cursor(){    
    if(tela_exibida==0){
        if(cursor==0) SetDDRamAddr(0x45); // TEMPERATURA MÍMIMA DEZENA.
        if(cursor==1) SetDDRamAddr(0x46); // TEMPERATURA MÍNIMA UNIDADE.
        if(cursor==2) SetDDRamAddr(0x4D); // TEMPERATURA MÁXIMA DEZENA.
        if(cursor==3) SetDDRamAddr(0x4E); // TEMPERATURA MÁXIMA UNIDADE. 
    }
}
