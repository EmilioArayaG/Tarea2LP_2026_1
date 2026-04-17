#include <stdlib.h>
#include "main.h"
#include "piezas.h"

static void invocar_pieza(struct Juego *juego, char tipo, int hp, int fila_y) {
    int r_x;
    while (true) {
        r_x = rand() % juego->t->W;
        Celda *c_t = (Celda*)juego->t->celdas[fila_y][r_x];
        if (c_t->pieza == NULL) {
            break;
        }
    }
    
    Pieza *p = (Pieza*)malloc(sizeof(Pieza));
    p->tipo = tipo;
    p->hp = hp;
    p->x = r_x;
    p->y = fila_y;
    p->desplz = false;
    ((Celda*)juego->t->celdas[fila_y][r_x])->pieza = p;
}

static void mover_peon(struct Juego *juego, Pieza *p) {
    Pieza *rey = juego->jugador;
    Tablero *t = juego->t;
    int diff_x = rey->x - p->x;
    int diff_y = rey->y - p->y;
    int nx = p->x, ny = p->y;

    if (abs(diff_x) <= 1 && abs(diff_y) <= 1 && diff_y != 0) {
        nx = rey->x;
        ny = rey->y;
    } else {
        if (abs(diff_x) > abs(diff_y)) {
            nx += (diff_x > 0) ? 1 : -1;
        } else {
            ny += (diff_y > 0) ? 1 : -1;
        }
    }
    
    Celda *dest = (Celda*)t->celdas[ny][nx];
    if(!dest->pieza || dest->pieza->tipo == 'R') {
        Celda *c = (Celda*)t->celdas[p->y][p->x];
        c->pieza = NULL;
        p->x = nx; 
        p->y = ny; 
        p->desplz = true;
        dest->pieza = p;
    }
}

static void mover_caballo(struct Juego *juego, Pieza *p) {
    Pieza *rey = juego->jugador;
    Tablero *t = juego->t;
    int mejor_x = p->x;
    int mejor_y = p->y;
    int dis_min = 9999;

    for (int dy = -2; dy <= 2; dy++) {
        for (int dx = -2; dx <= 2; dx++) {
            if (abs(dx) + abs(dy) == 3) { 
                int nx = p->x + dx;
                int ny = p->y + dy;

                if (nx >= 0 && nx < t->W && ny >= 0 && ny < t->H) {
                    Celda *c_d = (Celda*)t->celdas[ny][nx];
                    if (c_d->pieza == NULL || c_d->pieza->tipo == 'R') {
                        int dis = abs(rey->x - nx) + abs(rey->y - ny);
                        if (dis < dis_min) {
                            dis_min = dis;
                            mejor_x = nx;
                            mejor_y = ny;
                        }
                    }
                }
            }
        }
    }

    if (mejor_x != p->x || mejor_y != p->y) {
        Celda *c = (Celda*)t->celdas[p->y][p->x];
        Celda *c_df = (Celda*)t->celdas[mejor_y][mejor_x];
        c->pieza = NULL;
        p->x = mejor_x;
        p->y = mejor_y;
        p->desplz = true;
        c_df->pieza = p;
    }
}

static void mover_alfil(struct Juego *juego, Pieza *p) {
    Pieza *rey = juego->jugador;
    Tablero *t = juego->t;
    int m_x = p->x;
    int m_y = p->y;
    int dis_m = 9999;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (abs(dx) == 1 && abs(dy) == 1) { 
                for (int ps = 1; ps <= 3; ps++) {
                    int nx = p->x + (dx * ps);
                    int ny = p->y + (dy * ps);

                    if (nx >= 0 && nx < t->W && ny >= 0 && ny < t->H) {
                        Celda *c_d = (Celda*)t->celdas[ny][nx];

                        if (c_d->pieza != NULL && c_d->pieza->tipo != 'R') {
                            break;
                        }
                        int dis = abs(rey->x - nx) + abs(rey->y - ny);

                        if (dis < dis_m) {
                            dis_m = dis;
                            m_x = nx;
                            m_y = ny;
                        }
                        if (c_d->pieza != NULL && c_d->pieza->tipo == 'R') {
                            break;
                        }
                    } else {
                        break;
                    }
                }
            }
        }
    }

    if (m_x != p->x || m_y != p->y) {
        Celda *c = (Celda*)t->celdas[p->y][p->x];
        Celda *c_df = (Celda*)t->celdas[m_y][m_x];
        c->pieza = NULL;
        p->x = m_x;
        p->y = m_y;
        p->desplz = true;
        c_df->pieza = p;
    }
}

