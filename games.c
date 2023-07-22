#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Header.h"

static int gameCount = 0;

void createFile(const char* filename) {
    FILE* fp = fopen(filename, "rb");

    if (fp == NULL) {
        fp = fopen(filename, "wb");

        if (strcmp(filename, "games.bin") == 0) {
            fwrite(&gameCount, sizeof(int), 1, fp);
        }

        fclose(fp);
    }
    else {
        fclose(fp);
    }
}

int showMenu(const char* filename) {
    Game* gameArray = (Game*)loadGames(filename);

    int choice = 0;
    int exitFlag = 0;

    do {
        system("cls");

        printf("         ~ GAME DATABASE ~          \n");
        printf("1. Add a new game\n");
        printf("2. Display all games\n");
        printf("3. Search game by ID\n");
        printf("4. Search game by title\n");
        printf("5. Sort games by price\n");
        printf("6. Update a game\n");
        printf("7. Delete a game\n");
        printf("8. Change filename\n");
        printf("9. Delete file\n");
        printf("10. Exit the program\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            addGame(filename, &gameArray);
            break;
        case 2:
            displayAllGames(gameArray);
            break;
        case 3:
            if (gameArray != NULL) {
                searchGameById(gameArray);
            }
            else {
                printf("No games in the database.\n");
            }
            break;
        case 4:
            if (gameArray != NULL) {
                searchGameByTitle(gameArray);
            }
            else {
                printf("No games in the database.\n");
            }
            break;
        case 5:
            if (gameArray != NULL) {
                sortGamesByPrice(gameArray);
            }
            else {
                printf("No games in the database.\n");
            }
            break;
        case 6:
            if (gameArray != NULL) {
                updateGame(gameArray, filename);
            }
            else {
                printf("No games in the database.\n");
            }
            break;
        case 7:
            if (gameArray != NULL) {
                deleteGame(gameArray, filename);
            }
            else {
                printf("No games in the database.\n");
            }
            break;
        case 8:
            changeFileName(filename);
            break;
        case 9:
            deleteFile(filename);
            exitFlag = 1;
            break;
        case 10:
            exitFlag = 1;
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }

        printf("Press Enter to continue...");
        getchar();
        getchar();

    } while (!exitFlag);

    if (exitFlag) {
        exitProgram(gameArray);
        return 10;
    }

    return choice;
}

void addGame(const char* filename, Game** gameArrayPtr) {
    system("cls");

    FILE* fp = fopen(filename, "rb+");
    if (fp == NULL) {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }

    fread(&gameCount, sizeof(int), 1, fp);

    Game newGame = { 0 };

    newGame.id = gameCount + 1;
    getchar();

    printf("Enter the title of the game: ");
    fgets(newGame.title, sizeof(newGame.title), stdin);
    newGame.title[strcspn(newGame.title, "\n")] = '\0';

    printf("Enter the platform of the game: ");
    fgets(newGame.platform, sizeof(newGame.platform), stdin);
    newGame.platform[strcspn(newGame.platform, "\n")] = '\0';

    printf("Enter the price of the game: ");
    scanf("%f", &newGame.price);

    fseek(fp, 0, SEEK_END);

    fwrite(&newGame, sizeof(Game), 1, fp);

    gameCount++;
    fseek(fp, 0, SEEK_SET);
    fwrite(&gameCount, sizeof(int), 1, fp);

    fclose(fp);

    printf("New game added.\n");

    *gameArrayPtr = (Game*)loadGames(filename);
}







void* loadGames(const char* filename) {
    FILE* fp = fopen(filename, "rb");

    if (fp == NULL) {
        perror("Error opening the file");
        return NULL;
        exit(EXIT_FAILURE);
    }

    fread(&gameCount, sizeof(int), 1, fp);

    Game* gameArray = (Game*)calloc(gameCount, sizeof(Game));

    if (gameArray == NULL) {
        printf("Memory allocation failed");
        return NULL;
        exit(EXIT_FAILURE);
    }

    fread(gameArray, sizeof(Game), gameCount, fp);

    fclose(fp);

    return gameArray;
}



void displayAllGames(const Game* gameArray) {
    system("cls");

    if (gameArray == NULL) {
        printf("No games in the database.\n");
        return;
    }

    for (int i = 0; i < gameCount; i++) {
        printf("ID: %d\n", (gameArray + i)->id);
        printf("Title: %s\n", (gameArray + i)->title);
        printf("Platform: %s\n", (gameArray + i)->platform);
        printf("Price: $%.2f\n\n", (gameArray + i)->price);
    }
}


void searchGameById(const Game* gameArray) {
    system("cls");

    if (gameCount == 0) {
        printf("No games in the database.\n");
        return;
    }

    int searchId;

    printf("Enter the ID of the game to search for: ");
    scanf("%d", &searchId);

    int found = 0;

    for (int i = 0; i < gameCount; i++) {
        if (searchId == (gameArray + i)->id) {
            printf("ID: %d\n", (gameArray + i)->id);
            printf("Title: %s\n", (gameArray + i)->title);
            printf("Platform: %s\n", (gameArray + i)->platform);
            printf("Price: $%.2f\n\n", (gameArray + i)->price);
            found = 1;
        }
    }

    if (!found) {
        printf("No game found with the provided ID.\n");
    }
}

