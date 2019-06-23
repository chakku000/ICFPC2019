#include <bits/stdc++.h>
using namespace std;
//template{{{
#define rep(i,n) for(int i=0;i<(int)(n);i++)
#define all(v) (v).begin(),(v).end()
template<typename T> ostream& operator<<(ostream& os,const vector<T>& vec){ os << "["; for(const auto& v : vec){ os << v << ","; } os << "]"; return os; }
template<typename T,typename U> ostream& operator<<(ostream& os, const map<T,U>& mp){ os << "{"; for(auto& p : mp){ os << p << ","; } os << "}"; return os; }
template<typename T,typename U> ostream& operator<<(ostream& os,const pair<T,U>& p){ os << "(" << p.first << ","<< p.second <<")"; return os; }
template<typename T> ostream& operator<<(ostream& os,const set<T>& st){ os<<"{"; for(T v:st) os<<v<<","; os <<"}"; return os; }
typedef pair<int,int> pii;/*}}}*/
const int dx[]={1,0,-1,0} ,dy[] = {0,1,0,-1};
const char word[] = {'D','W','A','S'};
constexpr int INF = numeric_limits<int>::max()/2;
using P = pair<int,pii>;
/*}}}*/

int offset_x,offset_y;
int w,h;
int sx,sy;
vector<vector<char>> table;

constexpr int BEAM = 10;
constexpr int DEPTH = 50;

char direction(const pii& p1,const pii& p2){
    for(int i=0;i<4;i++){
        if(p1.first + dx[i] == p2.first and p1.second + dy[i] == p2.second){
            return word[i];
        }
    }
    assert(false);
}

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

struct State{/*{{{*/
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
        return value + actions.size();
    }

    bool operator<(const State& st)const{
        return eval() < st.eval();
    }
    void init(){
        visit.assign(w,vector<bool>(h,false));
    }
};/*}}}*/

void print(State& s){
    cerr << "(" << s.x << "," << s.y << ") , " << "value = " << s.value << " actions = " << s.actions << endl;
}

bool valid_index(int x,int y){ return !(x<0 or y<0 or x>=w or y>=h); }


void update_move(State& state, int dx,int dy){/*{{{*/
    if(table[state.x+dx][state.y+dy] == '#'){
        cerr << "Invalid Move" << endl;
        cerr << state.x << " " << state.y <<" " << dx << " " << dy << " " << table[state.x+dx] << endl;
        exit(0);
    }
    if(valid_index(state.x+dx,state.y+dy) and table[state.x+dx][state.y+dy]=='#'){
        return;
    }


    state.x += dx;
    state.y += dy;

    int cnt = 0; // 到達になる場所の個数

    // bodyの通過
    if(!state.visit[state.x][state.y]) cnt++;
    if(table[state.x][state.y] == 'B') state.arm++;
    if(table[state.x][state.y] == 'F') state.wheel++;
    state.visit[state.x][state.y] = true;

    // 手が届く範囲
    for(const auto& p : state.arms){
        int x = state.x + p.first;
        int y = state.y + p.second;
        if(x<0 or y<0 or x>=w or y>=h) continue;
        if(table[x][y]!='#' and !state.visit[x][y]) cnt++;
        state.visit[x][y] = true;
    }

    // 倍速移動中
    if(state.wheelturn>0){
        if(valid_index(state.x+dx,state.y+dy) and table[state.x+dx][state.y+dy]!='#'){
            state.x += dx;
            state.y += dy;
            // bodyの通過
            if(!state.visit[state.x][state.y]) cnt++;
            if(table[state.x][state.y] == 'B') state.arm++;
            if(table[state.x][state.y] == 'F') state.wheel++;
            state.visit[state.x][state.y] = true;

            // 手が届く範囲
            for(const auto& p : state.arms){
                int x = state.x + p.first;
                int y = state.y + p.second;
                if(x<0 or y<0 or x>=w or y>=h) continue;
                if(table[x][y]!='#' and !state.visit[x][y]) cnt++;
                state.visit[x][y] = true;
            }
        }
        state.wheelturn--;
    }
    state.value -= cnt;
}/*}}}*/

vector<vector<P>> dist;
void dfs(int x,int y,int px,int py,int d){
    if(dist[x][y].first!=INF) return;
    dist[x][y] = P(d+1,pii(px,py));

    rep(i,4){
        int nx = x + dx[i];
        int ny = y + dy[i];
        if(!valid_index(nx,ny)) continue;
        if(table[nx][ny] == '#') continue;
        dfs(nx,ny,x,y,d+1);
    }
}

void traverse(State& now){
    pii nowp = pii(now.x,now.y);
    rep(i,4){
        int nx = now.x + dx[i];
        int ny = now.y + dy[i];
        if(!valid_index(nx,ny)) continue;
        if(now.visit[nx][ny]) continue;
        if(table[nx][ny] == '#') continue;

        if(dist[nx][ny].second == nowp){
            cout << direction(nowp,pii(nx,ny));
            update_move(now,dx[i],dy[i]);
            traverse(now);
            cout << direction(pii(nx,ny),nowp);
            update_move(now,-dx[i],-dy[i]);
        }
    }
}

