#include <xc.h>
#include <p18f4550.h>
#include <xlcd.h>
#include <delays.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "relogio.h"
#include "display_LCD.h"
#include "variaveis_globais.h"
#include "ad.h"
#include "serial.h"

#define POLY 0x8408 // O QUE ISSO FAZ???
#define _XTAL_FREQ 20000000 // DEFINE FREQUENCIA. 

#pragma config PBADEN = OFF // O QUE ISSO FAZ???
#pragma config WDT = OFF // DESATIVA WATCH DOG. 

void main(void){
    
    Relogio.segundo=0;
    Relogio.minuto=0;
    Relogio.hora=0;
    
    Despertador.segundo=5;
    Despertador.minuto=0;
    Despertador.hora=0;
    
    Calendario.dia=1;
    Calendario.mes=1;
    Calendario.ano=19;
    
    T0CONbits.TMR0ON=1; // HABILITA O TIMER 0;
    
    INTCONbits.TMR0IE=1; // HABILITA A FLAG DE INTERRUP��O.
    INTCONbits.TMR0IF=0; // LIMPA A FLAG.
    
    // UTILIZAR CLOCK INTERNO.
    T0CONbits.T08BIT=0;
    T0CONbits.PSA=0;
    T0CONbits.T0PS0=1;
    T0CONbits.T0PS1=0;
    T0CONbits.T0PS2=1;
    T0CONbits.T0CS=0; 
    
    TMR0=2535; // DE ONDE INICIA O CONTADOR. 
    
    T0CONbits.TMR0ON=1;

    // HABILITA PRA USAR INTERRUP��O HIGH OU LOW. 
    INTCONbits.GIEH=1; 
    INTCONbits.GIEL=1;
    
    // COMANDOS DO SERIAL
    //SPBRG = spbrg_value;
    ADCON1 = 15;
    TRISAbits.RA1 = 1;
    TRISA = 1; 
    PORTAbits.AN1 = 0;
    SPBRG=spbrg_value; // Fill the SPBRG register to set the Baud Rate
    RCSTA = 0b10010000; // 0x90 (SPEN RX9 SREN CREN ADEN FERR OERR RX9D)
    TXSTA = 0b00100000; // 0x20 (CSRC TX9 TXEN SYNC - BRGH TRMT TX9D)
    TRISCbits.RC6 = 0; //TX pin set as output
    TRISCbits.RC7 = 1; //RX pin set as input
    //compare with the table above
    RCIF = 0; //reset RX pin flag
    RCIP = 0; //Not high priority
    RCIE = 1; //Enable RX interrupt
    PEIE = 1; //Enable pheripheral interrupt (serial port is a pheripheral)
    
    // INICIALIZA��ES.
    init_XLCD(); // FUN��O DE INICIALIZA��O DO DISPLAY.
    init_ADC(); // FUN��O DE INICIALIZA��O DO AD.
    
    // -------------------------------------------------------------------------
    
    // LEDS DE TESTE. 
    TRISAbits.TRISA1=0; // ASCENDE QUANDO DESPERTA.
    TRISAbits.TRISA2=0; // PISCA QUANDO PASSA 1 SEGUNDO.
    
    // LCD.
    TRISB=0x00; // PARA LIGAR O LCD.
    
    // BOT�ES. 
    TRISDbits.TRISD0=1; // BOT�O PARA CONTROLAR EXIBI��O REL�GIO/DESPERTADOR.
    TRISDbits.TRISD1=1; // BOT�O PARA INCREMENTAR MANUALMENTE.
    TRISDbits.TRISD2=1; // BOT�O PARA DECREMENTAR MANUALMENTE.
    TRISDbits.TRISD3=1; // BOT�O PARA MOVER O CURSOR. 
    
    while(1){    
        checa_botao(&Relogio, &Despertador, &Calendario); // LISTENER DO BOT�O.
        convert();
        maquina_estado();
        //pisca_cursor(); // PISCA O CURSOR NO LOCAL DA EDI��O.
        
        // FLAG ATIVADA PELA interrupt CONTA 1 SEGUNDO.
        if(flag_time==1){
            tratar_relogio(&Relogio, &Calendario); // TRATA O REL�GIO.
            
            // MANTER O CURSOR DESLIGADO AO EXIBIR ALGO NA TELA. 
            WriteCmdXLCD(0x0C); // DESLIGA O CURSOR. 
            exibir_lcd(&Relogio, &Despertador, &Calendario); // MANT�M O LDC FUNCIONANDO. 
            WriteCmdXLCD(0x0F); // LIGA O CURSOR. 

            pisca_cursor(); // PISCA O CURSOR NO LOCAL DA EDI��O.
            
            // PISCA UM LED PARA MOSTRAR A CONTAGEM DE SEGUNDOS. 
            PORTAbits.RA2=1;
            __delay_ms(70);
            PORTAbits.RA2=0;    
            if(despertar==true) controle_tempo_despertar++;          
        }
        
        // ATIVA O DESPERTADOR.
        if(Relogio.segundo==Despertador.segundo
        && Relogio.minuto==Despertador.minuto
        && Relogio.hora==Despertador.hora){           
            despertar=true;
        }
        if(despertar==true && controle_tempo_despertar<10){ // TEMPO DE DURA��O DO DESPERTADOR.
            PORTAbits.RA1=1;            
        }
        if(controle_tempo_despertar==10){
            controle_tempo_despertar=0;
            despertar=false;
        }     
    }
    return;
}

 void __interrupt() pic_isr(void){
    // INTERRUP��O DO REL�GIO.
    if(TMR0IF){  
        TMR0=2535;
        INTCONbits.TMR0IF=0;
        contador++;
        flag_time=0;
    }
    //if(contador>9){
    if(contador>0){ // PARA FAZER O TEMPO PASSAR MAIS R�PIDO.
        flag_time=1; // LEVANTA UMA FLAG PARA A MAIN EXIBIR O VALOR. 
        contador=0;
    }
    
    // INTERRUP��O DO SERIAL.
    if(PIR1bits.RCIF == 1){
        if(numero_entrada_terminal<3){
            while(!RCIF); // Wait until RCIF gets low
            BUFF_COM[numero_entrada_terminal]= RCREG; // Retrieve data from reception register
            numero_entrada_terminal++;
        }
        else{
            PIR1bits.RCIF=0; // clear rx flag
            numero_entrada_terminal=0;
    		ESTADO_COMUNICACAO = COMANDO_RECEBIDO;            
        }
    }
}
 
 void gambiarra(){
    gambiarra_pro_serial();
}
