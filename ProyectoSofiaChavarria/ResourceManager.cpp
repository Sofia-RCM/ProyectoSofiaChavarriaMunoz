#include "ResourceManager.h"

map<string, Texture> ResourceManager::textures;
Font ResourceManager::font;

bool ResourceManager::loadTexture(const string& name, const string& path) {
    if (textures.find(name) != textures.end()) return true;

    Texture tex;
    if (!tex.loadFromFile(path))
        return false;

    textures[name] = move(tex);
    return true;
}

Texture& ResourceManager::getTexture(const string& name) {
    if (textures.find(name) == textures.end())
        throw runtime_error("Texture not found: " + name);
    return textures[name];
}

bool ResourceManager::loadFont(const string& path) {
    return font.loadFromFile(path);
}

Font& ResourceManager::getFont() {
    return font;
}
