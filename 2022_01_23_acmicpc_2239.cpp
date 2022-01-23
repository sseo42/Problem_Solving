#include <iostream>

using namespace std;

/**
 * Problem
 *      스도쿠가 주어진다.
 *      이를 완성시켜서 출력하자
 * 
 * Ideas
 *      ROW, COL, SECTOR 별로 9x10 마킹배열을 준비한다.
 *      백트래킹을 통해 확인하며 진행한다.
 *      Section은 (row / 3) * 3 + (col / 3) 인덱스로 보내자.
 *      시간초과에 대한 부분은 염려스럽지만 다른 해결책이 생각 안남..
 * 
 * Comments
 *      염려와 다르게 시간이 문제가 되지는 않았다. 조건에 의해 탐색하지 않은
 *      영역이 상당한 것 같다.
 *      이미 쓰여져 있는 숫자에 대한 처리를 고려하지 않고, borad의 range에 대한
 *      착각으로 시간을 소모했다. 너무 나이브하게 로직에만 집중하지 않고 고려사항도
 *      자각해서 생각에 반영해야겠다.
 */

# define BOARD_SIZE 9

enum Mark
{
    VACANT = 0,
    OCCUPIED
};

int row_mark[BOARD_SIZE][10];
int col_mark[BOARD_SIZE][10];
int sector_mark[BOARD_SIZE][10];
int board[BOARD_SIZE][BOARD_SIZE];

void init_mark(void)
{
    int number;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            number = board[i][j];
            row_mark[i][number] = OCCUPIED;
            col_mark[j][number] = OCCUPIED;
            sector_mark[(i / 3) * 3 + (j / 3)][number] = OCCUPIED;
        }
    }
}

void print_board(void)
{
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            printf("%d", board[i][j]);
        }
        printf("\n");
    }
}

bool back_tracking(int row, int col)
{
    bool success_flag;
    int sector = (row / 3) * 3 + (col / 3);

    if (row >= BOARD_SIZE)
        return true;
    if (board[row][col] != VACANT) {
        if (col == BOARD_SIZE - 1)
            success_flag = back_tracking(row + 1, 0);
        else
            success_flag = back_tracking(row, col + 1);
        return success_flag;
    }
    for (int i = 1; i < 10; ++i) {
        if (row_mark[row][i] == VACANT && col_mark[col][i] == VACANT && \
            sector_mark[sector][i] == VACANT) {
            row_mark[row][i] = OCCUPIED;
            col_mark[col][i] = OCCUPIED;
            sector_mark[sector][i] = OCCUPIED;
            board[row][col] = i;
            if (col == BOARD_SIZE - 1)
                success_flag = back_tracking(row + 1, 0);
            else
                success_flag = back_tracking(row, col + 1);
            if (success_flag) {
                return true;
            }
            else {
                row_mark[row][i] = VACANT;
                col_mark[col][i] = VACANT;
                sector_mark[sector][i] = VACANT;
                board[row][col] = VACANT;
            }
        }
    }
    return false;
}

int main(void)
{
    char strbuf[BOARD_SIZE + 1];

    for (int i = 0; i < BOARD_SIZE; ++i) {
        scanf("%s", strbuf);
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = strbuf[j] - '0';
        }
    }
    init_mark();
    back_tracking(0, 0);
    print_board();
    return 0;
}