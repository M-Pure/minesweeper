#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
using namespace sf;
using namespace std;

class Tile {
    Sprite tileSprite;
    vector<Tile*> adjacentTiles;
    bool isRevealed; // Keeps track of status to avoid duplicate actions
    
    Sprite* flagSprite; // It's "own" flagSprite. Only exists if isFlagged == true
    bool isFlagged;
    
    Sprite* mineSprite; // It's "own" mineSprite. May not exist (yet) if isMine == true
    bool isMine;
    
    Sprite* numSprite;
    
    
    
    /* Creates, texturizes, and sets position of numSprite based on
     passed in num parameter */
    void Number(int num);
    
public:
    
    Tile();
    Tile(string textureName);
    ~Tile(); // For deleting any leftover flags/mines
    
    // Accessors
    
    Sprite GetSprite() const;
    
    Sprite* GetMineSprite() const;
    
    FloatRect GetBounds() const;
    
    bool GetIsFlagged() const;
    
    bool GetIsMine() const;
    
    bool GetIsRevealed() const;
    
    
    
    // For debugging links to adjacent tiles
    const vector<Tile*>& GetLinks() const;

    
    // Functionality
    
    void SetTexture(string textureName);
    
    void SetPosition(Vector2f location);
    
    /* Creates, texturizes, and sets the position of the mineSprite
    outside classes can only call ToggleMine */
    void Mine();
    
    void UnMine();
    
    void ToggleMine();
    
    // Adds parameter Tile as an adjacent tile using a pointer to it
    void Link(Tile* adjacentTile);
    
    // Updates isFlagged, only if tile has not been revealed. true if successful, false otherwise
    bool Flag();
    
    // Uses SetTexture but also updates isFlagged
    void UnFlag();
    
    int CheckSurroundingMines();
    
    void RevealSurroundingTiles();

    // Trips gameOverIndicator if mine was revealed
    void Reveal(bool ignoreFlags);
    
    
    
    void Draw(RenderWindow& window);
    
    
};
