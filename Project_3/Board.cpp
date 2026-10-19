#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>
#include <vector>
#include "RandomGenerator.h"
#include "TextureManager.h"
#include "Tile.h"
#include "Board.h"
using namespace sf;
using namespace std;

bool Board::gameOverIndicator = false;
bool Board::debugging = false;

void Board::ToggleGameOver() {
    if (Board::gameOverIndicator) {
        Board::gameOverIndicator = false;
    }
    else {
        Board::gameOverIndicator = true;
    }
}

void Board::ToggleDebug() {
    if (Board::debugging) {
        Board::debugging = false;
    }
    else {
        Board::debugging = true;
    }
}

Board::Board() {
    minesRemaining = 0;
}

//Board::Board(int xTiles, int yTiles, int mineCount) {
//    minesRemaining = mineCount;
//    Initialize(xTiles, yTiles);
//    SetRandomMines(mineCount);
//}

// High-level function handling all behind-the-scenes initialization of empty board objects
void Board::Initialize(int columns, int rows, int mines) {
    minesRemaining = mines;
    // 1. Create width # of rows with length (uninitialized) tiles each
    for (int i = 0; i < rows; i++) {
        AddRow(columns);
    }
    // 2. Move all tiles from (0,0) to correct positions
    Arrange();
    // 3. Give each tile a texture
    TexturizeAll("tile_hidden");
    // 4. Link tiles to each other
    LinkAll();
    SetRandomMines(minesRemaining);
}
// Handles all behind-the-scenes initialization of board objects with 2D template vector
void Board::Initialize(const vector<vector<int>>& vectorMap) {
    for (int i = 0; i < vectorMap.size(); i++) {
        AddRow(static_cast<int>(vectorMap[0].size()));
    }
    Arrange();
    TexturizeAll("tile_hidden");
    LinkAll();
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            if (vectorMap[i][j] == 1) {
                board[i][j].ToggleMine();
                minesRemaining++;
            }
        }
    }
}

// Accessors

const vector<vector<Tile>>& Board::Get2DBoard() const {
    return board;
}

bool Board::GetGameOverIndicator() {
    return Board::gameOverIndicator;
}

bool Board::GetDebugStatus() {
    return Board::debugging;
}

int Board::GetMinesRemaining() const {
    return minesRemaining;
}

int Board::GetRows() const {
    return static_cast<int>(board.size());
}

int Board::GetCols() const {
    return static_cast<int>(board[0].size());
}

// Mutators

int Board::SetMinesRemaining(int mines) {
    minesRemaining = mines;
}

// All tiles will be at 0,0 at this point, missing textures
void Board::AddRow(int length) {
    vector<Tile> row;
    for (int i = 0; i < length; i++) {
        Tile tile;
        row.push_back(tile);
    }
    board.push_back(row);
}

// Move all tiles from (0,0) to their correct locations
// At this point tiles may still be blank and not pointing to each other
void Board::Arrange() {
    for (int i = 0; i < board.size(); i++) { // For every row
        for (int j = 0; j < board[0].size(); j++) { // For every tile in the row
            board[i][j].SetPosition(Vector2f(32.0f * float(j), 32.0f * float(i)));
        }
    }
}

void Board::Clear() {
    board.clear();
}

// Give a single texture to all tiles
void Board::TexturizeAll(string textureName) {
    for (int i = 0; i < board.size(); i++) { // For every row
        for (int j = 0; j < board[0].size(); j++) { // For every tile in the row
            board[i][j].SetTexture(textureName);
        }
    }
}




/* Searches board for tile with target x/y location,
 returns pair of row/col indices if found, -1 in both indices otherwise */
pair<int, int> Board::Search(const Vector2f& target) {
    pair<int, int> indexPair;
    
    // Convert coordinates to tile index
    int col = floor(target.x / 32.0f);
    int row = floor(target.y / 32.0f);
    
    // Check if "row" and "col" are within domain
    if (row < board.size() && row >= 0 &&
        col < board[0].size() && col >= 0) {
        return {row, col};
    }
    else{
        return {-1, -1};
    }
//    for (int i = 0; i < board.size(); i++) { // For all rows
//        for (int j = 0; j < board[0].size(); j++) { // For each tile in the row
//            if (board[i][j].GetBounds().contains(target.x, target.y)) { // Tile found
//                indexPair.first = i;
//                indexPair.second = j;
//                return indexPair;
//            }
//        }
//    }
//    indexPair.first = -1;
//    indexPair.second = -1;
//    return indexPair;
}

/* Used with Search to get a reference to the tile at
(rowindex, colindex) in the 2D board vector */
Tile& Board::Retrieve(pair<int, int> p) {
    if (p.first == -1 || p.second == -1) {
        throw runtime_error("Retrieve error: Tile index -1 (does not exist).");
    }
    return board[p.first][p.second];
}




