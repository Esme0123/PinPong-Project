#include "pong.h"
#include "libgame.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

const size_t PELOTA::MAX_LONGITUD_ESTELA = 5;

JUGADOR::JUGADOR(int _x, int _y) : x(_x), y(_y){}

void JUGADOR::pintar() const {
    Color(PLATA);
    gotoxy(x,y-1);  printf("%c",219);
    gotoxy(x,y);    printf("%c",219);
    gotoxy(x,y+1);  printf("%c",219);
}

void JUGADOR::borrar() const {
    gotoxy(x,y-1);  printf(" ");
    gotoxy(x,y);    printf(" ");
    gotoxy(x,y+1);  printf(" ");
}

void JUGADOR::mover_cpu(int pelota_x, int pelota_y, int pelota_dx, int nivel) {
    if (pelota_dx > 0 && x > 40) {
        int umbral_reaccion_x = 40 + (3 - nivel) * 6; // Ej: N1: 52, N2: 46, N3: 40
        if (pelota_x > umbral_reaccion_x) {
            borrar();
            if (pelota_y > y && y + 1 < 22) {
                y++;
            } else if (pelota_y < y && y - 1 > 4) {
                y--;
            }

            if (nivel < 3) {
                int error_chance = rand() % 15;
                if (error_chance < (3 - nivel) * 2) {
                    if (rand()%2 == 0 && y+1 < 22) y++; else if(y-1 > 4) y--;
                }
            }
            if (y - 1 <= 3) y = 5;
            if (y + 1 >= 23) y = 21;

            pintar();
        }
    }
}


PELOTA::PELOTA(int _x, int _y, int _dx_init, int _dy_init)
    : x(_x), y(_y),
      velocidad_base_x(1),
      velocidad_base_y(1),
      ralentizacion_movimiento(2),
      pausa_base_nivel(20) {
    dx = (_dx_init > 0) ? velocidad_base_x : -velocidad_base_x;
    dy = (_dy_init > 0) ? velocidad_base_y : -velocidad_base_y;
    agregarPuntoEstela(x,y);
}

void PELOTA::setDireccion(int _dx_dir, int _dy_dir){
    dx = (_dx_dir > 0) ? velocidad_base_x : -velocidad_base_x;
    if (_dx_dir == 0) dx = 0;

    dy = (_dy_dir > 0) ? velocidad_base_y : -velocidad_base_y;
    if (_dy_dir == 0) dy = 0;
}


void PELOTA::pintar() const {
    pintarEstela();
    Color(AMARILLO);
    gotoxy(x,y);    printf("%c", 'O');
}

void PELOTA::borrar() const {
    borrarEstela();
    gotoxy(x,y);    printf(" ");
}

void PELOTA::borrarEstelaCompleta() {
    borrarEstela();
    estela.clear();
}


void PELOTA::mover(JUGADOR A, JUGADOR B) {
    borrar();
    x += dx;
    y += dy;
    agregarPuntoEstela(x, y);
    pintar();

    if (y + dy <= 3 || y + dy >= 23) {
        dy = -dy;
        if (y <= 3 && dy < 0) y = 4;
        else if (y >= 23 && dy > 0) y = 22;
    }

    // Colisión con Jugador A (izquierda)
    if (x <= A.RX() + 1 && x > A.RX() - std::abs(dx) && dx < 0) {
        if (y >= A.RY() - 2 && y <= A.RY() + 2) {
            dx = -dx;
            x = A.RX() + 1;
            if (y < A.RY()) dy = -std::abs(velocidad_base_y);
            else if (y > A.RY()) dy = std::abs(velocidad_base_y);
            else dy = (rand() % 2 == 0) ? std::abs(velocidad_base_y) : -std::abs(velocidad_base_y);
        }
    }

    // Colisión con Jugador B (derecha)
    if (x >= B.RX() - 1 && x < B.RX() + std::abs(dx) && dx > 0) {
        if (y >= B.RY() - 2 && y <= B.RY() + 2) {
            dx = -dx;
            x = B.RX() - 1;
            if (y < B.RY()) dy = -std::abs(velocidad_base_y);
            else if (y > B.RY()) dy = std::abs(velocidad_base_y);
            else dy = (rand() % 2 == 0) ? std::abs(velocidad_base_y) : -std::abs(velocidad_base_y);
        }
    }
}