int main(){
    string tmp;
    cin >> tmp >> offset_x >> offset_y;
    cin >> tmp >> w >> h;
    cin >> tmp >> sx >> sy;

    table = vector<vector<char>>(w,vector<char>(h));
    {
        vector<string> tmp(h);
        rep(i,h) cin >> tmp[i];

        rep(i,h) rep(j,w) table[j][h-1-i] = tmp[i][j];
    }

    deque<State> deq;

    State initial;
    initial.init();
    initial.x = sx;
    initial.y = sy;
    initial.arms = vector<pii>{pii(1,1),pii(1,0),pii(1,-1)};
    initial.visit[sx][sy] = true;
    for(pii p : initial.arms){
        int x = initial.x + p.first;
        int y = initial.y + p.second;
        if(valid_index(x,y) && table[x][y] != '#'){
            initial.visit[x][y] = true;
        }
    }
    initial.value = 0;
    for(int i=0;i<w;i++){
        for(int j=0;j<h;j++){
            if(table[i][j] != '#' && !initial.visit[i][j]) initial.value++;
        }
    }
    deq.push_front(initial);

    State now(initial);

    int turn = 0;
    // あんまりturn数を多くしても無駄が生じるのでパラメータを上手く定める関数が必要
    while(++turn < h*w*10){
        /* cerr << endl; */
        /* cerr << "turn = " << turn << endl; */
        /* cerr << table[now.x][now.y] << endl; */
        deq.clear();
        deq.push_back(now);
        for(int i=0;i<DEPTH;i++){
            deque<State> next_deq;
            while(!deq.empty()){
                State curr = deq.front();
                next_deq.push_back(curr);
                deq.pop_front();

                // 4方向へ移動{{{
                for(int k=0;k<4;k++){/*{{{*/
                    State next_state(curr);
                    if(!valid_index(curr.x+dx[k],curr.y+dy[k])) continue;
                    if(table[curr.x+dx[k]][curr.y+dy[k]] == '#') continue;
                    update_move(next_state,dx[k],dy[k]);

                    if(k==0) next_state.actions.push_back(Action('D'));
                    if(k==1) next_state.actions.push_back(Action('W'));
                    if(k==2) next_state.actions.push_back(Action('A'));
                    if(k==3) next_state.actions.push_back(Action('S'));
                    next_deq.push_back(next_state);
                }/*}}}*//*}}}*/

                if(curr.arm>0 and curr.arms.size()<4){/*{{{*/
                    State nxt(curr);
                    nxt.arms.emplace_back(pii(-1,0));
                    nxt.arm--;
                    nxt.actions.push_back(Action('B',-1,0));
                    next_deq.push_back(nxt);
                }/*}}}*/


                if(curr.wheel > 0){/*{{{*/
                    State nxt(curr);
                    nxt.wheel--;
                    nxt.wheelturn=50;
                    nxt.actions.push_back(Action('F'));
                    next_deq.push_back(nxt);
                }/*}}}*/

            }
            sort(all(next_deq));
            while(next_deq.size() > BEAM){
                next_deq.pop_back();
            }
            /* cerr << "depth " << i << " -> " << next_deq.size() << endl; */
            deq = move(next_deq);
        }

        // この状態になることがあるかは考える必要がある
        if(deq.empty()){
            break;
        }

        if(deq[0].value == now.value){
            break;
        }

        if(deq[0].actions[0].action == 'W'){
            cout << 'W';
            update_move(now,0,1);
        }
        else if(deq[0].actions[0].action == 'S'){
            cout << 'S';
            update_move(now,0,-1);
        }
        else if(deq[0].actions[0].action == 'A'){
            cout << 'A';
            update_move(now,-1,0);
        }
        else if(deq[0].actions[0].action == 'D'){
            cout << 'D';
            update_move(now,1,0);
        }
        else if(deq[0].actions[0].action == 'B'){
            cout << "B(" << deq[0].actions[0].x << "," << deq[0].actions[0].y << ")";
            now.arms.push_back(pii(deq[0].actions[0].x,deq[0].actions[0].y));
        }
        else if(deq[0].actions[0].action == 'F'){
            cout << "F";
            now.wheel--;
            now.wheelturn=50;
        }

        now.actions.clear();
    }


    cerr << "\nFinish Beam" << endl;
    return 0;

    // 残りはDFSで探索しきる
    dist = vector<vector<P>>(w,vector<P>(w,P(INF,pii(-1,-1))));
    dfs(now.x,now.y,now.x,now.y,0);

    // 未到達な場所へのリスト
    set<P> unreached;
    rep(i,w){
        rep(j,h){
            if(table[i][j]!='#' and !now.visit[i][j]){
                unreached.insert(dist[i][j]);
            }
        }
    }

    // setから要素を削除しながらloop回していいか不明

    while(!unreached.empty()){
        P elm = *unreached.begin();
        pii p = elm.second;

        while(!now.visit[p.first][p.second]){
            p = dist[p.first][p.second].second;
        }

        // pは既に到達しているが未到達領域と接している部分
        // pまで進めば良い
        // p -> nowまで遡る
        auto pp(p);
        vector<Action> va;
        while(pp.first != now.x || pp.second != now.y){
            pii pre = dist[pp.first][pp.second].second;
            va.emplace_back(direction(pre,pp));
            pp = pre;
        }
        for(Action& a : va){
            cout << a.action;
            if(a.action == 'W') update_move(now,0,1);
            if(a.action == 'S') update_move(now,0,-1);
            if(a.action == 'A') update_move(now,-1,0);
            if(a.action == 'D') update_move(now,1,0);
        }

        cout << direction(p,elm.second);
        update_move(now,elm.second.first-p.first,elm.second.second-p.second);
        p = elm.second;
        // now is p

        traverse(now);
        // unreachを再計算する

        dist = vector<vector<P>>(w,vector<P>(w,P(INF,pii(-1,-1))));
        dfs(now.x,now.y,now.x,now.y,0);

        rep(i,w){
            rep(j,h){
                if(table[i][j]!='#' and !now.visit[i][j]){
                    unreached.insert(dist[i][j]);
                }
            }
        }
    }
}
