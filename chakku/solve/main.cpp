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

constexpr int BEAM = 8;
constexpr int DEPTH = 30;

char direction(const pii& p1,const pii& p2){
    for(int i=0;i<4;i++){
        if(p1.first + dx[i] == p2.first and p1.second + dy[i] == p2.second){
            return word[i];
        }
    }
    //assert(false);
    cout << p1 << " " << p2 << endl;
    //exit(0);
    return 'X';
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
        print(state);
        cerr << dx << " " << dy << endl;
        cerr << "Invalid Move" << endl;
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
    if(table[x][y] == '#') return;
    dist[x][y] = P(d,pii(px,py));

    rep(i,4){
        int nx = x + dx[i];
        int ny = y + dy[i];
        if(!valid_index(nx,ny)) continue;
        dfs(nx,ny,x,y,d+1);
    }
}

vector<vector<P>> bfs(int x,int y){
    vector<vector<P>> dist(w,vector<P>(h,P(INF,pii(-1,-1))));
    queue<P> que;
    que.push(P(0,pii(x,y)));
    dist[x][y] = P(0,pii(x,y));
    while(!que.empty()){
        int d = que.front().first;
        int x = que.front().second.first;
        int y = que.front().second.second;
        que.pop();

        rep(i,4){
            int nx = x + dx[i];
            int ny = y + dy[i];
            if(!valid_index(nx,ny)) continue;
            if(table[nx][ny] == '#') continue;
            if(dist[nx][ny].first != INF) continue;

            dist[nx][ny] = P(d+1,pii(x,y));
            que.push(P(d+1,pii(nx,ny)));
        }
    }
    return dist;
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
    while(++turn < h*w*2){
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


    /* cerr << "\nFinish Beam" << endl; */

    while(now.wheelturn>0){
        for(int j=0;j<4;j++){
            int nx1 = now.x + dx[j];
            int ny1 = now.y + dy[j];
            if(!valid_index(nx1,ny1)){
                continue;
            }
            if(table[nx1][ny1] == '#'){
                continue;
            }

            update_move(now,dx[j],dy[j]);
            cout << word[j];
            break;
        }
    }

    // 残りはBFSで探索しきる
    while(1){
        auto dist = bfs(now.x,now.y);
        int to = INF;
        pii p(-1,-1);
        rep(i,w) rep(j,h){
            if(dist[i][j].first < to and !now.visit[i][j]){
                to = dist[i][j].first;
                p = pii(i,j);
            }
        }

        /* cerr << "from " << endl; */
        /* print(now); */

        /* cerr << "to " << endl; */
        /* cerr << p << endl; */

        vector<char> vc;
        if(p.first == -1 and p.second == -1) return 0;
        while(p != dist[p.first][p.second].second){
            pii pa = dist[p.first][p.second].second;
            if(p == pa) break;
            vc.push_back(direction(pa,p));
            p = pa;
            assert( p != pii(-1,-1));
        }

        /* cout << endl << "----" << endl; */
        for(int i=vc.size()-1;i>=0;i--){
            cout << vc[i];
            if(vc[i] == 'D') update_move(now,1,0);
            if(vc[i] == 'W') update_move(now,0,1);
            if(vc[i] == 'A') update_move(now,-1,0);
            if(vc[i] == 'S') update_move(now,0,-1);
        }
        /* cout << endl << "----" << endl; */
        /* cerr << "result "; */
        /* print(now); */
    }
}