void PELOTA::establecerVelocidadPorNivel(int nivel) {
    switch (nivel) {
        case 1:
            velocidad_base_x = 1;
            velocidad_base_y = 1;
            ralentizacion_movimiento = 2; // Pelota se mueve cada 3er ciclo de juego (0,1,2 -> mueve en 0)
            pausa_base_nivel = 25;
            break;
        case 2:
            velocidad_base_x = 1;
            velocidad_base_y = 1;
            ralentizacion_movimiento = 1; // Pelota se mueve cada 2do ciclo de juego (0,1 -> mueve en 0)
            pausa_base_nivel = 15;
            break;
        case 3:
            velocidad_base_x = 1; // Podría ser 2 para más reto
            velocidad_base_y = 1;
            ralentizacion_movimiento = 0; // Pelota se mueve en cada ciclo de juego
            pausa_base_nivel = 10;
            break;
        default: // Caso por defecto, igual a nivel 1
            velocidad_base_x = 1;
            velocidad_base_y = 1;
            ralentizacion_movimiento = 2;
            pausa_base_nivel = 20;
            break;
    }
    // Actualizar dx y dy con la nueva velocidad base, manteniendo la dirección
    dx = (dx > 0 || (dx==0 && rand()%2==0) ) ? velocidad_base_x : -velocidad_base_x;
    dy = (dy > 0 || (dy==0 && rand()%2==0) ) ? velocidad_base_y : -velocidad_base_y;
    if (dx == 0 && dy == 0) {
        dx = velocidad_base_x * ((rand()%2 == 0) ? 1 : -1);
        if (dx == 0) dx = velocidad_base_x;
    }
}


void PELOTA::agregarPuntoEstela(int _x, int _y) {
    if (estela.size() >= PELOTA::MAX_LONGITUD_ESTELA && !estela.empty()) {
        estela.erase(estela.begin());
    }
    estela.push_back(Punto(_x, _y));
}

void PELOTA::pintarEstela() const {
    for (size_t i = 0; i < estela.size(); ++i) {
        if (estela[i].x == x && estela[i].y == y && i == estela.size()-1) continue;

        if (i < estela.size() / 3) {
             Color(MORADO); gotoxy(estela[i].x, estela[i].y); printf(".");
        } else if (i < (estela.size() * 2) / 3) {
             Color(VERDE); gotoxy(estela[i].x, estela[i].y); printf("*");
        } else {
             Color(VERDE2); gotoxy(estela[i].x, estela[i].y); printf("o");
        }
    }
}

void PELOTA::borrarEstela() const {
    for (size_t i = 0; i < estela.size(); ++i) {
        gotoxy(estela[i].x, estela[i].y); printf(" ");
    }
}

