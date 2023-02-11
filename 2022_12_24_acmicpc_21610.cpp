#include <iostream>
#include <vector>

/**
 * Problem
 *      2차원 평면이 주어진다.
 *      아래 동작을 반복적으로 수행한 뒤 배열에 있는 총 합을 구하자.
 *      2 <= N <= 50            : 배열의 size
 *      1 <= M <= 100           : 이동 횟수
 *      0 <= A[i][j] <= 100     : 배열의 값
 *      1 <= d <= 8             : 이동 방향
 *      1 <= s <= 50            : 이동 거리
 * 
 *      처음 구름은 (N, 1) (N, 2) (N-1, 1) (N-1, 2) 에 생긴다.
 *      배열의 경계는 이어져있다.
 * 
 *      1.  모든 구름은 d 방향으로 s 만큼 이동한다.
 *      2.  구름이 위치한 곳에 + 1 을 한다.
 *      3.  구름은 사라진다.
 *      4.  2번의 행위로 값이 증가한 곳의 대각선의 값이 1 이상이라면 그 수만큼 현재 위치에 더한다(최대 4, 이때 카운트는 배열의 경계를 넘어서 하지 않는다).
 *      5.  2번의 행위의 대상이 아닌 위치 중 값이 2 이상인 곳에서 구름을 생성한다.
 * 
 * Ideas
 *      1번 행위는 경계를 넘어야하므로 row, col 을 파라미터로 받아서 나머지 계산으로 움직이자.
 *      2번 행위를 수행한 위치를 5번에서 이용해야하므로 별도의 테이블을 만들자.
 *      
 *      1번과 4번은 별도 함수로 분리한다.
 *      구름은 테이블로 관리되며, 3번에서 바로 삭제하지 않고 5번 단계에서 (2이상 or 구름이 이미존재) 를 xor 하는 것으로 처리하자.
 * 
 * Comments
 *      구름을 움직일 때 음수 방향으로 간다면 나머지 계산이 올바르게 안되는 것을 간과했다.
 *      음수 계산의 나머지는 적절한 bias 를 넣어주자.
 * 
 *      오랫만에 푸는 문제라 문법 잡는데 시간을 쓴거 말고는 괜찮은 거 같다.
 */

using namespace std;

#define MAX_BOARD_SIZE 50
#define DIRECTION_CASE 4
#define CHECK_SPOT_CNT 4

int board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
bool cloud[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
vector<pair<int, int> > move_order;
int board_size, order_cnt;

void move_cloud(int direction_case, int step, int bias)
{
    static int row_direction[] = {0, 0, -1, -1, -1, 0, 1, 1, 1};
    static int col_direction[] = {0, -1, -1, 0, 1, 1, 1, 0, -1};
    static bool tmp_cloud[MAX_BOARD_SIZE][MAX_BOARD_SIZE];

    int dr = row_direction[direction_case];
    int dc = col_direction[direction_case];

    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            int r_pos = (i + dr * step + bias) % board_size;
            int c_pos = (j + dc * step + bias) % board_size;

            tmp_cloud[r_pos][c_pos] = cloud[i][j];
        }
    }
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            cloud[i][j] = tmp_cloud[i][j];
        }
    }
}

void pour_water(void)
{
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            board[i][j] += (cloud[i][j] == true) ? 1 : 0;
        }
    }
}

void copy_water(void)
{
    static int row_spot[] = {-1, -1, 1, 1};
    static int col_spot[] = {-1, 1, 1, -1};

    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            if (cloud[i][j] == true) {
                for (int k = 0; k < CHECK_SPOT_CNT; k++) {
                    int r_pos = i + row_spot[k];
                    int c_pos = j + col_spot[k];

                    if (r_pos < 0 || r_pos >= board_size || c_pos < 0 || c_pos >= board_size || board[r_pos][c_pos] <= 0) {
                        continue;
                    }
                    board[i][j] += 1;
                }
            }
        }
    }
}

void regenerate_cloud(void)
{
    for (int i = 0;  i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            if (cloud[i][j] == true) {
                cloud[i][j] = false;
            }
            else if (board[i][j] >= 2) {
                board[i][j] -= 2;
                cloud[i][j] ^= true;
            } 
        }
    }
}

void magic_loop(void)
{
    int move_bias = board_size;
    vector<pair<int, int> >::iterator order_iter;

    while (move_bias < MAX_BOARD_SIZE) {
        move_bias += move_bias;
    }

    for (order_iter = move_order.begin(); order_iter != move_order.end(); ++order_iter) {
        move_cloud(order_iter->first, order_iter->second, move_bias);
        pour_water();
        copy_water();
        regenerate_cloud();
    }
}

void get_input(void)
{
    scanf("%d %d\n", &board_size, &order_cnt);

    //board
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            scanf("%d", board[i] + j);
        }
    }

    //move order
    for (int i = 0; i < order_cnt; i++) {
        int direction, step;

        scanf("%d %d", &direction, &step);
        move_order.push_back(make_pair(direction, step));
    }
}

void init_cloud(void)
{
    cloud[board_size - 1][0] = true;
    cloud[board_size - 1][1] = true;
    cloud[board_size - 2][0] = true;
    cloud[board_size - 2][1] = true;
}

int sum_board(void)
{
    int sum_val = 0;

    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            sum_val += board[i][j];
        }
    }
    return sum_val;
}

int main(void)
{
    get_input();

    init_cloud();

    magic_loop();

    printf("%d\n", sum_board());
}