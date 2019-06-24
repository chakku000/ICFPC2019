#ifndef __POI__
#define __POI__

#include <vector>
#include <utility>
#include <set>
#include <algorithm>
#include <map>
using namespace std;


template<typename T> ostream& operator<<(ostream& os,const vector<T>& vec){ os << "["; for(const auto& v : vec){ os << v << ","; } os << "]"; return os; }
template<typename T,typename U> ostream& operator<<(ostream& os, const map<T,U>& mp){ os << "{"; for(auto& p : mp){ os << p << ","; } os << "}"; return os; }
template<typename T,typename U> ostream& operator<<(ostream& os,const pair<T,U>& p){ os << "(" << p.first << ","<< p.second <<")"; return os; }
template<typename T> ostream& operator<<(ostream& os,const set<T>& st){ os<<"{"; for(T v:st) os<<v<<","; os <<"}"; return os; }

#define rep(i,n) for(int i=0;i<(int)(n);i++)
#define all(v) (v).begin(),(v).end()

using pii = std::pair<int,int>;
using P = std::pair<int,pii>;


const int dx[]={1,0,-1,0} ,dy[] = {0,1,0,-1};
const char word[] = {'D','W','A','S'};
constexpr int INF = numeric_limits<int>::max()/2;


int offset_x,offset_y;
int w,h;
int sx,sy;
vector<vector<char>> table;

//constexpr int BEAM = 8;
//constexpr int DEPTH = 30;


char direction(const pii& p1,const pii& p2){
    for(int i=0;i<4;i++){
        if(p1.first + dx[i] == p2.first and p1.second + dy[i] == p2.second){
            return word[i];
        }
    }
    assert(false);
}

int char_to_index(char c){
    for(int i=0;i<4;i++){
        if(word[i] == c){
            return i;
        }
    }
    assert(false);
}

bool valid_index(int x,int y){ return !(x<0 or y<0 or x>=w or y>=h); }

struct Action{
    char action;
    int x,y;
    Action(char c): action(c){}
    Action(char c,int xx,int yy) : action(c) , x(xx) , y(yy){}
};
ostream& operator<<(ostream& os, const Action& action){
    os << "Action(" << action.action<< ")";
    return os;
}

struct State{
public:
    int x,y;
    int value;
    int wheel=0;
    /* int drill=0; */
    int arm=0;
    int wheelturn=0;
    /* int drillturn=0; */
    vector<vector<bool>> visit;
    vector<pii> arms;
    vector<Action> actions;

    int eval()const{
        return value + actions.size() - arms.size();
    }

    bool operator<(const State& st)const{
        return eval() < st.eval();
    }
    void init(){
        visit.assign(w,vector<bool>(h,false));
    }
    pii add_arm(){
        if(arms.size() == 3) arms.push_back(pii(-1,0));
        else if(arms.size() == 4) arms.emplace_back(pii(-1,1));
        else if(arms.size() == 5) arms.emplace_back(pii(-1,-1));
        else if(arms.size() == 6) arms.emplace_back(pii(0,1));
        else if(arms.size() == 7) arms.emplace_back(pii(0,-1));
        else{
            arms.emplace_back(arms.size()-6,0);
        }
        return arms[arms.size()-1];
    }

    // 今いる場所から触れる領域を計算する
    void touch(){
        int cnt = 0;
        if(!visit[x][y]) cnt++;
        visit[x][y] = true;
        if(table[x][y]=='B'){
            table[x][y] = '.';
            arm++;
        }
        if(table[x][y]=='F'){
            table[x][y] = '.';
            wheel++;
        }

        // 手が届く範囲を計算
        for(const auto& p : arms){
            int nx = x + p.first;
            int ny = y + p.second;
            if(!valid_index(nx,ny)) continue;
            if(table[nx][ny] == '#') continue;
            if(p.first==0||p.second==0){ // 上下左右に伸びている
                if(p.first==0){
                    int sy = min(y,ny);
                    int gy = max(y,ny);
                    bool ok = true;
                    for(int yy=sy;yy<=gy;yy++){
                        if(table[x][yy] == '#'){
                            ok = false; break;
                        }
                    }
                    if(ok){
                        for(int yy=sy;yy<=gy;yy++){
                            if(!visit[x][yy]) cnt++;
                            visit[x][yy] = true;
                        }
                    }
                }else if(p.second==0){
                    int sx = min(x,nx);
                    int gx = max(x,nx);
                    bool ok = true;
                    for(int xx=sx;xx<=gx;xx++){
                        if(table[xx][y]=='#'){
                            ok = false; break;
                        }
                    }
                    if(ok){
                        for(int xx=sx;xx<=gx;xx++){
                            if(!visit[xx][y]) cnt++;
                            visit[xx][y] = true;
                        }
                    }
                }
            }else{  // 斜めにある
                if(!visit[nx][ny]) cnt++;
                visit[nx][ny] = true;
            }
        }
        value -= cnt;
    }
};

void print(State& s){
    cerr << "(" << s.x << "," << s.y << ") , " << "value = " << s.value << " actions = " << s.actions << endl;
}

#endif