void MENU::pintar_portada(int &a_ref){
    int i,j;
    Color(AZUL);
    char portada[18][72] =
    {
    "                                                                      ",
    "     PPPPPPPPP                                                        ",
    "   PP         PP                             1.- HUMANO VS HUMANO     ",
    "   PP          PP                            2.- HUMANO VS CPU        ",
    "   PP          PP                                                     ",
    "   PP         PP                                                      ",
    "   PPPPPPPPP                                                          ",
    "   PP                                                                 ",
    "   PP                                                                 ",
    "   PP        OOOOOOO   NNN    NN   GGGGGG                             ",
    "   PP      OO       OO NN NN   NN  GG    GG                            ",
    "   PP      OO       OO NN  NN  NN  GG                                  ",
    "   PP      OO       OO NN   NN NN  GG  GGGG                            ",
    "   PP      OO       OO NN    NNNN  GG    GG                            ",
    "   PP        OOOOOOO   NN     NNN   GGGGGG                             ",
    "                                                                      ",
    "          Selecciona una opcion (1 o 2) y presiona Enter              "
    };
    for(i=0;i<17;i++){
        for(j=0; j < 71 && portada[i][j] != '\0' ;j++){
            gotoxy(j+5,i+5);
            printf("%c",portada[i][j]);
        }
    }

    char seleccion_char = 0;
    int opcion_valida = 0;
    while(true) {
        if(kbhit()){
            seleccion_char = getch();
            if (seleccion_char == '1') {
                opcion_valida = 1;
                gotoxy(45, 7); Color(AMARILLO); printf("1.- HUMANO VS HUMANO      <"); Color(AZUL);
                gotoxy(45, 8); printf("2.- HUMANO VS CPU           ");
            } else if (seleccion_char == '2') {
                opcion_valida = 2;
                gotoxy(45, 7); printf("1.- HUMANO VS HUMANO        ");
                gotoxy(45, 8); Color(AMARILLO); printf("2.- HUMANO VS CPU         <"); Color(AZUL);
            } else if (seleccion_char == ENTER && opcion_valida != 0) {
                a_ref = (opcion_valida == 1) ? '1' : '2';
                break;
            }
        }
        pausa(10);
    }

    for(i=0;i<18;i++){
        for(j=0;j<71;j++){
            gotoxy(j+5,i+5);
            printf(" ");
        }
    }
}

void GANADOR::pintar_ganador1(int &g_key){
    int i,j;
    Color(ROJO);
    char ganador[20][73]=
    {
    "                                                                      ",
    "    aaaaas     aaaas   aaas   aas   aaaas                             ",
    "   aa    as  aa    aas aasas  aas aa    aas                           ",
    "   aa        aa    aas aas as aas aa    aas                           ",
    "   aa  aaaa  aa    aas aas  asaas aa    aas                           ",
    "   aa    aas aaaaaaaas aas   aaas aaaaaaaas                           ",
    "   aaaaaaaas aa    aas aas    aas aa    aas                           ",
    "    aaaaa a  aa    aas aas    aas aa    aas                           ",
    "                                                                      ",
    "                                                        aass          ",
    "                                                      aaaaas          ",
    "                                                        aass          ",
    "                                                        aass          ",
    "                                                        aass          ",
    "                                                        aass          ",
    "                                                        aass          ",
    "                                                                      ",
    "                                                                      ",
    };
    for(i=0;i<18;i++){
        for(j=0; j < 73 && ganador[i][j] != '\0';j++){
            gotoxy(j+5,i+5);
            printf("%c",ganador[i][j]);
        }
    }
    g_key=getch();
}

void GANADOR::pintar_ganador2(int &ga_key){
    int i,j;
    Color(VERDE);
    char ganador[20][73]=
    {
    "                                                                      ",
    "    aaaaas     aaaas   aaas   aas   aaaas                             ",
    "   aa    as  aa    aas aasas  aas aa    aas                           ",
    "   aa        aa    aas aas as aas aa    aas                           ",
    "   aa  aaaa  aa    aas aas  asaas aa    aas                           ",
    "   aa    aas aaaaaaaas aas   aaas aaaaaaaas                           ",
    "   aaaaaaaas aa    aas aas    aas aa    aas                           ",
    "    aaaaa a  aa    aas aas    aas aa    aas                           ",
    "                                             saaaaaaa                 ",
    "                                            saaaaaaass                ",
    "                                              saaaaaaas               ",
    "                                                  aass                ",
    "                                              aaaas                   ",
    "                                             aass                     ",
    "                                            aaaaaaaass                ",
    "                                            aaaaaaaass                ",
    "                                                                      ",
    "                                                                      ",
    };
    for(i=0;i<18;i++){
        for(j=0; j < 73 && ganador[i][j] != '\0';j++){
            gotoxy(j+5,i+5);
            printf("%c",ganador[i][j]);
        }
    }
    ga_key=getch();
}
