#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

bool arrayContains(char array[], int arrayLen, int val);
bool isAllUniqueIn(char array[9]);
bool isValid(char grid[9][9], int row, int col);
bool solve(char grid[9][9], int row, int col);
void loadSudoku(char grid[9][9], char *filepath);
bool isGoodSudoku(char *filepath);

int main(void) {
    // Read from sudoku.txt into a 9x9 array.
    char grid[9][9];
    char *filepath = "sudoku.txt";
    // Is the sudoku valid.
    if (!isGoodSudoku(filepath)) {
        printf("This sudoku is invalid.\n");
        return 1;
    }
    // Load the sudoku.
    loadSudoku(grid, filepath);

    // Solve it.
    clock_t t0 = clock();
    if (!solve(grid, 0, 0)) {
        printf("The sudoku was unsolvable.\n");
    }
    clock_t t1 = clock();
    double t = (double)(t1 - t0)/(CLOCKS_PER_SEC);
    printf("Completing the analysis took %f seconds.\n", t);
    return 0;
}

bool isGoodSudoku(char *filepath) {
    FILE *file = fopen(filepath, "r");
    char c;
    int row = 0;
    int col = 0;
    // read each char
    while (fread(&c, sizeof(char), 1, file)) {
        // if a newline is found, make sure it's the 9th character.
        if (c == '\n') {
            if (col != 9) { 
                fclose(file); 
                return false; 
            }
            // good newline.
            row++;
            col = 0;
            continue;
        }
        // make sure this non-newline char is a number.
        if (!(c >= '0' && c <= '9')) {
            fclose(file);
            return false;
        }
        col++;
        if (row == 9) {
            fclose(file);
            return false;
        }
    }
    return true;
}

void loadSudoku(char grid[9][9], char *filepath) {
    // Open file
    FILE *file = fopen(filepath, "r");
    // Read into it
    char c;
    int row = 0;
    int col = 0;
    while (fread(&c, sizeof(char), 1, file)) {
        if (c == '\n') {
            row++;
            col = 0;
            continue;
        }
        grid[row][col] = c;
        col++;
    }
    fclose(file);
}

void printGrid(char grid[9][9]) {
    FILE *solution = fopen("solution.txt", "w");
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            char cell = grid[r][c];
            fwrite(&cell, sizeof(char), 1, solution);
            printf("%c", cell);
            // new space
            if (c == 2 || c == 5) {
                fwrite(" ", sizeof(char), 1, solution);
                printf("%c", ' ');
            }
        }
        // new line
        fwrite("\n", sizeof(char), 1, solution);
        printf("\n");
        if (r == 2 || r == 5) {
            fwrite("\n", sizeof(char), 1, solution);
            printf("\n");
        }
    }
    fclose(solution);
}

bool solve(char grid[9][9], int row, int col) {
    // If it is overflowed completely, we have solved it
    if (row == 9) {
        printGrid(grid);
        return true;
    }

    // If col has overflowed, recall function
    if (col == 9) return solve(grid, row + 1, 0);

    // If the grid is already filled, call function on next
    if (grid[row][col] != '0') return solve(grid, row, col + 1);

    char currentGrid[9][9];
    memcpy(currentGrid, grid, 9 * 9 * sizeof(char));

    // This cell is to be filled. Try every number
    for (int i = 1; i <= 9; i++) {
        // convert int to char
        grid[row][col] = i + '0';
        // did that just break any rule
        if (isValid(grid, row, col)) {
            // it didnt. try solving the next one.
            if (solve(grid, row, col + 1)) return true;
            // reset grid since that didnt work
            memcpy(grid, currentGrid, 9 * 9 * sizeof(char));
        }
    }

    // all numbers we tried dont work. failed.
    return false;
}

bool isValid(char grid[9][9], int row, int col) {
    // Check for row validity
    if (!isAllUniqueIn(grid[row])) return false;
    // Check for column validity
    char column[9];
    for (int i = 0; i < 9; i++) {
        column[i] = grid[i][col];
    }
    if (!isAllUniqueIn(column)) return false;
    // Check for box validity
    // Look for which box it is in
    int rowLower = 0;
    int rowUpper = 2;
    int colLower = 0;
    int colUpper = 2;
    while ( !(rowLower <= row && row <= rowUpper) ) {
        rowLower += 3;
        rowUpper += 3;
    }
    while ( !(colLower <= col && col <= colUpper) ) {
        colLower += 3;
        colUpper += 3;
    }
    char box[9];
    int boxCount = 0;
    for (int r = rowLower; r <= rowUpper; r++) {
        for (int c = colLower; c <= colUpper; c++) {
            box[boxCount] = grid[r][c];
            boxCount++;
        }
    }
    if (!isAllUniqueIn(box)) return false;
    return true;
}

// helper function to check for validity
bool isAllUniqueIn(char array[9]) {
    char seen[9] = {'0'};
    int seenCount = 0;
    // Loop through array and add to seen if non-zero element found
    for (int i = 0; i < 9; i++) {
        if (array[i] == '0') continue;
        // Check if it already is in seen[]
        if (arrayContains(seen, seenCount, array[i])) return false;
        seen[seenCount] = array[i];
        seenCount++;
    }
    return true;
}

// helper function because C doesnt have enough functions
bool arrayContains(char array[], int arrayLen, int val) {
    for (int i = 0; i < arrayLen; i++) {
        if (array[i] == val) return true;
    }
    return false;
}