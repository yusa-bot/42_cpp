# CPP Module 07 Double-check Review Report

## 結論

rubric 上の必須要件については v1 の **PASS 判定を維持**する。ex00〜ex02 は C++98 と必須警告フラグでビルド・実行でき、テンプレートの基本動作、const 対応、深いコピー、範囲外例外、`new[]` / `delete[]` の対応も正しい。

ただし「一般的な観点でも完璧」とは言えない。v1 が拾っていない再現可能な問題を 2 件、API の refactor 提案を 1 件確認した。通常の subject 採点を落とすほどではないが、特に実行ファイル名 `array` と標準ヘッダ `<array>` の衝突は、外部テストの組み方によってコンパイルを壊す。

## 新規指摘

### [MEDIUM] ex02 の実行ファイル `array` が標準ヘッダ `<array>` と衝突する

- 該当: `ex02/Makefile:1`
- `make` 後、`ex02/array` という Mach-O 実行ファイルが作られる。
- 外部テストを一般的な `-Iex02` 方式でビルドすると、現在の libc++ が内部で `#include <array>` を行った際に、標準ヘッダより先にこの実行ファイルを拾う。その結果、バイナリを C++ ソースとして解釈して `source file is not valid UTF-8` で失敗する。

再現手順:

```sh
make -C cpp-module--ayusa/Module07/ex02
c++ -Wall -Wextra -Werror -pedantic -std=c++98 \
  -Icpp-module--ayusa/Module07/ex02 \
  cpp-module--ayusa/Module07/outputs/artifacts/tests/ex02_deep.cpp \
  -o /tmp/ex02_deep_after_make
```

実測: 2 番目のコマンドは exit 1。`cpp-module--ayusa/Module07/ex02/array:1:1: error: source file is not valid UTF-8` となった。`make fclean` 後には同じコマンドが通るため、原因も切り分け済み。

これは `Array.hpp` 自体の欠陥ではなく、生成物名と include path の相互作用である。rubric 添付テストを ex02 直下へコピーして `-I` なしでビルドする環境では発生しない一方、CI・外部 harness・sanitizer ビルドでは十分起こりうる。`NAME = array_demo` や `NAME = array_test` のように、標準ヘッダ名を避けることを推奨する。

### [LOW] `operator[]` は幅の広い整数を範囲判定前に切り詰める

- 該当: `ex02/Array.hpp:53-62`
- 引数型が `unsigned int` のため、LP64 環境で `unsigned long` の値を渡すと、関数に入る前に縮小変換される。
- `UINT_MAX + 1UL` は 0 へ変換されるため、サイズ 2 の配列に対して明らかな範囲外値を渡しても例外にならず、要素 0 を返した。

確認したケース（C++98）:

```cpp
Array<int> values(2);
values[0] = 17;
volatile unsigned long runtime_uint_max = UINT_MAX;
unsigned long index = runtime_uint_max + 1UL;
values[index]; // throw せず values[0] にアクセス
```

通常の課題利用では添字も `unsigned int` の範囲に収まるため採点上の重大問題ではない。しかし「out of range must throw」を一般的な API 契約として読むなら穴である。`operator[]` の引数と内部サイズを `std::size_t` に揃えると、64-bit 環境でのこの切り詰めを避けられる。少なくとも API が扱える最大範囲を同じ型で一貫させるべきである。

### [LOW / REFACTOR] サイズ指定コンストラクタを `explicit` にする

- 該当: `ex02/Array.hpp:17`
- 現状は `Array<int>` を要求する引数へ `2u` を渡すだけで、暗黙に 2 要素の配列が確保される (`void f(Array<int>); f(2u);`)。
- 数値から配列への暗黙変換に自然な意味はなく、意図しない allocation を隠す。`explicit Array(unsigned int n)` を推奨する。C++98 で利用可能であり、subject の要求も損なわない。

## v1 の記述に対する補正

### `min` / `max` の参照戻りは欠陥というより契約上の注意

v1 は一時オブジェクトを渡した際の dangling reference を「任意・堅牢性」の修正候補にしている。しかし `const T&` を返す設計はこの exercise の期待仕様であり、C++ 標準ライブラリの `std::min` / `std::max` と同じ性質を持つ。戻り値へ変えると不要なコピーや型要件の変化が生じるため、単純な改善とはいえない。これは修正要求ではなく、呼び出し側が戻り参照を一時オブジェクトより長く保持しないという lifetime 契約として説明するのが正確である。

### ADL による `swap` 衝突も課題上は不可避に近い

グローバルな `swap` / `min` / `max` は exercise 自体が要求する API である。非修飾呼び出しと ADL の組み合わせで標準関数と競合しうる点は正しいが、この提出固有の設計ミスではない。`main.cpp` は `::swap` 等で明示しており、現状の呼び出しは安全である。

## 追加検証結果

- 全 exercise: `make re` 成功、必須フラグと C++98 で警告なし。
- 提出 main 3 本: すべて正常終了し、期待動作を確認。
- v1 の ex00 複合型テスト: AddressSanitizer / UndefinedBehaviorSanitizer 付きで正常終了。
- ex02 の例外安全性: `outputs/artifacts/tests/module07_v2_test.cpp` で、要素型の copy assignment が途中で throw するテストを追加実行。新規領域は解放され、代入先の size と内容は保持され、live object 数も不変だった。`operator=` の strong guarantee に関する v1 の評価は妥当。
- ex02 の幅広添字テスト: 同じ `module07_v2_test.cpp` を sanitizer 付きで実行し、`UINT_MAX + 1UL` が 0 に切り詰められることを確認。これは未定義動作ではなく API 型による論理的な範囲チェック漏れ。
- 禁止 API、C++11 機能、STL コンテナの新たな使用は見つからなかった。

## 最終評価

**PASS（ただし改善事項あり）**。

rubric の中心課題は満たしており、メモリ管理・コピー・const correctness・例外安全性は良好である。重大な runtime crash や leak は再検出されなかった。一方、外部テストを壊しうる `array` という生成物名は優先して修正し、続いて添字型の一貫性と `explicit` コンストラクタを改善すると、一般用途でもより堅牢になる。
