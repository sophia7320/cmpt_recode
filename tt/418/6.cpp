#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;
using ll = long long;
using vll = vector<ll>;

void _() {
    int n = 11;
    vll ans;
    while (n--) {
        string s;getline(cin, s);
        ans.push_back(s.size());
    }
    for (auto i : ans)cout << i;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    //cin >> t;
    while (t--) _();

    cout.flush();

    system("pause");
}