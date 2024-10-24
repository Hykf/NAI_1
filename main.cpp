/**
* @file
* #Autorzy: Mateusz Wróbel & Paweł Mechliński
* #Repo: https://github.com/Hykf/NAI_1
* #Zasady: https://docs.google.com/spreadsheets/d/1yJF8SJIqf_2GHLcLyx-9ye85514xuBA7_wA-1tirqL0/edit?gid=0#gid=0
* #Przygotowanie: [Opcjonalnie] jeśli gracz ma zaczynac rozgrywke to mona uruchomic program z flaga -p
*
*/
#include "GameManager.h"

int main( int argc, char *argv[] ){
    if(argv[1])
        GameManager game{true};
    else
        GameManager game{};
    return 0;
}
