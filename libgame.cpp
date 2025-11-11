#include "libgame.h"
#include <windows.h>
#include <stdio.h>

void gotoxy(int x, int y)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hCon, dwPos);
}

void Color(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void pintar_marco()
{
    Color(MORADO);
    int i, v;
    for(i = 2; i < 78; i++){
        gotoxy(i, 3);    printf("%c", 205);
        gotoxy(i, 23);   printf("%c", 205);
    }
    for(v = 4; v < 23; v++){
        gotoxy(2, v);    printf("%c", 186);
        gotoxy(77, v);   printf("%c", 186);
    }
    gotoxy(2, 3);    printf("%c", 201);
    gotoxy(77, 3);   printf("%c", 187);
    gotoxy(2, 23);   printf("%c", 200);
    gotoxy(77, 23);  printf("%c", 188);

    Color(PLATA);
    for(i = 4; i < 23; i+=2) {
        gotoxy(39, i); printf("%c", 179);
    }
}

void pausa(int x){
    Sleep(x);
}
