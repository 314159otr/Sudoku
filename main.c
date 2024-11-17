#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define SIZE 9
int grid[9][9] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 0, 8, 5},
    {0, 0, 1, 0, 2, 0, 0, 0, 0},
    {0, 0, 0, 5, 0, 7, 0, 0, 0},
    {0, 0, 4, 0, 0, 0, 1, 0, 0},
    {0, 9, 0, 0, 0, 0, 0, 0, 0},
    {5, 0, 0, 0, 0, 0, 0, 7, 3},
    {0, 0, 2, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 4, 0, 0, 0, 9}
};
int errors[SIZE][SIZE] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0}
};
typedef struct {
    int x;
    int y;
} Coordinate;
Coordinate cursor = {0, 0};

typedef enum {
    KEY_UP = 72,
    KEY_LEFT = 75,
    KEY_RIGHT = 77,
    KEY_DOWN = 80,
} Keys;

typedef enum {
    BLACK = 0,
    BLUE,
    GREEN,
    AQUA,
    RED,
    PURPLE,
    YELLOW,
    LIGHT_GRAY,
    DARK_GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_AQUA,
    LIGHT_RED,
    LIGHT_PURPLE,
    LIGHT_YELLOW,
    WHITE
} Color;

typedef struct {
    Color background;
    Color text;
} CellColors;

void setColor(CellColors cellColors) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // Combine text and background colors
    SetConsoleTextAttribute(hConsole, (cellColors.background << 4) | cellColors.text);
}

const CellColors NORMAL = {BLACK, WHITE};
const CellColors SELECTED = {WHITE, BLACK};
const CellColors FAIL = {RED, WHITE};
const CellColors FAIL_SELECTED = {WHITE, RED};

void printGrid() {
    for (int i = 0; i < SIZE; i++) {
        if (i % 3 == 0) {
            printf("+---------+---------+---------+\n");
        }
        for (int j = 0; j < SIZE; j++) {
            if (j % 3 == 0) {
                printf("|");
            }
            if (cursor.x == j && cursor.y == i) {
                setColor(SELECTED);
                if (errors[i][j] == 1) {
                    setColor(FAIL_SELECTED);
                }

            } else if (errors[i][j] == 1) {
                setColor(FAIL);
            }

            if (grid[i][j] == 0) {
                printf("[ ]");
            } else {
                printf("[%d]", grid[i][j]);
            }
            setColor(NORMAL);
        }
        printf("|\n");
    }
    printf("+---------+---------+---------+\n");
}

void checkGridStatus() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            errors[i][j] = 0;
        }
    }

    for (int i = 0; i < SIZE; i++) {
        unsigned short seen = 0;
        unsigned short rowRepeats = 0;
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == 0)
                continue;
            unsigned short flag = 1 << (grid[i][j] - 1); // ex: 5 - 1 = 4 -> 000000001 << 4 = 000010000 -> represents the fifth bit
            if (seen & flag) {
                rowRepeats |= flag;
            } else {
                seen |= flag;
            }
        }
        for (int j = 0; j < SIZE; j++) {
            if ((1 << (grid[i][j] - 1)) & rowRepeats) {
                errors[i][j] = 1;
            }
        }
    }
    for (int j = 0; j < SIZE; j++) {
        unsigned short seen = 0;
        unsigned short colRepeats = 0;
        for (int i = 0; i < SIZE; i++) {
            if (grid[i][j] == 0)
                continue;
            unsigned short flag = 1 << (grid[i][j] - 1);
            if (seen & flag) {
                colRepeats |= flag;
            } else {
                seen |= flag;
            }
        }
        for (int i = 0; i < SIZE; i++) {
            if ((1 << (grid[i][j] - 1)) & colRepeats) {
                errors[i][j] = 1;
            }
        }
    }
    for (int startRow = 0; startRow < SIZE; startRow += 3) {
        for (int startCol = 0; startCol < SIZE; startCol += 3) {
            unsigned short seen = 0;
            unsigned short boxRepeats = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (grid[i + startRow][j + startCol] == 0)
                        continue;
                    unsigned short flag = 1 << (grid[i + startRow][j + startCol] - 1);
                    if (seen & flag) {
                        boxRepeats |= flag;
                    } else {
                        seen |= flag;
                    }
                }
            }
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if ((1 << grid[i + startRow][j + startCol] - 1) & boxRepeats) {
                        errors[i + startRow][j + startCol] = 1;
                    }
                }
            }
        }
    }
}

void solve() {
    int base[9][9];
    int counter = 0;
    memcpy(base, grid, sizeof(grid));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (base[i][j] != 0) {
                continue;
            }
            int error = 0;
            if (grid[i][j] != 9) {
                grid[i][j] = grid[i][j] + 1;
            } else {
                error = 1;
            }

            cursor.x = j;
            cursor.y = i;
            checkGridStatus();
            if (error == 1) {
                errors[i][j] = 1;
            }

            printGrid();
            printf("%d\n", counter++);
            // Sleep(200);

            if (errors[i][j] != 0 && grid[i][j] != 9) {
                j--;
            } else if (errors[i][j] != 0) {
                grid[i][j] = 0;
                do {
                    if (j != 0) {
                        j--;
                    } else {
                        i--;
                        j = 8;
                    }

                } while (base[i][j] != 0);
                j--;
            }
        }
    }
}

int main() {
    printGrid();
    system("pause");
    /*
        int x = 1;
        int key;
        while (x == 1) {
            checkGridStatus();
            printGrid();
            key = _getch();
            system("cls");
            printf("%d\n", key);
            if (key == 224) {
                switch (_getch()) {
                case KEY_UP:
                    cursor.y = (cursor.y - 1 + SIZE) % SIZE;
                    break;
                case KEY_DOWN:
                    cursor.y = (cursor.y + 1) % SIZE;
                    break;
                case KEY_LEFT:
                    cursor.x = (cursor.x - 1 + SIZE) % SIZE;
                    break;
                case KEY_RIGHT:
                    cursor.x = (cursor.x + 1) % SIZE;
                    break;
                }
                printf("%d,%d\n", cursor.x, cursor.y);
            }
            if (key >= '0' && key <= '9') {
                grid[cursor.y][cursor.x] = key - '0';
            }
        }
    */
    solve();
    system("pause");
    return 0;
}