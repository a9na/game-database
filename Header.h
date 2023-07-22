#pragma once

#ifndef HEADER_H
#define HEADER_H

typedef struct {
    int id;
    char title[30];
    char platform[20];
    float price;
} Game;


void createFile(const char* filename);
int showMenu(const char* filename);
void addGame(const char* filename);
void* loadGames(const char* filename);
void displayAllGames(const Game* gameArray);
void searchGameById(const Game* gameArray);
void searchGameByTitle(const Game* gameArray);
void swapGames(Game* game1, Game* game2);
void sortGamesByPrice(const Game* gameArray);
void updateGame(Game* gameArray, const char* filename);
void deleteGame(Game* gameArray, const char* filename);
void changeFileName(const char* oldName);
void deleteFile(const char* filename);
int exitProgram(Game* gameArray);

#endif // HEADER_H

