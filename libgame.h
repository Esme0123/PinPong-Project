#ifndef LIBGAME_H_INCLUDED
#define LIBGAME_H_INCLUDED

#define ARRIBA      72
#define IZQUIERDA   75
#define DERECHA     77
#define ABAJO       80
#define ESC         27
#define ENTER       13

#define AZUL            9
#define AMARILLO        14
#define ROJO            12
#define VERDE           10
#define VERDE2          11
#define MORADO          5
#define PLATA           7
#define BLANCO          15

void gotoxy(int x,int y);
void Color(int color);
void pintar_marco();
void pausa(int x);

#endif
