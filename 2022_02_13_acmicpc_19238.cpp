#include <iostream>
#include <utility>
#include <vector>
#include <queue>

using namespace std;

/**
 * Problem
 *      N x N 2차원 배열에서 총 M 명의 승객을 이동시키고자 한다.
 *      택시는 가장 가까운 승객을 태우며 거리가 같을 떄는 
 *      r, c 순으로 번호가 작은 승객을 태운다.(여러 승객을 태울 수는 없다.)
 *      승객을 목적지에 데려다주면 소모한 연료 양의 2배가 충전되며,
 *      연로가 바닥나면 시뮬레이션은 종료된다. 목적지에 도착함과 동시에 연료가 바닥나는
 *      경우는 성공으로 간주한다.
 * 
 *      모든 승객을 이동시켜줄 수 있을 경우에는 종료후 남은 연료양을 출력하고 그렇지 않을
 *      경우 -1 을 출력한다.
 * 
 * Ideas
 *      승객은 vector 로써 괸리하며 2번부터 사용하자. 
 *      각 승객마다 출발지에서 도착지를 bfs를 통해 계산하고, 거리를 승객vector에 담아 놓자.
 *      이때 벽으로 인해 이동이 불가능할 경우 시뮬레이션을 종료한다.
 * 
 *      배열 상에서 승객의 위치에 인덱스를 표시하자.
 * 
 *      bfs를 통해 택시에서 가장 가까운 승객을 탐색하고, (승객거리 + 도착 거리)를 현재 연료와 비교하자.
 *      불가능할 경우 종료한다.
 * 
 *      bfs 함수가 필요하다. 배열에서 시작위치와 조건 함수를 인자로 받자.
 *      이를 위해 mask, queue 가 필요하다. 위치와 거리를 리턴하자.
 *      bfs 탐색은 row 오름, col 오름 순으로 구성하자. dx dy
 * 
 *      update_distance_to_dest
 * 
 *      bfs_search
 * 
 *      ride_customers
 * 
 * Comments
 *      queue를 쓰고 탐색에 순서를 준다면 선택 조건을 만족할 수 있을 것이라 잘못 판단했다.
 *      벽으로 인해 구불구불하게 탐색하는 경우 결과 목표가 가장 작인 행, 열 번호를 가지는 
 *      대상이 아닌 처음 경로를 들어갈 떄 가장 작았던 행, 열을 가졌던 대상이 선택될 여지가 있다.
 *      따라서 queue가 아닌 priority_queue를 이용해서 정렬된 상태를 유지해야한다.
 */

#define IMPOSSIBLE -1

int board_size;

typedef struct s_loc_and_dist loc_and_dist;
struct s_loc_and_dist
{
    int r;
    int c;
    int dist;
};

vector<vector<int> > board;
vector<vector<bool> > board_mask;
vector<loc_and_dist> customers;

bool is_same_val(int a, int b)
{
    if (a == b)
        return 1;
    else
        return 0;
}

bool is_bigger(int a, int b)
{
    if (a > b)
        return 1;
    else
        return 0;
}

void init_board_mask(void)
{
    for (int i = 0; i < board_size; ++i) {
        for (int j = 0; j < board_size; ++j) {
            board_mask[i][j] = false;
        }
    }
}

loc_and_dist bfs_search(int start_r, int start_c, int comp_val, bool (*compare)(int a, int b))
{
    typedef pair<int, pair<int, int> > p3;

    static int dr[] = {-1, 0, 0, 1};
    static int dc[] = {0, -1, 1, 0};

    loc_and_dist res;
    priority_queue<p3, vector<p3>, greater<p3> > pq;
    int dist, r, c, r_expect, c_expect;

    init_board_mask();
    board_mask[start_r][start_c] = true;
    pq.push({0, {start_r, start_c}});

    while (!pq.empty()) {
        dist = pq.top().first;
        r = pq.top().second.first;
        c = pq.top().second.second;
        pq.pop();

        if (compare(board[r][c], comp_val)) {
            res.r = r;
            res.c = c;
            res.dist = dist;
            return res;
        }
        for (int j = 0; j < 4; ++j) {
            r_expect = r + dr[j];
            c_expect = c + dc[j];
            if (r_expect < 0 || r_expect >= board_size || c_expect < 0 || c_expect >= board_size)
                continue ;
            if (board[r_expect][c_expect] != 1 && board_mask[r_expect][c_expect] == false) {
                board_mask[r_expect][c_expect] = true;
                pq.push({dist + 1, {r_expect, c_expect}});
            }
        }
    }
    res.dist = IMPOSSIBLE;
    return res;
}

int update_customers(int idx, int dest_r, int dest_c)
{
    loc_and_dist res = bfs_search(dest_r, dest_c, idx, is_same_val);

    if (res.dist == IMPOSSIBLE)
        return IMPOSSIBLE;

    customers[idx].r = dest_r;
    customers[idx].c = dest_c;
    customers[idx].dist = res.dist;
    return res.dist;
}

int solution(int customer_cnt, int fuel, int taxi_r, int taxi_c)
{
    loc_and_dist closest_customer;
    int customer_idx;
    
    while (customer_cnt > 0) {
        closest_customer = bfs_search(taxi_r, taxi_c, 1, is_bigger);
        if (closest_customer.dist == IMPOSSIBLE)
            return IMPOSSIBLE;

        customer_idx = board[closest_customer.r][closest_customer.c];
        if (closest_customer.dist + customers[customer_idx].dist > fuel)
            return IMPOSSIBLE;
        board[closest_customer.r][closest_customer.c] = 0;
        customer_cnt--;
        fuel += customers[customer_idx].dist - closest_customer.dist;
        taxi_r = customers[customer_idx].r;
        taxi_c = customers[customer_idx].c;
    }
    return fuel;
}

int main(void)
{
    int customer_cnt;
    int fuel;
    int taxi_r, taxi_c;
    int start_r, start_c, dest_r, dest_c;
    int res;

    scanf("%d %d %d", &board_size, &customer_cnt, &fuel);
    board.resize(board_size);
    board_mask.resize(board_size);
    customers.resize(customer_cnt + 2);

    for (int i = 0; i < board_size; ++i) {
        board[i].resize(board_size);
        board_mask[i].resize(board_size);
        for (int j = 0; j < board_size; ++j) {
            scanf("%d", &(board[i][j]));
        }
    }

    scanf("%d %d", &taxi_r, &taxi_c);
    for (int i = 2; i < customer_cnt + 2; ++i) {
        scanf("%d %d %d %d", &start_r, &start_c, &dest_r, &dest_c);
        board[start_r - 1][start_c - 1] = i;
        res = update_customers(i, dest_r - 1, dest_c - 1);
        if (res == IMPOSSIBLE) {
            printf("-1\n");
            return 0;
        }   
    }
    res = solution(customer_cnt, fuel, taxi_r - 1, taxi_c - 1);
    if (res == IMPOSSIBLE)
        printf("-1\n");
    else
        printf("%d\n", res);
    return 0;
}