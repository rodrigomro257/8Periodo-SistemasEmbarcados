#include <xc.h>
#include <plib/usart.h>
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
#include "serial.h"

#define POLY 0x8408
#define FREQ 20000000 // DEFINE FREQUENCIA.
#define _XTAL_FREQ 20000000

#pragma config PBADEN = OFF
#pragma config WDT = OFF // DESATIVA WATCH DOG. 

void main(void){
    
    // INICIALIZAÇÕES DO SERIAL:
    SPBRG = spbrg_value;

    ADCON1 = 15;
    TRISAbits.RA1 = 1;
    TRISA = 1; 
    PORTAbits.AN1 = 0; 
    
    
     // -------------------------------------------------------------------------
    
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
    
    INTCONbits.TMR0IE=1; // HABILITA A FLAG DE INTERRUPÇÃO.
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

    // HABILITA PRA USAR INTERRUPÇÃO HIGH OU LOW. 
    INTCONbits.GIEH=1; 
    INTCONbits.GIEL=1;
   
    init_XLCD(); // FUNÇÃO DE INICIALIZAÇÃO DO DISPLAY.
    
    // -------------------------------------------------------------------------
    
    // LEDS DE TESTE. 
    TRISAbits.TRISA0=0; // PISCA QUANDO PASSA 1 SEGUNDO.
    TRISAbits.TRISA1=0; // ASCENDE QUANDO DESPERTA.
    TRISCbits.TRISC0=0; // PISCA LED PRA SABER SE O CÓDIGO PASSOU EM DETERMINADO PONTO. 
    
    // LCD.
    TRISB=0x00; // PARA LIGAR O LCD.
    
    // BOTÕES. 
    TRISDbits.TRISD0=1; // BOTÃO PARA CONTROLAR EXIBIÇÃO RELÓGIO/DESPERTADOR.
    TRISDbits.TRISD1=1; // BOTÃO PARA INCREMENTAR MANUALMENTE.
    TRISDbits.TRISD2=1; // BOTÃO PARA DECREMENTAR MANUALMENTE.
    TRISDbits.TRISD3=1; // BOTÃO PARA MOVER O CURSOR. 

    // TERMINAL.
    TRISCbits.RC6=0; // TX pin set as output
    TRISCbits.RC7=1; // RX pin set as input
    
    // INICIALIZAÇÕES DO PROTOCOLO.
    RCSTA = 0b10010000; // 0x90 (SPEN RX9 SREN CREN ADEN FERR OERR RX9D)
    TXSTA = 0b00100000; // 0x20 (CSRC TX9 TXEN SYNC - BRGH TRMT TX9D)
    
    // compare with the table above
    RCIF = 0; //reset RX pin flag
    RCIP = 0; //Not high priority
    RCIE = 1; //Enable RX interrupt
    PEIE = 1; //Enable pheripheral interrupt (serial port is a pheripheral)

    ESTADO_COMUNICACAO = ESPERA_COMANDO;
    
    // -------------------------------------------------------------------------
    
    while(1){     
        
        maquina_estado(); // CHAMA A MÁQUINA DE ESTADOS.
        
        checa_botao(&Relogio, &Despertador, &Calendario); // LISTENER DO BOTÃO.
        
        // FLAG ATIVADA PELA interrupt CONTA 1 SEGUNDO.
        if(flag_time==1){
            tratar_relogio(&Relogio, &Calendario); // TRATA O RELÓGIO.
            
            // MANTER O CURSOR DESLIGADO AO EXIBIR ALGO NA TELA. 
            WriteCmdXLCD(0x0C); // DESLIGA O CURSOR. 
            exibir_lcd(&Relogio, &Despertador, &Calendario); // MANTÉM O LDC FUNCIONANDO. 
            //gambiarra_pro_serial();
            WriteCmdXLCD(0x0F); // LIGA O CURSOR. 
            
            pisca_cursor(); // PISCA O CURSOR NO LOCAL DA EDIÇÃO.
            
            // PISCA UM LED PARA MOSTRAR A CONTAGEM DE SEGUNDOS. 
            PORTAbits.RA0=1;
            __delay_ms(70);
            PORTAbits.RA0=0;    
            if(despertar) controle_tempo_despertar++;          
        }
        
        // ATIVA O DESPERTADOR.
        if(Relogio.segundo==Despertador.segundo
        && Relogio.minuto==Despertador.minuto
        && Relogio.hora==Despertador.hora) despertar=true;         

        if(despertar==true && controle_tempo_despertar<10) PORTAbits.RA1=1; // TEMPO DE DURAÇÃO DO DESPERTADOR.
        else PORTAbits.RA1=0;
        
        if(controle_tempo_despertar==10){
            controle_tempo_despertar=0;
            despertar=false;
        }
    }
    return;
}

void interrupt SerialRxPinInterrupt(){

    // INTERRUPÇÃO DO RELÓGIO.
    if(TMR0IF){  
        TMR0=2535;
        INTCONbits.TMR0IF=0;
        contador++;
        flag_time=0;
    }
    if(contador>0){ // PARA FAZER O TEMPO PASSAR MAIS RÁPIDO.
        flag_time=1; // LEVANTA UMA FLAG PARA A MAIN EXIBIR O VALOR. 
        contador=0;
    }
    
    // INTERRUPÇÃO DO SERIAL.
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
