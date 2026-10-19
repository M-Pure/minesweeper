#include <ctime>
#include <random>
#include "RandomGenerator.h"
using namespace std;

mt19937 RandomGenerator::random(time(0));

int RandomGenerator::Int(int min, int max) {
    uniform_int_distribution<int> dist(min, max);
    return dist(random);
}

