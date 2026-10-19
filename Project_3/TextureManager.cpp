#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include "TextureManager.h"
using namespace sf;
using namespace std;

// Declare static varaible for .cpp file
unordered_map<string, Texture> TextureManager::textures;

void TextureManager::LoadTexture(string filename) {
    string path = "images/";
    path += filename + ".png";
    
    textures[filename].loadFromFile(path);
}

Texture& TextureManager::GetTexture(string textureName) {
    
    if (textures.find(textureName) == textures.end()) { // Not found
        LoadTexture(textureName);
    }
    return textures[textureName];
}

void TextureManager::Clear() {
    textures.clear();
}
