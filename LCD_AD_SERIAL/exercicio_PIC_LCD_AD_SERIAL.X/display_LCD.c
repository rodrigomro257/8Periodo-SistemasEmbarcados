#include "display_LCD.h"

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

void exibir_lcd(relogio *Relogio, relogio *Despertador, calendario *Calendario){
    // VARIÁVEIS PARA CONVERTER int EM char E EXIBIR NO DISPLAY.
    int a, b, c, d, e, f, g, h, i, j, k, l;
    char str[16];
    
    // RELÓGIO E DATA.
    if(tela_exibida==0){
        a=((Relogio->hora)/10)-(((Relogio->hora)%10)/10);
        b=Relogio->hora%10;
        c=((Relogio->minuto)/10)-(((Relogio->minuto)%10)/10);
        d=Relogio->minuto%10;
        e=((Relogio->segundo)/10)-(((Relogio->segundo)%10)/10);
        f=Relogio->segundo%10;       
        sprintf(str, "HORA: %d%d:%d%d:%d%d ", a, b, c, d, e, f); // CONCATENAÇÃO EM VETOR char.
        SetDDRamAddr(0x00); // POSICIONA O CURSOR NA LINHA 1.
        putrsXLCD(str); // ESCREVE. 
        //WriteCmdXLCD(CURSOR_ON); // ATIVA O CURSOR.
        //WriteCmdXLCD(BLINK_ON); // FAZ O CURSOR PISCAR.
        
        g=((Calendario->dia)/10)-(((Calendario->dia)%10)/10);
        h=Calendario->dia%10;
        i=((Calendario->mes)/10)-(((Calendario->mes)%10)/10);
        j=Calendario->mes%10;
        k=((Calendario->ano)/10)-(((Calendario->ano)%10)/10);
        l=Calendario->ano%10;
        sprintf(str, "DATA: %d%d:%d%d:%d%d ", g, h, i, j, k, l); // CONCATENAÇÃO EM VETOR char.
        SetDDRamAddr(0x40); // POSICIONA O CURSOR NA LINHA 2. 
        putrsXLCD(str); // ESCREVE. 
    }
   
    // DESPERTADOR.
    if(tela_exibida==1){
        a=((Despertador->hora)/10)-(((Despertador->hora)%10)/10);
        b=Despertador->hora%10;
        c=((Despertador->minuto)/10)-(((Despertador->minuto)%10)/10);
        d=Despertador->minuto%10;
        e=((Despertador->segundo)/10)-(((Despertador->segundo)%10)/10);
        f=Despertador->segundo%10;
        
        sprintf(str, "%d%d:%d%d:%d%d        ", a, b, c, d, e, f); // CONCATENAÇÃO EM VETOR char.
        SetDDRamAddr(0x00); // POSICIONA O CURSOR NA LINHA 1.
        putrsXLCD("DESPERTADOR:    "); // ESCREVE. 
        SetDDRamAddr(0x40); // POSICIONA O CURSOR NA LINHA 2. 
        putrsXLCD(str); // ESCREVE.
    }
    
    // AD - TEMPERATURA.
    if(tela_exibida==2){
        convert_float_to_char(valor_tensao);
        SetDDRamAddr(0x00); // POSICIONA O CURSOR NA LINHA 1.
        putrsXLCD("TEMPERATURA:    "); // ESCREVE.
        SetDDRamAddr(0x40); // POSICIONA O CURSOR NA LINHA 2. 
        putrsXLCD(temperatura_display); // ESCREVE.
    }
    
    // SERIAL.
    if(tela_exibida==3){
        gambiarra();
    }
}

