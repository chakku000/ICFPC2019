import sys
import csv

## python3 hikaku.py <file1(text)> <file2(csv)>

print(sys.argv[1],sys.argv[2])

chakku = []
sugopuro = []

with open(sys.argv[1],newline='') as f:
    chakku = list(map(int,f.readlines()))

with open(sys.argv[2],newline='') as f:
    reader = csv.reader(f)
    for row in reader:
        sugopuro.append(int(row[1]))

print(chakku)
print(sugopuro)

idx = 1
for c,s in zip(chakku,sugopuro):
    if c < s:
        print(idx,c,s)
    idx += 1
