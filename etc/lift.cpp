#include <iostream>
#include <vector>
#include <algorithm>
#include <stdint.h>

using namespace std;
using ll=long long;

vector<int> floors;
vector<int> stop_floors;

ll time_down(ll f,ll x){
    return (f-x)*20+(f-1)*4+stop_floors.size()*10;
}

int find_highest(int x,int time_limit){
    int y=0,h=floors.back();
    while (y<h){
        int mid=(y+h+1)/2;
        int f=mid;
        
        if (time_down(f,x) <= time_limit){
            y=mid;
        }else{
            h=mid-1;
        }
    }

    if (time_down(y,x) <= time_limit)
        return y;
    else
        return -1;

}

bool test(int t){
    stop_floors.clear();
    int id=0;
    while(id<floors.size()){
        int down=stop_floors.empty()?1:stop_floors.back();
        int x=floors[id];
        ll t_temp=(x-down)*20+(down-1)*4+(stop_floors.empty()?0:stop_floors.size()-1)*10;
        if (t_temp<=t){
            id++;
            continue;
        }

        int y=find_highest(x,t);
        if (y>=x){
            while ( id<floors.size() && floors[id]<=y) id++;
            stop_floors.push_back(y);
            continue;
        }else{
            return false;
        }
    }
    
    return true;

}

bool solve(){
    int n;
    cin>>n;
    if (n==0) return false;

    floors.resize(n);

    stop_floors.clear();
    stop_floors.reserve(n);
    for (auto& it : floors)
        cin>>it;
    sort(floors.begin(),floors.end());
    
    ll l=0,r=INT32_MAX;
    while (l<r){
        int mid=(l+r)/2;
        
        if (test(mid)){
            r=mid;
        }else{
            l=mid+1;
        }
    }

    cout<<l<<"\n";
    return true;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    while (solve());

    return 0;
}