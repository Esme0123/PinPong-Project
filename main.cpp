#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "libgame.h"
#include "pong.h"

using namespace std;

const string NOMBRE_ARCHIVO_RANKING = "ranking.txt";
const int MAX_JUGADORES_RANKING_MOSTRAR = 5;
const int MAX_EVENTOS_HISTORIAL = 5;//lista enlazada

struct JugadorRanking {
    string nombre;
    int victorias;

    JugadorRanking(string n, int v) : nombre(n), victorias(v) {}
    bool operator<(const JugadorRanking& otro) const {
        return victorias > otro.victorias;
    }
};

// Mapa para el ranking global, cargado/guardado desde archivo
std::map<std::string, int> mapa_ranking_global;
struct NodoEvento {
    string descripcion;
    NodoEvento* siguiente;

    NodoEvento(string desc) : descripcion(desc), siguiente(NULL) {}
};
class HistorialEventos {
public:
    NodoEvento* cabeza;
    int contador_eventos;

    HistorialEventos() : cabeza(NULL), contador_eventos(0) {}

    ~HistorialEventos() {
        NodoEvento* actual = cabeza;
        NodoEvento* proximo = NULL;
        while (actual != NULL) {
            proximo = actual->siguiente;
            delete actual;
            actual = proximo;
        }
        cabeza = NULL;
    }

    void agregarEvento(const string& desc) {
        NodoEvento* nuevo_nodo = new NodoEvento(desc);
        nuevo_nodo->siguiente = cabeza;
        cabeza = nuevo_nodo;
        contador_eventos++;

        // Mantener solo los últimos MAX_EVENTOS_HISTORIAL
        if (contador_eventos > MAX_EVENTOS_HISTORIAL) {
            NodoEvento* temp = cabeza;
            for (int i = 0; i < MAX_EVENTOS_HISTORIAL - 1 && temp != NULL; ++i) {
                temp = temp->siguiente;
            }
            if (temp != NULL && temp->siguiente != NULL) {
                NodoEvento* nodo_a_borrar_inicio = temp->siguiente;
                temp->siguiente = NULL; // Cortar la lista

                // Borrar los nodos restantes
                NodoEvento* actual = nodo_a_borrar_inicio;
                NodoEvento* proximo = NULL;
                while (actual != NULL) {
                    proximo = actual->siguiente;
                    delete actual;
                    actual = proximo;
                    contador_eventos--;
                }
                // contador_eventos ya se decrementó por cada nodo borrado,
                // pero para asegurar que sea MAX_EVENTOS_HISTORIAL:
                contador_eventos = MAX_EVENTOS_HISTORIAL;
            }
        }
    }

    void mostrarHistorialEventosPantalla() {
        // Se mostrará en una sección específica, no limpia toda la pantalla
        Color(PLATA);
        gotoxy(2, 24); // Debajo del marco del juego
        printf("Ultimos Eventos: ");
        NodoEvento* temp = cabeza;
        int eventos_mostrados = 0;
        string linea_eventos;
        while (temp != NULL && eventos_mostrados < MAX_EVENTOS_HISTORIAL) {
            linea_eventos += temp->descripcion;
            if (temp->siguiente != NULL && eventos_mostrados < MAX_EVENTOS_HISTORIAL -1) {
                linea_eventos += " | ";
            }
            temp = temp->siguiente;
            eventos_mostrados++;
        }
        // Limpiar línea anterior de eventos
        for(int i=0; i < 78; ++i) { gotoxy(2+i, 24); printf(" ");}
        gotoxy(2, 24); printf("Eventos: %s", linea_eventos.substr(0, 70).c_str()); // Limitar longitud
    }
};
void cargar_ranking() {
    mapa_ranking_global.clear();
    ifstream archivo(NOMBRE_ARCHIVO_RANKING.c_str());
    string nombre;
    int victorias;

    if (archivo.is_open()) {
        while (archivo >> nombre >> victorias) { // Lee nombre y victorias
            mapa_ranking_global[nombre] = victorias;
        }
        archivo.close();
    }
}

void guardar_ranking() {
    ofstream archivo(NOMBRE_ARCHIVO_RANKING.c_str());
    if (archivo.is_open()) {
        for (std::map<std::string, int>::const_iterator it = mapa_ranking_global.begin(); it != mapa_ranking_global.end(); ++it) {
            archivo << it->first << " " << it->second << endl;
        }
        archivo.close();
    } else {
        gotoxy(1,1); printf("Error: No se pudo guardar el ranking.");
        pausa(1000);
    }
}

