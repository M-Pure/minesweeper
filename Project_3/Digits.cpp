#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "Board.h"
#include "Digits.h"
#include "TextureManager.h"

using namespace std;
using namespace sf;

Digits::Digits(Board boardObject) {
    hundreds.setTexture(TextureManager::GetTexture("digits"));
    tens.setTexture(TextureManager::GetTexture("digits"));
    ones.setTexture(TextureManager::GetTexture("digits"));
    
    // Vertical position depends on board size!
    auto board = boardObject.Get2DBoard();
    float yPos = board.size() * 32;
    
    hundreds.setPosition(0.0f, yPos); // Width 21 pixels
    tens.setPosition(21.0f, yPos);
    ones.setPosition(42.0f, yPos);
    
    negativeExtra = nullptr;
    
    SetCount(boardObject.GetMinesRemaining());
}

Digits::~Digits() {
    if (negativeExtra != nullptr) {
        delete negativeExtra;
        negativeExtra = nullptr;
    }
}

// Updates TextureRect to get a different number
void Digits::SetCount(int num) {
    if (num > 999) {
        cout << "Number too large for digit display!" << endl;
        return;
    }
    if (num < -999) {
        cout << "Number too small for digit display!" << endl;
        return;
    }
    
    number = num;
    
    if (num >= 0) {
        if (negativeExtra != nullptr) {
            delete negativeExtra; // Remove extra slot when positive
            negativeExtra = nullptr;
        }
        
        // 100s place
        int hundredsDigit = int(num/100);
        hundreds.setTextureRect(IntRect(0 + 21 * hundredsDigit, 0, 21, 32));
        
        // 10s place
        int tensDigit = int(num/10) % 10;
        tens.setTextureRect(IntRect(0 + 21 * tensDigit, 0, 21, 32));
        
        // 1s place
        int onesDigit = num % 10;
        ones.setTextureRect(IntRect(0 + 21 * onesDigit, 0, 21, 32));
    }
    else { // Negative numbers
        
        // Create new negativeExtra sprite
        // Add texture
        // Position right
        negativeExtra = new Sprite;
        negativeExtra->setTexture(TextureManager::GetTexture("digits"));
        auto position = ones.getPosition();
        position.x += 21.0f; // Shift one slot over from the 1s place
        negativeExtra->setPosition(position);
        
        int absNum = abs(num);
        IntRect negativeSign(21 * 10, 0, 21, 32);
        
        int hundredsDigit = int(absNum/100);
        int tensDigit = int(absNum/10) % 10;
        int onesDigit = absNum % 10;
        
        
        hundreds.setTextureRect(negativeSign);
        
        tens.setTextureRect(IntRect(0 + 21 * hundredsDigit, 0, 21, 32));
        
        ones.setTextureRect(IntRect(0 + 21 * tensDigit, 0, 21, 32));
        
        negativeExtra->setTextureRect(IntRect(0 + 21 * onesDigit, 0, 21, 32));
    }
    
}

void Digits::Subtract(int num) {
    SetCount(number - num);
}

void Digits::operator++(int) {
    SetCount(number + 1);
}

void Digits::operator--(int) {
    SetCount(number - 1);
}


void Digits::Draw(RenderWindow& window) {
    window.draw(hundreds);
    window.draw(tens);
    window.draw(ones);
    if (negativeExtra != nullptr) {
        window.draw(*negativeExtra);
    }
}
