# 🏓 Pin pong (Proyecto en C++)

¡Un clásico juego de Pong desarrollado en C++ para la consola de Windows! Este proyecto fue creado como parte de un ejercicio de programación, implementando varias características clave de C++ y manejo de la consola.

<img width="721" height="446" alt="image" src="https://github.com/user-attachments/assets/8d755a57-e6eb-40aa-a5e7-6d2d2e4e37e3" />
Imagen del juego
<img width="737" height="468" alt="image" src="https://github.com/user-attachments/assets/139795ac-0566-4efc-81dc-1beece397803" />
Imagen del texto para el ganador
<img width="707" height="414" alt="image" src="https://github.com/user-attachments/assets/c795700e-f8de-4130-a27e-e904a5ad31f6" />
Imagen del ranking
<img width="718" height="411" alt="image" src="https://github.com/user-attachments/assets/9c580e32-3f6c-48ac-9e6e-793d4fe98592" />
Imagen del historial
<img width="721" height="407" alt="image" src="https://github.com/user-attachments/assets/3729d363-7c91-48c8-b1a1-84c7a830e328" />

---

## 🌟 Características

* **Juego Clásico de Pong:** Jugador contra un oponente CPU u otro jugador.
* **CPU con Dificultad:** La IA del oponente (`mover_cpu`) reacciona de forma diferente según el nivel de dificultad.
* **Ranking Persistente:** Las victorias se guardan en un archivo (`ranking.txt`) y se muestran en un ranking global.
* **Historial de Eventos:** El juego guarda un historial de los últimos eventos (ej. "Jugador 1 ganó") usando una lista enlazada.
* **Efecto de Estela:** La pelota deja una "estela" o rastro visual (`std::vector<Punto>`) para un efecto más dinámico.
* **Interfaz de Consola Colorida:** Utiliza funciones de la API de Windows (`<windows.h>`) para `gotoxy` y `Color`, dando vida a la consola.

---

## 🛠️ Tecnologías Utilizadas

* **Lenguaje:** C++ (compilado con estándar C++11).
* **IDE:** Desarrollado en [Code::Blocks](http://www.codeblocks.org/).
* **Bibliotecas Estándar de C++:**
    * `<iostream>` para I/O básico.
    * `<fstream>` para el manejo del archivo de ranking.
    * `<string>`, `<vector>`, `<map>` para el manejo de datos.
    * `<conio.h>` para la entrada de teclado (`getch`, `kbhit`).
* **APIs Específicas:**
    * `<windows.h>`: Usado para el control de la consola (posicionamiento con `gotoxy` y manejo de `Color`). **(Esto hace que el proyecto sea exclusivo para Windows)**.

---

## 🚀 Cómo Compilar y Ejecutar

Este proyecto está diseñado para ser compilado en un entorno **Windows**.

### Opción 1: Con Code::Blocks (Recomendado)

1.  Asegúrate de tener [Code::Blocks](http://www.codeblocks.org/downloads) instalado (con el compilador MinGW/GCC).
2.  Clona o descarga este repositorio.
3.  Abre el archivo `Pin pong.cbp` con Code::Blocks.
4.  Presiona **F9** (o el botón "Build and run") para compilar y ejecutar el juego.

### Opción 2: Manualmente (Línea de Comandos)

Si tienes un compilador de C++ (como MinGW/g++) en tu PATH de Windows:

1.  Abre una terminal (`cmd` o `PowerShell`) en la carpeta del proyecto.
2.  Ejecuta el siguiente comando para compilar todos los archivos fuente:

    ```bash
    g++ main.cpp pong.cpp libgame.cpp -o "Pin pong.exe" -std=c++11 -static-libgcc -static-libstdc++
    ```

3.  Ejecuta el programa:
    ```bash
    ./"Pin pong.exe"
    ```

---

## 🎮 Controles del Juego

* **Mover Paleta:** Usa las **Flechas Q y A** para el Jugador 1 y usa las **Flechas O y L** para el Jugador 2 del teclado para mover tu paleta.
* **Menú Post-partida:**
    * Presiona **'s'** para jugar de nuevo.
    * Presiona **'h'** para ver el historial de eventos recientes.
    * Presiona cualquier otra tecla para salir.

---

## 📂 Estructura de Archivos

* **`main.cpp`**: Contiene el bucle principal del juego (`main`), la gestión del menú, el sistema de ranking y el historial de eventos.
* **`pong.h` / `pong.cpp`**: Define las clases principales del juego:
    * `JUGADOR`: Controla la lógica de las paletas (jugador y CPU).
    * `PELOTA`: Controla la lógica de la pelota (movimiento, colisiones y estela).
* **`libgame.h` / `libgame.cpp`**: Biblioteca de utilidades para el manejo de la consola de Windows (color, `gotoxy`, pintar marco).
* **`Pin pong.cbp`**: Archivo de proyecto de Code::Blocks.
* **`ranking.txt`**: Archivo de texto plano donde se almacena el ranking de victorias.
