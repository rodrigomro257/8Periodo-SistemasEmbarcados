#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include "variaveis_globais.h"

    typedef struct{
        int minuto;
        int segundo;
        int hora;
    }relogio;
    
    void tratar_relogio(relogio *Relogio);
    void trata_relogio(relogio *Relogio);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