void pedir_nombre_jugador(string& nombre_out) {
    system("cls");
    pintar_marco();
    Color(AMARILLO);
    gotoxy(10, 10); printf("¡Has ganado! Ingresa tu nombre para el ranking:");
    gotoxy(10, 12); printf("(max. 15 chars, sin espacios, presiona Enter al final)");
    gotoxy(10, 14); printf("Nombre: ");

    string nombre_ingresado = "";
    char c;
    int char_count = 0;
    int current_x = 23, current_y = 14;
    gotoxy(current_x, current_y);

    while (true) {
        if (kbhit()) {
            c = getch();
            if (c == ENTER && char_count > 0) {
                break;
            } else if (c == 8 && char_count > 0) { // Backspace
                nombre_ingresado.erase(nombre_ingresado.length() - 1);
                char_count--;
                current_x--;
                gotoxy(current_x, current_y); printf(" "); gotoxy(current_x, current_y);
            } else if (isprint(c) && c != ' ' && char_count < 15) {
                nombre_ingresado += c;
                printf("%c", c);
                char_count++;
                current_x++;
            }
        }
        pausa(10);
    }
    nombre_out = nombre_ingresado;
}


void mostrar_ranking_pantalla() {
    system("cls");
    pintar_marco();
    Color(VERDE2);
    gotoxy(28, 5); printf("--- RANKING DE JUGADORES ---");

    if (mapa_ranking_global.empty()) {
        gotoxy(23, 10); printf("El ranking esta vacio. ¡Juega para aparecer aqui!");
    } else {
        // Transferir a vector para ordenar
        std::vector<JugadorRanking> ranking_vector;
        for (std::map<std::string, int>::const_iterator it = mapa_ranking_global.begin(); it != mapa_ranking_global.end(); ++it) {
            ranking_vector.push_back(JugadorRanking(it->first, it->second));
        }

        std::sort(ranking_vector.begin(), ranking_vector.end());

        int y_offset = 8;
        gotoxy(18, y_offset++); printf("Pos. Nombre            Victorias");
        gotoxy(18, y_offset++); printf("---- ----------------- ---------");

        for (size_t i = 0; i < ranking_vector.size() && i < MAX_JUGADORES_RANKING_MOSTRAR; ++i) {
            char linea_ranking[80];
            sprintf(linea_ranking, "%-4d %-17s %-9d", (int)i + 1, ranking_vector[i].nombre.c_str(), ranking_vector[i].victorias);
            gotoxy(18, y_offset++);
            printf("%s", linea_ranking);
        }
    }

    gotoxy(20, 20); printf("Presiona cualquier tecla para continuar...");
    getch();
}


// Función para pintar el marcador y el nivel
void pintar_info_juego(int puntosA, int puntosB, int nivel) {
    Color(PLATA);
    gotoxy(25, 2);
    printf("Jugador 1: %d - Jugador 2: %d", puntosA, puntosB);
    gotoxy(60, 2);
    printf("Nivel: %d", nivel);
}


