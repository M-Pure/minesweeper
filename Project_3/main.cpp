// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include "Board.h"
#include "Digits.h"
#include "TextureManager.h"
#include "Tile.h"
using namespace sf;
using namespace std;


void LoadConfig(vector<int>& output);

void LoadBoardTemplate(string filename, vector<vector<int>>& output);

void Print2D(vector<vector<int>>& v);


int main(int, char const**)
{
    // Load window/board information from config file
    vector<int> configDetails;
    LoadConfig(configDetails);
    enum Config {
        COLUMNS,
        ROWS,
        MINES
    };

    // Create the main window
    RenderWindow window(VideoMode(configDetails[COLUMNS] * 32, (configDetails[ROWS] * 32) + 100), "Minesweeper");

    
    vector<vector<int>> test1Template;
    LoadBoardTemplate("testboard1", test1Template);
    
    vector<vector<int>> test2Template;
    LoadBoardTemplate("testboard2", test2Template);
    
    vector<vector<int>> test3Template;
    LoadBoardTemplate("testboard3", test3Template);


    // Switched to true when user presses esc or X
    bool terminated = false;
    /* Flags to indicate the current gamemode and influence board creation.
     Others switched off when 1 is switched on */
    bool test1Mode = false;
    bool test2Mode = false;
    bool test3Mode = false;
    bool randomMode = true;
    
    
//    // Start the game loop - this should never stop unless esc or press X
    while (window.isOpen())
    {
        
        // Initialize template board
        Board board;
        if (randomMode) {
            board.Initialize(configDetails[COLUMNS], configDetails[ROWS], configDetails[MINES]);
        }
        else if (test1Mode) {
            board.Initialize(test1Template);
        }
        else if (test2Mode) {
            board.Initialize(test2Template);
        }
        else if (test3Mode) {
            board.Initialize(test3Template);
        }
        
        
        
        Digits digits(board);
        
        Sprite smiley;
        smiley.setTexture(TextureManager::GetTexture("face_happy"));
        smiley.setPosition((board.GetCols() / 2 - 1) * 32, board.GetRows() * 32);
        
        Sprite debug;
        debug.setTexture(TextureManager::GetTexture("debug"));
        // 4 spaces apart from smiley
        debug.setPosition(smiley.getPosition().x + (32.0f*4), board.GetRows() * 32);
        
        Sprite test1;
        test1.setTexture(TextureManager::GetTexture("test_1"));
        // 2 spaces from debug
        test1.setPosition(debug.getPosition().x + (32.0f * 2), board.GetRows() * 32);
        
        Sprite test2;
        test2.setTexture(TextureManager::GetTexture("test_2"));
        // 2 spaces from test1
        test2.setPosition(test1.getPosition().x + (32.0f * 2), board.GetRows() * 32);
        
        Sprite test3;
        // 2 spaces from test2
        test3.setTexture(TextureManager::GetTexture("test_3"));
        test3.setPosition(test2.getPosition().x + (32.0f * 2), board.GetRows() * 32);
        
        if (Board::GetDebugStatus()) {
            Board::ToggleDebug();
            board.Debug();
        }
        
        
        
        bool resetBoard = false;
        bool hasWon = false;
        
        // GAME ON - Full functionality
        while (!Board::GetGameOverIndicator() && !terminated)
        {
            bool restart = false;
            // Process events
            Event event;
            while (window.pollEvent(event))
            {
                // Click
                if (event.type == Event::MouseButtonPressed)
                {
                    // Left Click
                    if (event.mouseButton.button == Mouse::Left)
                    {
                        
                        Vector2i mousePos = Mouse::getPosition(window);
                        Vector2f worldPos = window.mapPixelToCoords(mousePos);
                        
                        
                        // User clicked on a valid TILE
                        pair<int, int> tilePair = board.Search(worldPos);
                        if (tilePair.first != -1 && tilePair.second != -1)
                        {
                            Tile& t = board.Retrieve(tilePair); // Tile from 2D vector
                            // Game over
                            if (t.GetIsMine() && !t.GetIsFlagged())
                            {
                                board.RevealAllMines();
                                smiley.setTexture(TextureManager::GetTexture("face_lose"));
                                Board::ToggleGameOver(); // Only "end" game if you lose
                            }
                            else {
                                // Reveal first, then check to see if that was the last hidden tile
                                t.Reveal(false);
                                // USER WINS
                                if (board.CheckWin()) {
                                    digits.Subtract(board.WinFlags());

                                    smiley.setTexture(TextureManager::GetTexture("face_win"));
                                    hasWon = true;
                                    restart = true;
                                    break;
                                }
                            }
                        }
                        // User clicked on smiley button
                        if (smiley.getGlobalBounds().contains(worldPos.x, worldPos.y)) {
                            restart = true;
                            resetBoard = true;
                            test1Mode = false;
                            test2Mode = false;
                            test3Mode = false;
                            randomMode = true;
                            break;
                        }
                        
                        // User clicked on the debug button
                        if (debug.getGlobalBounds().contains(worldPos.x, worldPos.y)) {
                            board.Debug();
                        }
                        
                        // User clicked on test1 button
                        if (test1.getGlobalBounds().contains(worldPos.x, worldPos.y)) {
                            restart = true;
                            resetBoard = true;
                            test1Mode = true;
                            test2Mode = false;
                            test3Mode = false;
                            randomMode = false;
                            break;
                        }
                        
                        // User clicked on test2 button
                        if (test2.getGlobalBounds().contains(worldPos.x, worldPos.y)) {
                            restart = true;
                            resetBoard = true;
                            test1Mode = false;
                            test2Mode = true;
                            test3Mode = false;
                            randomMode = false;
                            break;
                        }
                        
                        // User clicked on test3 button
                        if (test3.getGlobalBounds().contains(worldPos.x, worldPos.y)) {
                            restart = true;
                            resetBoard = true;
                            test1Mode = false;
                            test2Mode = false;
                            test3Mode = true;
                            randomMode = false;
                            break;
                        }
                        
                    }
                    // Right Click
                    if (event.mouseButton.button == Mouse::Right)
                    {
                        Vector2i mousePos = Mouse::getPosition(window);
                        Vector2f worldPos = window.mapPixelToCoords(mousePos);
                        
                        pair<int, int> tilePair = board.Search(worldPos);
                        
                        // User clicked on a valid tile
                        if (tilePair.first != -1 && tilePair.second != -1)
                        {
                            Tile& t = board.Retrieve(tilePair); // Reference to retrieved tile from 2D board vector
                            if (t.GetIsFlagged())
                            {
                                t.UnFlag();
                                digits++;
                            }
                            else
                            {
                                if(t.Flag()) { digits--; }
                            }
                        }
                    }
                }
                
                // Close window: exit
                if (event.type == sf::Event::Closed)
                {
                    terminated = true;
                    window.close();
                }

                // Escape pressed: exit
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                {
                    terminated = true;
                    window.close();
                }
                
                // Clear screen
                window.clear();
                
                // Draw all sprites
                board.Draw(window);
                digits.Draw(window);
                window.draw(smiley);
                window.draw(debug);
                window.draw(test1);
                window.draw(test2);
                window.draw(test3);

                // Update the window
                window.display();
            }
            
            if (restart) {
                break;
            }
            
        }
        if (resetBoard) {
            continue;
        }
        
        
        // GAME WON - Separate event loop with limited options
        if (hasWon) {
            // Update the screen with "win" conditions
            // Clear screen
            window.clear();
            
            // Draw all sprites
            board.Draw(window);
            digits.Draw(window);
            window.draw(smiley);
            window.draw(debug);
            window.draw(test1);
            window.draw(test2);
            window.draw(test3);

            // Update the window
            window.display();
        }
        while (hasWon && !terminated) {
            
            bool restart = false;
            // Process events
            Event event;
            while (window.pollEvent(event)) {
                
                // Click
                if (event.type == Event::MouseButtonPressed)
                {
                    // Left Click
                    if (event.mouseButton.button == Mouse::Left)
                    {
                        Vector2i mousePos = Mouse::getPosition(window);
                        Vector2f worldPos = window.mapPixelToCoords(mousePos);
                        
                        // !! Only modifies GameOver() from loss loop, not win loop !!
                        // Smiley
                        if (smiley.getGlobalBounds().contains(worldPos.x, worldPos.y))
                        {
                            restart = true;
                            test1Mode = false;
                            test2Mode = false;
                            test3Mode = false;
                            randomMode = true;
                            break;
                        }
                        // test1
                        if (test1.getGlobalBounds().contains(worldPos.x, worldPos.y))
                        {
                            restart = true;
                            test1Mode = true;
                            test2Mode = false;
                            test3Mode = false;
                            randomMode = false;
                            break;
                        }
                        // test2
                        if (test2.getGlobalBounds().contains(worldPos.x, worldPos.y))
                        {
                            restart = true;
                            test1Mode = false;
                            test2Mode = true;
                            test3Mode = false;
                            randomMode = false;
                            break;
                        }
                        // test3
                        if (test3.getGlobalBounds().contains(worldPos.x, worldPos.y))
                        {
                            restart = true;
                            test1Mode = false;
                            test2Mode = false;
                            test3Mode = true;
                            randomMode = false;
                            break;
                        }
                    }
                }
                
                
                // Close window: exit
                if (event.type == Event::Closed)
                {
                    terminated = true;
                    window.close();
                }

                // Escape pressed: exit
                if (event.type == Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                {
                    terminated = true;
                    window.close();
                }
            }
            
            if (restart) {
                break;
            }
        }
        if (hasWon) { // Prevent loop from entering GAME OVER state
            continue;
        }
        
        
        // GAME OVER - Separate event loop with limited options
        while (Board::GetGameOverIndicator && !terminated) {
            bool restart = false;
            // Process events
            Event event;
            while (window.pollEvent(event)) {
                
                // Click
                if (event.type == Event::MouseButtonPressed)
                {
                    // Left Click
                    if (event.mouseButton.button == Mouse::Left)
                    {
                        Vector2i mousePos = Mouse::getPosition(window);
                        Vector2f worldPos = window.mapPixelToCoords(mousePos);
                        
                        // !! Only modifies GameOver() from loss loop, not win loop !!
                        // Smiley
                        if (smiley.getGlobalBounds().contains(worldPos.x, worldPos.y))
                        {
                            Board::ToggleGameOver(); // Game is back on!
                            restart = true;
                            test1Mode = false;
                            test2Mode = false;
                            test3Mode = false;
                            randomMode = true;
                            break;
                        }
                        // test1
                        if (test1.getGlobalBounds().contains(worldPos.x, worldPos.y))
                        {
                            Board::ToggleGameOver(); // Game is back on!
                            restart = true;
                            test1Mode = true;
                            test2Mode = false;
                            test3Mode = false;
                            randomMode = false;
                            break;
                        }
                        // test2
                        if (test2.getGlobalBounds().contains(worldPos.x, worldPos.y))
                        {
                            Board::ToggleGameOver(); // Game is back on!
                            restart = true;
                            test1Mode = false;
                            test2Mode = true;
                            test3Mode = false;
                            randomMode = false;
                            break;
                        }
                        // test3
                        if (test3.getGlobalBounds().contains(worldPos.x, worldPos.y))
                        {
                            Board::ToggleGameOver(); // Game is back on!
                            restart = true;
                            test1Mode = false;
                            test2Mode = false;
                            test3Mode = true;
                            randomMode = false;
                            break;
                        }
                    }
                }
                
                
                // Close window: exit
                if (event.type == Event::Closed)
                {
                    terminated = true;
                    window.close();
                }

                // Escape pressed: exit
                if (event.type == Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                {
                    terminated = true;
                    window.close();
                }
            }
            
            if (restart) {
                break;
            }
        }
        // if (resetBoard) not needed because board is reset anyway (another loop starts)
        

    }
        

    TextureManager::Clear();
    return EXIT_SUCCESS;
}

void LoadConfig(vector<int>& output) {
    ifstream inFile("boards/config.cfg");
    string line;
    while (getline(inFile, line)) {
        output.push_back(stoi(line));
    }
    inFile.close();
}

void LoadBoardTemplate(string filename, vector<vector<int>>& output) {
    ifstream inFile("boards/" + filename + ".brd");
        string line;
        if (inFile.is_open()) {
            while (getline(inFile, line)) {
                vector<int> lineVector;
                while (!line.empty()) {
                    string someVal = line.substr(0, 1);
                    lineVector.push_back(stoi(someVal));
                    line.erase(0, 1);
                }
                output.push_back(lineVector);
            }
        }
        else {
            cout << "Failed to open file " << filename << endl;
        }
    
}

void Print2D(vector<vector<int>>& v) {
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[0].size(); j++) {
            cout << v[i][j];
        }
        cout << endl;
    }
}
