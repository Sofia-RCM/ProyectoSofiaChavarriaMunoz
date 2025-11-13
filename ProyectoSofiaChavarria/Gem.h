#pragma once
#include <SFML/Graphics.hpp>  
#include <string>            

// Declaración anticipada de la clase Board (solo se necesita el nombre aquí, no la definición completa)
class Board;

// Clase base para todas las gemas del juego
class Gem {
protected:
    // Estos atributos solo los pueden usar esta clase y las que hereden de ella
    std::string tipoGem;        // Nombre del tipo de gema (ej: "Totoro", "Ice", "PonyoEspecial")
    sf::Sprite  sprite;         // Objeto visual que se dibuja en pantalla
    sf::Texture texture;        // Imagen que se usa para el sprite
    bool empty = false;         // Indica si la gema está "vacía" (como una celda eliminada)
    bool isSpecial = false;     // Indica si es una gema especial (creada al hacer líneas de 4+)
    bool isActivated = false;   // Indica si una gema especial ya fue usada (para no activarla dos veces)

public:
    // Constructor y destructor
    Gem();
    virtual ~Gem();  // "virtual" permite que clases hijas hagan limpieza correcta

    // Métodos virtuales: pueden ser redefinidos (sobrescritos) por clases hijas
    virtual void draw(sf::RenderWindow& window);  // Dibuja la gema en la ventana
    virtual void setTipoGem(const std::string& tipo);  // Define el tipo e carga su imagen
    virtual std::string getTipoGem() const { return tipoGem; }  // Devuelve el nombre del tipo

    // Coloca la gema en una celda específica del tablero
    virtual void setGrid(int fila, int columna, int cellSize, float offsetX, float offsetY);

    // Dice si la gema está vacía (útil para saber si una celda está libre)
    virtual bool isEmpty() const { return empty; }

    // Acción que ocurre cuando esta gema se elimina en una combinación
    // Por defecto no hace nada, pero las clases hijas (como IceGem, TotoroGem, etc.)
    // pueden redefinirlo para hacer efectos especiales
    virtual void onMatch(Board& board, int row, int col) {}

    // Métodos para leer y cambiar si es especial o ya fue activada
    bool getIsSpecial() const { return isSpecial; }
    void setIsSpecial(bool special) { isSpecial = special; }
    bool getIsActivated() const { return isActivated; }
    void setIsActivated(bool activated) { isActivated = activated; }

    // Permite acceder al sprite (por ejemplo, para moverlo o cambiar su posición)
    sf::Sprite& getSprite() { return sprite; }

#include <string>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class Gem {
private:
    string tipoGem;
    int row = 0;
    int col = 0;
    int cellSize = 64;

    Texture texture;
    Sprite  sprite;
    bool loaded = false;
    string imagePath;

    RectangleShape cross1;
    RectangleShape cross2;

    void applyScaleAndPosition();

public:
    Gem();
    void setTipoGem(string& tipo);
    string getTipoGem();

    string getImage();
    bool   isLoaded();

    void setGrid(int r, int c, int cell);
    void setImage(string path, int cell);

    void draw(sf::RenderWindow& window);
    bool isEmpty();
};