void checa_botao(relogio *Relogio, relogio *Despertador, calendario *Calendario){
    
    // TROCAR DE TELA. 
    if(PORTDbits.RD0==1){
        __delay_ms(180); 
        tela_exibida++;
        cursor=0; // SEMPRE QUE TROCO DE TELA O CURSOR VOLTA PRO INÍCIO.
        while(PORTDbits.RD0==1){}
    }
    if(tela_exibida>3) tela_exibida=0; // TENHO 4 TELAS PARA EXIBIR. 
    
    // INCREMENTAR VARIÁVEL.
    if(PORTDbits.RD1==1){
        __delay_ms(180);  
       
        if(tela_exibida==0){
            if(cursor==0) Relogio->hora=Relogio->hora+10;
            if(cursor==1) Relogio->hora++;
            if(cursor==2) Relogio->minuto=Relogio->minuto+10;
            if(cursor==3) Relogio->minuto++;
            if(cursor==4) Relogio->segundo=Relogio->segundo+10;
            if(cursor==5) Relogio->segundo++;
            if(cursor==6) Calendario->dia=Calendario->dia+10;
            if(cursor==7) Calendario->dia++;
            if(cursor==8) Calendario->mes=Calendario->mes+10;
            if(cursor==9) Calendario->mes++;
            if(cursor==10) Calendario->ano=Calendario->ano+10;
            if(cursor==11) Calendario->ano++;
        }
        
        if(tela_exibida==1){
            if(cursor==0) Despertador->hora=Despertador->hora+10;
            if(cursor==1) Despertador->hora++;
            if(cursor==2) Despertador->minuto=Despertador->minuto+10;
            if(cursor==3) Despertador->minuto++;
            if(cursor==4) Despertador->segundo=Despertador->segundo+10;
            if(cursor==5) Despertador->segundo++;
            controla_despertador(Despertador);
        }
        
        while(PORTDbits.RD1==1){}
    }
    
    // DECREMENTAR VARIÁVEL.
    if(PORTDbits.RD2==1){
        __delay_ms(180);   
        
        if(tela_exibida==0){
            if(cursor==0) Relogio->hora=Relogio->hora-10;
            if(cursor==1) Relogio->hora--;
            if(cursor==2) Relogio->minuto=Relogio->minuto-10;
            if(cursor==3) Relogio->minuto--;
            if(cursor==4) Relogio->segundo=Relogio->segundo-10;
            if(cursor==5) Relogio->segundo--;
            if(cursor==6) Calendario->dia=Calendario->dia-10;
            if(cursor==7) Calendario->dia--;
            if(cursor==8) Calendario->mes=Calendario->mes-10;
            if(cursor==9) Calendario->mes--;
            if(cursor==10) Calendario->ano=Calendario->ano-10;
            if(cursor==11) Calendario->ano--;
        }
        
        if(tela_exibida==1){
            if(cursor==0) Despertador->hora=Despertador->hora-10;
            if(cursor==1) Despertador->hora--;
            if(cursor==2) Despertador->minuto=Despertador->minuto-10;
            if(cursor==3) Despertador->minuto--;
            if(cursor==4) Despertador->segundo=Despertador->segundo-10;
            if(cursor==5) Despertador->segundo--;
            controla_despertador(Despertador);
        }
        while(PORTDbits.RD2==1){}
    }
    
    // POSICIONAR CURSOR.
    if(PORTDbits.RD3==1){
        __delay_ms(180);
        cursor++;
        if(cursor>11 && tela_exibida==0) cursor=0; // 11 VARIÁVEIS DE HORA E DATA.
        if(cursor>5 && tela_exibida==1) cursor=0; // 6 VARIÁVEIS DO DESPERTADOR.
        while(PORTDbits.RD3==1){}
    }
}

void pisca_cursor(){    
    if(tela_exibida==0){
        if(cursor==0) SetDDRamAddr(0x06); // HORA DEZENA.
        if(cursor==1) SetDDRamAddr(0x07); // HORA UNIDADE.
        if(cursor==2) SetDDRamAddr(0x09); // MINUTO DEZENA.
        if(cursor==3) SetDDRamAddr(0x0A); // MINUTO UNIDADE.
        if(cursor==4) SetDDRamAddr(0x0C); // SEGUNDO DEZENA.
        if(cursor==5) SetDDRamAddr(0x0D); // SEGUNDO UNIDADE.
        if(cursor==6) SetDDRamAddr(0x46); // DIA DEZENA.
        if(cursor==7) SetDDRamAddr(0x47); // DIA UNIDADE.
        if(cursor==8) SetDDRamAddr(0x49); // MÊS DEZENA.
        if(cursor==9) SetDDRamAddr(0x4A); // MÊS UNIDADE.
        if(cursor==10) SetDDRamAddr(0x4C); // ANO DEZENA.
        if(cursor==11) SetDDRamAddr(0x4D); // ANO UNIDADE. 
    }
    
    if(tela_exibida==1){
        if(cursor==0) SetDDRamAddr(0x40); // HORA DEZENA.
        if(cursor==1) SetDDRamAddr(0x41); // HORA UNIDADE.
        if(cursor==2) SetDDRamAddr(0x43); // MINUTO DEZENA.
        if(cursor==3) SetDDRamAddr(0x44); // MINUTO UNIDADE.
        if(cursor==4) SetDDRamAddr(0x46); // SEGUNDO DEZENA.
        if(cursor==5) SetDDRamAddr(0x47); // SEGUNDO UNIDADE.
    }
}
