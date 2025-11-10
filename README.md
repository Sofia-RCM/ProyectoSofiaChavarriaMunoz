# Match-3 Studio Ghibli Edition

Proyecto desarrollado en **C++** utilizando la librería **SFML**.  
El juego es un *Match-3* ambientado en el universo de **Studio Ghibli**, donde el jugador combina gemas iguales para generar efectos especiales y alcanzar los objetivos de cada nivel.

---

## Instrucciones de instalación

1. **Clonar el repositorio**
   ```bash
   git clone https://github.com/tuusuario/Match3-StudioGhibli.git

    Abrir el proyecto en Visual Studio 2022 o superior

        Asegúrese de que la carpeta assets/ se encuentre en la misma ruta que el ejecutable (.exe).

        Dentro de assets/ deben estar las imágenes de las gemas y los fondos:

    assets/
    ├── gemaTotoro.png
    ├── gemaPonyo.png
    ├── gemaGato.png
    ├── gemaParti.png
    ├── gemaGalleta.png
    ├── gemaIce.png
    ├── FondoM.png
    ├── Fondo2.png

Configurar SFML

    En Propiedades del proyecto → C/C++ → General, añadir la ruta de inclusión de SFML (include).

    En Propiedades → Vinculador → General, añadir la ruta de las librerías SFML (lib).

    En Propiedades → Vinculador → Entrada, agregar las siguientes librerías:

        sfml-graphics.lib
        sfml-window.lib
        sfml-system.lib

        En la carpeta del ejecutable (por ejemplo x64/Debug o x64/Release), copiar las DLL de SFML correspondientes.

    Compilar y ejecutar

        Seleccionar la configuración Release.

        Compilar y ejecutar con Ctrl + F5 desde Visual Studio.

Instrucciones de uso

    Al ejecutar el programa, aparecerá el menú principal.

    Presione Jugar para iniciar la partida.

    Intercambie gemas adyacentes con el mouse para formar combinaciones de tres o más del mismo tipo.

    Al lograr combinaciones de cuatro o más, se generan gemas especiales con efectos únicos.

    Cumpla los objetivos de cada nivel, como eliminar cierta cantidad de gemas Totoro o romper gemas de hielo.

    El juego termina cuando se completan los tres niveles o se agotan los movimientos.

Estructura del proyecto

Match3-StudioGhibli/
│
├── assets/                # Imágenes y recursos visuales
│   ├── gemaTotoro.png
│   ├── gemaPonyo.png
│   ├── gemaParti.png
│   ├── gemaGato.png
│   ├── gemaGalleta.png
│   ├── gemaIce.png
│   ├── FondoM.png
│   ├── Fondo2.png
│
├── src/                   # Código fuente del proyecto
│   ├── main.cpp
│   ├── Game.cpp / Game.h
│   ├── Board.cpp / Board.h
│   ├── Gem.cpp / Gem.h
│   ├── TotoroGem.cpp / TotoroGem.h
│   ├── PonyoGem.cpp / PonyoGem.h
│   ├── GatoGem.cpp / GatoGem.h
│   ├── PartiGem.cpp / PartiGem.h
│   ├── GalletaGem.cpp / GalletaGem.h
│   ├── IceGem.cpp / IceGem.h
│
├── README.md
├── CHANGELOG.md
└── LICENSE (opcional)

Diagrama de clases (resumen textual)

Clase Game
 ├── Atributos: board, puntuación, nivel, contadores de gemas
 ├── Métodos: run(), startGame(), nextLevel(), updateHUD(), checkLevelAdvance()

Clase Board
 ├── Atributos: matrix[8][8], animMatrix, explosions, flashes
 ├── Métodos: fillBoard(), findAndClearMatches(), applyGravityAndRefill(), drawBoard(), placeRandomIce()
 ├── Relación: contiene punteros a objetos Gem

Clase Gem (abstracta)
 ├── Atributos: tipoGem, sprite, texture, isSpecial, isActivated
 ├── Métodos virtuales: setTipoGem(), draw(), onMatch(), setGrid()
 ├── Heredan de Gem:
 │   ├── TotoroGem      → elimina todo el tablero
 │   ├── PonyoGem       → elimina fila y columna
 │   ├── GatoGem        → elimina gemas diagonales
 │   ├── PartiGem       → elimina 3 filas y 3 columnas
 │   ├── GalletaGem     → elimina una fila completa
 │   ├── IceGem         → requiere golpes para romperse

Créditos y autoría

    Autora: Sofía Chavarría

    Institución: Universidad Nacional de Costa Rica

    Curso: Programación Orientada a Objetos (Etapa 3)

    Lenguaje: C++ con SFML 2.6.1

    Fecha de finalización: Noviembre 2025

Estado del repositorio

Rama principal: main
Rama de desarrollo: develop

Versiones etiquetadas:
- v1.0: Base del tablero y gemas normales
- v2.0: Gemas especiales y polimorfismo
- v3.0: Efectos visuales, mejoras y documentación
