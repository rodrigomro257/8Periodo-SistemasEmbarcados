#ifndef VARIAVEIS_GLOBAIS_H
#define	VARIAVEIS_GLOBAIS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "relogio.h"
    
relogio Relogio; // OBJETO DO RELÓGIO. 
bool flag_time=0; // CONTROLA QUANDO A main VAI ATUALIZAR O RELÓGIO.
int tela_exibida=0; // CONTROLA O QUE SERÁ EXIBIDO NO DISPLAY.
int contador=0; // PARA CONTROLAR A INTERRUPT.
int cursor=0; // PARA ESCOLHER O QUE SERÁ INCREMENTADO. 
unsigned int numero_entrada_terminal=0; // USADO NA INTERRUPT.
char str[16]; // PARA USAR O sprintf EM QUALQUER LUGAR.

#ifdef	__cplusplus
}
#endif

#endif	/* VARIAVEIS_GLOBAIS_H */

