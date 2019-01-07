#include <random>

#include "random.hpp"

using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;

namespace mtxpol {

static random_device* device = nullptr;
static mt19937* seeder = nullptr;
static uniform_int_distribution<>* distribution = nullptr;

int getRandomInteger() {
    if (device == nullptr) {
        device = new random_device();
        seeder = new mt19937((*device)());
        distribution = new uniform_int_distribution<>();
    }
    return (*distribution)(*seeder);
}

}
