#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "armas.h"

bool escopeta(struct Juego j, int dx, int dy) {
    Pieza *rey = NULL;
    for(int y=0; y<j.t->H; y++) {
        for(int x=0; x<j.t->W; x++) {
            Celda *c = (Celda*)j.t->celdas[y][x];
            if(c->pieza && c->pieza->tipo == 'R') { rey = c->pieza; break; }
        }
    }
    if(!rey) return false;
    int tx = rey->x + dx, ty = rey->y + dy;
    if(tx>=0 && tx<j.t->W && ty>=0 && ty<j.t->H) {
        Celda *c = (Celda*)j.t->celdas[ty][tx];
        if(c->pieza && c->pieza->tipo != 'R') {
            c->pieza->hp -= 2;
            if(c->pieza->hp <= 0) { free(c->pieza); c->pieza = NULL; }
            return true;
        }
    }
    return true;
}

bool francotirador(struct Juego *j, int dx, int dy) { return false; }
bool granada(struct Juego *j, int tx, int ty) { return false; }
bool especial(struct Juego *j, int dx, int dy) { return false; }