#ifndef PIEZAS_H
#define PIEZAS_H

#include <stdbool.h>
struct Juego;

typedef struct {
    char tipo;
    int hp;
    int x, y;
    bool desplz;
} Pieza;

typedef struct {
    Pieza *pieza;
} Celda;

void spawn_nivel(struct Juego *juego, int nivel);
void mover_enemigos(struct Juego *juego);
bool verificar_estado_rey(struct Juego *juego);

#endif