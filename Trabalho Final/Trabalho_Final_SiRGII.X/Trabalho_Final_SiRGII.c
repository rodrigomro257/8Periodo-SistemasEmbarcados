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
#include "eeprom.h"

#define POLY 0x8408
#define _XTAL_FREQ 20000000 // DEFINE FREQUENCIA. 

// A LINHA ABAIXO ENTRA EM CONFLITO COM UM #pragma NO eeprom_osc.h.
#pragma config WDT = OFF // DESATIVA WATCH DOG. 

void main(void){
    
    Relogio.segundo=0;
    Relogio.minuto=0;
    Relogio.hora=0;

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
    
    // COMANDOS DO SERIAL
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
    
    // INICIALIZAÇÕES.
    init_XLCD(); // FUNÇÃO DE INICIALIZAÇÃO DO DISPLAY.
    init_ADC(); // FUNÇÃO DE INICIALIZAÇÃO DO AD.
    USART_Init(9600); // FUNÇÃO DE INICIALIZAÇÃO DO EEPROM - initialize USART operation with 9600 baud rate */ 
    
    // -------------------------------------------------------------------------
    
    // LEDS. 
    TRISAbits.TRISA2=0; // LED VERDE - CONTAGEM DE SEGUNDOS.
    TRISDbits.TRISD4=0; // LED VERMELHO - COOLER AR QUENTE.
    TRISDbits.TRISD5=0; // LED AMARELO - PELTIER.
    TRISDbits.TRISD6=0; // LED AZUL - COOLER AR FRIO.
    
    // LCD.
    TRISB=0x00; // PARA LIGAR O LCD.
    
    // BOTÕES. 
    TRISDbits.TRISD0=1; // BOTÃO PARA CONTROLAR EXIBIÇÃO TEMPERATURA / SERIAL.
    TRISDbits.TRISD1=1; // BOTÃO PARA INCREMENTAR MANUALMENTE.
    TRISDbits.TRISD2=1; // BOTÃO PARA DECREMENTAR MANUALMENTE.
    TRISDbits.TRISD3=1; // BOTÃO PARA MOVER O CURSOR.
    
    // -------------------------------------------------------------------------
    
    // EEPROM.
    OSCCON=0x72; // Use internal Oscillator with 8 Mhz Frequency
    
    // -------------------------------------------------------------------------
    
    while(1){
        checa_botao(); // LISTENER DOS BOTÕES.
        convert(); // CONVERSOR DO AD.
        ativa_atuadores(); // CONTROLA PELTIER E COOLERS.
        maquina_estado(); // MÁQUINA DE ESTADOS DO SERIAL.
        WriteCmdXLCD(0x0C); // DESLIGA O CURSOR.
        exibir_lcd(); // MANTÉM O LDC FUNCIONANDO.
        WriteCmdXLCD(0x0F); // LIGA O CURSOR.
        pisca_cursor(); // PISCA O CURSOR NO LOCAL DA EDIÇÃO.
        // FLAG ATIVADA PELA interrupt CONTA 1 SEGUNDO.
        if(flag_time==1){
            tratar_relogio(&Relogio); // TRATA O RELÓGIO.           
            PORTAbits.RA2=1;
            __delay_ms(70); // PISCA UM LED PARA MOSTRAR A CONTAGEM DE SEGUNDOS. 
            PORTAbits.RA2=0;    
            tempo_eeprom++;
        }
        // PARA A EEPROM:
        io_eeprom();
    }
    return;
}

 void __interrupt() pic_isr(void){
     // INTERRUPÇÃO DO RELÓGIO.
    if(TMR0IF){  
        TMR0=2535;
        INTCONbits.TMR0IF=0;
        contador++;
        flag_time=0;
    }
    //if(contador>9){
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
    // POR ALGUM MOTIVO NÃO POSSO FAZER O display.h  HERDAR DE serial.h.
    // GAMBIARRA ALTAMENTE NECESSÁRIA.
    gambiarra_pro_serial();
}
