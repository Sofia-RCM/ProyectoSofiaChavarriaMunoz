#include "Gem.h"
#include "Board.h"

void Gem::applyScaleAndPosition() {
    float x = static_cast<float>(col * cellSize);
    float y = static_cast<float>(row * cellSize);
    FloatRect local = sprite.getLocalBounds();
    if (local.width > 0 && local.height > 0) {
        float sx = cellSize / local.width;
        float sy = cellSize / local.height;
        float base = (sx < sy ? sx : sy);
        float s = base * 0.85f;
        sprite.setScale(s, s);
        local = sprite.getLocalBounds();
        sprite.setOrigin(local.width * 0.5f, local.height * 0.5f);
        sprite.setPosition(x + cellSize * 0.5f, y + cellSize * 0.5f);
    }
}

void Gem::setGrid(int r, int c, int cell) {
    row = r;
    col = c;
    cellSize = cell;
    applyScaleAndPosition();
}

void Gem::setPosition(float cx, float cy) {
    sprite.setPosition(cx, cy);
}
