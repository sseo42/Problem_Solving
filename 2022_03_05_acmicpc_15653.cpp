#include <iostream>
#include <utility>
#include <vector>
#include <queue>

using namespace std;

/**
 * Problem
 *      보드는 세로 N, 가로 M으로 구성되있고 가장 바깥은 모두 막혀있다. 보드에는 구멍이 하나있다.
 *      보드에 빨간구슬과 파란 구술을 넣었을 때 빨간구슬을 구멍을 통해 빼내자.
 *      4방형으로 기울이기가 가능하며 파란 구슬이 빠져나와서는 안 된다.
 *      (3 <= N, M <= 10), 빨간 구슬과 파란 구슬은 각 1개씩만 주어진다.
 *
 *      . -> 빈칸
 *      # -> 벽
 *      O -> 구멍
 *      R -> 빨간구슬
 *      B -> 파란구슬
 *
 *      최소 몇 번만에 빨근 구슬을 빼낼 수 있는지 출력한다 불가능하다면 -1을 출력한다.
 *
 * Ideas
 *      구슬 위치의 경우의 수는 8*8 이다.
 *      따라서 빨간 구슬과 파란구슬 위치의 경우의 수는 2^12 이다. 즉 중복되는 위치에 도달하는 케이스에 대해서
 *      시뮬레이션을 진행하지 않는 방식으로 진행한다면 최대 2^12 경우 내에 끝나므로 시간복잡도를 만족할 수 있다.
 *      기울기 방향을 연속해서 시도하는 것은 의미가 없으므로 탐색에서 배제한다.
 *      
 *      bfs 방식을 차용하여 {{cnt, last_direction}, {red_r, red_c}, {blue_r, blue_c}} 로 queue를 구성하자.
 *      last_direction 과 일치하지 않는 모든 방향으로 이동을 시도한다.
 *      또한 red_visited 2-dim, blue_visited 2-dim 을 이용해 and연산 결과가 true 가 아닐 경우 queue에 넣는다.
 *
 *      기울기 방향과 위치에 따라 먼저 이동을 실행해야할 구슬이 달라지므로 interactive_move 함수로 제어하자.
 *      함수는 {result_flag, {red_r, red_c}, {blue_r, blue_c}} 를 반환하며 flag는 성공, 진행, 실패 3개로 나뉜다.
 *
 *      각 공은 single_move 로 이동하며 구멍에 빠질 시 NULL, 그렇지 않을 시 위치를 pair로 반환한다.
 *
 * Comments
 *      bfs 탐색을 하므로 보드에 공위치를 마킹하고 있으면 안된다. 시뮬레이션 마다 공을 그리고 지워야한다.
 *      또한 visited 을 이용해 방문여부를 체크하므로 last_direction은 불필요하다.
 *      + 객체(struct)를 return 하는 함수에서 null을 리턴하여 제어하는 구문이 있어 수정에 시간이 소모됬다.
 *      잘못된 생각을 가졌다.
 */

enum BOARD_ATTR
{
    EMPTY = 0,
    WALL,
    HOLE,
    BALL
};

enum DIRECTION
{
    NONE = 0,
    RIGHT,
    DOWN,
    LEFT,
    UP
};

enum MOVE_RESULT_FLAG
{
    ESCAPE = 0,
    NOT_ESCAPE,
    FAIL
};

typedef pair<int, int> location;

typedef struct s_move_result move_result;
struct s_move_result
{
    MOVE_RESULT_FLAG flag;
    location red;
    location blue;
};

typedef struct s_queue_contents queue_contents;
struct s_queue_contents 
{
    int cnt;
    location red;
    location blue;

    s_queue_contents() {}

    s_queue_contents(int cnt, location red, location blue) {
        this->cnt = cnt;
        this->red = red;
        this->blue = blue;
    }
};

int dr[] = {0, 0, 1, 0, -1};
int dc[] = {0, 1, 0, -1, 0};

