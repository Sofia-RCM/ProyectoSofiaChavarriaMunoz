# Match-3 Studio Ghibli Edition

Proyecto desarrollado en **C++** utilizando **SFML** como parte del curso de Programación (Etapa 3).

Este proyecto representa la culminación de tres etapas de desarrollo, con la implementación completa de un juego tipo *Match-3* (similar a *Candy Crush*) ambientado en el universo **Studio Ghibli**.  
El juego incluye **gemas especiales, animaciones visuales, efectos de resaltado verde, detección de combinaciones, caída de piezas y niveles progresivos.**

---

## Instalación y ejecución

###  Requisitos previos
- Visual Studio 2022 (o superior)
- SFML 2.5 (configurada en tu proyecto)
- Sistema operativo Windows 10 o superior

###  Instrucciones
1. Clona el repositorio:
   ```bash
   git clone https://github.com/TU_USUARIO/Match3-StudioGhibli.git
 2.Abre la solución ProyectoSofiaChavarria.sln en Visual Studio.
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

3.Configurar SFML
En Propiedades del proyecto → C/C++ → General, añadir la ruta de inclusión de SFML (include).
En Propiedades → Vinculador → General, añadir la ruta de las librerías SFML (lib).
En Propiedades → Vinculador → Entrada, agregar:
sfml-graphics.lib
sfml-window.lib
sfml-system.lib

En la carpeta del ejecutable (x64/Debug o Release), copiar las DLL de SFML correspondientes.
