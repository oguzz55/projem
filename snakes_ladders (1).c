#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Fonksiyonlar
void fillBoard(int **board, int size);
void printBoard(int **board, int size, int heroPosition1, int heroPosition2, int **snakes, int numSnakes, int **ladders, int numLadders);
int getInput();
void determineSnakesAndLadders(int size, int *numSnakes, int *numLadders);
void freeBoard(int **board, int size, int **snakes, int **ladders);
void fillSnakesAndLadders(int **snakes, int numSnakes, int **ladders, int numLadders, int size, int *filledCoordinates);
int isFilled(int filledCoordinates[], int filledCount, int number);
void adjustSnakesAndLadders(int **snakes, int numSnakes, int **ladders, int numLadders);
void swap(int *a, int *b);
void printCell(int cellNumber, int heroPosition1, int heroPosition2, int **snakes, int numSnakes, int **ladders, int numLadders);
int rollDice();
void moveHero(int *heroPosition, int diceRoll, int **snakes, int numSnakes, int **ladders, int numLadders, int size);

int main() {
    // Kullanicidan girdi alma
    int size = getInput();

    // Rastgele deger icin seed olusturma
    srand(time(NULL));

    // Yilan ve merdiven sayisini belirle
    int numSnakes, numLadders;
    determineSnakesAndLadders(size, &numSnakes, &numLadders);

    // Yilan ve merdiven arraylerini olustur
    int **snakes = (int **)malloc(numSnakes * sizeof(int *));
    int **ladders = (int **)malloc(numLadders * sizeof(int *));
    for (int i = 0; i < numSnakes; i++) {
        snakes[i] = (int *)malloc(2 * sizeof(int)); // Each snake has a start and end point
    }
    for (int i = 0; i < numLadders; i++) {
        ladders[i] = (int *)malloc(2 * sizeof(int)); // Each ladder has a start and end point
    }

    // Oyun alani arrayini olustur
    int **board = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        board[i] = (int *)malloc(size * sizeof(int));
    }

    // Oyun alanini doldur
    fillBoard(board, size);

    int filledCoordinates[size * 4 + 2];
    filledCoordinates[0] = 1; // baslangic
    filledCoordinates[1] = size * size; // bitis
    int filledCount = 2;

    // Yilanlar ve merdivenleri doldur
    fillSnakesAndLadders(snakes, numSnakes, ladders, numLadders, size, filledCoordinates);
    // Yilanlar ve merdivenleri duzenle
    adjustSnakesAndLadders(snakes, numSnakes, ladders, numLadders);

    // Yilanlarin ve merdivenlerin poziyonunu bastir
    printSnakesAndLadders(snakes, numSnakes, ladders, numLadders);

    int heroPosition1 = 1; // Oyuncu 1'in pozisyonu
    int heroPosition2 = 1; // Oyuncu 2'nin pozisyonu

    // Oyun alanini bastir
    printBoard(board, size, heroPosition1, heroPosition2, snakes, numSnakes, ladders, numLadders);

    // Enter komutu bekle
    getchar();
    while (heroPosition1 < size * size && heroPosition2 < size * size) {

        // Oyuncu 1'in turu
        printf("Player 1, press Enter to roll the dice...\n");
        getchar();

        // Zar at
        int diceRoll = rollDice();
        printf("Player 1 rolled: %d\n", diceRoll);
        moveHero(&heroPosition1, diceRoll, snakes, numSnakes, ladders, numLadders, size);
        printf("Player 1 is now at position: %d\n", heroPosition1);
        printBoard(board, size, heroPosition1, heroPosition2, snakes, numSnakes, ladders, numLadders);

        // Oyuncu 1 kazandi mi
        if (heroPosition1 >= size * size) {
            printf("Congratulations! Player 1 has reached the end!\n");
            break;
        }

        // Oyuncu 2'in turu
        printf("Player 2, press Enter to roll the dice...\n");
        getchar();

        // Zar at
        diceRoll = rollDice();
        printf("Player 2 rolled: %d\n", diceRoll);
        moveHero(&heroPosition2, diceRoll, snakes, numSnakes, ladders, numLadders, size);
        printf("Player 2 is now at position: %d\n", heroPosition2);
        printBoard(board, size, heroPosition1, heroPosition2, snakes, numSnakes, ladders, numLadders);

        // Oyuncu 2 kazandi mi
        if (heroPosition2 >= size * size) {
            printf("Congratulations! Player 2 has reached the end!\n");
            break;
        }
    }


    // Arrayleri serbest birakir
    freeBoard(board, size, snakes, ladders);

    return 0;
}

