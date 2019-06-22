# ICFPC2019: ルール
###### tags: `contest`

初期仕様

`specification-v1.pdf` を読んで理解する
- https://icfpcontest2019.github.io/download/specification-v1.pdf

## 1. Introduction

なんかsurfaceをwrapしたいので、1体のworker-wrapperを使ってmine内を全てwrapするぞ。

### 1.1 Contest Specification Updates

- First update: June 22, 02:00 JST (7時間経過)
- Second update: June 22, 09:00 JST (14時間経過)
- Third update: June 22, 19:01 JST (Lightning Division終了後)

更新は以下で行われる
- website: https://icfpcontest2019.github.io/
- twitter: https://twitter.com/icfpcontest2019

## 2. Maps and Worker-Wrapper Navigation

### 2.1 Basic navigation and wrapping rules

一つのmapが与えられる。
各mapには一体のworker-wrapperが存在する。

worker-wrapperは1×1の正方形を専有する(座標は左上の座標で表される)
- 初期座標は与えられる

各mapには、障害物(obstacle)が存在し、一つの障害物は四点の座標で表される
- 例: $[(4, 2), (6, 2), (6, 7), (4, 7)]$

worker-wrapperは以下の動きが取れる
- 隣接したsurfaceの正方形に移動する
- 90度回転する (manipulatorの相対的な位置も変更される)

manipulatorの初期設定は同じで、$(x, y)$にworker-wrapperが存在する時、$[(x+1, y), (x+1, y+1), (x+1, y-1)]$で記述される。
- worker-wrapperが壁や障害物に隣接した時、manipulatorはfoldされる
- 十分なスペースがある時は、unfoldされる

taskのsolutionは、worker-wrapperの行動の系列で表される
- 実行された時、surfaceのnon-wrapped squareが存在しないようにする
- 短ければ短いほど、よいsolutionになる

### 2.2 Boosters

いくつかの場所はmarkされ、そこにはboosterが置かれており、そこに移動することで拾うことができる。

boosterを拾うことで、拾って以降の任意のタイミングで、以下に記述する効果を得ることができる。
boosterは四種類のものが見つかっている。

#### 2.2.1 Extension of the Manipulator

Code: `B`

manipulatorが増設される。
新しいmanipulatorは、worker-wrapperのbodyや既に取りといているmanipulatorの隣に**1つ**取り付けられる。

例. worker-wrapper $(x, y)$
$[(x+1, y), (x+1, y+1), (x+1, y-1)]$にmanipulatorが存在する場合、
$(+1, +2), (-1, 0), (+2, +1)$に新しいmanipulatorを一つ取り付けられる。

#### 2.2.2 Fast Wheels

Code: `F`

一時的にworker-wrapperをスピードアップさせる。
単位時間で同じ方向に2進むことができる。1つしか進めない場合は一つしか進まない。

この効果は50単位の時間しか持続しない。
重ねがけしても、2倍効果の時間の残りが50単位時間に更新されるのみである。(50単位時間分増えるわけではない)

#### 2.2.3 Drill

Code: `L`

ドリルを使うと、障害物もしくは壁を壊して進むことができる。
幅1のトンネルを造ることができる。

しかし、mapのbounding boxを超えて壊すことができない。(座標が負もしくはmapのサイズ以上の座標は×)

一度起動すると、30単位時間の間使える。
重ねがけはfast wheelsと同じで、残りが30単位時間に更新されるのみ。

drillとfast wheelは同時に使うことができる

#### 2.2.4 Mysterious Point

Code: `X`

効果は分からないし拾えない (今後なんかわかるはず)

#### ex1. Teleports

Code: `R`

ビーコンを設置して、その後にその場所に何回でもワープすることができる

このビーコンは以前設置した場所に配置できず、`X`の上に配置することができない

### ex2. Cloning

Code: `C`

自身のコピー(replica)を生成する。これを実行した次の単位時間からコピーを操作できる。

生成できる場所は `X` の位置のみである。

