#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "armas.h"

bool escopeta(struct Juego *j, int dx, int dy) {
    if(!j->jugador) return false;
    
    int tx = j->jugador->x + dx;
    int ty = j->jugador->y + dy;
    
    if(tx>=0 && tx<j->t->W && ty>=0 && ty<j->t->H) {
        Celda *c = (Celda*)j->t->celdas[ty][tx];
        if(c->pieza && c->pieza->tipo != 'R') {
            c->pieza->hp -= 2;
            if(c->pieza->hp <= 0) { 
                free(c->pieza); 
                c->pieza = NULL; 
            }
            return true;
        }
    }
    return true;
}

bool francotirador(struct Juego *j, int dx, int dy) { return false; }
bool granada(struct Juego *j, int tx, int ty) { return false; }
bool especial(struct Juego *j, int dx, int dy) { return false; }