#include <stdlib.h>
#include "main.h"
#include "piezas.h"

void spawn_nivel(struct Juego *juego, int nivel) {
    int y_r = juego->t->H - 1;
    int x_r = 1 + (rand() % (juego->t->W - 2));
    
    Pieza *p_rey = (Pieza*)malloc(sizeof(Pieza));
    p_rey->tipo = 'R';
    p_rey->hp = 1;
    p_rey->x = x_r;
    p_rey->y = y_r;
    p_rey->desplz = false;
    juego->jugador = p_rey;
    
    Celda *c = (Celda*)juego->t->celdas[y_r][x_r];
    c->pieza = p_rey;

    int y_p = 1;
    int x_p = rand() % juego->t->W;
    Pieza *peon = (Pieza*)malloc(sizeof(Pieza));
    peon->tipo = 'C';
    peon->hp = 1;
    peon->x = x_p;
    peon->y = y_p;
    peon->desplz = false;
    Celda *cp = (Celda*)juego->t->celdas[y_p][x_p];
    cp->pieza = peon;
}

void mover_enemigos(struct Juego *juego) {
    Tablero *t = juego->t;
    Pieza *rey = juego->jugador;
    juego->turno_enemigos++;
    
    for(int y=0; y<t->H; y++) {
        for(int x=0; x<t->W; x++) {
            Celda *c = (Celda*)t->celdas[y][x];
            if(c->pieza) c->pieza->desplz = false;
        }
    }
    
    if(!rey) return;
    
    for(int y=0; y<t->H; y++) {
        for(int x=0; x<t->W; x++) {
            Celda *c = (Celda*)t->celdas[y][x];
            Pieza *p = c->pieza;
            if(p && p->tipo == 'P' && !p->desplz) {
                int dx = (rey->x > p->x) ? 1 : (rey->x < p->x ? -1 : 0);
                int dy = (rey->y > p->y) ? 1 : (rey->y < p->y ? -1 : 0);
                if(abs(rey->x - p->x) > abs(rey->y - p->y)) dy = 0; else dx = 0;
                
                int nx = p->x + dx, ny = p->y + dy;
                Celda *dest = (Celda*)t->celdas[ny][nx];
                
                if(!dest->pieza || dest->pieza->tipo == 'R') {
                    c->pieza = NULL;
                    p->x = nx; p->y = ny; p->desplz = true;
                    dest->pieza = p;
                }
            } else if (p && p->tipo == 'C' && !p->desplz){
                int s_x[8] = {1,2,2,1,-1,-2,-2,-1};
                int s_y[8] = {-2,-1,1,2,2,1,-1,-2};
                int mejor_x = p->x;
                int mejor_y = p->y;
                int dis_min = 9999;

                for(int i = 0; i<8; i++){
                    int nx = p->x + s_x[i];
                    int ny = p->y + s_y[i];

                    if(nx >= 0 && nx < t->W && ny >= 0 && ny < t->H){
                        Celda *c_d = (Celda*)t->celdas[ny][nx];
                        if (c_d->pieza == NULL || c_d->pieza->tipo == 'R'){
                            int dis = abs(rey->x - nx) + abs(rey->y - ny);
                            if (dis < dis_min){
                                dis_min = dis;
                                mejor_x = nx;
                                mejor_y = ny;
                            }
                        }
                    }
                }
                if (mejor_x != p->x || mejor_y != p->y){
                    Celda *c_df = (Celda*)t->celdas[mejor_y][mejor_x];
                    c->pieza = NULL;
                    p->x = mejor_x;
                    p->y = mejor_y;
                    p->desplz = true;
                    c_df->pieza = p;
                }
            } else if (p && p->tipo == 'A' && !p->desplz){
                int d_x[4] = {1,1,-1,-1};
                int d_y[4] = {1,-1,1,-1};
                int m_x = p->x;
                int m_y = p->y;
                int dis_m = 9999;

                for (int i = 0; i< 4; i++){
                    for (int ps = 1; ps <= 3; ps++){
                        int nx = p->x + (d_x[i]*ps);
                        int ny = p->y + (d_y[i]*ps);

                        if (nx >= 0 && nx < t->W && ny >= 0 && ny < t->H){
                            Celda *c_d = (Celda*)t->celdas[ny][nx];

                            if (c_d->pieza != NULL && c_d->pieza->tipo != 'R'){
                                break;
                            }
                            int dis = abs(rey->x - nx) + abs(rey->y - ny);

                            if (dis < dis_m){
                                dis_m = dis;
                                m_x = nx;
                                m_y = ny;
                            }
                            if (c_d->pieza != NULL && c_d->pieza->tipo == 'R'){
                                break;
                            }
                        } else {
                            break;
                        }
                    }
                }
                if (m_x != p->x || m_y != p->y){
                    Celda *c_df = (Celda*)t->celdas[m_y][m_x];
                    c->pieza = NULL;
                    p->x = m_x;
                    p->y = m_y;
                    p->desplz = true;
                    c_df->pieza = p;
                }
            } else if (p && p->tipo == 'T' && !p->desplz){
                if( juego->turno_enemigos % 2 == 0){
                    int d_x[4] = {0,0,-1,1};
                    int d_y[4] = {1,-1,0,0};
                    int m_x = p->x;
                    int m_y = p->y;
                    int dis_m = 9999;
                    for (int i = 0; i< 4; i++){
                        for (int ps = 1; ps <= 3; ps++){
                            int nx = p->x + (d_x[i]*ps);
                            int ny = p->y + (d_y[i]*ps);

                            if (nx >= 0 && nx < t->W && ny >= 0 && ny < t->H){
                                Celda *c_d = (Celda*)t->celdas[ny][nx];

                                if (c_d->pieza != NULL && c_d->pieza->tipo != 'R'){
                                    break;
                                }
                                int dis = abs(rey->x - nx) + abs(rey->y - ny);

                                if (dis < dis_m){
                                    dis_m = dis;
                                    m_x = nx;
                                    m_y = ny;
                                }
                                if (c_d->pieza != NULL && c_d->pieza->tipo == 'R'){
                                    break;
                                }
                            } else {
                                break;
                            }
                        }
                    }
                    if (m_x != p->x || m_y != p->y){
                        Celda *c_df = (Celda*)t->celdas[m_y][m_x];
                        c->pieza = NULL;
                        p->x = m_x;
                        p->y = m_y;
                        p->desplz = true;
                        c_df->pieza = p;
                    }
                } 
            } else if (p && p->tipo == 'Q' && !p->desplz){
                int d_x[8] = {0,0,-1,1,1,1,-1,-1};
                int d_y[8] = {1, -1, 0, 0, 1, -1, 1, -1};
                int m_x = p->x;
                int m_y = p->y;
                int dist_m = 9999;

                for (int i = 0; i< 8; i++){
                    for (int ps = 1; ps <= 4; ps++){
                        int nx = p->x + (d_x[i]*ps);
                        int ny = p->y + (d_y[i]*ps);

                        if (nx >= 0 && nx < t->W && ny >= 0 && ny < t->H){
                            Celda *c_d = (Celda*)t->celdas[ny][nx];

                            if (c_d->pieza != NULL && c_d->pieza->tipo != 'R'){
                                break;
                            }
                            int dis = abs(rey->x - nx) + abs(rey->y - ny);

                            if (dis < dist_m){
                                dist_m = dis;
                                m_x = nx;
                                m_y = ny;
                            }
                            if (c_d->pieza != NULL && c_d->pieza->tipo == 'R'){
                                break;
                            }
                        } else {
                            break;
                        }
                    }
                }
                if (m_x != p->x || m_y != p->y){
                    Celda *c_df = (Celda*)t->celdas[m_y][m_x];
                    c->pieza = NULL;
                    p->x = m_x;
                    p->y = m_y;
                    p->desplz = true;
                    c_df->pieza = p;
                }
            }
        }
    }
}

bool verificar_estado_rey(struct Juego *juego) { return false; }