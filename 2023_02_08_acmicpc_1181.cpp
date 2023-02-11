#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

vector<string> input_string;

int my_compare(string a, string b)
{
    if (a.size() == b.size()) {
        return a < b;
    }
    return a.size() < b.size();
}


int main()
{
    int n;
    string last_string = "";

    cin >> n;
    input_string.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> input_string[i];
    }
    sort(input_string.begin(), input_string.end(), my_compare);
    for (int i = 0; i < n; ++i) {
        if (input_string[i] == last_string) {
            continue;
        }
        last_string = input_string[i];
        cout << input_string[i] << "\n";
    }
}