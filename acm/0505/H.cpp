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


void _() {
    ll n;
    cin >> n;
    if (n % 3 == 0) {
        cout << 1 << ' ' << n / 3 << '\n';
    }
    else {
        cout << 0 << ' ' << n % 3 + n / 3 << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr), cout.tie(nullptr);

    int t = 1;
    cin >> t;
    while (t--) _();

    cout.flush();
}