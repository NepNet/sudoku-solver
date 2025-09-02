#include <stdio.h>
#include <string.h>

constexpr int display_digit_mask = 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1;

int board[9*9];

void printBoard() {
    char *digit_row = "%d %d %d | %d %d %d | %d %d %d\n";
    char *separator_row = "------+-------+------\n";

    for (int i = 0; i < 9; i++) {
        //draw one column
        printf(digit_row,
            board[i*9+0]&display_digit_mask,
            board[i*9+1]&display_digit_mask,
            board[i*9+2]&display_digit_mask,
            board[i*9+3]&display_digit_mask,
            board[i*9+4]&display_digit_mask,
            board[i*9+5]&display_digit_mask,
            board[i*9+6]&display_digit_mask,
            board[i*9+7]&display_digit_mask,
            board[i*9+8]&display_digit_mask
            );
        if (i == 2) printf("%s", separator_row);
        if (i == 5) printf("%s", separator_row);
    }
}

void getRow(int cellId, int *buffer) {
    int start = cellId / 9;
    for (int i = 0; i < 9; i++) {
        buffer[i] = board[start + i];
    }
}

void getColumn(int cellId, int *buffer) {
    int start = cellId % 9;
    for (int i = 0; i < 9; i++) {
        buffer[i] = board[start + 9 * i];
    }
}

void getGroup(int i, int *buffer) {

}

int main(void) {
    int test_board[9*9] = {
        5,0,1, 6,2,7, 0,0,0,
        8,2,0, 0,9,0, 0,1,3,
        6,4,0, 0,0,0, 0,0,0,

        9,6,0, 4,0,1, 3,0,0,
        0,8,0, 7,3,0, 4,2,9,
        0,0,4, 9,0,0, 5,0,0,

        0,0,6, 0,7,5, 0,3,0,
        2,0,0, 3,6,9, 0,0,5,
        0,5,0, 0,0,0, 1,9,0
    };

    memcpy(board, test_board, sizeof(test_board));

    int notes_masks[10] = {0};
    for (int i = 1; i <= 9; i++) {
        notes_masks[i] = 1 << (10 + i);
        notes_masks[0] |= notes_masks[i];
    }
/*
    for (int i = 0; i < 9*9; i++) {
        if (board[i] == 0) {
            board[i] = notes_masks[0];
        }
    }*/

    int buffer[9];
    getGroup(27, buffer);

    printBoard();

    return 0;
}
