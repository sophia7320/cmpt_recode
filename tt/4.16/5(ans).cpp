#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m;
    cin >> n >> m;
    int a[100001];//a[i]:编号为i的物品放在a[i]号箱子里
    for (int i = 1; i <= n; i++) cin >> a[i];
    int k;
    cin >> k;
    while (k--) {
        int x;
        cin >> x;
        int b[10001] = { 0 };//b[i]: 本轮查询中，编号为i的箱子被查询了多少次
        while (x != 0) {
            b[a[x]]++;//注意这里
            cin >> x;
        }
        int flag = 0;
        for (int i = 1; i <= 10000; i++) {
            if (b[i] != 0) {
                if (flag) cout << " ";
                printf("B%d-%d", i, b[i]);
                flag = 1;
            }
        }
        cout << endl;
    }
    return 0;
}
