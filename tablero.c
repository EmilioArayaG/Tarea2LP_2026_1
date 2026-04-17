#include <stdio.h>
#include <stdlib.h>
#include "tablero.h"
#include "piezas.h"
#include "main.h"

struct Tablero* tablero_crear(int ancho, int alto) {
    struct Tablero *new_t = (struct Tablero*)malloc(sizeof(struct Tablero));
    new_t->W = ancho;
    new_t->H = alto;
    new_t->celdas = (void***)malloc(sizeof(void**) * alto);
    for (int y = 0; y < alto; y++) {
        new_t->celdas[y] = (void**)malloc(sizeof(void*) * ancho);
        for (int x = 0; x < ancho; x++) {
            Celda *nc = (Celda*)malloc(sizeof(Celda));
            nc->pieza = NULL;
            new_t->celdas[y][x] = nc;
        }
    }
    return new_t;
}

void tablero_imprimir(struct Juego *juego) {
    Tablero *t = juego->t;
    printf("--- NIVEL %d ---\n", juego->nivel_actual);
    
    printf("Armas: [1] Escopeta (%d/%d) | [2] Sniper (%d/%d) | [3] Granada (%d/%d) | [4] Especial (%d/%d)\n",
        juego->arsenal.municion_actual[0], juego->arsenal.municion_maxima[0],
        juego->arsenal.municion_actual[1], juego->arsenal.municion_maxima[1],
        juego->arsenal.municion_actual[2], juego->arsenal.municion_maxima[2],
        juego->arsenal.municion_actual[3], juego->arsenal.municion_maxima[3]);

    for (int y = 0; y < t->H; y++) {
        printf("%2d ", t->H - y);
        for (int x = 0; x < t->W; x++) {
            Celda *c = (Celda*)t->celdas[y][x];
            if (c->pieza == NULL) printf("[ ]");
            else printf("[%c]", c->pieza->tipo);
        }
        printf("\n");
    }
    printf("   ");
    for (int x = 0; x < t->W; x++) {
        if (x + 1 >= 10) printf(" %d", x + 1);
        else printf(" %d ", x + 1);
    }
    printf("\n\n");
}

void tablero_liberar(Tablero *t) {
    for (int y = 0; y < t->H; y++) {
        for (int x = 0; x < t->W; x++) {
            Celda *c = (Celda*)t->celdas[y][x];
            if (c->pieza != NULL) {
                free(c->pieza);
                c->pieza = NULL;
            }
            free(c);
        }
        free(t->celdas[y]);
    }
    free(t->celdas);
    free(t);
}