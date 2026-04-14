#ifndef ARMAS_H
#define ARMAS_H

#include <stdbool.h>

struct Juego;

typedef bool (*FuncArma)(struct Juego *j, int dir_x, int dir_y);

typedef struct {
    int municion_actual[4];
    int municion_maxima[4];
    FuncArma disparar[4];
} Armas;

bool escopeta(struct Juego *j, int dir_x, int dir_y);
bool francotirador(struct Juego *j, int dir_x, int dir_y);
bool granada(struct Juego *j, int dir_x, int dir_y);
bool especial(struct Juego *j, int dir_x, int dir_y);
#endif
