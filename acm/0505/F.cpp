#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using ll = long long;
using vll = vector<ll>;
using vint = vector<int>;

#define endl "\n"


template<typename T>
void print(T& v) {
    for (auto& it : v)
        cout << it << " ";
    cout << endl;
}

pair<double, double> gett(double x, double v, double l, double r, double ra) {
    double left, right;

    if (v == 0) {
        if (l <= x - ra && x + ra <= r)
            return { -9999999,9999999 };
        else
            return { 2,1 };
    }

    if (v > 0) {
        left = (l + ra - x) / v; right = (r - ra - x) / v;
    }
    else {
        right = (l + ra - x) / v; left = (r - ra - x) / v;
    }

    return { left,right };
}

void _() {
    double x, y, r, vx, vy;
    cin >> x >> y >> r >> vx >> vy;
    double lx, ly, rx, ry;
    cin >> lx >> ly >> rx >> ry;

    auto [reslx, resrx] = gett(x, vx, lx, rx, r);
    auto [resly, resry] = gett(y, vy, ly, ry, r);

    auto resl = max(reslx, resly);
    auto resr = min(resrx, resry);

    if (resl <= resr && resr >= 0)
        cout << "Yes" << endl;
    else
        cout << "No" << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr), cout.tie(nullptr);

    int t = 1;
    cin >> t;
    while (t--) _();

    cout.flush();
}