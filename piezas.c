#include <stdlib.h>
#include "main.h"
#include "piezas.h"

void spawn_nivel(struct Juego *juego, int nivel) {
    int y_r = juego->t->H - 1;
    int x_r = 1 + (rand() % (juego->t->W - 2));
    
    Pieza *p_rey = (Pieza*)malloc(sizeof(Pieza));
    p_rey->tipo = 'R';
    p_rey->hp = 1;
    p_rey->x = x_r;
    p_rey->y = y_r;
    p_rey->desplz = false;
    juego->jugador = p_rey;
    
    Celda *c = (Celda*)juego->t->celdas[y_r][x_r];
    c->pieza = p_rey;

    int y_p = 1;
    int x_p = rand() % juego->t->W;
    Pieza *peon = (Pieza*)malloc(sizeof(Pieza));
    peon->tipo = 'P';
    peon->hp = 1;
    peon->x = x_p;
    peon->y = y_p;
    peon->desplz = false;
    Celda *cp = (Celda*)juego->t->celdas[y_p][x_p];
    cp->pieza = peon;
}

void mover_enemigos(struct Juego *juego) {
    Tablero *t = juego->t;
    Pieza *rey = juego->jugador;
    
    for(int y=0; y<t->H; y++) {
        for(int x=0; x<t->W; x++) {
            Celda *c = (Celda*)t->celdas[y][x];
            if(c->pieza) c->pieza->desplz = false;
        }
    }
    
    if(!rey) return;
    
    for(int y=0; y<t->H; y++) {
        for(int x=0; x<t->W; x++) {
            Celda *c = (Celda*)t->celdas[y][x];
            Pieza *p = c->pieza;
            if(p && p->tipo == 'P' && !p->desplz) {
                int dx = (rey->x > p->x) ? 1 : (rey->x < p->x ? -1 : 0);
                int dy = (rey->y > p->y) ? 1 : (rey->y < p->y ? -1 : 0);
                if(abs(rey->x - p->x) > abs(rey->y - p->y)) dy = 0; else dx = 0;
                
                int nx = p->x + dx, ny = p->y + dy;
                Celda *dest = (Celda*)t->celdas[ny][nx];
                
                if(!dest->pieza || dest->pieza->tipo == 'R') {
                    c->pieza = NULL;
                    p->x = nx; p->y = ny; p->desplz = true;
                    dest->pieza = p;
                }
            }
        }
    }
}

bool verificar_estado_rey(struct Juego *juego) { return false; }