int n, m;
vector<vector<BOARD_ATTR> > board;
vector<vector<vector<vector<bool> > > > visited;

location single_move(DIRECTION direction, int row, int col)
{
    MOVE_RESULT_FLAG flag = NOT_ESCAPE;

    board[row][col] = EMPTY;
    while (board[row + dr[direction]][col + dc[direction]] == EMPTY) {
        row += dr[direction];
        col += dc[direction];
    }
    if (board[row + dr[direction]][col + dc[direction]] == HOLE) {
        return location(row + dr[direction], col + dc[direction]);
    }
    board[row][col] = BALL;
    return location(row, col);
}

move_result interactive_move(DIRECTION direction, location red, location blue)
{
    move_result result;
    bool blue_first = false;

    if (direction == NONE)
        throw out_of_range("not allowed direction");

    if ((direction == RIGHT && blue.second > red.second) ||
        (direction == DOWN && blue.first > red.first) ||
        (direction == LEFT && blue.second < red.second) ||
        (direction == UP && blue.first < red.first))
        blue_first = true;

    board[red.first][red.second] = BALL;
    board[blue.first][blue.second] = BALL;

    if (blue_first) {
        blue = single_move(direction, blue.first, blue.second);
        red = single_move(direction, red.first, red.second);
    }
    else {
        red = single_move(direction, red.first, red.second);
        blue = single_move(direction, blue.first, blue.second);
    }
    result.red = red;
    result.blue = blue;

    if (board[blue.first][blue.second] == HOLE)
        result.flag = FAIL;
    else if (board[red.first][red.second] == HOLE)
        result.flag = ESCAPE;
    else
        result.flag = NOT_ESCAPE;
    
    if (board[red.first][red.second] == BALL)
        board[red.first][red.second] = EMPTY;
    if (board[blue.first][blue.second] == BALL)
        board[blue.first][blue.second] = EMPTY;
    return result;
}

bool check_and_mark_visited(location red, location blue)
{
    if (visited[red.first][red.second][blue.first][blue.second] == true)
        return false;
    visited[red.first][red.second][blue.first][blue.second] = true;
    return true;
}

int bfs_solution(location init_red, location init_blue)
{
    DIRECTION all_direction[4] = {RIGHT, DOWN, LEFT, UP};
    queue<queue_contents> q;
    queue_contents elem;
    move_result res;

    check_and_mark_visited(init_red, init_blue);
    q.push(queue_contents(0, init_red, init_blue));

    while (!q.empty()) {
        elem = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            res = interactive_move(all_direction[i], elem.red, elem.blue);
            if (res.flag == ESCAPE) {
                return elem.cnt + 1;
            }
            else if (res.flag == NOT_ESCAPE) {
                if (check_and_mark_visited(res.red, res.blue) == true) {
                    q.push(queue_contents(elem.cnt + 1, res.red, res.blue));
                }
            }
        }
    }
    return -1;
}

int main(void)
{
    char line[11];
    location red;
    location blue;

    scanf("%d %d", &n, &m);

    for (int i = 0; i < n; i++) {
        board.resize(n);
        for (int j = 0; j < m; j++) {
            board[i].resize(m, WALL);
        }
    }
        
    for (int i = 0; i < n; i++) {
        visited.resize(n);
        for (int j = 0; j < m; j++) {
            visited[i].resize(m);
            for (int p = 0; p < n; p++) {
                visited[i][j].resize(n);
                for (int q = 0; q < m; q++) {
                    visited[i][j][p].resize(m, false);
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        scanf("%s", line);
        for (int j = 0; j < m; j++) {
            if (line[j] == '.') {
                board[i][j] = EMPTY;
            }
            else if (line[j] == 'O') {
                board[i][j] = HOLE;
            }
            else if (line[j] == 'R') {
                red.first = i; red.second = j;
            }
            else if (line[j] == 'B') {
                blue.first = i; blue.second = j;
            }
        }
    }

    printf("%d\n", bfs_solution(red, blue));
}