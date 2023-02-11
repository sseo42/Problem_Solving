#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem
 *      1 <= n <= 10,000 : 행성의 개수
 *      
 *      n 줄에 걸쳐서 행성의 3차원 좌표가 주어진다. [-10^9, 10^9]
 * 
 *      두 행성을 연결하는 비용은 min(|xi - xj|, |yi - yj|, |zi - zj|)
 *      이다. 모든 행성을 연결하는 최소의 비용을 구하자.
 * 
 * Ideas
 *      행성을 연결의 최소비용에서 간선의 수는 n - 1 개이다(MST)
 * 
 *      이를 3차원으로만 풀면 된다
 *      최대 거리가 int로 표햔가능하므로 int 쓴다.
 * 
 * Comments
 *      메모리에서 터졌었다, 
 *      한 노드에서 이어지는 간선 중 가장 작은 값은 x, y, z 차가 최소가되는 값 3개 중 하나이다.
 *      MST 에 따라 간선은 선택할 떄 해당 노드가 포함된 간선이라면, 반드시 위 3개 중 하나가 선택되므로(최솟값을 선택하므로)
 *      나머지를 저장할 필요가 없다...!
 */

#define MAX_PLANET 10000

typedef struct s_coord
{
    int idx;
    int x;
    int y;
    int z;
} coord;

int n;
vector<pair<int, pair<int, int> > > cost;
vector<int> djs;

int find_djs_root(int node)
{
    static int local_buf[MAX_PLANET];
    int depth = 0;

    for (int parent = djs[node]; parent >= 0;) {
        local_buf[depth] = node;
        depth++;

        node = parent;
        parent = djs[node];
    }
    for (int i = 0; i < depth; i++) {
        djs[local_buf[i]] = node;
    }
    return node;
}

bool union_djs(int node_a, int node_b)
{
    int root_a = find_djs_root(node_a);
    int root_b = find_djs_root(node_b);

    if (root_a == root_b) {
        return false;
    }
    if (djs[root_a] < djs[root_b]) {
        djs[root_b] = root_a;
    } else if (djs[root_a] > djs[root_b]) {
        djs[root_a] = root_b;
    } else {
        djs[root_a] = root_b;
        djs[root_b]--;
    }
    return true;
}

long long get_mst_cost(void)
{
    int connection_cnt = 0;
    long long res = 0;
    
    for (vector<pair<int, pair<int, int> > >::iterator cost_iter = cost.begin(); cost_iter != cost.end(); ++cost_iter) {
        if (union_djs(cost_iter->second.first, cost_iter->second.second) == true) {
            res += cost_iter->first;
            connection_cnt++;
        }
        if (connection_cnt >= n - 1) {
            break;
        }
    }
    return res;
}

bool x_compare(coord loc_a, coord loc_b)
{
    if (loc_a.x == loc_b.x) {
        return loc_a.x < loc_b.x;
    }
    return loc_a.x < loc_b.x;
}

bool y_compare(coord loc_a, coord loc_b)
{
    if (loc_a.y == loc_b.y) {
        return loc_a.y < loc_b.y;
    }
    return loc_a.y < loc_b.y;
}

bool z_compare(coord loc_a, coord loc_b)
{
    if (loc_a.z == loc_b.z) {
        return loc_a.z < loc_b.z;
    }
    return loc_a.z < loc_b.z;
}

int main(void)
{
    vector<coord> x_list;
    vector<coord> y_list;
    vector<coord> z_list;
    int x, y, z;

    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &x, &y, &z);
        
        coord loc;

        loc.idx = i;
        loc.x = x;
        loc.y = y;
        loc.z = z;
        x_list.push_back(loc);
        y_list.push_back(loc);
        z_list.push_back(loc);
    }
    sort(x_list.begin(), x_list.end(), x_compare);
    sort(y_list.begin(), y_list.end(), y_compare);
    sort(z_list.begin(), z_list.end(), z_compare);

    for (int i = 0; i < n - 1; i++) {
        cost.push_back({abs(x_list[i].x - x_list[i + 1].x), {x_list[i].idx, x_list[i + 1].idx}});
        cost.push_back({abs(y_list[i].y - y_list[i + 1].y), {y_list[i].idx, y_list[i + 1].idx}});
        cost.push_back({abs(z_list[i].z - z_list[i + 1].z), {z_list[i].idx, z_list[i + 1].idx}});
    }
    sort(cost.begin(), cost.end());

    djs.assign(n, -1);

    printf("%lld\n", get_mst_cost());
}