任意のworker-wrapperが得たアイテムは別のworker-wrapperが使うことができる。(効果は使用者のみに現れる)

複数機ある場合のコマンドは、生成された順序で与える

#### 2.2.5 Using boosters

boosterを使うには1単位時間かかる。その間は他の操作を行うことができない。
そのboosterを一度使うと消費され、再び使うことができない。

### 2.3 Rules of manipulator reachability

manipulatorはめっちゃ柔軟だが、limited reachがある。

manipulatorは、**worker-wrapperのbodyが見える位置**であれば到達することができる。
(ある一点が別の点から見えるとは、その２つの点を(障害物や壁に阻まれずに)直線的に接続できる場合を意味する。cornerに接するのは許される)

## 3 Rules of the Contest

### 3.1 Task descriptions

各worker-wrapperのtaskは単一ファイル *prob-NNN.desc* に記述されており、以下の構成を含む。(`#`で区切られている)

- a contour map of the mine
- mine内のworker-wrapperの初期位置
- (possibly empty) セミコロン区切りの障害物リスト
- (possibly empty) boosterとその位置をセミコロンで区切ったリスト

boosterの種類はcodeで示される。

(ここにtask descriptionの正確な文法が含まれるが書き写すのがめんどいので省略)

### 3.2 Encoding solutions

*prob-NNN.desc* のsolutionは *prob-NNN.sol* で記述する。
単一行の行動系列を記述し、以下のコマンドを記述する。
- `W` ↑
- `S` ↓
- `A` ←
- `D` →
- `Z` 何もしない
- `E` manipulatorを90度時計回りに回転
- `Q` manipulatorを90度反時計回りに回転
- `B(dx, dy)` 新しいmanipulatorを相対位置$(dx, dy)$に接続
- `F` fast wheelをする
- `L` drillをする
- `R` teleportするためのビーコンを設置する
- `C` cloningをする
- `T(x, y)` 指定した場所に以前設置したビーコンの場所にワープする

コマンドは、各worker-wrapperごとに`#`で区切って与える

以下の条件を満たす時、solutionは有効であるとする
- 壁や障害物の中をdrillなしで突き進まない
- respects the rules of using boosters
- 終了時、全てのsquareがwrappedされている



### 3.3 Examples
- example task: https://icfpcontest2019.github.io/download/part-1-examples.zip

- visualizer: https://icfpcontest2019.github.io/solution_visualiser/
- checker: https://icfpcontest2019.github.io/solution_checker/


### 3.4 Registration and Submission

the initial pack with 150 tasks to solve
- https://icfpcontest2019.github.io/download/part-1-initial.zip

登録は https://icfpcontest2019.github.io/register/


提出は https://icfpcontest2019.github.io/submit/

もしくはcurl (submitの後の`/`はいらない)
```shell
$ curl -F "private_id=34fbde" -F "file=@solutions.zip" https://monadic-lab.org/submit
```

各チームは、**10分ごとに**solutionを提出できる

### 3.5 Scoring

task $T$ に対するスコアは以下で計算される

$\displaystyle score_{team, T} := \lceil 1000 \times \log_2(X_T \times Y_T) \times \frac{t_{best}}{t_{team}} \rceil$

$X_T, Y_T$: $T$のmapのx,yの最大座標
$t_{team}$: このチームが完遂までにかかった経過単位時間
$t_{best}$: 全チームの中で完遂が最も早かったチームの経過単位時間

solutionがなかったり無効だったりするとスコアは0

### 3.6 Lightning Division

はい

### 3.7 Determining the Winner

ほげ

### 3.8 Live Standings

live standings: https://icfpcontest2019.github.io/rankings/

- この結果は、Lightning Divisionの終わり一時間前に凍結
- June 22, 2019, 22:00 JSTに凍結解除し、コンテスト終了3時間前に再び凍結される

### 3.9 The Judges' Prize

うむ

----

First update

追加仕様: teleport
- https://icfpcontest2019.github.io/download/teleports-v1.pdf

----

Second update

----
Third update
