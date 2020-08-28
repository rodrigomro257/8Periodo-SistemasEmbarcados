#ifndef VARIAVEIS_GLOBAIS_H
#define	VARIAVEIS_GLOBAIS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "relogio.h"

relogio Relogio; // OBJETO DO RELÓGIO. 
relogio Despertador; // OBJETO DO DESPERTADOR. 
calendario Calendario;
bool flag_time=0; // CONTROLA QUANDO A main VAI ATUALIZAR O RELÓGIO.
int tela_exibida=0; // CONTROLA O QUE SERÁ EXIBIDO NO DISPLAY.
int contador=0; // PARA CONTROLAR A INTERRUPT.
int cursor=0; // PARA ESCOLHER O QUE SERÁ INCREMENTADO. 
bool despertar=false;  // DESPERTADOR ATIVADO OU DESATIVADO.
int controle_tempo_despertar=0; // É INCREMENTADO PARA PARAR O DESPERTADOR.

#ifdef	__cplusplus
}
#endif

#endif	/* VARIAVEIS_GLOBAIS_H */

