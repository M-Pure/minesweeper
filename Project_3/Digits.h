#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
using namespace std;
using namespace sf;

class Digits {
    
    int number;
    Sprite hundreds;
    Sprite tens;
    Sprite ones;
    Sprite* negativeExtra;
    
public:
    Digits(Board boardObject);
    
    ~Digits();
    
    void SetCount(int num);
    
    void Subtract(int num);
    
    void Draw(RenderWindow& window);
    
    void operator++(int);
    
    void operator--(int);

};

