# 🌸 Match-3 Studio Ghibli Edition

Proyecto desarrollado en **C++** utilizando la librería **SFML**.  
El juego es un *Match-3* inspirado en el universo de **Studio Ghibli**, donde el jugador combina gemas iguales para generar efectos especiales y cumplir los objetivos de cada nivel.

---

## 🎮 Características principales

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

## ⚙️ Instrucciones de instalación

1. **Clonar el repositorio**
   ```bash
   git clone https://github.com/tuusuario/Match3-StudioGhibli.git
Abrir el proyecto en Visual Studio 2022 o superior

Asegúrese de que la carpeta assets/ esté en la misma ruta que el ejecutable (.exe).

Dentro de assets/ deben estar las imágenes de las gemas, los fondos y los archivos auxiliares:
