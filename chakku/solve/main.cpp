#include <bits/stdc++.h>
#include "poi.hpp"
using namespace std;




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

    state.touch();
    // bodyの通過
    //if(!state.visit[state.x][state.y]) cnt++;
    //if(table[state.x][state.y] == 'B') state.arm++;
    //if(table[state.x][state.y] == 'F') state.wheel++;
    //state.visit[state.x][state.y] = true;

    //// 手が届く範囲
    //for(const auto& p : state.arms){
    //    int x = state.x + p.first;
    //    int y = state.y + p.second;
    //    if(x<0 or y<0 or x>=w or y>=h) continue;
    //    if(table[x][y]!='#' and !state.visit[x][y]) cnt++;
    //    state.visit[x][y] = true;
    //}

    // 倍速移動中
    if(state.wheelturn>0){
        if(valid_index(state.x+dx,state.y+dy) and table[state.x+dx][state.y+dy]!='#'){
            state.x += dx;
            state.y += dy;
            state.touch();
            // bodyの通過
            //if(!state.visit[state.x][state.y]) cnt++;
            //if(table[state.x][state.y] == 'B') state.arm++;
            //if(table[state.x][state.y] == 'F') state.wheel++;
            //state.visit[state.x][state.y] = true;

            //// 手が届く範囲
            //for(const auto& p : state.arms){
            //    int x = state.x + p.first;
            //    int y = state.y + p.second;
            //    if(x<0 or y<0 or x>=w or y>=h) continue;
            //    if(table[x][y]!='#' and !state.visit[x][y]) cnt++;
            //    state.visit[x][y] = true;
            //}
        }
        state.wheelturn--;
    }
    state.value -= cnt;
}/*}}}*/


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

// 最寄りの腕を手に入れる
// limitを指定した場合は距離がlimitを超えた場合は何もしない
int get_arm(State& now,int limit = INF){
    auto dist = bfs(now.x,now.y);
    int cost = INF;
    pii pos;
    rep(i,w) rep(j,h){
        if(table[i][j] == 'B' && dist[i][j].first < cost){
            cost = dist[i][j].first;
            pos = pii(i,j);
        }
    }
    if(cost == INF) return -1;


    if(cost > limit) return -1;

    vector<char> vc;
    while(pos != pii(now.x,now.y)){
        pii pre = dist[pos.first][pos.second].second;
        vc.emplace_back(direction(pre,pos));
        pos = pre;
    }

    for(int i=vc.size()-1;i>=0;i--){
        cout << vc[i];
        int idx = char_to_index(vc[i]);
        update_move(now,dx[idx],dy[idx]);
    }
    table[now.x][now.y] = '.';
    return 0;
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



    State now;
    now.init();
    now.x = sx;
    now.y = sy;
    now.arms = vector<pii>{pii(1,1),pii(1,0),pii(1,-1)};
    now.visit[sx][sy] = true;

    for(pii p : now.arms){
        int x = now.x + p.first;
        int y = now.y + p.second;
        if(valid_index(x,y) && table[x][y] != '#'){
            now.visit[x][y] = true;
        }
    }
    now.value = 0;
    for(int i=0;i<w;i++){
        for(int j=0;j<h;j++){
            if(table[i][j] != '#' && !now.visit[i][j]) now.value++;
        }
    }


    // 近い場合は腕を取りに行く
    while(get_arm(now,20) == 0){
        // 腕を生やす
        pii arm = now.add_arm();
        //cerr << "check1: " << now.x << " " << now.y << endl;
        cout << "B" << arm;
    }


    // ビームサーチ
    deque<State> deq;
    deq.push_front(now);

    int turn = 0;
    // あんまりturn数を多くしても無駄が生じるのでパラメータを上手く定める関数が必要
    while(++turn < h*w*2){
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

        vector<char> vc;
        if(p.first == -1 and p.second == -1){
            cerr << endl << "Error" << endl;
            int err = 0;
            rep(i,w) rep(j,h){
                if(table[i][j]!='#' && !now.visit[i][j]) err++;
            }
            cerr << err << endl;
            return 0;
        }
        while(p != dist[p.first][p.second].second){
            pii pa = dist[p.first][p.second].second;
            if(p == pa) break;
            vc.push_back(direction(pa,p));
            p = pa;
            assert( p != pii(-1,-1));
        }

        for(int i=vc.size()-1;i>=0;i--){
            cout << vc[i];
            if(vc[i] == 'D') update_move(now,1,0);
            if(vc[i] == 'W') update_move(now,0,1);
            if(vc[i] == 'A') update_move(now,-1,0);
            if(vc[i] == 'S') update_move(now,0,-1);
        }
    }

    int err = 0;
    rep(i,w) rep(j,h){
        if(table[i][j]!='#' && !now.visit[i][j]) err++;
    }
    cerr << err << endl;
}
