#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

/**
 * Problem
 *      2차원 평면이 주어진다. 평면은 벽과 문, 빈공간, 문서, 열쇠로 구성되있다.
 *      문을 열기 위해서는 해당하는 열쇠가 필요하며, 시작 시 일부 열쇠는 소지하고 있다.
 *      열쇠를 탐색해 문을 개방하여 얻을 수 있는 최대 문서의 수를 구하자.
 * 
 *      문제의 테스트 케이스는 100개를 넘지 않는다.
 *      각 테스트 케이스는 평면의 사이즈 h,w 와 평면을 주고, 시작 시 소지한 열쇠를 준다.
 *      열쇠를 하나도 가지고 있지 않는 경우 "0" 이 주어진다.
 * 
 *      시작 위치는 평면의 밖이며, 평면의 테두리 중 벽이 아닌 곳을 통해 평면에 진입/탈출 이 가능하다.
 * 
 *      문을 수 있는 열쇠의 개 수는 0 이상이며, 열쇠로 열수 있는 문의 개수 또한 0 이상이다.
 * 
 * Ideas
 *      알파벳 대문자는 26자이므로 26크기의 배열을 이용해서 열쇠로 딸 수 있는 문의 위치를 달아놓자.
 * 
 *      평면을 탐색하기 직전 소유한 열쇠 중 사용되지 않은 것들에 대해 매칭되는 문들을 빈공간으로 돌리고 문 위치를 터트리자.
 *      문을 터트리는 작업은 최대 100 x 100 의 코스트가 들고, 키는 최대 26개 이므로 26 x 100 x 100 이 최대 코스트이다.
 * 
 *      다른 열쇠를 1개씩 확보하게 된다면 평면 탐색을 최대 26번 실행해야되고 해당 코스트는 26 x 100 x 100 이 최대 코스트이다.
 * 
 *      사용되지 않았던 키를 이용해 문을 터트리는 작업 수행 할 수 없다면 루프를 종료하고 결과를 리턴한다.
 * 
 *      최초에 아무 열쇠도 가지고 있지 않을 수 있으므로 시작은 평면의 탐색을 1회 수행하는 것으로 루프를 진입한다.
 * 
 *      필요한 구성은 다음과 같다.
 *  
 *      인풋을 받아 board와 키에 대한 문 위치들을 달아놓은 객체(key_to_doors) 집합을 구성하는 함수.
 *      
 *      key_to_doors 집합과 보드를 인자로 받아 순회해 문을 따고 적용한 키 갯 수를 리턴하는 함수.
 * 
 *      key_to_doors 와 보드를 인자로 받아 보드를 순회해 문을 따는 함수.
 * 
 *      key_to_doors 집합과 보드를 인자로 받아 보드탐색을 통해 열쇠를 업데이트 하고 획득한 문서 갯수를 리턴하는 함수.
 * 
 *      board_attr는 enum 으로 구성한다. (wall~key)
 *      key_state 는 NOT_ACQUIRED, AQUIRED, USED 세개로 enum 구성한다.
 * ---------------------------------------------------------------------------------------------------------------
 * 2022_09_19 문제를 다시 풀기 위해 옴.
 *      키마다 배열을 순회하도록 생각한 것은 비효율적인 것 같다.
 *      배열을 순회해 Door 을 만난다면 target 에 등록하고 빈공간일 경우 그냥 진행, 열쇠를 습득할 경우 열쇠의 습득 상태를
 *      업데이트 후 진행한다(열쇠의 cnt를 할 필요는 없다).
 *      그 후 open 단계에서 target을 pop 하며 해당하는 key 가 있는지만 체크해 상태를 업데이트하면 된다.
 * 
 *      이 경우 (100 * 100 + 열어야 될 door 순회 비용) * loop cost(최악 100 * 100)[door 1개씩 따야되는 경우] 가 최대 cost로
 *      이전 아이디어 보다 효율적이다.
 * 
 *      종료 시점을 가르는 것은 순회 동안 마주친 Door 가 없는 경우 or Door 를 1개도 open 하지 못하는 경우이다.
 * ---------------------------------------------------------------------------------------------------------------
 * 2022_09_25 다시 생각한다
 *      이전 생각이 옳다...
 *      키마다 배열을 순회해서 매번 마주하는 문을 여는 것이 아닌 벽을 무시한채로 board 의 모든 door 를 제거한다.
 *      해당 구역에 접근 불가능하다면 내부의 door를 제거한다해도 문제가 될 것이 없을 것이므로 안전하며,
 *      키를 반복하여 사용하지 않으므로 최대 26 * 100 * 100 이 문을 여는 작업의 최대 cost이다.
 *      또한 별도로 board를 searching 하는 작업 또한 하나의 door 가 unique 한 key에 대한 접근을 가지고 있다면 최대 
 *      26 * 100 * 100 이 필요하다 따라서 최대 코스트는 (26 * 100 * 100) * 2 이다.
 * 
 *      key 에 대한 door 위치를 저장하 key_to_doors, unused_key를 이용한 문을 열을 openDoors 함수,
 *      탐색을 통해 key 및 document 를 돌려줄 함수, return struct 가 필요하다.
 *  
 * Comments
 *      코멘트를 안썼다....
 * 
 */

