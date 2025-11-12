#  Match-3 Studio Ghibli Edition

Proyecto desarrollado en **C++** utilizando la librería **SFML**.  
El juego es un *Match-3* inspirado en el universo de **Studio Ghibli**, donde el jugador combina gemas iguales para generar efectos especiales y cumplir los objetivos de cada nivel.

---

## Características principales

- Sistema de niveles progresivos (Totoro → Ice → Ponyo).  
- Gemas especiales con efectos únicos (explosiones, diagonales, filas, tableros completos).  
- Obstáculo **IceGem**, que requiere dos impactos para romperse.  
- **Pantalla de transición animada** entre niveles.  
- **Sistema de guardado por jugador**:  
  - Si un jugador nuevo ingresa su nombre, comienza desde el Nivel 1.  
  - Si un jugador existente vuelve, puede continuar desde el último nivel desbloqueado.  
- Pantalla final con **ranking global** y **botones interactivos** (“Reiniciar”, “Menú”, “Salir”).  
- Fondo e interfaz visual con la estética cálida de *Studio Ghibli*.  

---

## Instrucciones de instalación

1. **Clonar el repositorio**
   ```bash
   git clone https://github.com/tuusuario/Match3-StudioGhibli.git
2. **Abrir el proyecto en Visual Studio 2022 o superior**
 -Asegúrese de que la carpeta assets/ esté en la misma ruta que el ejecutable (.exe).
 -Dentro de assets/ deben estar las imágenes de las gemas, los fondos y los archivos auxiliares:
```
assets/
├── gemaTotoro.png
├── gemaPonyo.png
├── gemaGato.png
├── gemaParti.png
├── gemaGalleta.png
├── gemaIce.png
├── FondoM.png
├── Fondo2.png
├── fuente.ttf (opcional, se usa Arial si no está)
```
3. **Configurar SFML**
-En Propiedades del proyecto → C/C++ → General, añadir la ruta de inclusión de SFML (include).
-En Propiedades → Vinculador → General, añadir la ruta de las librerías SFML (lib).
-En Propiedades → Vinculador → Entrada, agregar las siguientes librerías:

```
sfml-graphics.lib
sfml-window.lib
sfml-system.lib
```
-En la carpeta del ejecutable (por ejemplo, x64/Debug o x64/Release), copiar las DLL de SFML correspondientes.
4. **Compilar y ejecutar**
-Seleccionar la configuración Release.
-Compilar y ejecutar con Ctrl + F5 desde Visual Studio.

## Instrucciones de uso

1. Al ejecutar el programa, aparecerá el menú principal con un campo para ingresar el nombre del jugador.
2. Si el jugador es nuevo, empezará desde el Nivel 1. Si ya ha jugado, podrá continuar desde su nivel desbloqueado.
3. Presione “Jugar” para acceder al menú de niveles.
4. En cada nivel:
-Intercambie gemas adyacentes con el mouse para formar combinaciones de tres o más del mismo tipo.
-Al formar combinaciones de cuatro o más, se generan gemas especiales:
-Totoro → Elimina todo el tablero.
-Ponyo → Elimina fila y columna.
-Gato → Elimina diagonales.
-Galleta → Elimina una fila completa.
-Parti → Elimina 3 filas y 3 columnas.
-Las gemas Ice requieren dos golpes adyacentes o el impacto de una gema especial para romperse.
5. Cumpla los objetivos de cada nivel:
-Nivel 1: eliminar 20 Totoro.
-Nivel 2: eliminar 10 Hielo.
-Nivel 3: eliminar 30 Ponyo.
6. Disfrute de la pantalla de transición cremita entre niveles.
7. Al finalizar, aparecerá una pantalla con puntaje, ranking y botones para reiniciar, volver al menú o salir.

## Estructura del proyecto
```
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
│   ├── Ranking.cpp / Ranking.h
│   ├── ProgressManager.cpp / ProgressManager.h
│
├── progress.txt            # Guarda progreso de jugadores
├── ranking.txt             # Guarda puntajes globales
├── README.md
└── CHANGELOG.md

```
## Diagrama de clases (resumen textual)
```
Clase Game
 ├── Atributos: board, puntuación, nivel, contadores de gemas, ProgressManager
 ├── Métodos: run(), startGame(), nextLevel(), updateHUD(), checkLevelAdvance(), drawFinalScreen()
 ├── Muestra pantallas de menú, niveles y resultados finales

Clase Board
 ├── Atributos: matrix[8][8], explosions, offset, etc.
 ├── Métodos: fillBoard(), findAndClearMatches(), applyGravityAndRefill(), drawBoard()
 ├── Relación: contiene punteros a objetos Gem

Clase Gem (abstracta)
 ├── Atributos: tipoGem, sprite, texture, isSpecial, isActivated, empty
 ├── Métodos virtuales: setTipoGem(), draw(), onMatch(), setGrid()
 ├── Heredan de Gem:
 │   ├── TotoroGem      → elimina todo el tablero
 │   ├── PonyoGem       → elimina fila y columna
 │   ├── GatoGem        → elimina gemas diagonales
 │   ├── PartiGem       → elimina 3 filas y 3 columnas
 │   ├── GalletaGem     → elimina una fila completa
 │   ├── IceGem         → obstáculo con 2 golpes de resistencia
```
## Lógica de progreso

Cada jugador tiene su propio progreso almacenado en progress.txt:
```
Sophie=3
Lily=2
Cris=1

```
-Si el nombre no existe, se crea y empieza desde el nivel 1.
-Si el nombre ya existe, puede continuar desde el último nivel desbloqueado.
-Los datos del ranking se guardan en ranking.txt con el formato:
```
Sophie 1200
Lily 900
Cris 780
```


## Créditos y autoría
Autora: Sofía Chavarría Muñoz
Institución: Universidad Nacional de Costa Rica (UNA)
Curso: Programación I
Lenguaje: C++ con SFML 2.6.1
Docente: Alejandro Flores Quesada

## Estado del repositorio
```
Rama principal: main
Rama de desarrollo: develop

Versiones etiquetadas:
- v1.0: Base del tablero y gemas normales
- v2.0: Gemas especiales y polimorfismo
- v3.0: Obstáculos, efectos y transición de niveles
- v4.0: Sistema de progreso por jugador y menú interactivo final
```

## Licencia

Este proyecto se desarrolló con fines académicos en la Universidad Nacional (UNA).
El uso del código está permitido únicamente para propósitos educativos y de aprendizaje.

## Vista Previa
![Menú del juego](assets/Screenshot2025-11-12.png)
