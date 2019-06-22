import sys
from collections import defaultdict

argv = sys.argv
argc = len(argv)
filename = argv[1]

data = open(filename, 'r').read()
components = data.split('#')

INF = 10**9

def prepare_poly(data):
    xmin = ymin = INF; xmax = ymax = -INF
    D = defaultdict(list)
    *ps, = map(lambda x: list(map(int, x.split(','))), data[1:-1].split("),("))
    for x, y in ps:
        xmin = min(xmin, x)
        xmax = max(xmax, x)
        ymin = min(ymin, y)
        ymax = max(ymax, y)
    ps.append(ps[0])
    for (x0, y0), (x1, y1) in zip(ps, ps[1:]):
        if x0 != x1:
            continue
        assert x0 >= 0 and x1 >= 0 and y0 >= 0 and y1 >= 0
        D[y0].append(x0)
        D[y1].append(x1)
    return D, xmin, ymin, xmax, ymax

# ===== a contour map of the mine
# '0': surface
# '1': wall or obstacle
poly_wall, _, _, X, Y = prepare_poly(components[0])

MP = [[0]*(X+1) for i in range(Y+1)]
R = [0]*(X+2)
for i in range(Y+1):
    if i in poly_wall:
        xs = poly_wall[i]
        xs.sort()
        for x in xs:
            R[x] ^= 1
    MPi = MP[i]
    wall = 1
    for j in range(X+1):
        if R[j]:
            wall ^= 1
        if wall:
            MPi[j] = 1

# ===== an initial location of the worker-wrapper inside the mine
sx, sy = map(int, components[1][1:-1].split(","))

# ===== a (possibly empty) list of obstacles separated by semicolons
obstacles = components[2].split(';')
for obstacle in obstacles:
    if len(obstacle) == 0:
        continue
    poly_ob, xmin, ymin, xmax, ymax = prepare_poly(obstacle)
    R = [0]*(X+2)
    for i in range(ymin, ymax+1):
        if i in poly_ob:
            xs = poly_ob[i]
            xs.sort()
            for x in xs:
                R[x] ^= 1
        MPi = MP[i]
        ob = 0
        for j in range(xmin, xmax+1):
            if R[j]:
                ob ^= 1
            if ob:
                MPi[j] = 1

# ===== a (possibly empty) list of boosters and their locations separated by semicolons
# 2: 'B': extension of manipulator
# 3: 'F': fast wheels
# 4: 'L': drill
# 5: 'X': (clone point)
# 6: 'R': teleport
# 7: 'C': cloning
CODE = "BFLXRC"
boosters = components[3].split(';')
for booster in boosters:
    if len(booster) == 0:
        continue
    code = booster[0]
    x, y = map(int, booster[2:-1].split(","))
    MP[y][x] = CODE.index(code) + 2

# generated map
for MPi in MP[:-1]:
    print(*MPi[:-1], sep='')

# worker-wrapper's initial position
print(sx, sy)