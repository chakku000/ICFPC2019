import desc_parser
import sys
sys.setrecursionlimit(10**6)

argv = sys.argv
argc = len(argv)
filename = argv[1]

data = open(filename, 'r').read()

MP, B, X, Y, sx, sy = desc_parser.parse(data)
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
    cnt -= 1
    for i in orders[y][x]:
        _, dx, dy = dd[i]
        nx = x + dx; ny = y + dy
        res.append(C[i])
        dfs(nx, ny)
        if cnt > 0:
            res.append(C[i^2])
dfs0(sx, sy)
dfs(sx, sy)
print(*res, sep='')