#include "GameManager.h"

int main( int argc, char *argv[] ){
    if(argv[1])
        GameManager game{true};
    else
        GameManager game{};
    return 0;
}
