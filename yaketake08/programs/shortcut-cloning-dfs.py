import desc_parser
import sys
from collections import deque
import util
import random
sys.setrecursionlimit(10**6)
random.seed()

argv = sys.argv
argc = len(argv)

# ファイル: prob-XXX.desc
filename = argv[1]
data = open(filename, 'r').read().strip()

# ファイル: prob-XXX.buy
buy_item = ""
if len(argv) >= 3:
    buyfile = argv[2]
    buy_item = open(buyfile, 'r').read().strip()


dd = ((0, -1, 0), (1, 0, -1), (2, 1, 0), (3, 0, 1))
C = "ASDW"

MP, B, X, Y, sx, sy = desc_parser.parse(data)
ITEM = [[0]*(X+1) for i in range(Y+1)]
CV = []
CP = [[0]*(X+1) for i in range(Y+1)]
XV = []
ci = 0
xi = 0
W = [[1]*(X+1) for i in range(Y+1)]
for i in range(Y+1):
    for j in range(X+1):
        if MP[i][j] == 0:
            W[i][j] = 0
#print(B)
for code, x, y in B:
    ITEM[y][x] = code
    if code == 5:
        CV.append((ci, x, y))
        ci += 1
        CP[y][x] = ci
    if code == 3:
        xi += 1
        XV.append((x, y))
#print(ci)

VS = [(sx, sy)]
for i, x, y in CV:
    VS.append((x, y))
for x, y in XV:
    VS.append((x, y))
DM = util.distance_between_poss(VS, MP, X, Y)

ALL = 2**ci-1
memo = {}
def cdfs(state, cx, cy):
    key = (state, cx, cy)
    if key in memo:
        return memo[key]
    dist = DM[cx, cy]
    if state == ALL:
        d = 10**9; k = None
        for x, y in XV:
            if dist[x, y] < d:
                k = (x, y)
                d = dist[x, y]
        assert k is not None
        memo[key] = res = [d, [(cx, cy), k]]
        return res
    d = 10**9; k = None
    for i, x, y in CV:
        b = 1 << i
        if state & b:
            continue
        r0, p0 = cdfs(state | b, x, y)
        r = r0 + dist[x, y]
        if r < d:
            d = r
            k = p0
    assert k is not None, (state, x, y)
    memo[key] = res = [d, [(cx, cy)] + k]
    return res

#print(CV)
d, ms = cdfs(0, sx, sy)
#print(d, ms)
#print(memo)

px = sx; py = sy
R = [(sx, sy)]
for x, y in ms:
    ps = util.move(MP, X, Y, px, py, x, y)
    R.extend(ps)
    px = x; py = y
for x, y in R:
    W[y][x] = W[y][x+1] = W[y-1][x+1] = W[y+1][x+1] = 1
r = util.pos_to_command(R)
#print(*r, sep='')

# cloning item from prob-XXX.buy
for code in buy_item:
    if code == 'C':
        ci += 1

# cloning
for i in range(ci):
    r.append('C')
OFFSET = [i for i in range(ci+1)]

PMP = []
for i in range(Y):
    for j in range(X):
        if MP[i][j] != 1:
            PMP.append((j, i))
d = 10**9; mmp = None
for t in range(100):
    cnts = [1]*(ci+1)
    colm = [[-1]*(X+1) for i in range(Y+1)]
    que = deque()
    random.shuffle(PMP)
    it = iter(PMP)
    for i in range(ci+1):
        x, y = next(it)
        colm[y][x] = i
        que.append((i, x, y))
    while que:
        c, x, y = que.popleft()
        for i, dx, dy in dd:
            nx = x + dx; ny = y + dy
            if not 0 <= nx < X or not 0 <= ny < Y or MP[ny][nx] == 1 or colm[ny][nx] != -1:
                continue
            que.append((c, nx, ny))
            colm[ny][nx] = c
            cnts[c] += 1
    r0 = max(cnts) - min(cnts)
    if r0 < d:
        d = r0
        mmp = colm
        #print(cnts)
