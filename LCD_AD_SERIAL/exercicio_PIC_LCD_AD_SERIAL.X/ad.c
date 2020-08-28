#include "ad.h"

void init_ADC(void){ //Configure ADC with 3 analog channels
ADCON2=0b10101110;    
ADCON0=0b00000011;
ADCON1=0b00001101;
}

void convert(){
    ADCResult = 0;
    //---sample and convert---- 
    for(unsigned char i=0;i<=2;i++){ //Loop 3 times to sample and read each channel individually.
        SetChanADC(ADC_CH0);//ADCON0 = 0b00000000;  //Select a channel to sample (index i = 0 (first channel) when the loop
        //runs for the first time, i will be 1 then 2 for each successive runs)
        ConvertADC(); // Convert Analog to Digital
        while(BusyADC());
        ADCResult += (unsigned int) ReadADC();   //Read each channel
        valor_tensao=((ADCResult/3)*5.0)/1023;            //convert 10-bit ADC data into voltage 
    }
}

void convert_float_to_char(float valor){
    float f1,f2;
    int i1,i2;
    valor=valor*100;
    f1=floor(valor);
    f2=valor - f1;
    i1 = (int)f1;
    i2 = (int)(100*f2);
    sprintf(temperatura_display,"%d,%d oC        ", i1,i2);
}