int rollDice() {
    return rand() % 6 + 1; // 1-6 arasi rastgele deger olusturur
}

// Array icindeki elemanlarin yerlerini degistirir
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

//Oyuncunun yerini gunceller
void moveHero(int *heroPosition, int diceRoll, int **snakes, int numSnakes, int **ladders, int numLadders, int size) {
    *heroPosition += diceRoll;

    // Yilanlari ve merdivenleri kontrol eder
    for (int i = 0; i < numSnakes; i++) {
        if (*heroPosition == snakes[i][0]) {
            *heroPosition = snakes[i][1];
            printf("Player has landed on the snake %d\n", i+1);
            return;
        }
    }
    for (int i = 0; i < numLadders; i++) {
        if (*heroPosition == ladders[i][0]) {
            *heroPosition = ladders[i][1];
                        printf("Player has landed on the ladder %d\n", i+1);
            return;
        }
    }

    // Oyuncu oyun alaninin disarisina ciktiysa oyuncuyu son kareye yerlestir
    int maxPosition = size * size;
    if (*heroPosition > maxPosition) {
        *heroPosition = maxPosition;
    }
}

// Yilanlari ve merdivenleri duzenler
void adjustSnakesAndLadders(int **snakes, int numSnakes, int **ladders, int numLadders) {

    for (int i = 0; i < numSnakes; i++) {
        if (snakes[i][0] < snakes[i][1]) {
            swap(&snakes[i][0], &snakes[i][1]);
        }
    }

    for (int i = 0; i < numLadders; i++) {
        if (ladders[i][0] > ladders[i][1]) {
            swap(&ladders[i][0], &ladders[i][1]);
        }
    }
}

// Yilanlari ve merdivenleri ekrana bastirir
void printSnakesAndLadders(int **snakes, int numSnakes, int **ladders, int numLadders) {
    printf("Snakes:\n");
    for (int i = 0; i < numSnakes; i++) {
        printf("Snake %d: Start %d, End %d\n", i + 1, snakes[i][0], snakes[i][1]);
    }

    printf("\nLadders:\n");
    for (int i = 0; i < numLadders; i++) {
        printf("Ladder %d: Start %d, End %d\n", i + 1, ladders[i][0], ladders[i][1]);
    }
    printf("\n");
}

// Yilanlari ve merdivenleri olusturur
void fillSnakesAndLadders(int **snakes, int numSnakes, int **ladders, int numLadders, int size, int *filledCoordinates) {
    int filledCount = 2;

    // Yilanlar
    for (int i = 0; i < numSnakes; i++) {
        int start, end;
        do {
            start = rand() % (size * size - 1) + 1;
        } while (isFilled(filledCoordinates, filledCount, start));
        filledCoordinates[filledCount++] = start;

        do {
            end = rand() % (size * size - 1) + 1;
        } while (isFilled(filledCoordinates, filledCount, end) || abs(end - start) <= size);
        filledCoordinates[filledCount++] = end;

        snakes[i][0] = start;
        snakes[i][1] = end;
    }

    // Merdivenler
    for (int i = 0; i < numLadders; i++) {
        int start, end;
        do {
            start = rand() % (size * size - 1) + 1;
        } while (isFilled(filledCoordinates, filledCount, start));
        filledCoordinates[filledCount++] = start;

        do {
            end = rand() % (size * size - 1) + 1;
        } while (isFilled(filledCoordinates, filledCount, end) || abs(end - start) <= size);
        filledCoordinates[filledCount++] = end;

        ladders[i][0] = start;
        ladders[i][1] = end;
    }
}

