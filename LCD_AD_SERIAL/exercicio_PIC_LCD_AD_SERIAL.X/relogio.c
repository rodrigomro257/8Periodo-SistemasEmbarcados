#include "relogio.h"

void tratar_relogio(relogio *Relogio){
    
    Relogio->segundo++;
    
    if(Relogio->segundo>59){
        Relogio->minuto++;
        Relogio->segundo=0;
    }
    if(Relogio->segundo<0){
        Relogio->segundo=59;
        Relogio->minuto--;
    }
    
    if(Relogio->minuto>59){
        Relogio->hora++;
        Relogio->minuto=0;
    }
    if(Relogio->minuto<0){
        Relogio->minuto=59;
        Relogio->hora--;
    }
    
    if(Relogio->hora>23){
        Relogio->hora=0;
    }    
    if(Relogio->hora<0){
        Relogio->hora=23;
    }
}

void trata_relogio(relogio *Relogio){}