#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;
using ll = long long;
using vll = vector<ll>;

int c[111000];

struct S {
    vector<int> books; int csum = 0;

    bool empty() { return books.empty(); };
};


void _() {
    int n, t;cin >> n >> t;
    for (int i = 1;i <= n;i++)cin >> c[i];

    S L, R, T;
    for (int i = n;i >= 1;i--) T.books.push_back(i), T.csum += c[i];

    vector<int> ans;ans.reserve(n + 1);

    while (!T.empty()) {
        while (!T.empty()) {
            auto book = T.books.back();T.books.pop_back();
            if (c[book] > t) {
                L.books.push_back(book), L.csum += c[book];
            }
            else {
                ans.push_back(book);
            }
        }

        if (!L.empty()) {
            t = L.csum / L.books.size();
            T = move(L);L = S();
        }
    }

    for (int i = 0;i < n;i++) {
        cout << ans[i];if (i != n - 1)cout << " ";
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