// Yilanlarin ve merdivenlerin ayni karede olup olmadigini check eder
int isFilled(int filledCoordinates[], int filledCount, int number) {
    for (int i = 0; i < filledCount; i++) {
        if (filledCoordinates[i] == number) {
            return 1;
        }
    }
    return 0;
}

// Yilanlarin merdivenlerin sayisini belirler
void determineSnakesAndLadders(int size, int *numSnakes, int *numLadders) {
    if (size % 2 == 0) {
        *numSnakes = *numLadders = size / 2;
    } else {
        *numSnakes = size / 2 + 1;
        *numLadders = size / 2;
    }
}

// Oyun alanini doldurur
void fillBoard(int **board, int size) {
    int count = 1, i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            board[i][j] = count++;
        }
    }
}

// Oyun alanini bastirir
void printBoard(int **board, int size, int heroPosition1, int heroPosition2, int **snakes, int numSnakes, int **ladders, int numLadders) {
    for (int i = 0; i < size; i++) {
        if ((size - i) % 2 == 1) {  // Asagidan yukariya zig zag seklinde bastirim
            for (int j = 0; j < size; j++) {
                int cellNumber = board[size - 1 - i][j];
                printCell(cellNumber, heroPosition1, heroPosition2, snakes, numSnakes, ladders, numLadders);
            }
        } else {  // Asagidan yukariya zig zag seklinde bastirim
            for (int j = size - 1; j >= 0; j--) {
                int cellNumber = board[size - 1 - i][j];
                printCell(cellNumber, heroPosition1, heroPosition2, snakes, numSnakes, ladders, numLadders);
            }
        }
        printf("\n");
    }
}

// Bir oyun karesini ekrana bastirir
void printCell(int cellNumber, int heroPosition1, int heroPosition2, int **snakes, int numSnakes, int **ladders, int numLadders) {
    if (cellNumber == heroPosition1 && cellNumber == heroPosition2) {
        printf("\033[1;35mP1&P2\033[0m"); // Iki oyuncuda ayni karede, mor
        return;
    } else if (cellNumber == heroPosition1) {
        printf("\033[1;94m  P1 \033[0m"); // Oyuncu 1, mavi
        return;
    } else if (cellNumber == heroPosition2) {
        printf("\033[1;96m  P2 \033[0m"); // Oyuncu 2, cyan
        return;
    }

    // Yilanlar, kirmizi
    for (int k = 0; k < numSnakes; k++) {
        if (snakes[k][0] == cellNumber) {
            printf("\033[1;31m S%dS \033[0m", k + 1);
            return;
        } else if (snakes[k][1] == cellNumber) {
            printf("\033[1;31m S%dE \033[0m", k + 1);
            return;
        }
    }

    // Merdivenler, yesil
    for (int k = 0; k < numLadders; k++) {
        if (ladders[k][0] == cellNumber) {
            printf("\033[1;32m L%dS \033[0m", k + 1);
            return;
        } else if (ladders[k][1] == cellNumber) {
            printf("\033[1;32m L%dE \033[0m", k + 1);
            return;
        }
    }

    // Normal, beyaz
    printf("%4d ", cellNumber);
}


// Kullanicidan girdi al
int getInput() {
    float tempSize;
    int size, result;
    while (1) {
        printf("Enter the size of the board (minimum 4): ");
        result = scanf("%f", &tempSize);
        if (result == 1 && (size = round(tempSize)) >= 4) {
            return size;
        } else {
            printf("Invalid input. Please enter a number greater or equal to 4.\n");
            while (getchar() != '\n');
        }
    }
}

// Arrayleri serbest birak
void freeBoard(int **board, int size, int **snakes, int **ladders) {
    for (int i = 0; i < size; i++) {
        free(board[i]);
    }
    free(board);

    for (int i = 0; i < size / 2 + 1; i++) {
        free(snakes[i]);
    }
    free(snakes);

    for (int i = 0; i < size / 2; i++) {
        free(ladders[i]);
    }
    free(ladders);
}
