#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "main.h"

int main() {
    srand(time(NULL));
    Juego juego;
    juego.nivel_actual = 1;
    
    juego.t = tablero_crear(12, 12);
    spawn_nivel(juego, juego.nivel_actual);

    juego.arsenal.disparar[0] = escopeta;
    juego.arsenal.disparar[1] = francotirador;
    juego.arsenal.disparar[2] = granada;
    juego.arsenal.disparar[3] = especial;

    juego.arsenal.municion_maxima[0] = 2;
    juego.arsenal.municion_actual[0] = 2; 

    bool jugando = true;
    char input; 
    
    while (jugando) {
        system("clear");
        tablero_imprimir(&juego);
        printf("\nIngresa tu movimiento (Q,W,E,A,S,D,Z,C) | Disparar (1,2,3,4) | X PARA SALIR: ");
        scanf(" %c", &input);
        
        if (input == 'X' || input == 'x'){
            jugando = false;
            break;
        }
        
        if (input >= '1' && input <= '4'){
            int in_arma = input - '1';
            if (juego.arsenal.municion_actual[in_arma] > 0){
                printf("Ingresa direccion de disparo (Q,W,E,A,S,D,Z,C): ");
                char dir_input;
                scanf(" %c", &dir_input); 
                
                int direc_x = 0, direc_y = 0;
                switch(dir_input){
                    case 'W': case 'w': direc_y = -1; break;
                    case 'A': case 'a': direc_x = -1; break;
                    case 'S': case 's': direc_y = 1; break;
                    case 'D': case 'd': direc_x = 1; break;
                    case 'Q': case 'q': direc_x = -1; direc_y = -1; break;
                    case 'E': case 'e': direc_x = 1; direc_y = -1; break;
                    case 'Z': case 'z': direc_x = -1; direc_y = 1; break;
                    case 'C': case 'c': direc_x = 1; direc_y = 1; break;
                    default: 
                        continue;
                }
                juego.arsenal.disparar[in_arma](juego, direc_x, direc_y);
                juego.arsenal.municion_actual[in_arma]--;
                mover_enemigos(juego);
                continue;
            } else {
                continue;
            }
        }
        
        int dir_x = 0, dir_y = 0;

        switch(input){
            case 'W': case 'w': dir_y = -1; break;
            case 'A': case 'a': dir_x = -1; break;
            case 'S': case 's': dir_y = 1; break;
            case 'D': case 'd': dir_x = 1; break;
            case 'Q': case 'q': dir_x = -1; dir_y = -1; break;
            case 'E': case 'e': dir_x = 1; dir_y = -1; break;
            case 'Z': case 'z': dir_x = -1; dir_y = 1; break;
            case 'C': case 'c': dir_x = 1; dir_y = 1; break;
            default: 
                continue;
        }
        
        Pieza *rey = NULL;
        for(int y = 0; y < juego.t->H; y++) {
            for(int x = 0; x < juego.t->W; x++) {
                Celda *c = (Celda*)juego.t->celdas[y][x];
                if(c->pieza != NULL && c->pieza->tipo == 'R') { 
                    rey = c->pieza; 
                    break; 
                }
            }
            if(rey != NULL) break;
        }

        if (rey != NULL) {
            int new_x = rey->x + dir_x;
            int new_y = rey->y + dir_y;
            
            if (new_x >= 0 && new_x < juego.t->W && new_y >= 0 && new_y < juego.t->H){
                Celda *celda_vieja = (Celda*)juego.t->celdas[rey->y][rey->x];
                Celda *celda_nueva = (Celda*)juego.t->celdas[new_y][new_x];
                
                if (celda_nueva->pieza == NULL) {
                    celda_vieja->pieza = NULL;
                    rey->x = new_x;
                    rey->y = new_y;
                    celda_nueva->pieza = rey;
                    
                    if (juego.arsenal.municion_actual[0] < juego.arsenal.municion_maxima[0]) {
                        juego.arsenal.municion_actual[0]++;
                    }
                    
                    mover_enemigos(juego);
                }
            }
        }
    }

    tablero_liberar(*(juego.t));
    free(juego.t);

    return 0;
}
