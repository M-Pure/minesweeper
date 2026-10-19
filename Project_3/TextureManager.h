#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using namespace sf;
using namespace std;

class TextureManager {
    static unordered_map<string, Texture> textures;
    static void LoadTexture(string filename);
public:
   
    static Texture& GetTexture(string textureName);
    static void Clear(); // Clear all textures at end of main()
};

