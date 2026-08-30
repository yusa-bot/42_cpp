# 全体
Ford–Johnson algorithm（Merge-Insertion Sort）
整数を昇順ソート

普通のマージソートとは違い、
2個ずつ比較して大きい側だけを先にソートし、小さい側を特殊な順番で二分挿入する

入力
 ↓
2個ずつペアにする
 ↓
各ペアを small / large に分ける
	small = b_i
	large = a_i
 ↓
largeだけを再帰的にFord–Johnsonソート (main chain)
 ↓
smallをpendingに並べる
 ↓
Jacobsthal順でsmallを選ぶ
 ↓
対応するlargeより前を二分探索
 ↓
main chainへ挿入

## 共通化

「何番目のpendingを挿入するか」は共通化し、「そのコンテナへどう挿入するか」は個別実装
├── 入力解析
├── 時間変換
├── Element / Pair / Pending
├── 2要素をsmall/largeに分ける処理
└── Jacobsthal境界の純粋な数値計算

コンテナごとに実装する
├── Ford–Johnson再帰本体
├── main chainの構築
├── pendingの構築
├── anchor検索
├── 二分探索
├── 挿入順を保存するコンテナ
└── main chainへの挿入

# vector
メモリ上で連続しています。
添字アクセスが速い
std::find の走査が速い
lower_bound の位置移動が速い
CPUキャッシュに載りやすい
途中挿入では後続要素をずらす必要がある
再確保は抑えられますが、少数要素でも reserve() の確保処理が発生

# deque
複数のメモリブロックに分かれています。
ランダムアクセス可能
先頭・末尾への挿入が速い
中央挿入では要素移動が必要
要素へのアクセスにブロック計算が入る
vectorよりキャッシュ効率が落ちやすい

# time計測

## 最初の実行による影響
メモリアロケータの初期動作
命令・データキャッシュが温まっていない
reserve() による事前確保
parsePositiveInteger() の初回実行
std::clock() の計測粒度
OSや実行環境の揺らぎ

## vectorが速い理由
連続メモリなので：
find → 前から連続して読む
lower_bound → ポインタ計算で中央へ移動
CPUキャッシュを効率よく使えます。

## dequeが遅い理由
挿入ごとにpairedLargeを探しています。
その後、挿入位置を探します。(lower_bound)

メモリが複数ブロックに分かれているため：
find → ブロックをまたいで読む
lower_bound → ブロック位置を計算して移動

## 特に差が出る箇所
ペア作成       → ランダムアクセス
pairedLarge探索 → 全体を順番に走査
lower_bound    → 複数回ランダムアクセス
mainChain挿入  → 要素移動
このうち、探索処理はvectorの連続メモリと相性がよいです。


----

# 実行結果

## vector先

./PmergeMe 3 5 9 7 4
Time to process a range of 5 elements with std::vector: 2103.00000 us
Time to process a range of 5 elements with std::deque: 1244.00000 us

./PmergeMe 1
Time to process a range of 1 elements with std::vector: 795.00000 us
Time to process a range of 1 elements with std::deque: 476.00000 us

./PmergeMe 5 5 3 3 1 2 2 4
Time to process a range of 8 elements with std::vector: 2110.00000 us
Time to process a range of 8 elements with std::deque: 1400.00000 us

./PmergeMe $(seq 1 3000 | shuf)
Time to process a range of 3000 elements with std::vector: 22594.00000 us
Time to process a range of 3000 elements with std::deque: 34328.00000 us

Time to process a range of 3000 elements with std::vector: 3463.00000 us
Time to process a range of 3000 elements with std::deque: 2940.00000 us

## deque先

./PmergeMe 5 5 3 3 1 2 2 4
Time to process a range of 8 elements with std::vector: 610.00000 us
Time to process a range of 8 elements with std::deque: 2341.00000 us

./PmergeMe $(seq 1 3000 | shuf)
Time to process a range of 3000 elements with std::vector: 19003.00000 us
Time to process a range of 3000 elements with std::deque: 32596.00000 us
