#include <iostream>
#include <vector>
#include <queue>

using namespace std;

/**
 * Problem
 *      1 <= n, m <= 50     : n -> 사람 수. m -> 파티의 수
 * 
 *      첫 줄에 n, m 이 주어진다.
 *      둘째 줄에 진실을 아는 사람 수, 이들의 번호가 주어진다.
 * 
 *      이어 m 줄에 걸쳐 각 파티에 참석하는 사람 수와 이들의 번호들이 주어진다.
 *      같은 파티에 참석한 사람끼리는 진실이 옮는다.
 * 
 *      진실을 모르는 파티의 수를 구하자.
 * 
 * Ideas
 *      n 명을 노드로 보고 같은 파티에 참석한 사람들을 양방향 간선으로 잇는다.
 * 
 *      간선의 수는 최대 50 * 50 일 것이다.
 * 
 *      음... 그낭 각 파티마다 사람 순회해서 감염자 있으면 전부 감염하는 건?
 *      이러면 체크 순서에 영향 받음.
 * 
 * Comments
 *      인덱스 안의 값과 인덱스 자체를 좀 헷갈려하는 듯..
 * 
 *      다른 특이사항은 없었다
 */

#define MAX_PEOPLE 50

int n, m;
vector<vector<int> > attended_party;
vector<vector<int> > party_attendent;

bool infected[MAX_PEOPLE + 1];

void infect(vector<int> &infected_people)
{
    queue<int> infection_q;

    for (int i = 0; i < infected_people.size(); ++i) {
        infected[infected_people[i]] = true;
        infection_q.push(infected_people[i]);
    }
    while (!infection_q.empty()) {
        int man = infection_q.front();

        infection_q.pop();
        for (vector<int>::iterator party = attended_party[man].begin(); party != attended_party[man].end(); ++party) {
            for (vector<int>::iterator attendent = party_attendent[*party].begin(); attendent != party_attendent[*party].end(); ++attendent) {
                if (!infected[*attendent]) {
                    infected[*attendent] = true;
                    infection_q.push(*attendent);
                }
            }
        }
    }
}

int get_bultshit_days(void)
{
    int counter = m;

    for (vector<vector<int> >::iterator attendents = party_attendent.begin(); attendents != party_attendent.end(); ++attendents) {
        for (vector<int>::iterator attendent = attendents->begin(); attendent != attendents->end(); ++attendent) {
            if (infected[*attendent]) {
                --counter;
                break ;
            }
        }
    }
    return counter;
}

int main(void)
{
    vector<int> infected_people;

    int infected_amount;
    int infected_one;
    int attendent_amount;
    int attendent;

    scanf("%d %d", &n, &m);
    attended_party.resize(n + 1);
    party_attendent.resize(m);

    scanf("%d", &infected_amount);
    infected_people.resize(infected_amount);
    for (int i = 0; i < infected_amount; ++i) {
        scanf("%d", &infected_one);
        infected[infected_one] = true;
        infected_people[i] = infected_one;
    }
    for (int i = 0; i < m; ++i) {
        scanf("%d", &attendent_amount);
        for (int j = 0; j < attendent_amount; ++j) {
            scanf("%d", &attendent);
            attended_party[attendent].push_back(i);
            party_attendent[i].push_back(attendent);
        }
    }
    infect(infected_people);
    printf("%d\n", get_bultshit_days());
}