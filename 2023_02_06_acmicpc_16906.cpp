#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <cstring>

using namespace std;

/**
 * Problem
 *      단어의 개수 n[1, 1000] 이 주어진다. 단어는 0과 1로만 구성되있으며 다음줄에
 *      n 개에 걸쳐 각 단어의 길이가 주어진다. 단어의 길이는 최대 1000이다.
 * 
 *      한 단어가 다른 단어의 접두어가 되지않게 단어를 만들자.
 *      불가능하다면 -1 을 출력하고 가능하다면,
 *      1을 출력한 뒤 n 줄에 걸쳐 입력순으로 단어를 출력하자.
 * 
 * Ideas
 *      길이 1 이 주어졌다면 2는 01 혹은 00 2가지 경우를 가진다.
 *      만약 길이 1, 2가 주어졌다면 3은 001 000 2가지 경우를 갖는다.
 *      만약 1만 주어졌다면 3은 011 010 001 000 4가지 경우를 갖는다.
 *      만약 1, 3이 주어졌다면 4는 0101 0100 .. 6가지 경우를 갖는다.
 * 
 *      즉, 단어의 길이를 오름차순으로 정렬한 뒤,
 * 
 *      최초 경우의 수를 1로 두고 [1,1000] 까지 루프를 구성해 2를 곱한 뒤 해당 길이가 등장한 만큼 경우의 수에서 뺀다.
 * 
 *      ------------------------------------------------------------------------------------------------------
 *      실제 단어 구성하기 힘들다. 내림차순으로 구성한뒤 1을 우신하여 단어를 할당한다.
 *      zero_fixed = max(word_len) + 1 으로 둔다.
 * 
 *      해당 길이의 개수가 m개라 할 때, len <= zero_fixed 라면 ㅎ
 * 
 *      m < 2^k 를 만족하는 k를 구한다. 이때 자신 + 1 길이가 
 * 
 *      최소 available_bit 를 max(word_len) 으로 두고 k 만큼 빼는 것을 반복한다.
 * 
 *      ------------------------------------------------------------------------------------------------------
 *      힌트 봤다. 위에꺼 너무 복잡하다. 트라이로 구하고 오름차순 타는게 간단하다.
 * 
 * Comments
 *      경우의 수를 구하는 로직에서는 실제 단어를 구하기 어렵다는 것을 알고 긴거부터 할당하면서 잡아줘야하는 부분을
 *      찾으려했다.
 * 
 *      문제는 간단하게 이해하는 것이 중요하다. 복잡하게 생각하면 틀릴 확률이 매우매우 높다.
 *      가능한 심플하게 생각하자!
 */

stack<string> available_word[1001];
vector<int> input_len;

struct Trie
{
private:
    string word;

public:
    bool is_end;
    Trie *next[2];

    Trie(string word) {
        this->word = word;
        is_end = false;
        memset(next, 0, sizeof(next));
    }
    ~Trie() {
        for (int i = 0; i < 2; ++i) {
            if (next[i]) {
                delete next[i];
            }
        }
    }
    string find_word(int len) {
        string tmp;
        if (is_end == true) {
            return "";
        }
        if (len <= 0) {
            is_end = true;
            return this->word;
        }
        if (next[0] == NULL) {
            next[0] = new Trie(this->word + "0");
        }
        tmp = next[0]->find_word(len - 1);
        if (tmp != "") {
            return tmp;
        }
        if (next[1] == NULL) {
            next[1] = new Trie(this->word + "1");
        }
        return next[1]->find_word(len - 1);
    }
};

int main()
{
    int n;
    Trie root("");
    vector<int> sorted_len;

    scanf("%d\n", &n);
    input_len.resize(n);
    sorted_len.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> input_len[i];
        sorted_len[i] = input_len[i];
    }
    sort(sorted_len.begin(), sorted_len.end());
    for (vector<int>::iterator iter = sorted_len.begin(); iter != sorted_len.end(); ++iter) {
        string allocated_word = root.find_word(*iter);
        if (allocated_word == "") {
            printf("-1\n");
            return 0;
        }
        available_word[*iter].push(allocated_word);
    }
    printf("1\n");
    for (vector<int>::iterator iter = input_len.begin(); iter != input_len.end(); ++iter) {
        cout << available_word[*iter].top() << "\n";
        available_word[*iter].pop();
    }
}