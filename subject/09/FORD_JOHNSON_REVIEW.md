# PmergeMe: Ford–Johnson 実装の比較調査

## 結論

このディレクトリの `PmergeMe.cpp` は、Knuth が *The Art of Computer
Programming, Volume 3* の印刷ページ 184–185 で説明している
merge-insertion sort（Ford–Johnson algorithm）の構造に対応している。

一方、比較対象の次の2実装は、どちらも小さい区間に insertion sort を使う
通常の merge sort である。関数名に `mergeInsert` が含まれていても、Knuth
のいう merge-insertion sort ではない。

- [GowthertG/CPP-Module-09 の PmergeMe.cpp](https://github.com/GowthertG/CPP-Module-09/blob/master/ex02/src/PmergeMe.cpp)
- [chahid001/cpp09 の PmergeMe.cpp](https://github.com/chahid001/cpp09/blob/main/ex02/PmergeMe.cpp)

## 参照資料

- Donald E. Knuth, *The Art of Computer Programming, Volume 3: Sorting and
  Searching*, Section 5.3.1, printed pp. 184–185
- [参照したPDF（印刷ページ184はPDF内の196ページ目）](https://seriouscomputerist.atariverse.com/media/pdf/book/Art%20of%20Computer%20Programming%20-%20Volume%203%20%28Sorting%20%26%20Searching%29.pdf#page=196)
- 42 C++ Module 09 subject, Exercise 02: `subject/09.pdf`, pp. 13–14

42のsubjectは、使用すべきアルゴリズムを単に「merge sort と insertion sort
の組み合わせ」とは説明していない。明示的に Ford–Johnson algorithm を使う
よう要求し、その参照先として Knuth の Merge Insertion, p. 184 を挙げている。

## Knuth の merge insertion

印刷ページ184では、21要素をソートする例を使って、Ford–Johnsonの中心的な
考え方が説明されている。一般的な手順は次ページの印刷ページ185に続いている。

手順を要約すると次のようになる。

1. 要素を互いに重ならないペアに分け、各ペアを1回比較する。奇数個なら1要素を
   ペアに含めず残す。
2. 各ペアの大きい要素を `a_i`、小さい要素を `b_i` とする。
3. `a_i` の列を、同じ merge-insertion sort を使って再帰的にソートする。
4. ソート後も `b_i <= a_i` というペア関係を維持する。
5. `b_1` と、ソート済みのすべての `a_i` から main chain を作る。
6. 残りの `b_i` を、通常の添字順ではなく次のグループ順で挿入する。

```text
b3, b2;
b5, b4;
b11, b10, b9, b8, b7, b6;
...
```

グループの境界は次の数列になる。

```text
1, 3, 5, 11, 21, 43, ...
```

これはJacobsthal数に由来する順序であり、ローカル実装では漸化式
`next = current + 2 * previous` で生成している。

`b_i` の挿入位置を探すときは、main chain全体を無条件に探索しない。
すでに `b_i <= a_i` と分かっているので、対応する `a_i` より前だけを二分探索
すればよい。この探索範囲と挿入順の組み合わせにより、各グループで必要になる
最悪比較回数を抑えることがFord–Johnsonの重要な点である。

したがって、「区間を左右に分けてmergeする」「小区間だけinsertion sortに
切り替える」という処理だけでは、Ford–Johnsonにはならない。

## ローカル実装とKnuthの対応

ローカル実装では、Knuthの `a_i` を `Pair::large`、`b_i` を
`Pair::small` と表現している。

### 1. ペアを比較する

`makePair()` は2要素を1回比較し、`small` と `large` に分ける。

```cpp
if (first.value <= second.value) {
    pair.small = first;
    pair.large = second;
} else {
    pair.small = second;
    pair.large = first;
}
```

vector版では `fordJohnsonVector()` 内で2要素ずつ `makePair()` に渡す。
deque版も同じ処理を独立して実装している。

### 2. large側を再帰的にソートする

各ペアの `large` だけを `mainChain` に入れ、その列に対して
`fordJohnsonVector()` または `fordJohnsonDeque()` を再帰的に呼び出す。

これはKnuthの「各ペアの大きい要素をmerge insertionでソートする」という
手順に対応する。

### 3. ペア関係を保持する

再帰ソートにより `large` の順序が変わっても、対応する `small` を見失わない
よう、すべての `Element` は一意な `id` を持つ。

`pairForLargeId` は、ソート後の `large.id` から元の `Pair` を引くために使われる。
これはKnuthの `b_i <= a_i` という対応関係をプログラム上で維持するための実装
上の仕組みである。

値が重複していてもIDは異なるため、値そのものを使ってペアを復元する場合に
起こり得る曖昧さも避けられている。

### 4. Knuthと同じ挿入順を作る

`vectorInsertionOrder()` と `dequeInsertionOrder()` が生成する0-based indexは
次のようになる。

```text
0, 2, 1, 4, 3, 10, 9, 8, 7, 6, 5, ...
```

これを `b_i` の名前に直すと次の通りである。

```text
b1, b3, b2, b5, b4, b11, b10, b9, b8, b7, b6, ...
```

これはKnuthが示す挿入順と一致する。

### 5. b1の扱い

Knuthの説明では、`b_1` は最初からmain chainに含まれる。

ローカル実装は、最初はソート済みの `a_i` だけを `mainChain` に置き、`b_1`
も `pending` の先頭に入れる。ただし、`b_1` のanchorである `a_1` はmain chain
の先頭にある。そのため `b_1` に対する二分探索範囲は長さ0となり、値の比較を
行わずに `a_1` の直前へ挿入される。

表現方法は異なるが、比較回数と挿入後のmain chainはKnuthの手順と同等である。

### 6. anchorまでに探索範囲を制限する

各 `Pending` は、対応する `large` のIDを `anchorId` として保持する。

挿入時には `vectorAnchorIndex()` / `dequeAnchorIndex()` で現在のanchor位置を
見つけ、`vectorLowerBound()` / `dequeLowerBound()` にその位置を上限として渡す。

```cpp
const std::size_t end = item.hasAnchor
    ? vectorAnchorIndex(mainChain, item.anchorId)
    : mainChain.size();

const std::size_t position =
    vectorLowerBound(mainChain, end, item.value.value);
```

探索区間は `[0, anchorIndex)` であり、anchor自身は含まない。すでに
`small <= large` が分かっているため、anchorとの再比較は必要ない。

奇数個のときにペアから外した要素にはanchorがないので、`hasAnchor == false`
としてmain chain全体を探索する。これはKnuthの奇数要素の扱いに対応する。

## 比較対象1: GowthertG版

この実装の `mergeInsertSortVector()` は、区間の長さによって次の2処理を
切り替える。

- 小さい区間では `insertSortVector()` を実行する。
- それ以外では区間を中央で二分し、左右を再帰ソートした後、
  `mergeSortVector()` で通常のmergeを行う。

deque版も同じ構造である。

参照箇所:

- [vector版の分割処理](https://github.com/GowthertG/CPP-Module-09/blob/master/ex02/src/PmergeMe.cpp#L15-L30)
- [vector版の通常merge](https://github.com/GowthertG/CPP-Module-09/blob/master/ex02/src/PmergeMe.cpp#L32-L58)
- [deque版](https://github.com/GowthertG/CPP-Module-09/blob/master/ex02/src/PmergeMe.cpp#L82-L138)

この方式は一般にhybrid merge sortと呼ばれる種類の最適化であり、正常に
昇順ソートすることはできる。しかし、次のFord–Johnson固有の処理が存在しない。

- disjoint pairの比較
- small/large間のpartner関係
- largeだけに対する再帰的なFord–Johnson
- main chainとpending chain
- Jacobsthal由来の挿入順
- partnerを上限とする二分挿入

したがって、現在の42 subjectが要求するFord–Johnson実装としては正しくない。

## 比較対象2: chahid001版

この実装の `mergeInsertVec()` も、区間が10要素程度以下なら `InsertVec()`、
それより大きければ左右を再帰ソートして `mergeVec()` でmergeする。

deque版も同じ処理を複製している。

参照箇所:

- [vector版](https://github.com/chahid001/cpp09/blob/main/ex02/PmergeMe.cpp#L109-L138)
- [deque版](https://github.com/chahid001/cpp09/blob/main/ex02/PmergeMe.cpp#L276-L305)

この実装にもFord–Johnson固有のペア、main chain、特殊な挿入順、anchor付き
二分探索は存在しない。そのため、ソート結果が正しくても、要求された
アルゴリズムを実装したことにはならない。

また、現在のローカルsubjectのGeneral rulesとの比較では、次の別の問題もある。

- `std::printf()` を使っているが、subjectでは `*printf()` が禁止されている。
- `atoi()` を使っており、正の `int` の厳密な範囲検査になっていない。
- 計測範囲が主にソート呼び出しだけで、入力とデータ管理を含めるという現在の
  subjectの指示を満たさない。

## コード量について

比較した `PmergeMe.cpp` の物理行数はおおよそ次の通りである。

| 実装 | 行数 | 内容 |
|---|---:|---|
| ローカル | 485 | Ford–Johnsonをvector/dequeで個別実装 |
| GowthertG | 185 | hybrid merge sortをvector/dequeで個別実装 |
| chahid001 | 305 | hybrid merge sortをvector/dequeで個別実装 |

ローカル実装が長い主な理由は、次の処理を実際に持っているためである。

- ペアとpartner関係の管理
- large側の再帰ソート
- main/pending chainの構築
- Jacobsthal由来の挿入順生成
- anchor位置の追跡
- anchorを上限とする二分探索
- vector版とdeque版の独立した実装

特に42のsubjectは、2コンテナに対してgeneric functionを使い回すよりも、各
コンテナ用にアルゴリズムを実装することを強く推奨している。そのため、vector
版とdeque版の重複は課題上説明可能である。

ただし、行数自体が品質を保証するわけではない。ローカル版には学習用コメントと
空行が多く、Makefileで使用されていない `luna.cpp` / `luna.hpp` も存在する。
Ford–Johnsonのアルゴリズム部分とコンテナ別実装は維持しつつ、提出前にこれらを
整理する余地はある。

## 最終評価

- Knuthのp.184だけで例の概要が示され、p.185で一般手順が明文化されている。
- ローカル実装は、その一般手順をvectorとdequeの両方について実装している。
- `b_1` の初期配置方法には表面的な違いがあるが、比較なしの挿入なのでKnuthと
  同等である。
- 外部の2実装は通常のmerge sortとinsertion sortのhybridであり、Knuthの
  merge insertionではない。
- したがって、3実装の中ではローカル実装が現在のExercise 02のアルゴリズム要件
  に最も適合している。

なお、アルゴリズムとは別に、ローカルの `main.cpp` は例外メッセージを
`std::cout` に出している。subjectはエラーを標準エラーへ出すよう要求している
ため、提出前に `std::cerr` へ変更する必要がある。
