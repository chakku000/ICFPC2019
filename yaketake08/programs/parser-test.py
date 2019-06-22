import desc_parser
import desc_rparser
import sys
sys.setrecursionlimit(10**6)

argv = sys.argv
argc = len(argv)
filename = argv[1]

data = open(filename, 'r').read()

MP, B, X, Y, sx, sy = desc_parser.parse(data)

result = desc_rparser.rparse(MP, B, X, Y, sx, sy)
print(data)
print(result)

# check
vs = set(data.split("#")[0][1:-1].split("),("))
ws = set(result.split("#")[0][1:-1].split("),("))
assert vs == ws

