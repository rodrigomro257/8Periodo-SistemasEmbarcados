#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

    int qtdDias_mes[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    //int contador=0;

    typedef struct{
        int dia;
        int mes;
        int ano;
    }calendario;

    typedef struct{
        int minuto;
        int segundo;
        int hora;
    }relogio;
    
    void tratar_relogio(relogio *Relogio, calendario *Calendario);
    void controla_despertador(relogio *Despertador);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

