#include <iostream>
#include <utility>
#include <vector>

using namespace std;

/**
 * Problem 
 *      2차원 격자 안에 상어들이 있다.
 *      상어들은 [1, M] 으로 번호가 메겨져 있으며 상어가 같은 위치에 있다면 
 *      가장 높은 번호의 상어를 제외한 나머지 상어는 격자에서 배제된다.
 *      상어는 자신의 위치에 '냄새'를 남기며 이는 상어가 k번 이동 후 사라진다.
 * 
 *      상어의 이동 규칙은 다음과 같다.
 *      1. 인접한 칸 중 아무 냄새가 없는 칸의 방향으로 이동한다.
 *      2. 1번은 만족하는 칸이 없다면 자신의 냄새가 남아 있는 칸의 방향으로 이동한다.
 * 
 *      이 때 이동가능한 칸이 여러 개일 수 있는데, 그 경우에는 특정 우선순위를 따르며
 *      이는 상어마다 다르다.
 * 
 *      1번 상어만이 격자에 남을 때 까지의 시간을 구하자.
 * 
 *      첫 줄에 N, M, k가 주어진다. (2<= N <= 20, 2 <= M <= N^2, 1 <= k <= 1000)
 * 
 * Ideas
 *      2차원 격자에는 {상어 번호들, 냄새의 번호, k카운트} 3개를 각 각 가지고 있게 하자.
 *      각 상어는 1차원 배열로 관리하고 인덱스는 번호, value로는
 *      (r, c), 4x4 사이즈의 우선순위 목록을 가지게 하자.
 * 
 *      상어의 이동은 하위부터 상위로 실시하며 이동에는 냄새를 배열에 남기는 것을 포함하자.
 *      배열을 순회하며 상어를 배제할 때 따로 냄새에 조작을 안 가해도 된다.
 * 
 *      함수는 각 상어의 이동, 전체 이동 제어, 배열에서 k 카운트 조작, 상어 추방, 배열 제어
 *      로 구성하자.
 * 
 * Comments
 *      처리하는 순서와 방식에 대해서 명확하게 개념을 잡지 않고 들어가 문제가 발생했다.
 *      이로 안한 오류를 잡기 위해 시간을 소모하였고, 정리 되지않은 채로 진행한 부분을 수정하느라
 *      결과적으로 오히려 코드 또한 가독성이 떨어지게 되었다.
 *      중복되지 않는 명확한 함수, 변수 명과 처리 순서를 반드시 잡고 들어가도록 해야한다.
 */

# define NO_SMELL 0

typedef struct shark_s shark_data;
struct  shark_s
{
    int direction;
    int r, c;
    int preference[5][5];
};

typedef pair<int, int> smell_and_k_cnter;
typedef pair<vector<int>, smell_and_k_cnter> board_attr;

vector<vector<board_attr> > board;
vector<shark_data> sharks;

int dr[5] = {0, -1, 1, 0, 0};
int dc[5] = {0, 0, 0, -1, 1};

int N, M, k;
int shark_num;

void update_board_new_shark(board_attr &attr, int shark_id)
{
    attr.first.push_back(shark_id);
    attr.second.first = shark_id;
    attr.second.second = k;
}

void update_board_smell(void)
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (board[i][j].second.second > 0) {
                board[i][j].second.second--;
            }
            else {
                board[i][j].second.first = NO_SMELL;
            }
        }
    }
}

void exclude_sharks_from_board(vector<int> &shark_ids)
{
    for (int i = 0; i < shark_ids.size() - 1; ++i) {
        shark_num--;
        sharks[shark_ids[i]].direction = 0;
    }
}

void update_shark_data(shark_data *data, int direction, int r, int c)
{
    data->direction = direction;
    data->r = r;
    data->c = c;
}

void move_shark(int shark_id)
{
    shark_data *data = &(sharks[shark_id]);
    int direction = data->direction;
    int *preference_order = (data->preference)[direction];
    int r = data->r;
    int c = data->c;

    if (data->direction == 0)
        return ;

    for (int i = 1; i <= 4; ++i) {
        int expected_r = r + dr[preference_order[i]];
        int expected_c = c + dc[preference_order[i]];

        if (expected_r < 0 || expected_r >= N || expected_c < 0 || expected_c >= N)
            continue ;
        if (board[expected_r][expected_c].second.first == NO_SMELL) {
            update_shark_data(data, preference_order[i], expected_r, expected_c);
            return ;
        }
    }
    for (int i = 1; i <= 4; ++i) {
        int expected_r = r + dr[preference_order[i]];
        int expected_c = c + dc[preference_order[i]];

        if (expected_r < 0 || expected_r >= N || expected_c < 0 || expected_c >= N)
            continue ;
        if (board[expected_r][expected_c].second.first == shark_id) {
            update_shark_data(data, preference_order[i], expected_r, expected_c);
            return ;
        }
    }
}

void move_all_sharks(void)
{
    for (int i = M; i > 0; --i) {
        move_shark(i);
    }
    for (int i = M; i > 0; --i) {
        if (sharks[i].direction > 0) {
            update_board_new_shark(board[sharks[i].r][sharks[i].c], i);
        }
    }
}


void update_board(void)
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (board[i][j].first.size() > 1) {
                exclude_sharks_from_board(board[i][j].first);
            }
            board[i][j].first.clear();
        }
    }
    update_board_smell();
}

int main(void)
{
    int num;

    scanf("%d %d %d", &N, &M, &k);
    board.resize(N);
    sharks.resize(M + 1);
    shark_num = M;
    for (int i = 0; i < N; ++i) {
        board[i].resize(N);
        for (int j = 0; j < N; ++j) {
            scanf("%d", &num);
            if (num > 0) {
                sharks[num].r = i;
                sharks[num].c = j;
                board[i][j].second.first = num;
                board[i][j].second.second = k;
            }
        }
    }
    for (int i = 1; i <= M; ++i) {
        scanf("%d", &num);
        sharks[i].direction = num;
    }
    for (int i = 1; i <= M; ++i) {
        for (int j = 1; j <= 4; ++j) {
            for (int k = 1; k <= 4; ++k) {
                scanf("%d", &num);
                sharks[i].preference[j][k] = num;
            }
        }
    }
    for (int i = 0; i <= 1000; ++i) {
        update_board();
        move_all_sharks();

        if (shark_num == 1) {
            printf("%d\n", i);
            return 0;
        }
    }
    printf("-1\n");
    return 0;
}