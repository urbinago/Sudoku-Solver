/*
-------------------------------
Author: Ignacio Urbina Gonzalez
-------------------------------

               Reference Sudoku Board
          (Index values are row-1 & column-1)

                       Columns
                1 2 3   4 5 6   7 8 9
              -------------------------
            1 | . . . | . . . | . . . |
Section 1   2 | Box 1 | Box 2 | Box 3 |
            3 | . . . | . . . | . . . |
              -------------------------
            4 | . . . | . . . | . . . |
Section 2   5 | Box 4 | Box 5 | Box 6 | Rows
            6 | . . . | . . . | . . . |
              -------------------------
            7 | . . . | . . . | . . . |
Section 3   8 | Box 7 | Box 8 | Box 9 |
            9 | . . . | . . . | . . . |
              -------------------------
*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//Displays the rules for
//file(I/O) and its requirements.
//Parameters: rules
void display_rules(bool &);

//Creates a 9x9 board (a pointer).
//Parameters: board
void create_board(int [9][9]);

//Identical to create_board, but
//used in a different context.
//Sets all elements equal to 0.
//Parameters: board
void wipe_board(int [9][9]);

//Gets the inputs from the user.
//Parameters: row, column, number, continue
void get_inputs(int &, int &, int &);

//Returns information on the modifications
//of the board. If the input violates a rule,
//function tells the user where the number already appears.
//Parameters: board, row, column, number
void input_feedback(int [9][9], int, int, int);

//Displays the sudoku board.
//Parameters: board
void display_board(int [9][9]);

//Checks if number is already in row.
//Parameters: board, number, row
bool check_row(int [9][9], int, int);

//Checks if number is already in column.
//Parameters: board, number, column
bool check_col(int [9][9], int, int);

//Returns the box of the cell.
//Parameters: row, column
int box_number(int, int);

//Checks the 3x3 box for a repeating number.
//Parameters: board, box number, number
bool check_box(int [9][9], int, int);

//Checks if the number inputted by the user
//is valid, abides by sudoku rules.
//Parameters: board, row, col, num
bool valid_num(int [9][9], int, int, int);

//Determines where there are empty cells
//(cells that equal 0) in the board.
//Parameters: board, row, column
bool empty_cell(int [9][9], int &, int &);

//Iterates through the board and solves it
//Uses recursion, guess and check method.
//Parameters: board
bool solve_board(int [9][9]);

//Main function
int main() {

    //Var used to make sure
    //rules are only displayed once.
    bool rules = true;

    //Variables for control structures.
    char file_loop;
    char menu_loop;
    char modify_loop;
    int menu_option;

    //Determines if the
    //user wants to reset
    //their board.
    char wipe;

    //Index values of board.
    int row, col;

    //User inputted number.
    int num;

    //The sudoku board.
    int board[9][9];

    cout << "\nSudoku Board Solver"
         << "\n-------------------"
         << "\nMenu (Y/N): ";
    cin >> menu_loop;

    create_board(board);

    //Menu Loop which includes the file,
    //modify, and solve loop.
    while (menu_loop=='Y') {

        cout << string( 100, '\n' );
        cout << "\nMenu Options:\n"
             << "File Loop (1)\n"
             << "Modify Loop (2)\n"
             << "Solve (3)\n"
             << "Exit (4)\n\n";

        cout << "Enter: ";
        cin >> menu_option;
        cout << string( 100, '\n' );

        switch (menu_option) {
            //File (I/O) portion of the code.
            //User is able to pass in a board that they
            //want to pass through the solver.
            case 1:
                if (rules) {
                    display_rules(rules);
                }
                do {
                    int file_row, count;
                    string line;
                    string filename;

                    cout << "Enter File Name: ";
                    cin >> filename;

                    ofstream testfile(filename.c_str(),ios::app);
                    testfile.close();

                    ifstream file (filename);
                    if (file.is_open()) {

                        file_row = 0;
                        count = 8;

                        //Reads the file
                        while (getline (file, line)) {
                            for (int i = 0; i <= count; i++) {

                                //char -> int conversion.
                                int j = line[i] - '0';
                                board[file_row][i] = j;
                            }

                            file_row++;
                            count = count + 9;

                        } cout << string( 100, '\n' );
                        display_board(board);
                        file.close();
                    } else cout << "Unable to open file";

                    cout << "New File (Y/N): ";
                    cin >> file_loop;

                    if (file_loop == 'Y')
                        wipe_board(board);

                } while (file_loop == 'Y');
                break;
            case 2:
                do {
                    get_inputs(row, col, num);
                    input_feedback(board, row, col, num);

                    //Determines if the board will be modified.
                    if (valid_num(board, row, col, num)) {
                        board[row][col] = num;
                        cout << "\nValid";
                    } else cout << "\nNot Valid";

                    display_board(board);

                    cout << "Modify (Y/N): ";
                    cin >> modify_loop;

                } while (modify_loop == 'Y');
                break;
            case 3:
                if (solve_board(board)) {
                    display_board(board);
                    cout << "   Solution Found\n";
                } else
                    cout << "Board has no solution.\n";

                cout << "\nWipe Board (Y/N): ";
                cin >> wipe;

                if (wipe == 'Y')
                    wipe_board(board);
                break;
            case 4:
                menu_loop = 'N';
                break;
            default:
                cout << "Invalid input";
                break;
        }
    }
    return 0;
}

void display_rules(bool &rules) {
    cout << "\n(File must follow this format)\n"
            "1. Each line consists of 9 numbers.\n"
            "2. Numbers must be between 0 and 9.\n"
            "3. Empty cells are denoted with a 0.\n"
            "4. Board must follow all the sudoku rules.\n\n"
            "(0s are displayed as '.' for visibility)\n"
            "Example File:\n"
            "..5..8...\n"
            "1.6..3..4\n"
            "..2..... 7\n"
            "..1..4...\n"
            "....8..6.\n"
            ".7.......\n"
            "9..53....\n"
            "...6..8.1\n"
            "....2..49\n\n";
    rules = false;
}

void create_board(int board[9][9]) {

    //Iterates through the board via
    //floor division and the modulo operator.
    for (int i = 0; i < 81; i++)
        board[i / 9][i % 9] = 0;
}

void wipe_board(int board[9][9]) {

    //Sets all elements back to 0
    for (int i = 0; i < 81; i++)
        board[i / 9][i % 9] = 0;
}

void get_inputs(int &row, int &col, int &num) {
    cout << "Row: ";
    cin >> row;
    row = row - 1;

    cout << "Column: ";
    cin >> col;
    col = col - 1;

    cout << "Number: ";
    cin >> num;

    cout << string( 100, '\n' );
}

void input_feedback(int board[9][9], int row, int col, int num) {
    if (check_row(board, row, num))
        cout << "Row is clear\n";
    else
        cout << num << " is in Row " << row + 1 << endl;

    if (check_col(board, col, num))
        cout << "Column is clear\n";
    else
        cout << num << " is in Column " << col + 1 << endl;

    if (check_box(board, box_number(row, col), num))
        cout << "Box is clear\n";
    else
        cout << num << " is in Box " << box_number(row, col) << endl;
}

void display_board(int board[9][9]) {

    //Uses the modulo operator to display
    //the elements in the board.
    for (int j = 0; j < 81; j++) {
        if (j % 1 == 0)
            cout << " ";
        if (j % 9 == 0) {
            cout << endl;
        } else if (j % 3 == 0) {
            cout << "| ";
        } if (j % 27 == 0) {
            cout << "---------------------" << endl;
        }
        if (board[j / 9][j % 9] == 0)
            cout << '.';
        else
            cout << board[j / 9][j % 9];
    } cout << "\n---------------------" << endl;
}

bool check_row(int board[9][9], int row, int num) {

    //Iterates horizontally through the board
    //by changing the value of col (i).
    //Checks every element in the row.
    for (int i = 0; i < 9; i++) {
        if (num == board[row][i])
            return false;
    } return true;
}

bool check_col(int board[9][9], int col, int num) {

    //Iterates vertically through the board
    //by changing the value of row (i).
    //Checks every element in the column.
    for (int i = 0; i < 9; i++) {
        if (num == board[i][col])
            return false;
    } return true;
}

int box_number(int row, int col) {

    //The box number.
    int box;

    //Determines whether the box is in
    //either horizontal section 1, 2 or 3.
    int section_start = ((row / 3) * 3) + 1;

    //Final piece to determining
    //which box the user is in.
    int col_box = col / 3;

    box = section_start + col_box;

    return box;
}

bool check_box(int board[9][9], int box, int num) {

    //Starting index values
    //for the row and the column.
    int r_start, c_start;

    //Ending index values
    //for the row and the column.
    int r_end, c_end;

    r_start = ((box - 1) / 3) * 3;
    c_start = ((box - 1) % 3) * 3;

    r_end = r_start + 2;
    c_end = c_start + 2;

    //Nested for loop that iterates through the box.
    //Checks the first row via changing col, then changes the row
    //after the three cols are checked.
    for (int i = r_start; i <= r_end; i++) {
        for (int j = c_start;j <= c_end; j++) {
            if (num == board[i][j])
                return false;
        }
    } return true;
}

bool valid_num(int board[9][9], int row, int col, int num) {

    //Checks if the number doesn't violate any of the sudoku rules.
    //All values must be true for valid_num() to return true
    if (check_row(board, row, num) && check_col(board, col, num) && check_box(board, box_number(row, col), num))
        return true;
    return false;
}

bool empty_cell(int board[9][9], int &row, int &col) {

    //Iterates through the board via floor division and
    //the modulo operator, same method used to create the board.
    for (int i = 0; i < 81; i++) {
        row = i / 9;
        col = i % 9;

        //If the number in the current cell
        //equals 0, then the cell is empty.
        if (board[row][col] == 0)
            return true;
    } return false;
}

bool solve_board(int board[9][9]) {

    //New local variable
    //declaration row & col.
    int row, col;

    //(Base Case)
    //If there are no empty cells,
    //then the board is solved.
    if (!empty_cell(board, row, col))
        return true;

    //For loop that attempts numbers 1-9 on the
    //empty cell on the board.
    for (int num = 1; num <= 9; num++) {

        //To change the cell to a number, it must abide by all
        //sudoku rules, no same number in a row, column, or box.
        if (board[row][col] == 0 && valid_num(board, row, col, num)) {
            board[row][col] = num;

            //If base case is true,
            //then this will be true.
            if (solve_board(board))
                return true;

            //Cell is emptied so next
            //number can be tried.
            board[row][col] = 0;
        }
    }
    //(General Case)
    //If no numbers work for
    //that empty cell, algorithm
    //backtracks and changes the value
    //of the previous empty cell.
    return false;
}