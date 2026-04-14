#include <stdio.h>
#include <stdlib.h>
#include "tablero.h"
#include "piezas.h"
#include "main.h"

struct Tablero* tablero_crear(int ancho, int alto) {
    Tablero *new_t = (struct Tablero*)malloc(sizeof(Tablero));
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

void tablero_imprimir(Juego *juego) {
    Tablero *t = juego->t;
    printf("--- NIVEL %d ---\n", juego->nivel_actual);
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
    for (int x = 0; x < t->W; x++) printf(" %d ", x + 1);
    printf("\n");
}

void tablero_liberar(Tablero tablero) {
    for (int y = 0; y < tablero.H; y++) {
        for (int x = 0; x < tablero.W; x++) {
            Celda *c = (Celda*)tablero.celdas[y][x];
            if (c->pieza != NULL) 
            free(c->pieza);
            free(c);
        }
        free(tablero.celdas[y]);
    }
    free(tablero.celdas);
}