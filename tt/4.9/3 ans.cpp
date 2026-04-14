//来自赵祖恩的代码
#include<iostream>
#include<string>
#include<algorithm>
#include<cmath>
typedef long long ll;
#define code using
#define by namespace
#define zze std
code by zze;
ll arr[100005];//记录各个篮子里苹果个数
int main() {
    ll n, k;
    cin >> n >> k;//n个篮子，k个苹果
    for (int i = 1; i <= n; i++)
        cin >> arr[i];
    ll a = n;//假设每个有苹果篮子都吃一个，则走一轮要吃掉a个苹果，初始假设当前每个篮子都有苹果
    while (k != 0) {
        ll cnt = k / a;//当还剩k个苹果的时候要走至少cnt轮才能吃完，则代表每个篮子都要减少cnt个苹
        //果
        a = 0;//用a记录当前有多少个篮子里面有苹果
        if (!cnt)//如果要走0轮，说明当前只需要在k不为零时走过的篮子都吃一个苹果，将cnt设为
            cnt++;
        for (int i = 1; i <= n && k != 0; i++) {
            if (arr[i] >= cnt) {
                arr[i] -= cnt;
                k -= cnt;
                if (arr[i])//如果吃完后该篮子里的苹果还有多余的，将记录当前有多少个篮子有苹
                    a++;
            }
            else {
                k -= arr[i];
                arr[i] -= arr[i];
            }
        }
    }
    for (int i = 1; i <= n; i++)//输出最后每个篮子里剩余苹果数量
        cout << arr[i] << ' ';
}
//来自麦思