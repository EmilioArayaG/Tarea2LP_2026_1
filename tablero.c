#include <stdio.h>
#include <stdlib.h>
#include "tablero.h"
#include "piezas.h"
#include "main.h"

/*
***
Parametro 1: struct juego *juego
***
Retorno: void
***
Esta funcion se encarga de contar los enemigos, mostrar en pantalla los restantes y el tipo de enemigo.
*/
static void contar_enemigos(struct Juego *juego) {
    int p=0, c=0, a=0, t=0, q=0;
    Tablero *tablero = juego->t;
    
    for(int y=0; y<tablero->H; y++) {
        for(int x=0; x<tablero->W; x++) {
            Celda *celda = (Celda*)tablero->celdas[y][x];
            if(celda->pieza && celda->pieza->tipo != 'R') {
                switch(celda->pieza->tipo) {
                    case 'P': p++; break;
                    case 'C': c++; break;
                    case 'A': a++; break;
                    case 'T': t++; break;
                    case 'Q': q++; break;
                }
            }
        }
    }
    
    int total = p + c + a + t + q;
    
    printf("\n ENEMIGOS RESTANTES: %d\n", total);
    printf("  Peones: %d | Caballos: %d | Alfiles: %d | Torres: %d | Reinas: %d\n", 
           p, c, a, t, q);
}

/*
***
Parametro 1: int ancho
Parametro 2: int alto
***
Retorno: struct Tablero*
***
Esta funcion reserva memoria en el heap para creal el tablero con el alto y ancho correspondiente, con sus respectivas celdas vacias.
*/
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
/*
***
Parametro 1: Struct Juego *juego
***
Retorno: void
***
Esta funcion imprime el HUD en la consola, mostrando toda la informacion de armas, movimientos. nivel y dibujando la matriz del tablero. 
*/
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

    contar_enemigos(juego);
}
/*
***
Parametro 1: tablero *t
***
Retorno: void
***
Esta funcion se encarga de liberar toda la memoria asociada al tablero, incluyendo las piezas y celdas que esten en el tablero.
*/
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