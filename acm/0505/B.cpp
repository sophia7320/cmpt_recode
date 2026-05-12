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
    int arr[6];
    int ans = 0;
    for (int i = 1;i <= 5;i++) cin >> arr[i];
    int i = 1, j = 5;
    while (i < j) {
        int t = min(arr[i], arr[j]);
        ans += t;
        arr[i] -= t;
        arr[j] -= t;
        i++;
        j--;
    }
    ans += arr[3] / 2;
    arr[3] %= 2;
    for (int i = 1;i < 5 && arr[5];i++) {
        int t = min(arr[i], arr[5]);
        ans += t;
        arr[i] -= t;
        arr[5] -= t;
    }
    ans += arr[5] / 2;
    int t = min(arr[1] / 2, arr[4]);
    ans += t;
    arr[1] -= t * 2;
    arr[4] -= t;

    if (arr[4] && arr[3] && arr[1]) {
        arr[3]--;
        arr[1]--;
        arr[4]--;
        ans++;
    }

    ans += arr[4] / 3;
    arr[4] %= 3;
    if (arr[4] % 3 == 2 && arr[3]) {
        ans++;
        arr[3]--;
    }
    if (arr[4] % 3 == 2 && arr[1]) {
        ans++;
        arr[1]--;
    }
    if (arr[3] && arr[2] && arr[1]) {
        ans++;
        arr[2]--;
        arr[1]--;
    }
    else if (arr[3] && arr[1] / 3) {
        ans++;
        arr[1] -= 3;
    }
    else if (arr[3] && arr[2] / 2) {
        ans++;
        arr[2] -= 2;
    }
    ans += (arr[2] * 2 + arr[1]) / 6;
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr), cout.tie(nullptr);

    int t = 1;
    cin >> t;
    while (t--) _();

    cout.flush();
}