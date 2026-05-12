#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;
using ll = long long;
using vll = vector<ll>;
using vint = vector<int>;

#define endl "\n"

vector<vint>arr;
vector<vector<char>>grid;
template<typename T>
void print(T& v) {
    for (auto& it : v)
        cout << it << " ";
    cout << endl;
}
int n, m;

map<char, pair<int, int>>mp = {
    {'U',{-1,0}},
    {'C',{0,0}},
    {'D',{1,0}},
    {'R',{0,1}},
    {'L',{0,-1}}
};

bool isingrid(int i, int j) {
    return i >= 1 && i <= n && j >= 1 && j <= m;
}

bool solve(int i, int j) {
    if (mp.count(grid[i][j])) {
        auto [di, dj] = mp[grid[i][j]];
        int nexti = i + di, nextj = j + dj;
        if (!isingrid(nexti, nextj)) return false;
        if (abs(di)) {
            if ((arr[nexti][nextj] >> 1) & 1) return false;
            arr[nexti][nextj] |= (1 << 1);
        }
        else if (abs(dj)) {
            if ((arr[nexti][nextj] >> 2) & 1) return false;
            arr[nexti][nextj] |= (1 << 2);
        }
        else arr[nexti][nextj]++;
    }
    else if (grid[i][j] != '.') return false;
    return true;
}

bool check(int i, int j) {
    if (grid[i][j] == '.') return i == 1 && j == m && arr[i][j] == 0;
    if (i == 1 && j == m) return grid[i][j] == '.' && arr[i][j] == 0;
    if (grid[i][j] == 'C') return arr[i][j] == 7;
    return arr[i][j] == 0;
}

void _() {

    cin >> n >> m;
    arr.assign(n + 2, vector<int>(m + 2, -1));
    grid.assign(n + 2, vector<char>(m + 2, '#'));
    for (int i = 1;i <= n;i++) {
        for (int j = 1;j <= m;j++) {
            arr[i][j] = 0;
        }
    }
    for (int i = 1;i <= n;i++) {
        for (int j = 1;j <= m;j++) {
            cin >> grid[i][j];
        }
    }
    bool f = 1;
    for (int i = 1;i <= n;i++) {
        if (!f) break;
        for (int j = 1;j <= m;j++) {
            if (!solve(i, j)) { f = 0; break; };
        }
    }
    // 2
    //     4 4
    //     UUD.
    //     LDCL
    //     DCLD
    //     CLRC
    //     2 3
    //     DRC
    //     CLU
    if (f) {
        for (int i = 1;i <= n;i++) {
            if (!f) break;
            for (int j = 1;j <= m;j++) {
                if (!check(i, j)) { f = 0; break; };
            }
        }
    }
    if (f) cout << "Yes\n";
    else cout << "No\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr), cout.tie(nullptr);

    int t = 1;
    cin >> t;
    while (t--) _();

    cout.flush();
}