#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <stdexcept>
using namespace std;
using namespace sf;

class ResourceManager {
private:
    static map<string, Texture> textures;
    static Font font;

public:
    static bool loadTexture(const string& name, const string& path);
    static Texture& getTexture(const string& name);

    static bool loadFont(const string& path);
    static Font& getFont();
};
