#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>

using namespace std;

/**
 * Problem
 *      노드를 지나는 경로의 개수 n[1, 1000] 이 주어진다.
 *      이후 n 줄에 걸쳐 루트노드 부터 리프노드까지의 직선 정보가 주어진다.
 *      직선 정보는 중복되지 않으며,
 *      길이 k[1,15] 그리고 지나가는 각 노드의 string 값이 주어진다(길이: [1,15])
 * 
 *      해당 트리의 구조를 출력하자.
 *      자식노드가 여려개일 경우 string 의 사전 순서대로 나타내며 
 *      depth 가 늘어날떄 마다 '--'를 붙여 출력한다.
 * 
 * Ideas
 *      하나의 노드에 자식이 여렷일수 있다, 이때 매번 돌아가며 일치하는 것을 찾는 것은
 *      비효율적이므로 trie 로 찾는다. trie 단어의 끝에 연결된 노드 번호를 두고
 *      각 번호마다 trie 를 운용하자. trie 안써도 풀리긴할듯...?
 * 
 *      메모리 최대 26 * 15 * 15 * 1000 -> 약 5,000,000 바이트, ㄱㅊ하다.
 * 
 */

#define MAX_PATH 1000
#define MAX_DEPTH 15
#define MAX_LEN 15

int discovery;

class Trie
{
private:
    int key_to_idx(const char *s) {
        return (int)((*s) - 'A');
    }

public:
    static const int alphabet = 26;

    int next_den;
    Trie *next[alphabet];

    Trie() {
        next_den = 0;
        memset(next, 0, sizeof(next));
    }
    ~Trie() {
        for (int i = 0; i < alphabet; ++i) {
            if (next[i]) {
                delete next[i];
            }
        }
    }
    int insert(const char *s) {
        if (*s == '\0') {
            next_den = ++discovery;
            return next_den;
        }
        int idx = key_to_idx(s);

        if (next[idx] == NULL) {
            next[idx] = new Trie();
        }
        return next[idx]->insert(s + 1);
    }
    int find(const char *s) {
        if (*s == '\0') {
            return next_den;
        }
        int idx = key_to_idx(s);

        if (next[idx] == NULL) {
            return 0;
        }
        return next[idx]->find(s + 1);
    }
};

Trie den[MAX_PATH * MAX_DEPTH + 1];
vector<vector<pair<string, int> > > links(MAX_PATH * MAX_DEPTH + 1);

void print_all(int node, int depth)
{
    for (vector<pair<string, int> >::iterator next_den = links[node].begin(); next_den != links[node].end(); ++next_den) {
        for (int i = 0; i < depth; ++i) {
            printf("--");
        }
        printf("%s\n", next_den->first.c_str());
        print_all(next_den->second, depth + 1);
    }
}

int main()
{
    int total_path;
    char buf[MAX_LEN + 1];

    scanf("%d", &total_path);
    for (int i = 0; i < total_path; ++i) {
        int node = 0;
        int depth;

        scanf("%d ", &depth);
        for (int j = 0; j < depth; ++j) {
            int next_den;

            scanf("%s", buf);
            next_den = den[node].find(buf);

            if (next_den == 0) {
                next_den = den[node].insert(buf);
                links[node].push_back({buf, next_den});
            }
            node = next_den;
        }
    }
    for (int i = 0; i < MAX_PATH * MAX_DEPTH; ++i) {
        sort(links[i].begin(), links[i].end());
    }
    print_all(0, 0);
}