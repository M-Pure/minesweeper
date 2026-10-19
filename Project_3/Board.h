#pragma once
#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>
#include "Tile.h"
using namespace sf;
using namespace std;

class Board {
    vector<vector<Tile>> board;
    static bool gameOverIndicator;
    static bool debugging;
    int minesRemaining;
    
public:
    static void ToggleGameOver();
    static void ToggleDebug();
    
    Board();
    
    // Uses initialize()
    Board(int xTiles, int yTiles, int mineCount);
    
    
    // Accessors
    const vector<vector<Tile>>& Get2DBoard() const;
    
    static bool GetGameOverIndicator();
    
    static bool GetDebugStatus();
    
    int GetMinesRemaining() const;
    
    int GetRows() const;
    
    int GetCols() const;
    
    
    // Mutators
    
    int SetMinesRemaining(int mines);
    
    
    
    // Board creates its own tiles 1 row at a time
    void AddRow(int length);
    
    // Moves Tile sprites to the correct location based on 2D Vector
    void Arrange();
    
    void Clear();
    
    // Give a single texture to all tiles
    void TexturizeAll(string textureName);
    
    // Handles all behind-the-scenes initialization of board objects with config info
    void Initialize(int columns, int rows, int mines);
    
    // Handles all behind-the-scenes initialization of board objects with 2D template vector
    void Initialize(const vector<vector<int>>& vectorMap);
    
    
    
    /* Searches board for tile with target x/y location.
     Returns pair (rowindex, colindex) if found, (-1, -1) otherwise */
    pair<int, int> Search(const Vector2f& target);
    
    /* Used with Search to get a reference to the tile at
    (rowindex, colindex) in the 2D board vector */
    Tile& Retrieve(pair<int, int> p);
    
    void LoadTemplate(const vector<vector<int>>& templateBoard);
    
    
    // Update adjacentTiles pointers within every tile of the board
    void LinkAll();
    
    // Use RandomGenerator class to initialize random mines on the board
    void SetRandomMines(int count);
    
//    void RevealAll();
    
    void RevealAllMines();
    
    void Debug();

    bool CheckWin();
    
    // Returns how many nonflagged mines were flagged as a result of the function
    int WinFlags();
    
    // Utilize Draw() function of all tiles
    void Draw(RenderWindow& window);
    
    // Debug functions
    
    // Checking if links to adjacent tiles were made
    void PrintAllLinks();
    
    // Prints 1s and 0s according to mines
    void Print();
    
    // Make all tiles mines for mine testing
    void ToggleAllMines();
};