#define ALL_KEY_CATEGORY 26
#define MAX_BOARD_SIZE 100

typedef enum _KEY_STATE
{
    NOT_ACQUIRED = 0,
    UNUSED,
    USED
} KEY_STATE;

typedef enum _BOARD_ELEM
{
    WALL = '*',
    EMPTY = '.',
    DOCUMENT = '$',
    DOOR_BASE = 'A',
    KEY_BASE = 'a'
}  BOARD_ELEM;

struct coordinate
{
    int row;
    int col;
};

typedef pair<KEY_STATE, vector<coordinate> > key_to_door;

char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
bool visited[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
vector<key_to_door> key_to_doors(ALL_KEY_CATEGORY);

void init_visited(int row_max, int col_max)
{
    for (int i = 0; i < row_max; i++) {
        for (int j = 0; j< col_max; j++)
            visited[i][j] = false;
    }
}

void init_board(int row_max, int col_max)
{
    for (int i = 0; i < row_max; i++) {
        for (int j = 0; j< col_max; j++)
            board[i][j] = char(BOARD_ELEM::WALL);
    }
}

void init_key_to_doors()
{
    for (int i = 0; i < ALL_KEY_CATEGORY; i++) {
        key_to_doors[i].first = NOT_ACQUIRED;
        key_to_doors[i].second.clear();
    }
}

bool open_doors()
{
    vector<coordinate> opened_doors;
    vector<coordinate>::iterator coord_iter;
    bool ret = false;

    for (int i = 0; i < ALL_KEY_CATEGORY; i++) {
        if (key_to_doors[i].first == KEY_STATE::UNUSED) {
            for (coord_iter = key_to_doors[i].second.begin(); coord_iter != key_to_doors[i].second.end(); ++coord_iter) {
                board[coord_iter->row][coord_iter->col] = char(BOARD_ELEM::EMPTY);
            }
            key_to_doors[i].first = KEY_STATE::USED;
            ret = true;
        }
    }
    return ret;
}

bool is_accessible(BOARD_ELEM elem)
{
    if (elem == BOARD_ELEM::EMPTY)
        return true;
    if (elem == BOARD_ELEM::DOCUMENT)
        return true;
    if (elem >= BOARD_ELEM::KEY_BASE && elem < BOARD_ELEM::KEY_BASE + ALL_KEY_CATEGORY)
        return true;
    return false;
}

bool attemp_to_get_key(coordinate coord)
{
    char elem = board[coord.row][coord.col];

    if (elem < BOARD_ELEM::KEY_BASE || elem >= BOARD_ELEM::KEY_BASE + ALL_KEY_CATEGORY)
        return false;
    if (key_to_doors[elem - BOARD_ELEM::KEY_BASE].first == KEY_STATE::NOT_ACQUIRED)
        key_to_doors[elem - BOARD_ELEM::KEY_BASE].first = KEY_STATE::UNUSED;
    board[coord.row][coord.col] = char(BOARD_ELEM::EMPTY);
    return true;
}

bool attemp_to_get_document(coordinate coord)
{
    char elem = board[coord.row][coord.col];

    if (elem != BOARD_ELEM::DOCUMENT)
        return false;

    board[coord.row][coord.col] = char(BOARD_ELEM::EMPTY);
    return true;
}

vector<coordinate> get_initial_entrances(int row_max, int col_max)
{
    static const int dr[] = {1, 0, -1, 0};
    static const int dc[] = {0, 1, 0, -1};

    int r_pos = 0;
    int c_pos = 0;
    BOARD_ELEM elem;
    vector<coordinate> entrances = vector<coordinate>();

    for (int i = 0; i < 4; i++) {
        while (r_pos + dr[i] < row_max && r_pos + dr[i] >= 0 &&
               c_pos + dc[i] < col_max && c_pos + dc[i] >= 0) {
            r_pos += dr[i];
            c_pos += dc[i];

            elem = BOARD_ELEM(board[r_pos][c_pos]);
            if (!is_accessible(elem))
                continue;

            coordinate coord;
            coord.row = r_pos;
            coord.col = c_pos;
            entrances.push_back(coord);
        }
    }
    return entrances;
}

void register_door_to_key(int row_max, int col_max)
{
    for (int i = 0; i < row_max; i++) {
        for (int j = 0; j < col_max; j++) {
            char elem = board[i][j];

            if (board[i][j] < BOARD_ELEM::DOOR_BASE || board[i][j] >= BOARD_ELEM::DOOR_BASE + ALL_KEY_CATEGORY)
                continue;
            coordinate coord;

            coord.row = i;
            coord.col = j;
            key_to_doors[elem - BOARD_ELEM::DOOR_BASE].second.push_back(coord);
        }
    }
}

int get_document_and_update_keys(queue<coordinate> search_q, int row_max, int col_max)
{
    static const int dr[] = {1, 0, -1, 0};
    static const int dc[] = {0, 1, 0, -1};
    int document_cnt = 0;

    while (!search_q.empty()) {
        coordinate coord = search_q.front();
        search_q.pop();

        for (int i = 0; i < 4; i++) {
            if (coord.row + dr[i] < 0 || coord.row + dr[i] >= row_max || coord.col + dc[i] < 0 || coord.col + dc[i] >= col_max)
                continue;
            
            BOARD_ELEM elem = BOARD_ELEM(board[coord.row + dr[i]][coord.col + dc[i]]);
            if (visited[coord.row + dr[i]][coord.col + dc[i]] == true || !is_accessible(elem))
                continue;
            
            coordinate new_coord;
            new_coord.row = coord.row + dr[i];
            new_coord.col = coord.col + dc[i];
            if (!attemp_to_get_key(new_coord)) {
                if (attemp_to_get_document(new_coord)) {
                    document_cnt++;
                }
            }
            visited[new_coord.row][new_coord.col] = true;
            search_q.push(new_coord);
        }
    }
    return document_cnt;
}

int solution()
{
    int row_max, col_max;
    string input_line;
    int answer = 0;

    scanf("%d %d", &row_max, &col_max);
    init_board(row_max, col_max);
    init_visited(row_max, col_max);
    init_key_to_doors();

    for (int i = 0; i < row_max; i++) {
        scanf("%s\n", board[i]);
    }
    getline(cin, input_line);
    if (input_line[0] != '0') {
        for (int i = 0; i < input_line.size(); i++) {
            key_to_doors[input_line[i] - BOARD_ELEM::KEY_BASE].first = KEY_STATE::UNUSED;
        }
    }
    register_door_to_key(row_max, col_max);
    open_doors();

    vector<coordinate> entrances_v = get_initial_entrances(row_max, col_max);
    queue<coordinate> entrances_q = queue<coordinate>();

    for (int i = 0; i < entrances_v.size(); i++) {
        if (!attemp_to_get_key(entrances_v[i])) {
            if (attemp_to_get_document(entrances_v[i]))
                answer++;
        }
        visited[entrances_v[i].row][entrances_v[i].col] = true;
        entrances_q.push(entrances_v[i]);
    }
    answer += get_document_and_update_keys(entrances_q, row_max, col_max);

    while (open_doors() == true) {
        init_visited(row_max, col_max);
        entrances_v = get_initial_entrances(row_max, col_max);
        for (int i = 0; i < entrances_v.size(); i++) {
            visited[entrances_v[i].row][entrances_v[i].col] = true;
            entrances_q.push(entrances_v[i]);
        }
        answer += get_document_and_update_keys(entrances_q, row_max, col_max);
    }
    return answer;
}

int main()
{
    int test_case;

    scanf("%d", &test_case);
    for (int i = 0; i < test_case; i++) {
        printf("%d\n", solution());
    }
    return 0;
}
