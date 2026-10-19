#pragma once
#include <random>
using namespace std;


class RandomGenerator {
    static mt19937 random;
public:
    static int Int(int min, int max);
};

