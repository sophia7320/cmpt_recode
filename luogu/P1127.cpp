#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <stdint.h>
#include <string>
#include <climits>

#define endl "\n"

using namespace std;
using ll = long long;
using vll = vector<ll>;
using vint = vector<int>;

vector<string> vs;
vector<bool> vis(9999);

priority_queue<string, vector<string>, greater<string>> pq;

vector<string> ans;

void _() {
    int n;cin >> n;
    ans.reserve(n);
    while (n--) {
        string s;cin >> s;
        pq.push(move(s));
    }

    queue<string> q;

    while (!pq.empty()) {
        auto t = move(const_cast<string&>(pq.top()));pq.pop();
        q.push(move(t));
    }

    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0;i < sz;i++) {
            auto t = move(const_cast<string&>(q.front()));q.pop();

            if (ans.back().back() == t[0]) {
                ans.push_back(move(t));
                break;
            }
        }

        break;
    }

    if (q.size()) {

    }


}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--) _();

    cout.flush();

    system("pause");
}