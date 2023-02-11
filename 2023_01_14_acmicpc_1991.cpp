#include <iostream>

using namespace std;

/**
 * @brief 
 * 
 */

#define LAST_ALPHABET 'Z'

int links[LAST_ALPHABET + 1][2];

void print_pre_order(int node)
{
    printf("%c", node);
    if (links[node][0] > 0) {
        print_pre_order(links[node][0]);
    }
    if (links[node][1] > 0) {
        print_pre_order(links[node][1]);
    }
}

void print_in_order(int node)
{
    if (links[node][0] > 0) {
        print_in_order(links[node][0]);
    }
    printf("%c", node);
    if (links[node][1] > 0) {
        print_in_order(links[node][1]);
    }
}

void print_post_order(int node)
{
    if (links[node][0] > 0) {
        print_post_order(links[node][0]);
    }
    if (links[node][1] > 0) {
        print_post_order(links[node][1]);
    }
    printf("%c", node);

}

int main(void)
{
    int n;
    char a, b, c;

    scanf("%d\n", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%c %c %c\n", &a, &b, &c);
        if (b != '.') {
            links[a][0] = b;
        }
        if (c != '.') {
            links[a][1] = c;
        }
    }
    print_pre_order('A');
    printf("\n");
    print_in_order('A');
    printf("\n");
    print_post_order('A');
    printf("\n");
}