import desc_parser
import sys
from collections import deque
import util
sys.setrecursionlimit(10**6)

argv = sys.argv
argc = len(argv)
filename = argv[1]

data = open(filename, 'r').read()

MP, B, X, Y, sx, sy = desc_parser.parse(data)
MP1 = [e[:] for e in MP]
for i in range(1, Y-1):
    for j in range(1, X):
        if MP[i][j-1] != 1 and MP[i][j] != 1 and MP[i][j+1] == 1:
            if (MP[i-1][j-1] == 1 and MP[i-1][j] != 1) or (MP[i+1][j-1] == 1 and MP[i+1][j] != 1):
                continue
            MP1[i][j] = 1

cnt = 0
W = [[1]*(X+1) for i in range(Y+1)]
for i in range(Y):
    for j in range(X):
        if MP1[i][j] != 1:
            cnt += 1
        if MP[i][j] == 0:
            W[i][j] = 0

dd = ((0, -1, 0), (1, 0, -1), (2, 1, 0), (3, 0, 1))
C = "ASDW"
res = []
used = [[0]*(X) for i in range(Y)]
orders = [[None]*X for i in range(Y)]
def dfs0(x, y):
    used[y][x] = 1
    r = 1
    R = []
    for i, dx, dy in dd:
        nx = x + dx; ny = y + dy
        if not 0 <= nx < X or not 0 <= ny < Y or used[ny][nx] or MP1[ny][nx] == 1:
            continue
        sz = dfs0(nx, ny)
        R.append((i, sz))
        r += sz
    # DFS-treeのサイズが小さい方を優先的に探索するようにする:
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
        if cnt > 0:
            res.append((x, y, 0))
dfs0(sx, sy)
dfs(sx, sy)
#print(*res, sep='')
prv = None
R = []
px = py = -1
md = 1
for x, y, m in res:
    if m:
        if not md:
            ps = util.move(MP, X, Y, px, py, x, y)
            R.extend(ps)
            md = 1
        else:
            R.append((x, y))
        px = x; py = y
    else:
        md = 0
#print(R)
ans = util.pos_to_command(R)
print(*ans, sep='')
