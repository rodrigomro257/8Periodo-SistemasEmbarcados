#ifndef VARIAVEIS_GLOBAIS_H
#define	VARIAVEIS_GLOBAIS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "relogio.h"

relogio Relogio; // OBJETO DO REL�GIO. 
relogio Despertador; // OBJETO DO DESPERTADOR. 
calendario Calendario;
bool flag_time=0; // CONTROLA QUANDO A main VAI ATUALIZAR O REL�GIO.
int tela_exibida=0; // CONTROLA O QUE SER� EXIBIDO NO DISPLAY.
int contador=0; // PARA CONTROLAR A INTERRUPT.
int cursor=0; // PARA ESCOLHER O QUE SER� INCREMENTADO. 
bool despertar=false;  // DESPERTADOR ATIVADO OU DESATIVADO.
int controle_tempo_despertar=0; // � INCREMENTADO PARA PARAR O DESPERTADOR.

#ifdef	__cplusplus
}
#endif

#endif	/* VARIAVEIS_GLOBAIS_H */

