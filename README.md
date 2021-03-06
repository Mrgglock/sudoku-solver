# sudoku-solver
A small project to understand backtracking.

This program was made in JavaScript as well as C.
Node.js (version 12.19.0), gcc (version 10.3.0). 

This project has **0** dependencies.

This program returns the first solution found to a sudoku problem.

# How To Use
1. Open `sudoku.txt`.
2. Using `0`s for blanks and the numbers `1` to `9`, fill it up row-by-row with your Sudoku puzzle, separating it with line breaks `\n` after every row.
3. Run the program using `node index.js`.
  - If you are using JavaScript:
    - Run the program using `node index.js`.
  - If you are using C:
    - Compile the program using gcc and run `index.exe`.
4. Wait.
5. You should see something happen, depending on if the Sudoku was solvable.
  - If it was solvable:
    - You should see a completed Sudoku grid printed on your console.
    - In addition, the solution is written to a new file `solution.txt`.
  - If it was unsolvable:
    - You should see `The sudoku was unsolvable.` printed on your console.
  - If it was invalid:
    - You should see `This sudoku is invalid.` printed on your console.
6. You should also see `Completing the analysis took n seconds.` printed on your console, stating how long it took to process the result.

# Known Issues
None.

# Example
### Sudoku Problem
![image](https://user-images.githubusercontent.com/9843579/118370960-bf30a180-b5dc-11eb-832e-e349b1f4f43e.png)
### sudoku.txt
```
000801000
000000430
500000000
000070800
000000100
020030000
600000075
003400000
000200600
```

### Console
```$ node index.js
234 891 567
169 725 438
578 346 912

316 574 829
497 682 153
825 139 746

642 918 375
953 467 281
781 253 694
Solving the sudoku took 172.355930399999 seconds.
```
In C:
```
Solving the sudoku took 11.20400 seconds.
```

`solution.txt` will also contain the above solution for future reference.
