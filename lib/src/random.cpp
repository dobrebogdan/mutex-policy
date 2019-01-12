#include <cstdlib>
#include <ctime>

#include "random.hpp"

namespace mtxpol {

static bool sranded = false;

int getRandomInteger() {
    if (!sranded) {
        srand(time(0));
    }
    return rand() + 1;
}

}
