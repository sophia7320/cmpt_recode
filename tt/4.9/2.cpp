#include <stdint.h>

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;
using ll = long long;
using vll = vector<ll>;

using pq = priority_queue<int>;

ll n, t;

vector<string> v;

void _() {
    while (true) {
        char c; string tp;
        while ((c = cin.get()) != ' ' && c != '.') tp += c;
        v.push_back(tp);
        if (c == '.') break;
    }

    for (int i = 0;i < v.size();i++) {
        cout << v[i].length();
        if (i != n - 1) cout << " ";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cout.tie(nullptr);
    cin.tie(nullptr);

    int t = 1;
    // cin>>t;
    while (t--) _();
    cout.flush();

    system("pause");
}