#print(d)

Q = [[(px, py)] for i in range(ci+1)]

first_move = [None]*(ci+1)
ct = ci+1
que = deque([(px, py)])
U = [[0]*(X+1) for i in range(Y+1)]
U[py][px] = 1
while que and ct:
    x, y = que.popleft()
    c = mmp[y][x]
    if first_move[c] is None:
        first_move[c] = (x, y)
        ct -= 1
    for i, dx, dy in dd:
        nx = x + dx; ny = y + dy
        if not 0 <= nx < X or not 0 <= ny < Y or MP[ny][nx] == 1 or U[ny][nx]:
            continue
        que.append((nx, ny))
        U[ny][nx] = 1

"""
for i in range(Y):
    rrr = []
    for j in range(X):
        if mmp[i][j] != -1:
            if first_move[mmp[i][j]] == (j, i):
                rrr.append('#')
            else:
                rrr.append(str(mmp[i][j]))
        else:
            rrr.append(' ')
    print(*rrr, sep='')
#"""

rs = [None]*(ci+1)
used = [[0]*X for i in range(Y)]
orders = [[None]*X for i in range(Y)]
for i in range(ci+1):
    cnt = cnts[i]
    res = []
    x, y = first_move[i]
    Q[i].extend(util.move(MP, X, Y, px, py, x, y))
    assert mmp[y][x] == i, (mmp[y][x], i)
    j = i
    def dfs0(x, y):
        used[y][x] = 1
        r = 1
        R = []
        for i, dx, dy in dd:
            nx = x + dx; ny = y + dy
            if not 0 <= nx < X or not 0 <= ny < Y or used[ny][nx] or mmp[ny][nx] != j:
                continue
            sz = dfs0(nx, ny)
            R.append((i, sz))
            r += sz
        R.sort(key=lambda x: x[1])
        orders[y][x] = [i for i, sz in R]
        return r
    def dfs(x, y):
        global cnt
        if W[y][x] == 0 or W[y][x+1] == 0 or W[y-1][x+1] == 0 or W[y+1][x+1] == 0:
            res.append((x, y, 1))
        else:
            res.append((x, y, 0))
        # だいたい改善する (悪化する場合もある)
        # 部屋の左上に塗られていないマスを放置してしまう挙動をするため、これに対応するもの
        if MP[y][x-1] == 1 and MP[y+1][x] == 0 and W[y+1][x] == 0 and MP[y+2][x] == 1 and MP[y+2][x+1] == 1 and MP[y+1][x-1] == 1:
            res.append((x, y+1, 1))
            res.append((x, y, 0))
            W[y+1][x] = 1
        W[y][x] = W[y][x+1] = W[y-1][x+1] = W[y+1][x+1] = 1
        cnt -= 1
        for i in orders[y][x]:
            _, dx, dy = dd[i]
            nx = x + dx; ny = y + dy
            dfs(nx, ny)
            res.append((x, y, 0))
    dfs0(x, y)
    dfs(x, y)

    md = 1
    res0 = []
    cx = x; cy = y
    for x, y, m in res:
        #assert mmp[y][x] == j
        if m:
            if not md:
                ps = util.move(MP, X, Y, cx, cy, x, y)
                res0.extend(ps)
                md = 1
            else:
                res0.append((x, y))
            cx = x; cy = y
        else:
            md = 0
    Q[i].extend(res0)
    rs[i] = util.pos_to_command(Q[i])

max_len = max(len(rs[i]) + OFFSET[i] for i in range(ci+1))
for i in range(ci+1):
    rs[i].extend('Z'*(max_len - (len(rs[i]) + OFFSET[i])))
r.extend(rs[0])
for i in range(1, ci+1):
    r.append('#')
    r.extend(rs[i])
print(*r, sep='')
