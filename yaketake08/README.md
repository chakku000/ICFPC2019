# 実装色々

## プログラム

- `programs/simple-dfs.py`: 単純なdfsで探索するプログラム

```sh
$ python3 programs/simple-dfs.py path/to/part-1-examples/example-01.desc
DDDDDDDDDWAAAAAAAAAWDDDWAAAWDDDWAAAWDDDWAAAWDDDDSDDSSSSSDDDWAAWDDWAAWDDWAAWAWDDSDWSAWAAAAAAAA
```

- `programs/shortcut-simple-dfs.py`: 1体のworker-wrapperで探索するdfsプログラム
  - 無駄な塗りを省略して次に塗る箇所へ移動する処理が含まれている

```sh
$ python3 programs/shortcut-simple-dfs.py path/to/part-1-examples/example-01.desc
DDDDDDDDWAAAAAAAAWDDWAAWDDWAAWDDWAAWWSDDDSDDDSSSSSDDWAWDWAWDWAWAWDDAAA
```

- `programs/shortcut-cloning-dfs.py`: worker-wrapperを始めにcloningして領域を分割して探索するプログラム
  - `prob-XXX.buy` ファイルも与えることができる (cloningのみ考慮する)

```sh
# prob-XXX.buy を含まない場合
$ python3 programs/shortcut-cloning-dfs.py path/to/problems/prob-035.desc > path/to/prob-035.sol
# prob-XXX.buy を含む場合
$ python3 programs/shortcut-cloning-dfs.py path/to/problems/prob-035.desc path/to/prob-035.buy > path/to/prob-035.sol
```

## ツール

- `tools/parser.py`: `prob-XXX.desc` をパースして情報を出力するプログラム

```sh
$ python3 tools/parser.py path/to/part-1-examples/example-01.desc
0000000000
2200000000
3300110000
4000110000
0000110000
0000110000
0000110000
0000000000
0000010000
5000000000
0 0
```

- `tools/merger.sh`: 出力した `prob-XXX.sol` を比較して、サイズが小さい方で更新する
  - 1体で塗る場合は サイズが小さい = スコアが良い になる
  - cloning する場合は サイズが小さい = スコアが良い ではないため使えない

```sh
# submit000内の .sol ファイルのうち
# output内の同じ .sol ファイルの方がサイズが短い場合に submit000 にコピーする
$ sh tools/merger.sh path/to/submit000 path/to/output
```

## 一括実行

`run.sh` で `prob-XXX.desc` を与えて一括実行できる

```sh
$ mkdir submit000
$ sh run.sh path/to/part-1-initial submit000
```