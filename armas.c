#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "armas.h"

/*
***
Parametro 1: char tipo_enemigo
Parametro 2: int hp_actual
Parametro 3: int daño
***
Retorno: void
***
Esta funcion reporta el impacto de un disparo mostrando el tipo de enemigo dañado, el daño infligido y los puntos de vida restantes. 
*/
static void reportar_impacto(char tipo_enemigo, int hp_actual, int daño) {
    const char *nombre[] = {"Peón", "Caballo", "Alfil", "Torre", "Reina"};
    char tipos[] = "PCATQ";
    
    int idx = -1;
    for(int i = 0; i < 5; i++) {
        if(tipos[i] == tipo_enemigo) { idx = i; break; }
    }
    
    if(idx == -1) return;
    
    printf("\n ¡IMPACTO! ");
    printf("Daño: %d HP | ", daño);
    
    if(hp_actual <= 0) {
        printf("%s DESTRUIDO (HP = 0/%d)\n", nombre[idx], daño);
    } else {
        printf("%s herido (HP = %d)\n", nombre[idx], hp_actual);
    }
}

/*
***
Parametro 1: struct Juego *j
Parametro 2: int dx
Parametro 3: int dy
***
Retorno: bool
***
Esta funcion recibe el estado actual del juego y la direccion de el disparo (dx, dy), efectua el disparo a corta distancia que inflige 2 de daño al primer objetivo y 1 de daño a las 3 casillas que estan inmediatamente detras de este. Retorna true en caso de disparar con exito o false en caso de fallo
*/
bool escopeta(struct Juego *j, int dx, int dy) {
    if(!j->jugador) return false;
    
    int tx = j->jugador->x + dx;
    int ty = j->jugador->y + dy;
    
    if(tx>=0 && tx<j->t->W && ty>=0 && ty<j->t->H) {
        Celda *c = (Celda*)j->t->celdas[ty][tx];
        if(c->pieza && c->pieza->tipo != 'R') {
            c->pieza->hp -= 2;
            reportar_impacto(c->pieza->tipo, c->pieza->hp, 2);
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
                reportar_impacto(c_back->pieza->tipo, c_back->pieza->hp, 1);
                if(c_back->pieza->hp <= 0) { 
                    free(c_back->pieza); 
                    c_back->pieza = NULL; 
                }
            }
        }
    }
    return true;
}

/*
***
Parametro 1: struct Juego *j
Parametro 2: int dx
Parametro 3: int dy
***
Retorno: bool
***
Esta funcion recibe el estado actual del juego y la direccion de el disparo (dx, dy), se traza una linea recta desde el rey hasta colisionar con un enemigo o el limite de el tablero, inflingiendo 3 de daño al primer daño. Retorna true en caso de ser exitoso el disparo o false en caso de que no lo sea.
*/
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
            reportar_impacto(c->pieza->tipo, c->pieza->hp, 3);
            if (c->pieza->hp <= 0){ 
                free(c->pieza); 
                c->pieza = NULL; 
            }
            return true;
        }
    }
    printf("\n❌ SNIPER: Disparo perdido en el vacío...\n");
    return true;
}

/*
***
Parametro 1: struct Juego *j
Parametro 2: int dx
Parametro 3: int dy
***
Retorno: bool
***
Esta funcion recibe el estado actual del juego y la direccion de el disparo (dx, dy), proyecta un ataque a 3 casillas de distancia que genera una explosion en un area de 3x3, haciendo 2 de daño a las piezas dentro de este area. Retorna true en caso de lanzamiento exitoso o false en el caso contrario.
*/
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
                reportar_impacto(c->pieza->tipo, c->pieza->hp, 2);
                if (c->pieza->hp <= 0) { 
                    free(c->pieza); 
                    c->pieza = NULL; 
                }
            }
        }
    }
    return true;
}

/*
***
Parametro 1: struct Juego *j
Parametro 2: int dx
Parametro 3: int dy
***
Retorno: bool
***
Esta funcion recibe el estado actual del juego y la direccion de el disparo (dx, dy), Dispara un proyectil que avanza en la direccion correspondiente hasta colisionar con un limite del tablero o un enemigo, ahi explotta y crea una cruz donde inflinge 2 de daño a las piezas que esten en ese radio. retorna true si se ejecuta con exito.
*/
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
            reportar_impacto(c_r->pieza->tipo, c_r->pieza->hp, 2);
            if (c_r->pieza->hp <= 0) { 
                free(c_r->pieza); 
                c_r->pieza = NULL; 
            }
        }
    }
    return true;
}