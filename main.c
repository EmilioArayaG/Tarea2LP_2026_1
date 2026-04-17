#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "main.h"

int main() {
    srand(time(NULL));
    Juego juego;
    juego.nivel_actual = 1;
    juego.turno_enemigos = 0;
    
    juego.t = (Tablero*)tablero_crear(12, 12);
    spawn_nivel(&juego, juego.nivel_actual);

    juego.arsenal.disparar[0] = escopeta;
    juego.arsenal.disparar[1] = francotirador;
    juego.arsenal.disparar[2] = granada;
    juego.arsenal.disparar[3] = especial;

    juego.arsenal.municion_maxima[0] = 2;
    juego.arsenal.municion_maxima[1] = 1;
    juego.arsenal.municion_maxima[2] = 2;
    juego.arsenal.municion_maxima[3] = 3;
    for (int i = 0; i < 4; i++){
        juego.arsenal.municion_actual[i] = juego.arsenal.municion_maxima[i];
    }

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

        int dir_x = 0;
        int dir_y = 0;
        bool turno_valido = false;

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
                juego.arsenal.disparar[in_arma](&juego, direc_x, direc_y);
                juego.arsenal.municion_actual[in_arma]--;
                turno_valido = true;
            } else {
                continue;
            }
        } else {
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
            
            Pieza *rey = juego.jugador;
            if (rey != NULL) {
                int n_x = rey->x + dir_x;
                int n_y = rey->y + dir_y;
                
                if (n_x >= 0 && n_x < juego.t->W && n_y >= 0 && n_y < juego.t->H){
                    Celda *celda_vieja = (Celda*)juego.t->celdas[rey->y][rey->x];
                    Celda *celda_nueva = (Celda*)juego.t->celdas[n_y][n_x];
                    
                    if (celda_nueva->pieza == NULL) {
                        celda_vieja->pieza = NULL;
                        rey->x = n_x;
                        rey->y = n_y;
                        celda_nueva->pieza = rey;
                        turno_valido = true;
                        
                        if (juego.arsenal.municion_actual[0] < juego.arsenal.municion_maxima[0]) {
                            juego.arsenal.municion_actual[0]++;
                        }
                    }
                }
            }
        }

        if (turno_valido){
            mover_enemigos(&juego);
            if(verificar_estado_rey(&juego)){
                system("clear");
                tablero_imprimir(&juego);
                printf("\nEl rey ha caido...\n");
                printf("--- Fin Del Juego! ---\n");
                jugando = false;
                
                Celda *celda_rey = (Celda*)juego.t->celdas[juego.jugador->y][juego.jugador->x];
                celda_rey->pieza = NULL;
                free(juego.jugador);
                juego.jugador = NULL;
                
                continue;
            }

            int enem_vivos = 0;
            for(int y = 0; y < juego.t->H; y++){
                for(int x = 0; x < juego.t->W; x++){
                    Celda *c_check = (Celda*)juego.t->celdas[y][x];
                    if(c_check->pieza != NULL && c_check->pieza->tipo != 'R'){
                        enem_vivos++;
                    }
                }
            }
            if (enem_vivos == 0){
                if (juego.nivel_actual == 3){
                    system("clear");
                    tablero_imprimir(&juego);
                    printf("\n¡Victoria! Has completado todos los niveles.\n");
                    printf("---Ganaste---\n");
                    jugando = false;
                } else {
                    juego.nivel_actual++;
                    tablero_liberar(juego.t);
                    if (juego.nivel_actual == 2){
                        juego.t = (Tablero*)tablero_crear(8,8);
                    } else if (juego.nivel_actual == 3){
                        juego.t = (Tablero*)tablero_crear(6,6);
                    }
                    spawn_nivel(&juego, juego.nivel_actual);
                    for(int i = 0; i < 4 ; i++){
                        juego.arsenal.municion_actual[i] = juego.arsenal.municion_maxima[i];
                    }
                    printf("\nNivel Completado! Presiona ENTER par continual al nivel:%d \n", juego.nivel_actual);
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);
                }
            }
        }
    }

    tablero_liberar(juego.t);
    return 0;
}
