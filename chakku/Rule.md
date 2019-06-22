## 1. Introduction

- lambdaがもともとあった空の鉱山は古い廃棄物の補完庫にできる
- worker wrapper
    - bit rotの表面全体をdecay-containing substanceで包むロボット
- Goal
    - 空の鉱山にたいして最適な行動を決める


## 2. Maps and Worker-Wrapper Navigation

- Mapの境界は左回りに与えられる
- 障害物同士と障害物と壁は接していない

### 2.1 基本的な移動とwrapping rules

ロボットは3つの腕があり、腕が届く場所にある1x1のマスをwrapできる。
また、腕だけでなくbodyもwrap可能。

- 隣のマスに移動する
- 90度回転する
- なにもしない

腕は最初(x+1,y),(x+1,y+1),(x+1,y-1)にある(ただし(x,y)はロボットのbodyの位置)

- 腕が壁や障害物とぶつかる時は、折りたたまれるので損傷なく移動可能
    - 展開可能なときは展開される


worker-wrapperの一連の操作列を出力する
- 操作の結果、すべてのマス(障害物以外)はwrapされる
- 操作の数は少ないほうがよい


### 2.2 Boosters

mapにはいくつかのboosterが落ちている。boosterのある場所に行くと獲得できる。
boosterは獲得すると、1度だけ使うことができる。
boosterは4種類ある。

- Extension of the Manipulator (Code: B)
    - 永続的に追加の腕を手に入れる
    - 追加の腕は、bodyか腕に接続可能
    - 追加の腕はbodyからの相対位置で指定する

- Fast Wheels (Code: F)
    - 1unit timeで2回同じ方向に移動可能
    - ただし50 unit of time しか続かない
    - Fast wheelsがactiveなときにFast wheelsを使用するとその時点から50 unit time継続する

- Drill (Code: L)
    - 壁や障害物を破壊可能
    - 30 unit of time継続する

- Mysterious Point (Code: X)
     - 不明

#### Using Booster
Boosterを使用するには1 unit timeを要する.
