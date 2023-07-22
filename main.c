#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "Header.h"

int main(void) {
    int choice = 0;
    int menuFlag = 1;

    char* gamesFile = "games.bin";

    createFile(gamesFile);

    while (choice != 10) {
        choice = showMenu(gamesFile, &menuFlag);
    }

    printf("\nExiting the program.\n");

    return 0;
}
