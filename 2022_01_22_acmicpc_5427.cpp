#include <iostream>
#include <utility>
#include <queue>

using namespace std;

/**
 * Problem
 *      테스트 케이스는 최대 100개 주어진다.
 *      각 케이스의 첫줄에는 지도의 w, h 가 주어진다.
 *      . -> 빈공간
 *      # -> 벽
 *      @ -> 시작위치
 *      * -> 불
 *      불은 동서남북으로 퍼지며 벽은 불이 붙지 않는다.
 *      주인공의 위치에 불이 옮겨옴과 동시에 다른칸으로 이동 가능하다.
 *      불과 주인공이 이동하는데는 1초가 걸리며 주인공은 불이 옮겨진 칸
 *      또는 불이 붙으려는 칸으로 이동할 수 없다.
 *      가장 빠르게 지도를 탈출 할 수 있는 시간을 구하자.
 * 
 * Ideas
 *      불이 붙은 곳은 시간에 따라 고정적이다.
 *      따라서 주인공과 불의 위치를 queue에 넣고 bfs 로 주인공을 퍼트리고
 *      주인공의 위치 좌표에 0 이 있다면 탐색을 종료하고 시간 + 1을 출력한다.
 *      루프를 빠져나오지 못했는데 주인공의 queue가 empty라면 IMPOSSIBLE 이다.
 *      주인공은 불이 퍼질 예정인 곳으로 못 움직이므로
 *      불 bfs -> 주인공 bfs 로 순회 한다면 동시에 움직일 때 불의 예상 위치를 차단하는
 *      효과이므로 조건을 만족할 수 있다.
 *      구현 시작
 * 
 * Comments
 *      enum을 사용해 가독성을 높이고자 했다.
 *      scope를 제대로 잡지 못하고 구현에 들어가 헤메는 시간이 있었다.
 *      bfs시 마킹을 신경쓰지 않아 불필요한 동작을 하며 주인공 queue의 메모리가 넘치는 문제가 있었다.
 */

typedef pair<int, int> p;

int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};

enum board_attr{
    EMPTY_SPACE = 0,
    WALL,
    FIRE,
    MY_LOC
};

int board[1000][1000];

int solution(int h, int w, queue<p> &fire_q, queue<p> &my_q)
{
    int loc_h, loc_w;
    int loop_size;
    int rc = 0;

    while (1) {
        loop_size = fire_q.size();
        for (int i = 0; i < loop_size; ++i) {
            loc_h = fire_q.front().first;
            loc_w = fire_q.front().second;
            fire_q.pop();
            for (int j = 0; j < 4; ++j) {
                if (loc_h + dy[j] < 0 || loc_h + dy[j] >= h || \
                        loc_w + dx[j] < 0 || loc_w + dx[j] >= w) {
                    continue;
                }
                else if (board[loc_h + dy[j]][loc_w + dx[j]] == board_attr::EMPTY_SPACE || \
                            board[loc_h + dy[j]][loc_w + dx[j]] == board_attr::MY_LOC) {
                    board[loc_h + dy[j]][loc_w + dx[j]] = board_attr::FIRE;
                    fire_q.push({loc_h + dy[j], loc_w + dx[j]});
                }
            }
        }
        loop_size = my_q.size();
        if (loop_size == 0)
            return -1;
        for (int i = 0; i < loop_size; ++i) {
            loc_h = my_q.front().first;
            loc_w = my_q.front().second;
            my_q.pop();
            for (int j = 0; j < 4; ++j) {
                if (loc_h + dy[j] < 0 || loc_h + dy[j] >= h || \
                        loc_w + dx[j] < 0 || loc_w + dx[j] >= w) {
                    return rc + 1;
                }
                else if (board[loc_h + dy[j]][loc_w + dx[j]] == board_attr::EMPTY_SPACE) {
                    board[loc_h + dy[j]][loc_w + dx[j]] = board_attr::MY_LOC;
                    my_q.push({loc_h + dy[j], loc_w + dx[j]});
                }
            }
        }
        rc++;
    }
}

int main(void)
{
    int test_case_cnt;
    int h, w;
    char strbuf[1000];
    int rc;

    scanf("%d", &test_case_cnt);
    for (int i = 0; i < test_case_cnt; ++i) {
        queue<p> fire_q;
        queue<p> my_q;

        scanf("%d %d", &w, &h);

        for (int i = 0; i < h; ++i) {
            scanf("%s", strbuf);
            for (int j = 0 ; j < w; ++j) {
                if (strbuf[j] == '.') {
                    board[i][j] = board_attr::EMPTY_SPACE;
                }
                else if (strbuf[j] == '#') {
                    board[i][j] = board_attr::WALL;
                }
                else if (strbuf[j] == '*') {
                    board[i][j] = board_attr::FIRE;
                    fire_q.push({i, j});
                }
                else if (strbuf[j] == '@') {
                    if (my_q.size() > 1)
                        return -1;
                    board[i][j] = board_attr::MY_LOC;
                    my_q.push({i, j});
                }
                else {
                    return -1;
                }
            }
        }
        rc = solution(h, w, fire_q, my_q);
        if (rc == -1)
            printf("IMPOSSIBLE\n");
        else
            printf("%d\n", rc);
    }

    return 0;
}