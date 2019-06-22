#include <bits/stdc++.h>
using namespace std;
/*{{{*/  //template
#define rep(i,n) for(int i=0;i<(int)(n);i++)
#define rrep(i,n) for(int i=(n)-1;i>=0;i--)
constexpr int INF = numeric_limits<int>::max()/2;
constexpr long long LINF = numeric_limits<long long>::max()/3;
#define pb push_back
#define eb emplace_back
#define fi first
#define se second
#define all(v) (v).begin(),(v).end()
#define sz(x) (int)(x).size()
#define debug(x) cerr<<#x<<":"<<x<<endl
#define debug2(x,y) cerr<<#x<<","<<#y":"<<x<<","<<y<<endl
//struct fin{ fin(){ cin.tie(0); ios::sync_with_stdio(false); } } fin_;
struct Double{ double d; explicit Double(double x) : d(x){} };
ostream& operator<<(ostream& os,const Double x){ os << fixed << setprecision(20) << x.d; return os; }
template<typename T> ostream& operator<<(ostream& os,const vector<T>& vec){ os << "["; for(const auto& v : vec){ os << v << ","; } os << "]"; return os; }
template<typename T,typename U> ostream& operator<<(ostream& os, const map<T,U>& mp){ os << "{"; for(auto& p : mp){ os << p << ","; } os << "}"; return os; }
template<typename T,typename U> ostream& operator<<(ostream& os,const pair<T,U>& p){ os << "(" << p.first << ","<< p.second <<")"; return os; }
template<typename T> ostream& operator<<(ostream& os,const set<T>& st){ os<<"{"; for(T v:st) os<<v<<","; os <<"}"; return os; }
template<typename T,typename U> inline void chmax(T &x,U y){ if(y>x) x = y; }
template<typename T,typename U> inline void chmin(T &x,U y){ if(y<x) x = y; }
typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef vector<vi> vvi;
ll gcd(ll a,ll b){ if(b==0) return a; else return gcd(b,a%b); }
//constexpr double eps = 1e-14; 
constexpr double eps = 1e-10; 
constexpr ll mod = 1e9+7;
const int dx[]={1,0,-1,0} ,dy[] = {0,1,0,-1};
/*}}}*/

using pii = pair<int,int>;



pii read_point(const string& s,int& cur){
    assert(s[cur] == '(');
    cur++;

    int x = 0;
    while('0' <= s[cur] and s[cur] <= '9'){
        x *= 10;
        x += s[cur] - '0';
        cur++;
    }
    assert(s[cur] == ',');
    cur++;
    int y=0;
    while('0' <= s[cur] and s[cur] <= '9'){
        y *= 10;
        y += s[cur] - '0';
        cur++;
    }
    assert(s[cur] == ')');
    cur++;
    return pii(x,y);
}


vector<pii> read_map(const string& s,int& cur){
    vector<pii> res;
    while(s[cur]!='#'){
        if(s[cur] == ',') cur++;
        res.emplace_back(read_point(s,cur));
    }
    cur++;
    return res;
}

vector<vector<pii>> read_obstacles(const string& s,int& cur){
    assert(s[cur]=='#');
    cur++;
    vector<vector<pii>> res;
    while(s[cur]=='('){
        vector<pii> obst;
        while(s[cur]=='('){
            obst.push_back(read_point(s,cur));
            if(s[cur] == ',') cur++;
        }
        res.push_back(obst);
        if(s[cur] == ';') cur++;
    }
    return res;
}

void fill_wall(vector<vector<char>>& table,vector<pii>& maps){
    int max_x = table.size();
    int max_y = table[0].size();
    for(int i=0;i<(int)maps.size();i++){
        int j = (i+1)%maps.size();
        int sx,sy,gx,gy;
        tie(sx,sy) = maps[i];
        tie(gx,gy) = maps[j];
        /* cout << maps[i] << " "<< maps[j] << endl; */
        /* cout << maps[i] << " " << maps[j] << endl; */
        /* debug2(sx,gx); */
        if(sy == gy){
            /* cout << "P1" << endl; */
            if(sx < gx){
                if(sy-1>=0){
                    for(int x = sx; x<gx; x++){
                        /* cout << x << " " << sy-1 << endl; */
                        if(x<max_x) table[x][sy-1] = '#';
                    }
                }
            }else{
                for(int x=sx-1;x>=gx;x--){
                    /* cout << x <<" " << sy << endl; */
                    if(x<max_x and sy<max_y) table[x][sy] = '#';
                }
            }
        }else{
            /* cout << "P2" << endl; */
            if(sy < gy){
                for(int y=sy;y<gy;y++){
                    cout << sx << " " << y << endl;
                    if(sx<max_x and y<max_y) table[sx][y] = '#';
                }
            }else{
                if(sx-1>=0){
                    for(int y=sy-1; y>=gy; y--){
                        /* cout << sx-1 << " " << y << endl; */
                        if(sx-1<max_x and y<max_y) table[sx-1][y] = '#';
                    }
                }
            }
        }
    }

}

void fill_obstacle(vector<vector<char>>& table,vector<pii>& maps){
}


int main(int argc,char** argv){
    if(argc < 2){
        cerr << "Invalid args" << endl;
        return 1;
    }

    ifstream ifs(argv[1]);
    string input;
    ifs >> input;

    int cur = 0;
    auto maps = read_map(input,cur);

    int offset_x = 0;
    int offset_y = 0;
    for(auto& p : maps){
        offset_x = min(offset_x,p.first);
        offset_y = min(offset_y,p.second);
    }

    cout << "offset " << offset_x << " " << offset_y << endl;

    for(auto& p : maps){
        p.first -= offset_x;
        p.second -= offset_y;
    }

    int max_x = 0;
    int max_y = 0;
    for(auto& p : maps){
        max_x = max(max_x,p.first);
        max_y = max(max_y,p.second);
    }

    cout << "size " << max_x << " " << max_y << endl; 

    vector<vector<char>> table(max_x,vector<char>(max_y,'?'));
    fill_wall(table,maps);

    /* cout << maps << endl; */

    auto start = read_point(input,cur);

    cout << "start " << start.first << " " << start.second << endl;

    vector<vector<pii>> obstacles = read_obstacles(input,cur);
    cout << obstacles << endl;
    for(auto& ps : obstacles){
        /* cout << ps << endl; */
        fill_wall(table,ps);
    }


    queue<pii> que;
    vector<vector<bool>> visited(max_x,vector<bool>(max_y,false));
    visited[start.first][start.second] = true;
    que.push(start);
    while(!que.empty()){
        int x = que.front().first;
        int y = que.front().second;
        que.pop();

        for(int i=0;i<4;i++){
            int nx = x + dx[i];
            int ny = y + dy[i];
            if(nx < 0 or ny < 0 or nx >= max_x or ny >= max_y) continue;
            if(visited[nx][ny]) continue;
            if(table[nx][ny] == '#') continue;

            visited[nx][ny] = true;
            que.push(pii(nx,ny));
        }
    }

    for(int i=0;i<max_x;i++){
        for(int j=0;j<max_y;j++){
            if(visited[i][j]){
                table[i][j] = '.';
            }
        }
    }

    return 0;
    for(int y=max_y-1;y>=0;y--){
        for(int x=0;x<max_x;x++){
            cout << table[x][y];
        }
        cout << endl;
    }
}
