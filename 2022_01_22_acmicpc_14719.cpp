#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem
 *      1 <= H,W <= 500 이 주어진다.
 *      이 후 W 개의 숫자가 주어지며 각 숫자는
 *      H x W 2차원 배열에서 순서대로 탑의 높이이다.
 *      비가 충분히 온다고 가정할 때 고이는 물의 총량을 구하자.
 * 
 * Ideas
 *      양쪽에서 접근하여 해당 층의 빗물을 제거하면 된다.
 *      이를 탑블록을 마주칠 때 까지 진행하며 마친 이후는 하나 낮은 높이에서
 *      진행한다. 이를 통해 양쪽 시작지점에 바운드 시킬 수 있으며 중간에 탑에 의해
 *      고이는 빗물도 카운팅 가능하다.
 * 
 *      모든 작업을 마치고 총 빗물 블록을 구하면 된다.
 *      500 x 500 안에 작업이 마쳐지므로 시간 문제가 없을 것이다.
 * 
 * Comments
 *      특이사항 없음 지울 때 마크해서 중복되는 작업은 피하자.
 */

enum board_attr{
    EMPTY = 0,
    WATER,
    BLOCK
};


int main(void)
{
    int h, w;
    int block_height;
    int ans;
    vector<vector<int> > board;

    scanf("%d %d", &h, &w);
    board.resize(h);
    ans = h * w;
    for (int i = 0; i < h; ++i) {
        board[i].resize(w, board_attr::WATER);
    }
    for (int i = 0; i < w; ++i) {
        scanf("%d", &block_height);
        for (int j = 0; j < block_height; ++j) {
            board[j][i] = board_attr::BLOCK;
            ans--;
        }
    }
    for (int i = h - 1; i >= 0; --i) {
        for (int j = 0; j < w; ++j) {
            if (board[i][j] == board_attr::WATER) {
                board[i][j] = board_attr::EMPTY;
                ans--;
            }
            else {
                break ;
            }
        }
        for (int j = w - 1; j >= 0; --j) {
            if (board[i][j] == board_attr::WATER) {
                board[i][j] = board_attr::EMPTY;
                ans--;
            }
            else {
                break ;
            }
        }
    }
    printf("%d\n", ans);
    return 0;
}