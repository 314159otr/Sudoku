#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define SIZE 9

typedef struct {
    int x;
    int y;
} Coordinate;

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

void setColor(int textColor, int backgroundColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // Combine text and background colors
    SetConsoleTextAttribute(hConsole, (backgroundColor << 4) | textColor);
}

void printGrid(int grid[SIZE][SIZE], Coordinate cursor) {
    for (int i = 0; i < SIZE; i++) {
        if (i % 3 == 0) {
            printf("+---------+---------+---------+\n");
        }
        for (int j = 0; j < SIZE; j++) {
            if (j % 3 == 0) {
                printf("|");
            }
            if (cursor.x == j && cursor.y == i) {
                setColor(BLACK, LIGHT_GRAY);
            }
            if (grid[i][j] == 0)
            {
                printf("[ ]");
            }
            else
            {
                printf("[%d]", grid[i][j]);
            }
            setColor(WHITE, BLACK);
        }
        printf("|\n");
    }
    printf("+---------+---------+---------+\n");

}



int main() {
    int grid[SIZE][SIZE] = {
     {0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 1},
     {0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    Coordinate cursor = { 0,0 };
    int x = 1;
    int key;
    while (x == 1) {
        printGrid(grid, cursor);
        key = _getch();
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


    system("pause");
    return 0;
}