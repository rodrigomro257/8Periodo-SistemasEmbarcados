#include "eeprom.h"

void USART_Init(int baud_rate){
    float temp;
    TRISC6=0;                       /*Make Tx pin as output*/
    TRISC7=1;                       /*Make Rx pin as input*/
    temp=(((float)(F_CPU)/(float)baud_rate)-1);     
    SPBRG=(int)temp;                /*baud rate=9600 SPBRG=(F_CPU /(64*9600))-1*/
    TXSTA=0x20;                     /*TX enable*/ 
    RCSTA=0x90;                     /*RX enable and serial port enable */
}

//*******************RECEIVE FUNCTION*****************************************//
char USART_ReceiveChar(){

    while(RCIF==0);                 /*wait for receive interrupt flag*/
    return(RCREG);                  /*receive data is stored in RCREG register and return to main program */
}

//******************TRANSMIT FUNCTION*****************************************// 
void USART_TransmitChar(char out){        
        while(TXIF==0);            /*wait for transmit interrupt flag*/
        TXREG=out;                 /*wait for transmit interrupt flag to set which indicates TXREG is ready
                                    for another transmission*/    
}

void USART_SendString(char *out){
   while(*out!='\0'){    
        while(TXIF==0);            /*wait for transmit interrupt flag to set which indicates TXREG is ready
                                    for another transmission*/
        USART_TransmitChar(*out);
        out++;
   }
}

void MSdelay(unsigned int val){
 unsigned int i,j;
 for(i=0;i<=val;i++)
     for(j=0;j<100;j++);
 }

void EEPROM_Write(int address,char data){
    //Write Operation
    EEADR=address;               //Write address to the EEADR register
    EEDATA=data;                 //copy data to the EEDATA register for write to EEPROM location
    EECON1bits.EEPGD=0;          //Access data EEPROM memory
    EECON1bits.CFGS=0;           //Access flash program or data memory
    EECON1bits.WREN=1;           //Allow write to the memory
    INTCONbits.GIE=0;            //Disable global interrupt
    
    //below sequence in EECON2 Register is necessary to write data to EEPROM memory
    EECON2=0x55;
    EECON2=0xaa;
    
    EECON1bits.WR=1;             //Start writing data to EEPROM memory
    INTCONbits.GIE=1;            //Enable interrupt
    
    while(PIR2bits.EEIF==0);     //EEIF bit is set when write operation is completed
                                 //monitor this bit for write operation
    PIR2bits.EEIF=0;             //Reset EEIF for further write operation
    
}

void EEPROM_WriteString(int address,char *data){
    //Write Operation for String
    while(*data!=0){
        EEPROM_Write(address,*data);
        address++;
        *data++;
    }    
}

char EEPROM_Read(int address){
    //Read operation
    EEADR=address;               //Read data at location 0x00
    EECON1bits.WREN=0;           //WREN bit is clear for Read operation
    EECON1bits.EEPGD=0;          //Access data EEPROM memory
    EECON1bits.RD=1;             //To Read data of EEPROM memory set RD=1
    return(EEDATA);
}

void io_eeprom(){
    int e=0;
    char Data_read;
        
    EEPROM_WriteString(0, string_gravar);
    //  As EEPROM memory location set to 0xff default
    // So while reading data from EEPROM memory we are comparing it with 0xff
    while(Data_read!=0xff){    
        Data_read=EEPROM_Read(e);
        USART_TransmitChar(Data_read);  // transmit read data serially to the serial monitor
        e++;
        // AGORA MANDA EXIBIR NO DISPLAY.
    }
}
