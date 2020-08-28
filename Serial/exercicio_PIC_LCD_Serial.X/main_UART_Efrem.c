#include <xc.h>
#include <plib/usart.h>
#include <p18f4550.h>

#define _XTAL_FREQ 12000000 //The speed of your internal(or)external oscillator

void tx_data(unsigned char);

unsigned char rx_data(void);
unsigned char serial_data;
unsigned int i=0;

unsigned char dado[5];
unsigned char BUFF_RESP[5];

#define FREQ 12000000 // Frequency = 12MHz
#define baud 9600
#define spbrg_value (((FREQ/64)/baud)-1) // Refer to the formula for Baud rate calculation in Description tab

#define ESPERA_COMANDO 0x00
#define COMANDO_RECEBIDO 0x01
#define VERIFICAR_COMANDO 0x02
#define COMANDO_OK 0x03
#define COMANDO_NOK 0x04
#define ENVIA_RESPOSTA 0x05

#define COMANDO_LEITURA 0x01
#define COMANDO_ALTERACAO 0x02

#define HORA 0x01

int ESTADO_COMUNICACAO;
int segundo=0, minuto=0, hora=0;

void verifica_comando(){
	int comando_ta_ok_tem_que_implementar_esta_vetificacao = 1;
	if(comando_ta_ok_tem_que_implementar_esta_vetificacao)
		ESTADO_COMUNICACAO = COMANDO_OK;
	else
		ESTADO_COMUNICACAO = COMANDO_NOK;
}

void envia_respostaTX(){
	tx_data(BUFF_RESP[0]);
	tx_data(BUFF_RESP[1]);
	tx_data(BUFF_RESP[2]);
	tx_data(BUFF_RESP[3]);
	tx_data(BUFF_RESP[4]);
}

void trata_comando(){
    
	switch(dado[0]){

		case COMANDO_LEITURA:

			switch(dado[1]){
				case HORA:
					BUFF_RESP[0] = dado[0];
					BUFF_RESP[1] = dado[1];
					BUFF_RESP[2] = hora;
 					BUFF_RESP[3] = minuto;
					BUFF_RESP[4] = segundo;
					BUFF_RESP[5] = 0xFF;
					BUFF_RESP[6] = 0xFF;
					ESTADO_COMUNICACAO = ENVIA_RESPOSTA;										
                    break;
			}
		break;
        
		case COMANDO_ALTERACAO:
            break;		
	}
}

void maquina_estado(){
	switch(ESTADO_COMUNICACAO){
		case COMANDO_RECEBIDO:
			ESTADO_COMUNICACAO = VERIFICAR_COMANDO;		
		break;

		case VERIFICAR_COMANDO:
			verifica_comando();		
		break;
		
		case COMANDO_OK:
			trata_comando();
		break;

		case COMANDO_NOK:
			ESTADO_COMUNICACAO = ESPERA_COMANDO;	
		break;

		case ENVIA_RESPOSTA:
			envia_respostaTX();
			ESTADO_COMUNICACAO = ESPERA_COMANDO;
		break;

		case ESPERA_COMANDO:	
		break;
	};
}

void tx_data(unsigned char data1){
    while(!TXIF); // Wait until RCIF gets low
    TXREG = data1;
}

unsigned char rx_data(void){
    while(!RCIF); // Wait until RCIF gets low
    return RCREG; // Retrieve data from reception register
}

void interrupt SerialRxPinInterrupt(){
    //check if the interrupt is caused by RX pin
    if(PIR1bits.RCIF == 1){
        if(i<5){
            while(!RCIF); // Wait until RCIF gets low
            dado[i]= RCREG; // Retrieve data from reception register
            i++;  
        }
        else{
            PIR1bits.RCIF = 0; // clear rx flag
            i=0;
    		ESTADO_COMUNICACAO = COMANDO_RECEBIDO;
        }
    }
}

void main(){
    SPBRG=spbrg value; // Fill the SPBRG register to set the Baud Rate
    //RCSTA.SPEN=1; // To activate Serial port (TX and RX pins)
    // TXSTA.TXEN=1; // To enable transmission
    // RCSTA.CREN=1; // To enable continuous reception
   
    RCSTA = 0b10010000; // 0x90 (SPEN RX9 SREN CREN ADEN FERR OERR RX9D)
    TXSTA = 0b00100000; // 0x20 (CSRC TX9 TXEN SYNC - BRGH TRMT TX9D)
    
    TRISCbits.RC6 = 0; //TX pin set as output
    TRISCbits.RC7 = 1; //RX pin set as input
    
    //compare with the table above
    RCIF = 0; //reset RX pin flag
    RCIP = 0; //Not high priority
    RCIE = 1; //Enable RX interrupt
    PEIE = 1; //Enable pheripheral interrupt (serial port is a pheripheral)

    ESTADO_COMUNICACAO = ESPERA_COMANDO;
    
    while(1){
        // serial_data=rx_data();    // Receive data from PC
        // tx_data(serial_data);        // Transmit the same data back to PC
        maquina_estado();
    }
}
