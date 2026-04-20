#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

#define endl "\n"

using namespace std;
using ll = long long;
using vll = vector<ll>;

string s;

void op1(string& s1) {
    int t = 0;
    vll ans;
    for (int i = 0;i < s.length();i++)
        if (s[i] == s1[0])
            if (s.substr(i, s1.length()) == s1)
                ans.push_back(i);
    if (ans.empty()) {
        cout << -1 << endl;
        return;
    }

    for (int i = 0;i < ans.size() && i < 3;i++) {
        cout << ans[i];if (i != ans.size() - 1 && i != 2) cout << " ";
    }
    cout << endl;
}

void op2(int p, string& s2) {
    s = s.substr(0, p) + s2 + s.substr(p);
    cout << s << endl;
}

void op3(int l, int r) {
    reverse(s.begin() + l, s.begin() + r + 1);
    cout << s << endl;
}

void _() {
    int n;cin >> n;
    cin >> s;
    while (n--) {
        int op;cin >> op;
        if (op == 1) {
            string t;cin >> t;
            op1(t);
        }
        else if (op == 2) {
            int p;string s2;cin >> p >> s2;
            op2(p, s2);
        }
        else {
            int l, r;cin >> l >> r;
            op3(l, r);
        }
    }
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