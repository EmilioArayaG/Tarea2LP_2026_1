#ifndef TABLERO_H
#define TABLERO_H

struct Juego;

typedef struct Tablero{
    int W, H;
    void ***celdas;

} Tablero;

struct Tablero* tablero_crear(int ancho, int alto);
void tablero_imprimir(struct Juego *juego);
void tablero_liberar(struct Tablero *tablero);

#endif


