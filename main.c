#include <stdio.h>
#include <string.h>

int display_digit_mask = 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1;

int board[9*9];

char row_buffer[7*3+1];

void printBoard() {
    char *digit_row = "%d %d %d | %d %d %d | %d %d %d\n";
    char *separator_row = "------+-------+------\n";
    display_digit_mask = ~0;
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

void getRowIds(int cellId, int *buffer) {
    int start = cellId / 9;
    for (int i = 0; i < 9; i++) {
        buffer[i] = start + i;
    }
}

void getColumn(int cellId, int *buffer) {
    int start = cellId % 9;
    for (int i = 0; i < 9; i++) {
        buffer[i] = board[start + 9 * i];
    }
}

void getColumnIds(int cellId, int *buffer) {
    int start = cellId % 9;
    for (int i = 0; i < 9; i++) {
        buffer[i] = start + 9 * i;
    }
}

void getBox(int cellId, int *buffer) {
    int vbx = cellId / 27;
    int hbx = cellId % 9 / 3;

    int start = vbx * 27 + hbx * 3;

    for (int i = 0; i < 9; i++) {
        buffer[i] = board[start + i % 3 + i / 3 * 9];
    }
}

void getBoxIds(int cellId, int *buffer) {
    int vbx = cellId / 27;
    int hbx = cellId % 9 / 3;

    int start = vbx * 27 + hbx * 3;

    for (int i = 0; i < 9; i++) {
        buffer[i] = start + i % 3 + i / 3 * 9;
    }
}

int notes_masks[10] = {0};

void fillNotes() {
    for (int i = 0; i < 9 * 9; i++) {
        if (board[i] == 0) {
            int notes_mask = notes_masks[0];
            board[i] = notes_mask;
        }
    }
}

void collapse_notes(int cellId) {
    int cellValue = board[cellId];
    if ((cellValue & display_digit_mask) != 0) {
        return;
    }

    int mask = notes_masks[0];

    int buffer[3*9];
    getBox(cellId, buffer);
    getColumn(cellId, &buffer[9]);
    getRow(cellId, &buffer[18]);

    for (int i = 0; i < 3*9; i++) {
        int val = buffer[i] & display_digit_mask;
        if (val == 0)
            continue;
        mask = mask & ~notes_masks[val];
    }
    board[cellId] = cellValue & mask;
}

void check_single_occurence(int cellId) {
    if (board[cellId]&display_digit_mask != 0)
        return;

    int bitcount = 0;


    int rowIds[9];
    int columnIds[9];
    int boxIds[9];

    getRowIds(cellId, rowIds);
    getColumnIds(cellId, columnIds);
    getBoxIds(cellId, boxIds);

    int need_row = notes_masks[0];
    int need_col = notes_masks[0];
    int need_box = notes_masks[0];

    int val = 0;

    for (int i = 0; i < 9; i++) {
        val = board[rowIds[i]] & display_digit_mask;
        if (val == 0)
            continue;

        need_row &= ~notes_masks[val];
    }

    for (int i = 0; i < 9; i++) {
        val = board[columnIds[i]] & display_digit_mask;
        if (val == 0)
            continue;

        need_col &= ~notes_masks[val];
    }

    for (int i = 0; i < 9; i++) {
        val = board[boxIds[i]] & display_digit_mask;
        if (val == 0)
            continue;

        need_box &= ~notes_masks[val];
    }

    int possible = board[cellId] & need_row & need_col & need_box;
    int dig = 0;

    for (int i = 11; i <= 19; i++) {
        if (possible & (1 << i)) {
            bitcount++;
            dig = i - 10;
        }
    }
    if (bitcount == 1) {
        board[cellId] = dig;
    }

}

int main(void) {
    for (int i = 1; i <= 9; i++) {
        notes_masks[i] = 1 << (10 + i);
        notes_masks[0] |= notes_masks[i];
    }

    int test_board[9*9] = {
        1,0,3, 0,8,0, 4,2,7,
        0,6,7, 3,0,4, 0,1,5,
        5,0,4, 1,7,0, 6,8,3,

        4,3,0, 9,6,2, 0,0,1,
        9,0,0, 0,0,7, 2,5,6,
        0,0,6, 8,1,0, 0,0,0,

        0,4,0, 6,0,0, 0,3,0,
        0,1,2, 0,4,3, 5,0,0,
        0,5,8, 0,0,1, 0,0,0
    };

    int solved_board[9*9] = {
        1,9,3, 5,8,6, 4,2,7,
        8,6,7, 3,2,4, 9,1,5,
        5,2,4, 1,7,9, 6,8,3,

        4,3,5, 9,6,2, 8,7,1,
        9,8,1, 4,3,7, 2,5,6,
        2,7,6, 8,1,5, 3,4,9,

        7,4,9, 6,5,8, 1,3,2,
        6,1,2, 7,4,3, 5,9,8,
        3,5,8, 2,9,1, 7,6,4
    };

    memcpy(board, test_board, sizeof(test_board));

    fillNotes();
    for (int i = 0; i < 9*9; i++) {
        collapse_notes(i);
    }

    for (int i = 0; i < 9*9; i++) {
        check_single_occurence(i);
    }

    printBoard();

    board[9*9-1] = 0;

    return 0;
}
