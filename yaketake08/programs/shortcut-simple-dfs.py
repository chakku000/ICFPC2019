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
for i in range(1, Y-1):
    for j in range(1, X):
        if MP[i][j-1] != 1 and MP[i][j] != 1 and MP[i][j+1] == 1:
            if (MP[i-1][j-1] == 1 and MP[i-1][j] != 1) or (MP[i+1][j-1] == 1 and MP[i+1][j] != 1):
                continue
            MP[i][j] = 1

cnt = 0
for i in range(Y):
    for j in range(X):
        if MP[i][j] != 1:
            cnt += 1

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
        if not 0 <= nx < X or not 0 <= ny < Y or used[ny][nx] or MP[ny][nx] == 1:
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
    res.append((x, y, 1))
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
