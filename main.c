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
    int row;
    int col;
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
    KEY_C = 67,
    KEY_c = 99,
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

void initializeGrid(Cell grid[9][9]);
void printGrid(Cell grid[9][9], Cursor cursor);
void printOptions();
bool checkCell(Cell grid[9][9], Cursor cursor);
void checkGrid(Cell grid[9][9]);
bool solveGrid(Cell grid[9][9], Cursor cursor);

int main() {
    Cell grid[9][9];
    Cursor cursor = {0, 0};
    int key;

    initializeGrid(grid);
    int sudoku[9][9] = {
        {0, 1, 5, 0, 7, 0, 0, 0, 0},
        {4, 0, 0, 8, 0, 0, 7, 5, 0},
        {0, 0, 8, 0, 0, 9, 0, 1, 6},
        {9, 6, 4, 1, 0, 7, 0, 3, 0},
        {0, 8, 2, 3, 9, 0, 5, 0, 0},
        {5, 0, 0, 0, 0, 4, 0, 9, 0},
        {0, 2, 0, 4, 1, 0, 8, 0, 0},
        {0, 0, 1, 7, 0, 3, 9, 0, 4},
        {0, 0, 0, 9, 2, 0, 0, 6, 5}
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
            cursor.row = (cursor.row - 1 + 9) % 9;
            break;
        case KEY_DOWN:
        case KEY_J:
            cursor.row = (cursor.row + 1) % 9;
            break;
        case KEY_LEFT:
        case KEY_H:
            cursor.col = (cursor.col - 1 + 9) % 9;
            break;
        case KEY_RIGHT:
        case KEY_L:
            cursor.col = (cursor.col + 1) % 9;
            break;
        case KEY_S:
        case KEY_s:
            cursor.col = 0;
            cursor.row = 0;
            solveGrid(grid, cursor);
            break;
        case KEY_C:
        case KEY_c:
            initializeGrid(grid);
            break;
        case KEY_Q:
        case KEY_q:
            return 0;
        }
        if (key >= '0' && key <= '9') {
            grid[cursor.row][cursor.col].value = key - '0';
            if (grid[cursor.row][cursor.col].error) {
                checkGrid(grid);
            } else {
                checkCell(grid, cursor);
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
            if (cursor.col == j && cursor.row == i && grid[i][j].error) {
                setColor(FAIL_SELECTED);
            } else if (grid[i][j].error) {
                setColor(FAIL);
            } else if (cursor.col == j && cursor.row == i) {
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
    printf("\n");
}

void printOptions() {
    printf("   Use the arrow keys to move the cursor\n");
    printf("   Press 0-9 to place a number\n");
    printf("   Press %c to quit\n", KEY_Q);
    printf("   Press %c to solve\n", KEY_S);
    printf("   Press %c to clear\n", KEY_C);
}
bool checkCell(Cell grid[9][9], Cursor cursor) {
    if (grid[cursor.row][cursor.col].value == 0) {
        return true;
    }

    int value = grid[cursor.row][cursor.col].value;
    bool error = false;
    for (int i = 0; i < 9; i++) {
        if (i != cursor.col && grid[cursor.row][i].value == value) {
            grid[cursor.row][i].error = true;
            error = true;
        }
    }
    for (int i = 0; i < 9; i++) {
        if (i != cursor.row && grid[i][cursor.col].value == value) {
            grid[i][cursor.col].error = true;
            error = true;
        }
    }
    int xBox = cursor.row / 3 * 3;
    int yBox = cursor.col / 3 * 3;
    for (int i = xBox; i < xBox + 3; i++) {
        for (int j = yBox; j < yBox + 3; j++) {
            if (i != cursor.row && j != cursor.col && grid[i][j].value == value) {
                grid[i][j].error = true;
                error = true;
            }
        }
    }

    grid[cursor.row][cursor.col].error = error;
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
                Cursor cursor;
                cursor.row = i;
                cursor.col = j;
                checkCell(grid, cursor);
            }
        }
    }
}

bool solveGrid(Cell grid[9][9], Cursor cursor) {
    static int counter = 0;
    if (cursor.row == 9) { // no hay mas filas
        printf("Numeros probados: %d\n", counter);
        counter = 0;
        return true;
    } else if (cursor.col == 9) { // no hay mas columnas en esta fila
        cursor.row += 1;
        cursor.col = 0;
        return solveGrid(grid, cursor);
    } else if (grid[cursor.row][cursor.col].value != 0) { // ya existe un valor
        cursor.col += 1;
        return solveGrid(grid, cursor);
    } else { // probamos valores
        for (int i = 1; i < 10; i++) {
            counter++;
            grid[cursor.row][cursor.col].value = i;
            bool isValid = checkCell(grid, cursor);
            printGrid(grid, cursor);
            cursor.col += 1;
            if (isValid && solveGrid(grid, cursor)) {
                return true;
            }
            cursor.col -= 1;
            grid[cursor.row][cursor.col].value = 0;
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    grid[i][j].error = false;
                }
            }
        }
        return false;
    }
}