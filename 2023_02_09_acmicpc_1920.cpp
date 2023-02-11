#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int n;
vector<int> input_number;

bool b_search(int num)
{
    int left = 0;
    int right = n - 1;

    while (left < right) {
        int mid = (left + right) / 2;

        if (input_number[mid] < num) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    if (input_number[left] == num) {
        return true;
    }
    return false;
}

int main()
{
    int m;
    int number;

    scanf("%d\n", &n);
    input_number.resize(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &(input_number[i]));
    }
    sort(input_number.begin(), input_number.end());
    scanf("%d", &m);
    for (int i = 0; i < m; ++i) {
        scanf("%d", &number);

        printf("%d\n", (int)b_search(number));
    }
}