void searchGameByTitle(const Game* gameArray) {
    system("cls");

    if (gameCount == 0) {
        printf("No games in the database.\n");
        return;
    }

    char searchTitle[30];

    printf("Enter the title of the game to search for: ");
    getchar();
    scanf("%29[^\n]", searchTitle);

    int found = 0;

    for (int i = 0; i < gameCount; i++) {
        if (strcmp(searchTitle, (gameArray + i)->title) == 0) {
            printf("ID: %d\n", (gameArray + i)->id);
            printf("Title: %s\n", (gameArray + i)->title);
            printf("Platform: %s\n", (gameArray + i)->platform);
            printf("Price: $%.2f\n\n", (gameArray + i)->price);
            found = 1;
        }
    }

    if (!found) {
        printf("No game found with the provided title.\n");
    }
}

void swapGames(Game* game1, Game* game2) {
    Game temp = { 0 };

    temp = *game1;
    *game1 = *game2;
    *game2 = temp;
}

void sortGamesByPrice(Game* gameArray) {
    system("cls");

    if (gameCount == 0) {
        printf("No games in the database.\n");
        return;
    }

    for (int i = 0; i < gameCount - 1; i++) {
        for (int j = i + 1; j < gameCount; j++) {
            if ((gameArray + j)->price < (gameArray + i)->price) {
                swapGames((gameArray + i), (gameArray + j));
            }
        }
    }

    displayAllGames(gameArray);
}

void updateGame(Game* gameArray, const char* filename) {
    system("cls");

    if (gameCount == 0) {
        printf("No games in the database.\n");
        return;
    }

    FILE* fp = fopen(filename, "rb+");

    if (fp == NULL) {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }

    fseek(fp, sizeof(int), SEEK_CUR);

    int searchId;

    printf("Enter the ID of the game to update: ");
    scanf("%d", &searchId);

    int found = 0;

    for (int i = 0; i < gameCount; i++) {
        if (searchId == (gameArray + i)->id) {
            printf("Game found.\n");

            Game updatedGame = { 0 };

            updatedGame.id = searchId;
            getchar();

            printf("Enter the new title (current: %s): ", (gameArray + i)->title);
            scanf("%29[^\n]", updatedGame.title);
            getchar();

            printf("Enter the new platform (current: %s): ", (gameArray + i)->platform);
            scanf("%19[^\n]", updatedGame.platform);
            getchar();

            printf("Enter the new price (current: %.2f): ", (gameArray + i)->price);
            scanf("%f", &updatedGame.price);

            fseek(fp, sizeof(Game) * i, SEEK_CUR);
            fwrite(&updatedGame, sizeof(Game), 1, fp);

            printf("Game updated.\n");

            found = 1;
            break;
        }
    }

    if (!found) {
        printf("No game found with the provided ID.\n");
    }

    fclose(fp);
}

void deleteGame(Game* gameArray, const char* filename) {
    system("cls");

    if (gameCount == 0) {
        printf("No games in the database.\n");
        return;
    }

    FILE* fp = fopen(filename, "rb+");
    if (fp == NULL) {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }

    fseek(fp, sizeof(int), SEEK_SET);

    int searchId;

    printf("Enter the ID of the game to delete (0 to cancel): ");
    scanf("%d", &searchId);

    if (searchId == 0) {
        fclose(fp);
        printf("Delete operation canceled.\n");
        return;
    }

    int found = 0;

    int counter = 0;

    for (int i = 0; i < gameCount; i++) {
        if (searchId == (gameArray + i)->id) {
            found = 1;
            continue;
        }

        fwrite((gameArray + i), sizeof(Game), 1, fp);
        counter++;
    }

    if (!found) {
        printf("No game found with the provided ID.\n");
    }
    else {
        gameCount = counter;
        printf("Game deleted.\n");
    }

    fseek(fp, 0, SEEK_SET);
    fwrite(&gameCount, sizeof(int), 1, fp);

    fclose(fp);

    free(gameArray);
    gameArray = (Game*)loadGames(filename);
}






void changeFileName(const char* oldName) {
    char newName[20];

    printf("Enter the new filename: ");
    getchar();
    scanf("%19[^\n]", newName);

    printf("Are you sure you want to change the filename?\n");
    printf("Type \"yes\" to confirm or \"no\" to cancel.\n");
    char confirmation[3];
    scanf("%2s", confirmation);

    if (strcmp("yes", confirmation) == 0) {
        if (rename(oldName, newName) == 0) {
            printf("Filename changed successfully.\n");
        }
        else {
            printf("Failed to change the filename.\n");
        }
    }
}

void deleteFile(const char* filename) {
    printf("Are you sure you want to delete the file %s?\n", filename);
    printf("Type \"yes\" to confirm or \"no\" to cancel.\n");
    char confirmation[3];
    scanf("%2s", confirmation);

    if (strcmp("yes", confirmation) == 0) {
        if (remove(filename) == 0) {
            printf("File %s deleted successfully.\n", filename);
        }
        else {
            printf("Failed to delete the file %s.\n", filename);
        }
    }
}

int exitProgram(Game* gameArray) {
    free(gameArray);
    return 10;
}

