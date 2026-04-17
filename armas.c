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
        }
    }

    int bx = j->jugador->x + (dx * 2);
    int by = j->jugador->y + (dy * 2);
    int c_x[3], c_y[3];
    
    if (dx != 0 && dy == 0) { 
        c_x[0] = bx; c_y[0] = by - 1; c_x[1] = bx; c_y[1] = by; c_x[2] = bx; c_y[2] = by + 1;
    } else if (dx == 0 && dy != 0) { 
        c_x[0] = bx - 1; c_y[0] = by; c_x[1] = bx; c_y[1] = by; c_x[2] = bx + 1; c_y[2] = by;
    } else { 
        c_x[0] = bx - dx; c_y[0] = by; c_x[1] = bx; c_y[1] = by; c_x[2] = bx; c_y[2] = by - dy;
    }

    for(int i = 0; i < 3; i++) {
        int cx = c_x[i], cy = c_y[i];
        if(cx >= 0 && cx < j->t->W && cy >= 0 && cy < j->t->H) {
            Celda *c_back = (Celda*)j->t->celdas[cy][cx];
            if(c_back->pieza && c_back->pieza->tipo != 'R') {
                c_back->pieza->hp -= 1;
                if(c_back->pieza->hp <= 0) { 
                    free(c_back->pieza); 
                    c_back->pieza = NULL; 
                }
            }
        }
    }
    return true;
}

bool francotirador(struct Juego *j, int dx, int dy) {
    if (!j->jugador) return false;
    int b_x = j->jugador->x;
    int b_y = j->jugador->y;

    while (true){
        b_x += dx; b_y += dy;
        if (b_x < 0 || b_x >= j->t->W || b_y < 0 || b_y >= j->t->H) break;
        Celda *c = (Celda*)j->t->celdas[b_y][b_x];
        if(c->pieza != NULL && c->pieza->tipo != 'R'){
            c->pieza->hp -= 3;
            if (c->pieza->hp <= 0){ 
                free(c->pieza); 
                c->pieza = NULL; 
            }
            return true;
        }
    }
    return true;
}

bool granada(struct Juego *j, int dx, int dy) {
    if (!j->jugador) return false;
    int centro_x = j->jugador->x + (dx * 3);
    int centro_y = j->jugador->y + (dy * 3);
    
    for(int y = centro_y - 1; y <= centro_y + 1; y++) {
        for(int x = centro_x - 1; x <= centro_x + 1; x++) { 
            if (x < 0 || x >= j->t->W || y < 0 || y >= j->t->H) continue; 
            Celda *c = (Celda*)j->t->celdas[y][x];
            if(c->pieza != NULL && c->pieza->tipo != 'R') {
                c->pieza->hp -= 2;
                if (c->pieza->hp <= 0) { 
                    free(c->pieza); 
                    c->pieza = NULL; 
                }
            }
        }
    }
    return true;
}

bool especial(struct Juego *j, int dx, int dy) {
    if(!j->jugador) return false;
    int b_x = j->jugador->x;
    int b_y = j->jugador->y;

    while (true){
        int nx = b_x + dx;
        int ny = b_y + dy;
        if (nx < 0 || nx >= j->t->W || ny < 0 || ny >= j->t->H) break; 
        b_x = nx;
        b_y = ny;
        Celda *c = (Celda*)j->t->celdas[b_y][b_x];
        if (c->pieza != NULL && c->pieza->tipo != 'R') break; 
    }

    int cruz_x[5] = {b_x, b_x + 1, b_x - 1, b_x, b_x};
    int cruz_y[5] = {b_y, b_y, b_y, b_y + 1, b_y - 1};

    for(int i = 0; i < 5; i++){
        int ex = cruz_x[i];
        int ey = cruz_y[i];

        if (ex < 0 || ex >= j->t->W || ey < 0 || ey >= j->t->H) continue; 
        
        Celda *c_r = (Celda*)j->t->celdas[ey][ex];
        if(c_r->pieza != NULL && c_r->pieza->tipo != 'R') {
            c_r->pieza->hp -= 2; 
            if (c_r->pieza->hp <= 0) { 
                free(c_r->pieza); 
                c_r->pieza = NULL; 
            }
        }
    }
    return true;
}