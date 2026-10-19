#include <SFML/Graphics.hpp>
#include <vector>
#include "Board.h"
#include "TextureManager.h"
#include "Tile.h"
using namespace sf;
using namespace std;

Tile::Tile() {
    isRevealed = false;
    
    flagSprite = nullptr;
    isFlagged = false;

    
    mineSprite = nullptr;
    isMine = false;
    
    numSprite = nullptr;
    
    for (int i = 0; i < adjacentTiles.size(); i++) {
        adjacentTiles[i] = nullptr;
    }
    
}

Tile::Tile(string textureName) {
    isRevealed = false;
    
    tileSprite.setTexture(TextureManager::GetTexture(textureName));
    
    // Tiles are initialized without connections to surrounding tiles
    for (int i = 0; i < adjacentTiles.size(); i++) {
        adjacentTiles[i] = nullptr;
    }
    
    flagSprite = nullptr;
    isFlagged = false;
    
    mineSprite = nullptr;
    isMine = false;
    
    numSprite = nullptr;
}

Tile::~Tile() {
    if (flagSprite != nullptr && isFlagged) { // Flag still exists on top of tile
        delete flagSprite;
        flagSprite = nullptr;
    }
    if (mineSprite != nullptr && isMine) { // Mine has been activated
        delete mineSprite;
        mineSprite = nullptr;
    }
    if (numSprite != nullptr) { // Number still remains
        delete numSprite;
        numSprite = nullptr;
    }
}

// Private methods



void Tile::Number(int num) {
    string numFileName = "number_" + to_string(num);
    numSprite = new Sprite;
    numSprite->setTexture(TextureManager::GetTexture(numFileName));
    numSprite->setPosition(tileSprite.getPosition());
}

// Accessors

Sprite Tile::GetSprite() const {
    return tileSprite;
}

Sprite* Tile::GetMineSprite() const {
    return mineSprite;
}

FloatRect Tile::GetBounds() const {
    return tileSprite.getGlobalBounds();
}

bool Tile::GetIsFlagged() const {
    return isFlagged;
}

bool Tile::GetIsMine() const {
    return isMine;
}

bool Tile::GetIsRevealed() const {
    return isRevealed;
}

// For debugging links to adjacent tiles
const vector<Tile*>& Tile::GetLinks() const {
    return adjacentTiles;
}

// Functionality

void Tile::SetTexture(string textureName) {
    tileSprite.setTexture(TextureManager::GetTexture(textureName));
}

void Tile::SetPosition(Vector2f location) {
    tileSprite.setPosition(location);
}

void Tile::ToggleMine() {
    if (isMine) {
        isMine = false;
    }
    else {
        isMine = true;
    }
}


void Tile::Link(Tile* adjacentTile) {
    adjacentTiles.push_back(adjacentTile);
}

bool Tile::Flag() {
    if (isRevealed) { // Can't flag a revealed tile
        return false;
    }
    flagSprite = new Sprite;
    flagSprite->setTexture(TextureManager::GetTexture("flag"));
    flagSprite->setPosition(tileSprite.getPosition());
    isFlagged = true;
    return true;
}

void Tile::UnFlag() {
    delete flagSprite;
    flagSprite = nullptr;
    isFlagged = false; // Will no longer be drawn
}



int Tile::CheckSurroundingMines() {
    int surroundingMines = 0;
    for (int i = 0; i < adjacentTiles.size(); i++) { // For every adjacent tile
        if (adjacentTiles[i]->isMine) {
            surroundingMines++;
        }
    }
    return surroundingMines;
}

void Tile::RevealSurroundingTiles() {
    for (int i = 0; i < adjacentTiles.size(); i++) {
        if (!adjacentTiles[i]->isRevealed) {
            adjacentTiles[i]->Reveal(false);
        }
    }
}

// Creates, texturizes, and sets the position of the mineSprite
void Tile::Mine() {
    mineSprite = new Sprite;
    mineSprite->setTexture(TextureManager::GetTexture("mine"));
    mineSprite->setPosition(tileSprite.getPosition());
}

// Helps debug to remove mine sprites
void Tile::UnMine() {
    if (mineSprite != nullptr) {
        delete mineSprite;
        mineSprite = nullptr;
        // Does not change status of mine to isMine = false, just hides the mine
    }
}

// If ignoreFlags == true, will reveal mines over flags
void Tile::Reveal(bool ignoreFlags) {
    if (!ignoreFlags) {
        if (!isFlagged) { // Flagged tiles cannot be revealed
            SetTexture("tile_revealed");
            isRevealed = true;
            if (isMine) {
                Mine();
                return;
            }
            int surroundingMines = CheckSurroundingMines();
            if (surroundingMines == 0) {
                RevealSurroundingTiles();
            }
            else {
                Number(surroundingMines);
            }
        }
    }
    else {
        SetTexture("tile_revealed");
        isRevealed = true;
        if (isMine) {
            Mine();
            return;
        }
        int surroundingMines = CheckSurroundingMines();
        if (surroundingMines == 0) {
            RevealSurroundingTiles();
        }
        else {
            Number(surroundingMines);
        }
    }
    
}

void Tile::Draw(RenderWindow& window) {
    window.draw(tileSprite);
    if (isFlagged) { // If tile is flagged, draws flag "on top of" tile
        window.draw(*flagSprite);
    }
    // Tile is a mine and HAS BEEN CLICKED (mineSprite has materialized)
    if (isMine && mineSprite != nullptr) {
        window.draw(*mineSprite);
    }
    if (numSprite != nullptr) { // Tile has been revealed and has adjacent mines
        window.draw(*numSprite);
    }
}



