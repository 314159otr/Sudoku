#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct {
    unsigned int value;
    bool error;
} Cell;

typedef struct {
    int x;
    int y;
} Cursor;

typedef enum {
    KEY_UP = 72,
    KEY_LEFT = 75,
    KEY_RIGHT = 77,
    KEY_DOWN = 80,
    KEY_ARROWS = 224,
    KEY_H = 104,
    KEY_J = 106,
    KEY_K = 107,
    KEY_L = 108,
    KEY_S = 83,
    KEY_s = 115,
    KEY_Q = 81,
    KEY_q = 113,
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
/*
void printGrid() {
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) {
            printf("+---------+---------+---------+\n");
        }
        for (int j = 0; j < 9; j++) {
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
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            errors[i][j] = 0;
        }
    }

    for (int i = 0; i < 9; i++) {
        unsigned short seen = 0;
        unsigned short rowRepeats = 0;
        for (int j = 0; j < 9; j++) {
            if (grid[i][j] == 0)
                continue;
            unsigned short flag = 1 << (grid[i][j] - 1); // ex: 5 - 1 = 4 -> 000000001 << 4 = 000010000 -> represents the fifth bit
            if (seen & flag) {
                rowRepeats |= flag;
            } else {
                seen |= flag;
            }
        }
        for (int j = 0; j < 9; j++) {
            if ((1 << (grid[i][j] - 1)) & rowRepeats) {
                errors[i][j] = 1;
            }
        }
    }
    for (int j = 0; j < 9; j++) {
        unsigned short seen = 0;
        unsigned short colRepeats = 0;
        for (int i = 0; i < 9; i++) {
            if (grid[i][j] == 0)
                continue;
            unsigned short flag = 1 << (grid[i][j] - 1);
            if (seen & flag) {
                colRepeats |= flag;
            } else {
                seen |= flag;
            }
        }
        for (int i = 0; i < 9; i++) {
            if ((1 << (grid[i][j] - 1)) & colRepeats) {
                errors[i][j] = 1;
            }
        }
    }
    for (int startRow = 0; startRow < 9; startRow += 3) {
        for (int startCol = 0; startCol < 9; startCol += 3) {
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
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
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
}*/
void initializeGrid(Cell grid[9][9]);
void printGrid(Cell grid[9][9], Cursor cursor);
void printOptions();
bool checkCell(Cell grid[9][9], int row, int column);
void checkGrid(Cell grid[9][9]);
bool solveGrid(Cell grid[9][9], int row, int column);
int main() {

    Cell grid[9][9];
    Cursor cursor = {3, 2};
    int key;

    initializeGrid(grid);
    int sudoku[9][9] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            grid[i][j].value = sudoku[i][j];
        }
    }

    while (true) {
        printGrid(grid, cursor);
        printOptions();
        key = _getch();
        system("cls");
        if (key == KEY_ARROWS) {
            key = _getch();
        }
        switch (key) {
        case KEY_UP:
        case KEY_K:
            cursor.y = (cursor.y - 1 + 9) % 9;
            break;
        case KEY_DOWN:
        case KEY_J:
            cursor.y = (cursor.y + 1) % 9;
            break;
        case KEY_LEFT:
        case KEY_H:
            cursor.x = (cursor.x - 1 + 9) % 9;
            break;
        case KEY_RIGHT:
        case KEY_L:
            cursor.x = (cursor.x + 1) % 9;
            break;
        case KEY_S:
        case KEY_s:
            solveGrid(grid, 0, 0);
            break;
        case KEY_Q:
        case KEY_q:
            printGrid(grid, cursor);
            system("pause");
            return 0;
        }
        printf("Key:%d\n", key);
        printf("Cursor (%d,%d)\n", cursor.x, cursor.y);
        if (key >= '0' && key <= '9') {
            grid[cursor.y][cursor.x].value = key - '0';
            if (grid[cursor.y][cursor.x].error) {
                checkGrid(grid);
            } else {
                checkCell(grid, cursor.y, cursor.x);
            }
        }
    }
}

