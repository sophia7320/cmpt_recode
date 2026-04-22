#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <cstring>
using namespace std;

typedef long long ll;
// 修正1：MAXN 定义为正整数 20
const int MAXN = 40;
const ll INF = 1e18;

// 修正2：数组大小匹配 MAXN，避免越界
ll dp[1 << MAXN];
ll mask[MAXN + 1];
vector<int> son[MAXN + 1];
ll c[MAXN + 1];
int n;

// 预处理每个节点的污染范围（递归版，安全可控）
void dfs_mask(int u, ll& st) {
    st |= (1LL << (u - 1)); // 标记当前节点（u从1开始，位移u-1）
    for (int v : son[u]) {
        ll tmp = st;
        dfs_mask(v, tmp); // 递归污染子节点
    }
    mask[u] = st;
}

// 记忆化搜索DP（带边界判断，防越界）
ll dfs_dp(ll S) {
    // 所有节点污染，乘积初始值为1
    if (S == (1LL << n) - 1) {
        return 1;
    }
    // 状态已计算，直接返回
    if (dp[S] != -1) {
        return dp[S];
    }

    ll res = INF;
    // 遍历所有未污染节点，尝试建厂
    for (int u = 1; u <= n; ++u) {
        // 节点已污染，跳过
        if ((S >> (u - 1)) & 1) {
            continue;
        }
        // 计算新状态和本轮新增污染数
        ll newS = S | mask[u];
        int pollute_cnt = __builtin_popcountll(mask[u] & (~S));

        // 计算罚款 c[u]^pollute_cnt（避免浮点误差）
        ll cost = 1;
        for (int i = 0; i < pollute_cnt; ++i) {
            cost *= c[u];
        }
        // 递归计算后续状态，取最小值
        res = min(res, cost * dfs_dp(newS));
    }
    return dp[S] = res;
}

int main() {
    // 修正3：初始化dp数组为-1（未计算状态）
    memset(dp, -1, sizeof(dp));

    // 输入节点数
    cin >> n;
    // 输入树的父子关系（构建邻接表）
    for (int i = 2; i <= n; ++i) {
        int f;
        cin >> f;
        son[f].push_back(i);
    }
    // 输入每个节点的罚款系数
    for (int i = 1; i <= n; ++i) {
        cin >> c[i];
    }

    // 预处理每个节点建厂的污染掩码
    for (int i = 1; i <= n; ++i) {
        ll st = 0;
        dfs_mask(i, st);
    }

    // 从初始状态（无节点污染）开始计算
    cout << dfs_dp(0) << endl;

    system("pause");
    return 0;
}