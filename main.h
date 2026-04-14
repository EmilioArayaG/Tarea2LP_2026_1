#ifndef MAIN_H
#define MAIN_H

#include "tablero.h"
#include "armas.h"
#include "piezas.h"

typedef struct Juego {
    Tablero *t;
    Armas arsenal;
    Pieza *jugador;
    int nivel_actual;
    int turno_enemigos;
} Juego;

#endif