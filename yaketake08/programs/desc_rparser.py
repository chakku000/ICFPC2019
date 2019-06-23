from collections import defaultdict, deque
# MP: (X+1)*(Y+1) のMap (x = X+1, y = Y+1 は壁 '1' にすること)
# B: アイテムのtuple(code, x, y) のlist
# X: 盤面xサイズ
# Y: 盤面yサイズ
# sx, sy: 最初のworker-wrapperの配置
def rparse(MP, B, X, Y, sx, sy):
    Q = []
    R = []
    G = defaultdict(list)
    S = [1]*(X+1)
    P = [-1]*(X+1)
    for i in range(Y+1):
        xs = []
        MPi = MP[i]
        for j in range(X+1):
            if S[j] != MPi[j]:
                if S[j-1] == MPi[j-1]:
                    xs.append(j)
                    if P[j] == -1:
                        P[j] = i
                    else:
                        G[j, i].append((j, P[j]))
                        G[j, P[j]].append((j, i))
                        P[j] = -1
                if S[j+1] == MPi[j+1]:
                    xs.append(j+1)
                    if P[j+1] == -1:
                        P[j+1] = i
                    else:
                        G[j+1, i].append((j+1, P[j+1]))
                        G[j+1, P[j+1]].append((j+1, i))
                        P[j+1] = -1
        S = MPi
        assert len(xs) % 2 == 0
        for j in range(len(xs)//2):
            x0 = xs[2*j]; x1 = xs[2*j+1]
            G[x0, i].append((x1, i))
            G[x1, i].append((x0, i))
    v = list(G.keys())[0]
    U = set([v])

    while 1:
        R.append("({},{})".format(*v))
        for w in G[v]:
            if w in U:
                continue
            U.add(w)
            v = w
            break
        else:
            break
    Q.append(",".join(R))
    Q.append("#")

    Q.append("({},{})".format(sx, sy))
    Q.append("##")

    # code に対応する item
    # 0: 'B': extension of manipulator
    # 1: 'F': fast wheels
    # 2: 'L': drill
    # 3: 'X': (clone point)
    # 4: 'R': teleport
    # 5: 'C': cloning
    CODE = "BFLXRC"
    R = []
    for code, x, y in B:
        R.append("{}({},{})".format(CODE[code], x, y))
    Q.append(",".join(R))
    return "".join(Q)
