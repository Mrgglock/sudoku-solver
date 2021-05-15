/**
 * Takes in a filepath to `sudoku.txt` and returns a 9x9 array.
 * @param  {string} filepath
 * @returns  {number[][]}
 */ 
function loadSudoku(filepath) {
    return require('fs')
            .readFileSync(filepath, 'utf8')
            .split('\r\n')
            .map(row => row.split('').map(ele => Number(ele)));
}

/**
 * Takes in a 9x9 array, and validates the grid only based on the new `row` and `col` of the cell.
 * @param  {number[][]} sudoku
 * @param  {number} row
 * @param  {number} col
 * @returns  {boolean}
 */
function validate(sudoku, row, col) {
    // Check row
    if (!allUniqueInArray(sudoku[row])) return false;
    // Check col
    let colArray = new Array(9)
                .fill()
                .map((val, index) => sudoku[index][col]);
    if (!allUniqueInArray(colArray)) return false;
    // Check box
    let box = getBox(sudoku, row, col);
    if (!allUniqueInArray(box)) return false;
    // All are unique.
    return true;
}

/**
 * A helper function which returns a `number[]` array of numbers of the box the cell is in.
 * @param  {number[][]} sudoku
 * @param  {number} row
 * @param  {number} col
 * @returns  {number[]}
 */ 
function getBox(sudoku, row, col) {
    const box = whichBox(row, col);
    let boxNumbers = [];
    const [rowLower, rowUpper] = box.rowRange;
    const [colLower, colUpper] = box.colRange;
    for (let r = rowLower; r <= rowUpper; r++) {
        for (let c = colLower; c <= colUpper; c++) {
            boxNumbers.push(sudoku[r][c]);
        }
    }
    return boxNumbers;
}

/**
 * A helper function which gives the upper and lower bounds of the box a cell is in.
 * @param  {number} row
 * @param  {number} col
 * @returns  {Object} intervals
 * @returns  {number[]} intervals.rowRange
 * @returns  {number[]} intervals.colRange
 */
function whichBox(row, col) {
    const intervals = [[0, 2], [3, 5], [6, 8]];
    let colRange;
    let rowRange;
    for (const [lower, upper] of intervals) {
        if (lower <= col && col <= upper) {
            colRange = [lower, upper];
        }
        if (lower <= row && row <= upper) {
            rowRange = [lower, upper];
        }
    }   
    return {
        rowRange,
        colRange
    }
}

/**
 * A helper function which checks if the numbers in an array are all unique, bar `0`.
 * @param  {number[]} array
 * @returns  {boolean}
 */
function allUniqueInArray(array) {
    array = array.filter(number => number != 0);
    let seen = [];
    for (let element of array) {
        if (seen.includes(element)) return false;
        seen.push(element);
    }
    return true;
}

/**
 * Deep clones the array.
 * @param  {any[]} array
 * @returns  {any[]}
 */
function deepClone(array) {
    return JSON.parse(JSON.stringify(array));
}

/**
 * The brain of the function. Given the start position `row` and `col` of the cell from where you want to solve, it either returns `false` if it is unsolvable or a `number[][]` sudoku grid.
 * @param  {number[][]} puzzle
 * @param  {number} row
 * @param  {number} col
 * @returns  {boolean | number[][]}
 */
function solve(puzzle, row, col) {
    const currentPuzzle = deepClone(puzzle);
    // For any sudoku grid, 

    // If it has been fully solved, 
    if (row == 9) return puzzle;

    // If your column has been overflowed, solve the row below and set col to 0.
    if (col == 9) return solve(puzzle, row + 1, 0);

    // If the current number has already been filled, solve the next number
    if (puzzle[row][col] != 0) return solve(puzzle, row, col + 1);

    // This cell is to be filled. Loop through every number 1 to 9 and try filling it.
    for (let num = 1; num <= 9; num++) {
        puzzle[row][col] = num;
        // Did this filling break any rule?
        if (!validate(puzzle, row, col)) continue;
        // It did not break a rule. Try solving the next box.
        const next = solve(puzzle, row, col + 1);
        if (next) {
            // If that solve was successful, pass it on.
            return next;
        }
        // Solve unsuccessful. Try next filling.
        puzzle = deepClone(currentPuzzle);
    }

    // We tried every number, and nothing was found. Failed.
    return false;
}

/**
 * Given a 9x9 array, convert it to a human-readable string.
 * @param  {number[][]} puzzle
 * @return {string}
 */
function toHumanReadableGrid(puzzle) {
    puzzle = puzzle.map(row => toHumanReadableRow(row));
    puzzle.splice(6, 0, ' ');
    puzzle.splice(3, 0, ' ');
    return puzzle.join('\n');

    function toHumanReadableRow(array) {
        array.splice(6, 0, ' ');
        array.splice(3, 0, ' ');
        return array.join('');
    }
}

// Load the sudoku.
const sudoku = loadSudoku("sudoku.txt");

// Log the time taken.
const { performance } = require('perf_hooks');
const t0 = performance.now();

// Solve the sudoku.
const solvedSudoku = solve(sudoku, 0, 0);
const t1 = performance.now();

if (solvedSudoku) {    
    // Print everything out and write it to a file.
    const humanReadableSudoku = toHumanReadableGrid(solvedSudoku);
    require('fs').writeFileSync('./solution.txt', humanReadableSudoku);
    console.log(humanReadableSudoku);
} else {
    console.log('The sudoku was unsolvable.');
}

console.log(`Completing the analysis took ${(t1 - t0)/1000} seconds.`);