static void mover_torre(struct Juego *juego, Pieza *p) {
    if (juego->turno_enemigos % 2 != 0) return;
    Pieza *rey = juego->jugador;
    Tablero *t = juego->t;
    int m_x = p->x;
    int m_y = p->y;
    int dis_m = 9999;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if ((dx == 0 || dy == 0) && !(dx == 0 && dy == 0)) { 
                for (int ps = 1; ps <= 3; ps++) {
                    int nx = p->x + (dx * ps);
                    int ny = p->y + (dy * ps);

                    if (nx >= 0 && nx < t->W && ny >= 0 && ny < t->H) {
                        Celda *c_d = (Celda*)t->celdas[ny][nx];

                        if (c_d->pieza != NULL && c_d->pieza->tipo != 'R') {
                            break;
                        }
                        int dis = abs(rey->x - nx) + abs(rey->y - ny);

                        if (dis < dis_m) {
                            dis_m = dis;
                            m_x = nx;
                            m_y = ny;
                        }
                        if (c_d->pieza != NULL && c_d->pieza->tipo == 'R') {
                            break;
                        }
                    } else {
                        break;
                    }
                }
            }
        }
    }

    if (m_x != p->x || m_y != p->y) {
        Celda *c = (Celda*)t->celdas[p->y][p->x];
        Celda *c_df = (Celda*)t->celdas[m_y][m_x];
        c->pieza = NULL;
        p->x = m_x;
        p->y = m_y;
        p->desplz = true;
        c_df->pieza = p;
    }
}

static void mover_reina(struct Juego *juego, Pieza *p) {
    Pieza *rey = juego->jugador;
    Tablero *t = juego->t;
    int m_x = p->x;
    int m_y = p->y;
    int dist_m = 9999;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue; 
            
            for (int ps = 1; ps <= 4; ps++) {
                int nx = p->x + (dx * ps);
                int ny = p->y + (dy * ps);

                if (nx >= 0 && nx < t->W && ny >= 0 && ny < t->H) {
                    Celda *c_d = (Celda*)t->celdas[ny][nx];

                    if (c_d->pieza != NULL && c_d->pieza->tipo != 'R') {
                        break;
                    }
                    int dis = abs(rey->x - nx) + abs(rey->y - ny);

                    if (dis < dist_m) {
                        dist_m = dis;
                        m_x = nx;
                        m_y = ny;
                    }
                    if (c_d->pieza != NULL && c_d->pieza->tipo == 'R') {
                        break;
                    }
                } else {
                    break;
                }
            }
        }
    }

    if (m_x != p->x || m_y != p->y) {
        Celda *c = (Celda*)t->celdas[p->y][p->x];
        Celda *c_df = (Celda*)t->celdas[m_y][m_x];
        c->pieza = NULL;
        p->x = m_x;
        p->y = m_y;
        p->desplz = true;
        c_df->pieza = p;
    }
}

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

    switch(nivel){
        case 1:
            for (int i = 0; i < 4; i++) invocar_pieza(juego, 'P', 1, 1);
            for (int i = 0; i < 2; i++) invocar_pieza(juego, 'C', 2, 0);
            for (int i = 0; i < 2; i++) invocar_pieza(juego, 'A', 2, 0);
            break;
        case 2:
            for (int i = 0; i < 4; i++) invocar_pieza(juego, 'P', 1, 1);
            for (int i = 0; i < 2; i++) invocar_pieza(juego, 'C', 2, 0);
            for (int i = 0; i < 2; i++) invocar_pieza(juego, 'T', 4, 0);
            break;
        case 3:
            for (int i = 0; i < 2; i++) invocar_pieza(juego, 'P', 1, 1);
            for (int i = 0; i < 1; i++) invocar_pieza(juego, 'T', 4, 0);
            for (int i = 0; i < 1; i++) invocar_pieza(juego, 'A', 2, 0);
            for (int i = 0; i < 1; i++) invocar_pieza(juego, 'Q', 3, 0);
            break;
    }
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
            
            if(p && !p->desplz) {
                switch(p->tipo) {
                    case 'P': mover_peon(juego, p); break;
                    case 'C': mover_caballo(juego, p); break;
                    case 'A': mover_alfil(juego, p); break;
                    case 'T': mover_torre(juego, p); break;
                    case 'Q': mover_reina(juego, p); break;
                }
            }
        }
    }
}

bool verificar_estado_rey(struct Juego *juego) {
    for(int y = 0; y < juego->t->H; y++){
        for(int x = 0; x < juego->t->W; x++){
            Celda *c = (Celda*)juego->t->celdas[y][x];
            if (c->pieza != NULL && c->pieza->tipo == 'R'){
                return false;
            }
        }
    }
    return true;
}