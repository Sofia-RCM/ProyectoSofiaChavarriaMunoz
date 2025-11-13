#include "Gem.h"
#include <iostream>

// Constructor y destructor básicos de la clase Gem (gema)
Gem::Gem() {}
Gem::~Gem() {}

// Establece el tipo de gema (como "Totoro", "Ponyo", etc.) y carga su imagen
void Gem::setTipoGem(const std::string& tipo) {
    tipoGem = tipo;  // Guarda el nombre del tipo (por ejemplo: "Totoro" o "TotoroEspecial")

    // Crea la ruta del archivo de imagen: "assets/gemaTotoro.png", etc.
    std::string ruta = "assets/gema" + tipo + ".png";

    // Intenta cargar la imagen desde esa ruta
    if (!texture.loadFromFile(ruta)) {
        // Si falla, lanza un error con el nombre del archivo que no se pudo cargar
        throw std::string("Error: No se pudo cargar la textura: ") + ruta;

#include "Game.h"
using namespace std;
using namespace sf;


Gem::Gem() {
    cross1.setFillColor(Color::Magenta);
    cross2.setFillColor(Color::Magenta);
}


void Gem::applyScaleAndPosition() {
    float x = static_cast<float>(col * cellSize);
    float y = static_cast<float>(row * cellSize);

    cross1.setPosition(x, y);
    cross1.setSize(Vector2f(static_cast<float>(cellSize), 2.f));
    cross1.setRotation(45.f);

    cross2.setPosition(x + static_cast<float>(cellSize), y);
    cross2.setSize(Vector2f(static_cast<float>(cellSize), 2.f));
    cross2.setRotation(135.f);

    if (!loaded) return;

    FloatRect local = sprite.getLocalBounds();
    if (local.width > 0.f && local.height > 0.f) {
        float sx = static_cast<float>(cellSize) / local.width;
        float sy = static_cast<float>(cellSize) / local.height;
        float base = (sx < sy ? sx : sy);
        float s = base * 0.85f;
        sprite.setScale(s, s);

        local = sprite.getLocalBounds();
        sprite.setOrigin(local.width * 0.5f, local.height * 0.5f);
        sprite.setPosition(x + cellSize * 0.5f, y + cellSize * 0.5f);

    }

    // Asigna la imagen al sprite (objeto visual que se dibuja en pantalla)
    sprite.setTexture(texture);

    // Reduce el tamaño de la imagen al 10% (porque las imágenes originales son muy grandes)
    sprite.setScale(0.1f, 0.1f);

    // Marca si esta gema es especial (si su nombre contiene la palabra "Especial")
    isSpecial = (tipo.find("Especial") != std::string::npos);

    // Al crearla, no está activada (solo importa para gemas especiales)
    isActivated = false;

    // Indica que la gema no está vacía (es una gema real, no un espacio en blanco)
    empty = false;
}

// Coloca la gema en una celda específica del tablero
void Gem::setGrid(int fila, int columna, int cellSize, float offsetX, float offsetY) {
    // Calcula el centro de la celda en la ventana
    float cellCenterX = offsetX + columna * cellSize + cellSize * 0.5f;
    float cellCenterY = offsetY + fila * cellSize + cellSize * 0.5f;

    // Obtiene el tamaño original de la imagen cargada
    sf::Vector2u ts = texture.getSize();
    // Calcula el ancho y alto reales del sprite después de escalarlo
    float w = ts.x * sprite.getScale().x;
    float h = ts.y * sprite.getScale().y;

    // Centra el sprite en la celda (ajusta la posición para que quede bien alineado)
    sprite.setPosition(cellCenterX - w * 0.5f, cellCenterY - h * 0.5f);
}

// Dibuja la gema en la ventana, pero solo si no está marcada como "vacía"
void Gem::draw(sf::RenderWindow& window) {
    if (!empty) window.draw(sprite);  // Si no es vacía, la muestra

void Gem::setTipoGem(string& tipo) {
    tipoGem = tipo;
    loaded = true;

    if (TEXTURES.find(tipo) != TEXTURES.end()) {
        sprite.setTexture(TEXTURES[tipo]);

        auto bounds = sprite.getLocalBounds();
        float scale = (float)cellSize / bounds.width;
        sprite.setScale(scale, scale);

        sprite.setPosition(col * cellSize, row * cellSize);

    }
}
string Gem::getTipoGem() { return tipoGem; }

string Gem::getImage() { return imagePath; }
bool   Gem::isLoaded() { return loaded; }

void Gem::setGrid(int r, int c, int cell) {
    row = r;
    col = c;
    cellSize = cell;
    applyScaleAndPosition();
}

void Gem::setImage(string path, int cell) {
    this->imagePath = path;
    this->cellSize = cell;

    if (!loaded) {
        applyScaleAndPosition();
        ;
    }
}

void Gem::draw(sf::RenderWindow& window) {

    if (loaded) {
        window.draw(sprite);
    }
    else {
        window.draw(cross1);
        window.draw(cross2);
    }
}

bool Gem::isEmpty() {
    return tipoGem.empty();

}