void Board::LinkAll() {
    for (int i = 0; i < board.size(); i++) { // For every row
        for (int j = 0; j < board[0].size(); j++) { // For every tile in the row
            if (i-1 >= 0 && j-1 >= 0) {
                board[i][j].Link(&board[i-1][j-1]);
            }
            if (i-1 >= 0) {
                board[i][j].Link(&board[i-1][j]);
            }
            if (i-1 >= 0 && j+1 <= (board[0].size()-1)) {
                board[i][j].Link(&board[i-1][j+1]);
            }
            if (j-1 >= 0) {
                board[i][j].Link(&board[i][j-1]);
            }
            if (j+1 <= (board[0].size()-1)) {
                board[i][j].Link(&board[i][j+1]);
            }
            if (i+1 <= (board.size()-1) && j-1 >= 0) {
                board[i][j].Link(&board[i+1][j-1]);
            }
            if (i+1 <= (board.size()-1)) {
                board[i][j].Link(&board[i+1][j]);
            }
            if (i+1 <= (board.size()-1) && j+1 <= (board[0].size()-1)) {
                board[i][j].Link(&board[i+1][j+1]);
            }
        }
    }
}

void Board::SetRandomMines(int count) {
    
    int totalTiles = static_cast<int>(board.size() * board[0].size());
    
    if (board.size() == 0) {
        cout << "Cannot SetRandomMines on an empty board." << endl;
        return;
    }
    if (count <= 0 || count > totalTiles) {
        cout << "Invalid mine count for SetRandomMines." << endl;
        return;
    }
    
    int validMineCount = 0;
    map<int, int> mineMap;
    
    while (validMineCount < count) { // While mines are still to be made
        int random = RandomGenerator::Int(0, totalTiles - 1);
        /* Any tile on board, totalTiles - 1 because random will be used to determine indices of mines with 0 indexing (prevents off by 1) */
        // Mine does not yet exist
        if (mineMap.find(random) == mineMap.end()) {
            
            int row = int(random / board[0].size());
            int col = random % board[0].size();

            board[row][col].ToggleMine();
            validMineCount++;
            mineMap[random]; // No more mines at this location
            
        }
        // Mine already exists at that location. Generate new random
        else {
            continue;
        }
    }
}

void Board::RevealAllMines() {
    for (int i = 0; i < board.size(); i++) { // For every row
        for (int j = 0; j < board[0].size(); j++) { // For every tile in the row
            if (board[i][j].GetIsMine()) {
                board[i][j].Reveal(true);
            }
        }
    }
}

// Switch static board indicator and perform the right action
void Board::Debug() {
    Board::ToggleDebug();
    // Debugging: ON
    if (Board::GetDebugStatus()) {
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[0].size(); j++) {
                if (board[i][j].GetMineSprite() == nullptr &&
                    board[i][j].GetIsMine()) {
                    board[i][j].Mine();
                }
            }
        }
    }
    // Debugging: OFF
    else {
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[0].size(); j++) {
                if (board[i][j].GetMineSprite() != nullptr &&
                    board[i][j].GetIsMine()) {
                    board[i][j].UnMine();
                }
            }
        }
    }
}

bool Board::CheckWin() {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            if (!board[i][j].GetIsRevealed()) {
                if (board[i][j].GetIsMine()) {
                    continue;
                }
                return false;
            }
        }
    }
    return true;
}

// Returns how many nonflagged mines were flagged as a result of the function
int Board::WinFlags() {
    int count = 0;
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            if (board[i][j].GetIsMine())
            {
                if (!board[i][j].GetIsFlagged()) {
                    board[i][j].Flag();
                    count++;
                }
            }
        }
    }
    return count;
}

// Utilize Draw() function of all tiles
void Board::Draw(RenderWindow& window) {
    for (int i = 0; i < board.size(); i++) { // For every row
        for (int j = 0; j < board[0].size(); j++) { // For every tile in the row
            board[i][j].Draw(window);
        }
    }
}


// Debug functions


// Checking if links to adjacent tiles were made
void Board::PrintAllLinks() {
    for (int i = 0; i < board.size(); i++) { // For every row
        for (int j = 0; j < board[0].size(); j++) { // For every tile in the row
            cout << "Tile (" + to_string(i) + ", " + to_string(j) + ") has ";
            cout << board[i][j].GetLinks().size() << " links." << endl;
        }
    }
}

void Board::Print() {
    cout << "Board width is: " << board[0].size() << endl;
    cout << "Board height is: " << board.size() << endl;
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            if (board[i][j].GetIsMine()) {
                cout << "1";
            }
            else {
                cout << "0";
            }
        }
        cout << endl;
    }
}

// Make all tiles mines for mine testing
void Board::ToggleAllMines() {
    for (int i = 0; i < board.size(); i++) { // For every row
        for (int j = 0; j < board[0].size(); j++) { // For every tile in the row
            board[i][j].ToggleMine();
        }
    }
}
