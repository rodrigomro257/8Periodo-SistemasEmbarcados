/* 
 * File:   serial.h
 * Author: Windows
 *
 * Created on 29 de Maio de 2019, 09:14
 */

#ifndef SERIAL_H
#define	SERIAL_H

#include "display_LCD.h"
#include "ad.h"

unsigned char rx_data(void);
unsigned char serial_data;

unsigned char BUFF_COM[3];
unsigned char BUFF_RESP[3];

#define FREQ 20000000 // DEFINE FREQUENCIA.
#define _XTAL_FREQ 20000000
#define baud 9600
#define spbrg_value (((FREQ/64)/baud)-1) // Refer to the formula for Baud rate calculation in Description tab

#define ESPERA_COMANDO '0'
#define COMANDO_RECEBIDO '1'
#define ENVIA_RESPOSTA '2'
 
int ESTADO_COMUNICACAO;

void tx_data() {
    int cont;
    while (!TXIF);

    for (cont=0; cont<3; cont++) {
        while (!TXIF);
        TXREG = BUFF_RESP[cont];
    }
    //ESTADO_COMUNICACAO = ESPERA_COMANDO;
}

unsigned char rx_data(void){
    while(!RCIF); // Wait until RCIF gets low
    return RCREG; // Retrieve data from reception register
} 

void gambiarra_pro_serial(){
    // EXIBE NO DISPLAY O QUE FOI LIDO DO TERMINAL.
    WriteCmdXLCD(0x0C); // DESLIGA O CURSOR. 
    SetDDRamAddr(0x00);
    sprintf(str, "SERIAL:         ");
    putrsXLCD(str);
    SetDDRamAddr(0x40); // POSICIONA O CURSOR NA LINHA 2. 
    sprintf(str, "%d %d %d           ", BUFF_RESP[1]-48, BUFF_RESP[2]-48, BUFF_RESP[0]-48);
    putrsXLCD(str);    
}

void trata_comando(){
    // MONTA RESPOSTA.
    BUFF_RESP[0] = BUFF_COM[0];
    BUFF_RESP[1] = BUFF_COM[1];
    BUFF_RESP[2] = BUFF_COM[2];
    
    switch(BUFF_COM[1]){
		case 49: // REFERENTE AO VALOR 1.
			switch(BUFF_COM[2]){
                case 49: // REFERENTE AO VALOR 1.
                    temperatura_minima=((int)BUFF_COM[0]-48)*10+(temperatura_minima%10);
                    break;
                    
                case 50: // REFERENTE AO VALOR 2.
                    temperatura_minima=((int)BUFF_COM[0]-48)+(temperatura_minima-(temperatura_minima%10));
                    break;
                    
                default:
                    break;
            };
            break;

		case 50: // REFERENTE AO VALOR 2.
			switch(BUFF_COM[2]){
                case 49: // REFERENTE AO VALOR 1.
                    temperatura_maxima=((int)BUFF_COM[0]-48)*10+(temperatura_maxima%10);
                    break;
                    
                case 50: // REFERENTE AO VALOR 2.
                    temperatura_maxima=((int)BUFF_COM[0]-48)+(temperatura_maxima-(temperatura_maxima%10));
                    break;
                    
                default:
                    break;
            };
            break;
        
        default:
            break;
	};
    
    ESTADO_COMUNICACAO = ENVIA_RESPOSTA;            
}

void maquina_estado(){
	switch(ESTADO_COMUNICACAO){
		case COMANDO_RECEBIDO:
			trata_comando();		
		break;

		case ENVIA_RESPOSTA:
			tx_data();
			ESTADO_COMUNICACAO = ESPERA_COMANDO;
		break;

		case ESPERA_COMANDO:
		break;
	};
}

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* SERIAL_H */
