#ifndef PONG_H_INCLUDED
#define PONG_H_INCLUDED

#include <vector>
#include <cstddef>
#include <string>

struct Punto {
    int x, y;
    Punto(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

class JUGADOR{
    int x,y;
public:
    JUGADOR(int _x, int _y);
    void pintar() const;
    void borrar() const;
    void mover_cpu(int pelota_x, int pelota_y, int pelota_dx, int nivel);
    void Y(int _y){ y += _y; }
    int RY() const { return y; }
    int RX() const { return x; }
};

class PELOTA{
    int x,y,dx,dy;
    int velocidad_base_x;
    int velocidad_base_y;
    int ralentizacion_movimiento;
    int pausa_base_nivel;

    std::vector<Punto> estela;
    static const size_t MAX_LONGITUD_ESTELA;


public:
    PELOTA(int _x, int _y, int _dx, int _dy);
    void pintar() const;
    void borrar() const;
    void borrarEstelaCompleta();
    void mover(JUGADOR A, JUGADOR B);
    void asign(int _x, int _y){ x = _x; y = _y; estela.clear(); agregarPuntoEstela(x,y); }
    void setDireccion(int _dx, int _dy);

    int PX() const { return x; }
    int PY() const { return y; }
    int DX() const { return dx; }

    void establecerVelocidadPorNivel(int nivel);
    int obtenerRalentizacion() const { return ralentizacion_movimiento; }
    int obtenerPausaBaseNivel() const { return pausa_base_nivel; }

private:
    void agregarPuntoEstela(int _x, int _y);
    void pintarEstela() const;
    void borrarEstela() const;
};

class MENU{
public:
    void pintar_portada(int &a);
};

class GANADOR{
public:
    void pintar_ganador1(int &g);
    void pintar_ganador2(int &ga);
};

#endif
