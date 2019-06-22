import desc_parser
import sys
sys.setrecursionlimit(10**6)

argv = sys.argv
argc = len(argv)
filename = argv[1]

data = open(filename, 'r').read()

MP, B, X, Y, sx, sy = desc_parser.parse(data)

dd = ((0, -1, 0), (1, 0, -1), (2, 1, 0), (3, 0, 1))
C = "ASDW"
res = []
used = [[0]*(X) for i in range(Y)]
def dfs(x, y):
    used[y][x] = 1
    for i, dx, dy in dd:
        nx = x + dx; ny = y + dy
        if not 0 <= nx < X or not 0 <= ny < Y or used[ny][nx] or MP[ny][nx] == 1:
            continue
        res.append(C[i])
        dfs(nx, ny)
        res.append(C[i^2])
dfs(sx, sy)
print(*res, sep='')