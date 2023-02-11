#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem
 *      BOARD_SIZE x BOARD_SIZE 사이즈의 2차원배열이 총 BOARD_SIZE 개 주어진다.
 *      배열은 회전 가능하나 뒤집을 수는 없다.
 *      각 배열을 적절하게 회전하여 임의로 쌓아 BOARD_SIZE 사이즈의 큐브를 만들었을 때,
 *      입구는 큐브의 꼭지점 중 하나를 임의로 설정할 수 있고
 *      출구는 그 반대편, 입구와 면을 공유하지 않는 꼭지점이 된다.
 * 
 *      배열의 0 값은 뚫려있는 곳이고 1 은 막혀있는 곳이다.
 * 
 *      입구부터 출구까지 가장 적은 이동 횟수를 출력하자.
 * 
 *      어떻게 설계하더라도 탈출이 불가능할 경우 -1을 출력한다.
 * 
 * Ideas
 *      설계하는 총 경우의 수는 BOARD_SIZE! 이고 입구를 정하는 경우는 6가지이다.
 *      따라서 6! = 30 x 12 x 2 = 720
 * 
 *      설계한 큐브 내에서 dp를 통해 마킹하며 서청 한다면, 최악의 경우 5x5x5 이므로
 *      시간복잡도를 만족할 수 있다.
 * 
 *      설계마다 새로 3차원 배열을 할당한다면 오버헤드가 크므로 정적으로 잡은다음 초기화하여 사용하자
 * 
 *      회전마다 배열을 재구성하는 것은 비효율적이므로 쌓을 때 board, height, rotation을 받아 구성하자.
 *      accumulate_board 라하자.
 * 
 *      설계 큐브는 cube 라하자. -> 전역
 * 
 *      dp를 위한 큐브는 dp_cube 라하자. -> 전역
 *      초기화 -> init_dp_cube
 * 
 *      큐브의 masking 은 cube_mask 라하자 -> 전역, 이를 통해 어느쪽을 입구로 설정하던지 같은 로직으로 풀이가능하다.
 * 
 *      최소거리는 get_short_cut 함수를 통해 구하자.
 * 
 *      배열은 5x5x5 로 저장해 놓는다. -> all_board -> 전역
 *      배열의 사용을 체크하는 함수는 board_mask 라하자. -> 전역
 * 
 *      함수들을 활용해 값을 얻는 제어함수는 dp_solution 이라하자.
 * 
 * Comments
 *      큐브의 masking 은 필요치 않다, dp 에서 처리 가능,
 *      또한 큐브를 부루트하게 구성하면 중복되는 구조가 있다.
 *      시간복잡도를 계산할 떄는 문제가 되지 않을 것이라 판단했지만, 경우에 따른 로직처리에서
 *      생각보다 시간소모가 크다. 따라서 중복되는 경우를 없애줘야했다.
 * 
 *      또한 큐브를 구성하고 시뮬레이션을 돌릴 함수에 대해서도 망각했다.
 * 
 *      + 입구나 출구가 막힌경우에 대한 처리지시를 누락했다.
 */

#define IMPOSSIBLE 987654321
#define NOT_VISTED 987654320
#define BOARD_SIZE 5

int all_board[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];
bool board_mask[BOARD_SIZE];
int cube[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];
int dp_cube[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];

int used_cube[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];

enum ROTATION
{
    ZERO = 0,
    NINETY,
    ONE_EIGHTY,
    TWO_SEVENTY
};

void init_dp_cube(void)
{
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            for (int k = 0; k < BOARD_SIZE; ++k) {
                if (cube[i][j][k] == 1)
                    dp_cube[i][j][k] = NOT_VISTED;
                else
                    dp_cube[i][j][k] = IMPOSSIBLE;
            }
        }
    }
}

