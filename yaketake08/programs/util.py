from collections import deque

dd = ((0, -1, 0), (1, 0, -1), (2, 1, 0), (3, 0, 1))

def move(MP, X, Y, sx, sy, tx, ty):
    que = deque([(sx, sy)])
    prv = {(sx, sy): (-1, -1)}
    while que:
        x, y = pkey = que.popleft()
        if x == tx and y == ty:
            break
        for i, dx, dy in dd:
            nx = x + dx; ny = y + dy
            key = (nx, ny)
            if not 0 <= nx < X or not 0 <= ny < Y or key in prv or MP[ny][nx] == 1:
                continue
            prv[key] = pkey
            que.append(key)
    else:
        assert 0
    ps = []
    x = tx; y = ty
    while x != sx or y != sy:
        ps.append((x, y))
        x, y = key = prv[x, y]
    ps.reverse()
    return ps

M = {(-1, 0): "A", (0, -1): "S", (1, 0): "D", (0, 1): "W"}
def pos_to_command(R):
    res = []
    for (x0, y0), (x1, y1) in zip(R, R[1:]):
        dx = x1 - x0; dy = y1 - y0
        if dx == dy == 0:
            continue
        assert (dx, dy) in M, ((x0, y0), (x1, y1), dx, dy)
        res.append(M[dx, dy])
    return res

def distance_between_poss(VS, MP, X, Y):
    DM = {}
    for cx, cy in VS:
        que = deque([(cx, cy)])
        dist = [[-1]*(X+1) for i in range(Y+1)]
        dist[cy][cx] = 0
        while que:
            x, y = que.popleft()
            d = dist[y][x] + 1
            for i, dx, dy in dd:
                nx = x + dx; ny = y + dy
                if not 0 <= nx < X or not 0 <= ny < Y or MP[ny][nx] == 1 or dist[ny][nx] != -1:
                    continue
                dist[ny][nx] = d
                que.append((nx, ny))
        D = {}
        for x, y in VS:
            D[x, y] = dist[y][x]
        DM[cx, cy] = D
    return DM

