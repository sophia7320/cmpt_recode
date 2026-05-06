#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <stdint.h>
#include<map>

using namespace std;
using ll = long long;
using vll = vector<ll>;
using vint = vector<int>;

struct p {
	ll x, y;

};

map<char, pair<int, int>>mp = {
	{'U',{0,1}},
	{'D',{0,-1}},
	{'L',{-1,0}},
	{'R',{1,0}}
};

void _() {
	ll  n, k;
	cin >> n >> k;
	string str;
	vector<p>arr(n);
	cin >> str;
	auto [ta, tb] = mp[str[0]];
	arr[0].x = ta;
	arr[0].y = tb;
	for (int i = 1;i < n;i++) {
		auto [dx, dy] = mp[str[i]];
		arr[i].x = arr[i - 1].x + dx;
		arr[i].y = arr[i - 1].y + dy;
	}
	ll ans = 0;
	for (int i = 0;i < n;i++) {
		ll t = max(abs(arr[i].x) + abs(arr[i].y), abs(arr[i].x + (k - 1) * arr[n - 1].x) + abs(arr[i].y + (k - 1) * arr[n - 1].y));
		ans = max(ans, t);
	}
	cout << ans << endl;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);

	int t = 1;
	//cin>>t;
	while (t--) _();
	cout.flush();
}