void iniciar_juego(int & op_juego_ref, HistorialEventos& historial_eventos_juego){
    int cont = 0, puntosA = 0, puntosB = 0, gan1_key, gan2_key;
    int nivel = 1;
    int puntos_para_subir_nivel = 0;

    system("cls");
    pintar_marco();
    historial_eventos_juego.agregarEvento("Inicio Partida");
    historial_eventos_juego.mostrarHistorialEventosPantalla();

    MENU M;
    GANADOR G;

    M.pintar_portada(op_juego_ref);

    system("cls");
    pintar_marco();
    historial_eventos_juego.mostrarHistorialEventosPantalla();

    JUGADOR A(6,15);    A.pintar();
    JUGADOR B(74,15);   B.pintar();

    PELOTA P(38,14,1,1);
    P.establecerVelocidadPorNivel(nivel);

    pintar_info_juego(puntosA, puntosB, nivel);

    char tecla;
    bool juego_terminado = false;
    string nombre_ganador_partida;

    while(!juego_terminado){
        if(kbhit()){
            A.borrar(); B.borrar();
            tecla = getch();
            if(tecla == 'q' && A.RY() > 5) A.Y(-1);
            else if(tecla == 'a' && A.RY() < 21) A.Y(1);

            if(op_juego_ref == '1'){
                if(tecla == 'o' && B.RY() > 5)  B.Y(-1);
                else if(tecla == 'l' && B.RY() < 21) B.Y(1);
            }
            A.pintar(); B.pintar();
        }

        if(op_juego_ref == '2' && cont == 0) {
            B.mover_cpu(P.PX(), P.PY(), P.DX(), nivel);
        }

        if(cont == 0) P.mover(A,B);
        cont++;
        if(cont > P.obtenerRalentizacion())  cont = 0;

        if(P.PX() < 4){
            puntosB++;
            string evento_punto = (op_juego_ref == '1' ? "Pto J2" : "Pto CPU");
            historial_eventos_juego.agregarEvento(evento_punto);
            P.borrarEstelaCompleta();
            P.asign(38,14);
            P.setDireccion(1,1);
            if(op_juego_ref == '1') puntos_para_subir_nivel++;
            historial_eventos_juego.mostrarHistorialEventosPantalla();
        }
        if(P.PX() > 75){
            puntosA++;
            historial_eventos_juego.agregarEvento("Pto J1");
            P.borrarEstelaCompleta();
            P.asign(38,14);
            P.setDireccion(-1,-1);
            puntos_para_subir_nivel++;
            historial_eventos_juego.mostrarHistorialEventosPantalla();
        }

        if (puntos_para_subir_nivel >= 2 && nivel < 3) {
            nivel++;
            char buffer_evento_nivel[20];
            sprintf(buffer_evento_nivel, "Nivel %d!", nivel);
            historial_eventos_juego.agregarEvento(buffer_evento_nivel);
            P.establecerVelocidadPorNivel(nivel);
            puntos_para_subir_nivel = 0;
            gotoxy(30,1); printf("SUBISTE DE NIVEL!");
            pausa(1000);
            gotoxy(30,1); printf("                  ");
            historial_eventos_juego.mostrarHistorialEventosPantalla();
        }

        pintar_info_juego(puntosA, puntosB, nivel);

        if (puntosA >= 3) { // Gana Jugador 1
            system("cls");
            pintar_marco();
            G.pintar_ganador1(gan1_key);
            if (op_juego_ref == '1') { // Humano vs Humano
                pedir_nombre_jugador(nombre_ganador_partida);
            } else {
                pedir_nombre_jugador(nombre_ganador_partida);
            }
            mapa_ranking_global[nombre_ganador_partida]++;
            guardar_ranking();
            historial_eventos_juego.agregarEvento(nombre_ganador_partida + " GANA");
            juego_terminado = true;
        } else if (puntosB >= 3) { // Gana Jugador 2
            system("cls");
            pintar_marco();
            G.pintar_ganador2(gan2_key);
            if (op_juego_ref == '1') { // Humano vs Humano, J2 ganó
                pedir_nombre_jugador(nombre_ganador_partida);
                mapa_ranking_global[nombre_ganador_partida]++;
                guardar_ranking();
                historial_eventos_juego.agregarEvento(nombre_ganador_partida + " GANA");
            }else { // CPU Gana
                 historial_eventos_juego.agregarEvento("CPU GANA");
            }
            juego_terminado = true;
        }
        pausa(P.obtenerPausaBaseNivel());
    }
    historial_eventos_juego.mostrarHistorialEventosPantalla(); // Última actualización visual
    pausa(1500);
    mostrar_ranking_pantalla();
}

int main()
{
    srand(time(NULL));
    cargar_ranking();
    HistorialEventos historial_global_eventos;

    while(true){
        int opcion_juego_main;
        iniciar_juego(opcion_juego_main, historial_global_eventos);

        system("cls");
        pintar_marco();
        gotoxy(13, 10); Color(AZUL);
        printf("Presiona 's' para jugar de nuevo, ");
        gotoxy(13, 12); Color(AZUL);
        printf("'r' para ver ranking, ");
        gotoxy(13,14); Color(AZUL);
        printf("'h' para historial, otra para salir.");
        char jugar_de_nuevo = getch();
        if (jugar_de_nuevo == 's' || jugar_de_nuevo == 'S') {
            // Continuar al siguiente bucle
        } else if (jugar_de_nuevo == 'r' || jugar_de_nuevo == 'R') {
            mostrar_ranking_pantalla();
            // Después de mostrar ranking, preguntar de nuevo
            gotoxy(13, 22); printf("Presiona 's' para jugar de nuevo, otra para salir.");
            jugar_de_nuevo = getch();
            if (jugar_de_nuevo != 's' && jugar_de_nuevo != 'S') {
                 break;
            }
        }else if (jugar_de_nuevo == 'h' || jugar_de_nuevo == 'H') {
            system("cls"); pintar_marco();
            // Para mostrar el historial de forma más prominente:
            Color(VERDE2); gotoxy(25,5); printf("--- HISTORIAL DE EVENTOS RECIENTES ---");
            NodoEvento* temp = historial_global_eventos.cabeza;
            int y_pos = 8;
            while(temp != NULL && y_pos < 20) {
                gotoxy(25, y_pos++); printf("- %s", temp->descripcion.c_str());
                temp = temp->siguiente;
            }
            gotoxy(25, 22); printf("Presiona cualquier tecla para continuar...");
            getch();

            system("cls"); pintar_marco();
            gotoxy(10, 10); printf("Presiona 's' para jugar de nuevo, otra para salir.");
            jugar_de_nuevo = getch();
            if (jugar_de_nuevo != 's' && jugar_de_nuevo != 'S') {
                 break;
            }
        }
        else {
            break; // Salir del bucle principal
        }
    }

    system("cls");
    gotoxy(30,12); Color(VERDE);
    printf("Gracias por jugar!\n");
    pausa(2000);
    return 0;
}