void accululate_board(int board[BOARD_SIZE][BOARD_SIZE], int height, ROTATION angle)
{
    if (angle == ZERO) {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                cube[height][i][j] = board[i][j];
            }
        }
    }
    else if (angle == NINETY) {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                cube[height][i][j] = board[BOARD_SIZE - 1 - j][i];
            }
        }
    }
    else if (angle == ONE_EIGHTY) {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                cube[height][i][j] = board[BOARD_SIZE - 1 - i][BOARD_SIZE - 1 - j];
            }
        }
    }
    else if (angle == TWO_SEVENTY) {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                cube[height][i][j] = board[j][BOARD_SIZE - 1 - i];
            }
        }
    }
}

int get_short_cut(int x, int y, int z)
{
    static int dz[] = {-1, 0, 0, 0, 0, 1};
    static int dx[] = {0, -1, 0, 1, 0, 0};
    static int dy[] = {0, 0, -1, 0, 1, 0};
    
    int min_val = IMPOSSIBLE;
    int x_expect, y_expect, z_expect;

    if (dp_cube[x][y][z] != NOT_VISTED)
        return dp_cube[x][y][z];

    dp_cube[x][y][z] = IMPOSSIBLE;
    for (int i = 0; i < 6; ++i) {
        x_expect = x + dx[i];
        y_expect = y + dy[i];
        z_expect = z + dz[i];

        if (x_expect < 0 || x_expect >= BOARD_SIZE || y_expect < 0 || y_expect >= BOARD_SIZE || \
            z_expect < 0 || z_expect >= BOARD_SIZE)
            continue ;

        if (cube[x_expect][y_expect][z_expect] == 1 && \
            dp_cube[x_expect][y_expect][z_expect] != IMPOSSIBLE)
            min_val = min(min_val, get_short_cut(x_expect, y_expect, z_expect) + 1);
    }
    dp_cube[x][y][z] = min_val;
    return min_val;
}

int dp_solution(void)
{
    static int z_start[] = {0, 0, 0, 0};
    static int x_start[] = {0, BOARD_SIZE - 1, 0, BOARD_SIZE - 1};
    static int y_start[] = {0, 0, BOARD_SIZE - 1, BOARD_SIZE - 1};
    static int z_end[] = {BOARD_SIZE - 1, BOARD_SIZE - 1, BOARD_SIZE - 1, BOARD_SIZE - 1};
    static int x_end[] = {BOARD_SIZE - 1, 0, BOARD_SIZE - 1, 0};
    static int y_end[] = {BOARD_SIZE - 1, BOARD_SIZE - 1, 0, 0};

    int min_val = IMPOSSIBLE;

    for (int i = 0; i < 4; ++i) {
        if (cube[x_start[i]][y_start[i]][z_start[i]] == 1 && cube[x_end[i]][y_end[i]][z_end[i]] == 1) {
            init_dp_cube();
            dp_cube[x_start[i]][y_start[i]][z_start[i]] = 0;
            min_val = min(min_val, get_short_cut(x_end[i], y_end[i], z_end[i]));
        }
    }
    return min_val;
}

int solution(int height)
{
    static ROTATION rot[4] = {ZERO, NINETY, ONE_EIGHTY, TWO_SEVENTY};

    if (height >= BOARD_SIZE)
        return dp_solution();

    int min_val = IMPOSSIBLE;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board_mask[i] == false) {
            if (height == 0) {
                board_mask[i] = true;
                accululate_board(all_board[i], height, ZERO);
                min_val = min(min_val, solution(height + 1));
                board_mask[i] = false;
            }
            else {
                for (int j = 0; j < 4; ++j) {
                    board_mask[i] = true;
                    accululate_board(all_board[i], height, rot[j]);
                    min_val = min(min_val, solution(height + 1));
                    board_mask[i] = false;
                }
            }
        }
    }
    return min_val;
}

int main(void)
{
    int ans;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            for (int k = 0; k < BOARD_SIZE; ++k) {
                scanf("%d", &(all_board[i][j][k]));
            }
        }
    }
    ans = solution(0);
    if (ans == IMPOSSIBLE)
        printf("-1\n");
    else
        printf("%d\n", ans);
    return 0;
}