void initializeGrid(Cell grid[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            grid[i][j].value = 0;
            grid[i][j].error = false;
        }
    }
}

void printGrid(Cell grid[9][9], Cursor cursor) {
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) {
            printf("   +---------+---------+---------+\n");
        }
        printf("   ");
        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0) {
                printf("|");
            }
            if (cursor.x == j && cursor.y == i && grid[i][j].error) {
                setColor(FAIL_SELECTED);
            } else if (grid[i][j].error) {
                setColor(FAIL);
            } else if (cursor.x == j && cursor.y == i) {
                setColor(SELECTED);
            }

            if (grid[i][j].value == 0) {
                printf("[ ]");
            } else {
                printf("[%d]", grid[i][j].value);
            }
            setColor(NORMAL);
        }
        printf("|\n");
    }
    printf("   +---------+---------+---------+\n");
}

void printOptions() {
    printf("\n");
    printf("   Use the arrow keys to move the cursor\n");
    printf("   Press 0-9 to place a number\n");
    printf("   Press Q to quit\n");
    printf("   Press S to solve\n");
}
bool checkCell(Cell grid[9][9], int row, int column) {
    if (grid[row][column].value == 0) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                grid[i][j].error = false;
            }
        }
        return true;
    }

    int value = grid[row][column].value;
    bool error = false;
    for (int i = 0; i < 9; i++) {
        if (i != column && grid[row][i].value == value) {
            grid[row][i].error = true;
            error = true;
        }
    }
    for (int i = 0; i < 9; i++) {
        if (i != row && grid[i][column].value == value) {
            grid[i][column].error = true;
            error = true;
        }
    }
    int xBox = row / 3 * 3;
    int yBox = column / 3 * 3;
    for (int i = xBox; i < xBox + 3; i++) {
        for (int j = yBox; j < yBox + 3; j++) {
            if (i != row && j != column && grid[i][j].value == value) {
                grid[i][j].error = true;
                error = true;
            }
        }
    }

    grid[row][column].error = error;
    return !error;
}

void checkGrid(Cell grid[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            grid[i][j].error = false;
        }
    }
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (grid[i][j].value != 0) {
                checkCell(grid, i, j);
            }
        }
    }
}

bool solveGrid(Cell grid[9][9], int row, int column) {
    Cursor cursor = {column, row};
    system("cls");
    printGrid(grid, cursor);
    if (row == 9) { // no hay mas filas
        return true;
    } else if (column == 9) { // no hay mas columnas en esta fila
        return solveGrid(grid, row + 1, 0);
    } else if (grid[row][column].value != 0) { // ya existe un valor
        return solveGrid(grid, row, column + 1);
    } else { // probamos valores
        for (int i = 1; i < 10; i++) {
            grid[row][column].value = i;
            if (checkCell(grid, row, column) && solveGrid(grid, row, column + 1)) {
                return true;
            }
            system("cls");
            printGrid(grid, cursor);
            grid[row][column].value = 0;
            checkCell(grid, row, column);
        }
        system("cls");
        checkCell(grid, row, column);
        return false;
    }
}
/*
int main2() {
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
                    cursor.y = (cursor.y - 1 + 9) % 9;
                    break;
                case KEY_DOWN:
                    cursor.y = (cursor.y + 1) % 9;
                    break;
                case KEY_LEFT:
                    cursor.x = (cursor.x - 1 + 9) % 9;
                    break;
                case KEY_RIGHT:
                    cursor.x = (cursor.x + 1) % 9;
                    break;
                }
                printf("%d,%d\n", cursor.x, cursor.y);
            }
            if (key >= '0' && key <= '9') {
                grid[cursor.y][cursor.x] = key - '0';
            }
        }

solve();
system("pause");
return 0;
}
*/