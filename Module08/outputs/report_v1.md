# CPP Module 08 Review Report (Rubric Based + Extra Strict Audit)
## 【判定】: PASS

ex00〜ex02 の必須ファイルが存在し、全 exercise が `-Wall -Wextra -Werror -pedantic -std=c++98` で警告なくビルド・実行できた。rubric の Template function / Span (STL algorithms, 改良 addNumber) / MutantStack を全て満たす。**Module 08 は STL 使用が必須・推奨**であり、`std::find`/`std::sort`/`std::min_element`/`std::max_element` を適切に使用。C++11 機能・禁止関数の使用はなく、全 exercise で 0 leaks。人間レビュアーの最終確認を前提とした総合 PASS 候補。

## 禁止関数チェック: PASS
- **Description**: `*printf` / `*alloc` / `free()` / `using namespace` / `friend` / C++11 機能 (`nullptr`/`auto`/range-for/`std::move` 等) の使用有無を検査。**Module 08 は STL コンテナ・`<algorithm>` が許可**されるため、それらは対象外。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module08
  grep -rEn "printf|[mcr]alloc|free\(|using[[:space:]]+namespace|friend" ex00 ex01 ex02 --include="*.cpp" --include="*.hpp"
  grep -rEn "nullptr|\boverride\b|\bfinal\b|constexpr|std::move|decltype|<cstdint>|<unordered" ex00 ex01 ex02 --include="*.cpp" --include="*.hpp"
  ```
- **Observation**: いずれも該当 0 件 (`for` 行の一致は `std::size_t`/`::iterator` 内の `::` に反応した誤検出で range-for ではない)。subject Module-specific rules に従い STL を積極使用しているが、これは Module 08 では要件であり違反ではない (C++ Module 監査ルール STL Policy: Module 08/09 は許可)。

## Preliminary tests

### Prerequisites
- **Description**: `c++ -Wall -Wextra -Werror` かつ C++98 でビルドできるか。各 exercise の `make`, `make re`, `make fclean`、ヘッダ単独コンパイルを検証。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module08/ex00 && make && make re && make fclean
  cd ~/42_cpp/Module08/ex01 && make && make re && make fclean
  cd ~/42_cpp/Module08/ex02 && make && make re && make fclean
  cd ~/42_cpp/Module08 && for h in ex00/easyfind.hpp ex01/Span.hpp ex02/MutantStack.hpp; do c++ -Wall -Wextra -Werror -std=c++98 -fsyntax-only -x c++ "$h" && echo "OK $h"; done
  ```
- **Observation**: 全 exercise でビルド exit 0・警告 0。各 Makefile は CXX=`c++`、CXXFLAGS=`-Wall -Wextra -Werror -pedantic -std=c++98` (`ex00/Makefile:3-4` 他)、`re` は `fclean all`。ヘッダ 3 本すべて `-fsyntax-only` で単独コンパイル成功 (self-contained + include guard)。例外型は `<stdexcept>` を各ヘッダで明示 include (`easyfind.hpp:5`, `Span.hpp:5`) しており、推移的インクルード依存はない。

### Forbidden Function
- **Description**: Forbidden Function フラグ対象の有無。
- **Status**: PASS (該当なし)
- **Evidence/Test Command**: 上記「禁止関数チェック」の全コマンド。
- **Observation**: 該当 0 件。

## Ex00: Easy find

### Template function
- **Description**: `easyfind(T, int)` が templated で subject どおり動作し、**STL アルゴリズムを使う**か (手動イテレータ探索は不可)。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module08/ex00 && make && ./easyfind; make fclean
  grep -n "std::find" ~/42_cpp/Module08/ex00/easyfind.hpp
  mkdir -p /tmp/Module08_review && cd ~/42_cpp/Module08 && c++ -Wall -Wextra -Werror -std=c++98 -Iex00 ~/42_cpp/Module08/outputs/artifacts/tests/ex00_empty.cpp -o /tmp/Module08_review/ex00_empty && /tmp/Module08_review/ex00_empty
  ```
- **Observation**:
  - `easyfind` は **`std::find`** を使用 (`ex00/easyfind.hpp:11-12,21-22`)。手動探索ではないため rubric「HAS to use STL algorithms」を満たす。
  - 非 const 版は `T::iterator`、const 版は `T::const_iterator` を返し、見つからなければ `std::out_of_range` を throw (`ex00/easyfind.hpp:9-26`)。subject の「throw するか error value を返す」の前者を採用。
  - main は vector(発見/未発見)、const vector、list、deque を網羅 (`ex00/main.cpp`)。空コンテナでも `std::out_of_range` を throw しクラッシュしない (`ex00_empty`)。
  - easyfind はクラスでなく関数テンプレートのため OCF 対象外 (rubric「non-interface class」なし)。

## Ex01: Span

### Class and member functions
- **Description**: subject 制約 (最大 N 個、`addNumber`、`shortestSpan`/`longestSpan`、不足時例外) に準拠し、**STL アルゴリズムで結果を求める**か。shortestSpan が「最小2数の差」だけでないか。OCF か。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module08/ex01 && make && ./span; make fclean
  grep -nE "std::sort|std::min_element|std::max_element" ~/42_cpp/Module08/ex01/Span.cpp
  ```
- **Observation**:
  - `shortestSpan` は `std::sort` 後に隣接差の最小を取る (`ex01/Span.cpp:55-65`)。「最小2数の差だけ」ではなく全隣接ペアを比較しており rubric の注意点を満たす。`longestSpan` は `std::min_element`/`std::max_element` の差 (`ex01/Span.cpp:72-76`)。
  - subject 例と一致: 入力 {6,3,17,9,11} で `shortest: 2 / longest: 14` (実測)。
  - 最大 N 超過で例外 (`ex01/Span.cpp:45-49`, 実測 `full: Span capacity exceeded`)、要素 0/1 で例外 (`ex01/Span.cpp:52-53,69-70`, 実測 `empty: A span requires at least two numbers`)。
  - OCF 4 メンバ完備 (`ex01/Span.hpp:18-22`)、`operator=` は copy-then-swap で例外安全 (`ex01/Span.cpp:16-24`)。
  - 10,000 要素テストあり (`ex01/main.cpp:32-41`, 実測 shortest 3 / longest 29997)。subject「at least 10,000 numbers」を満たす。

### Improved addNumber function
- **Description**: `addNumber()` を繰り返すより実用的な追加手段 (イテレータ範囲) があるか。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module08/ex01 && make && ./span; make fclean
  sed -n '26,35p' ~/42_cpp/Module08/ex01/Span.hpp
  ```
- **Observation**: `template <typename InputIterator> void addNumber(InputIterator first, InputIterator last)` を提供 (`ex01/Span.hpp:26-35`)。範囲を一括追加し、容量超過は `std::length_error` を throw。main で `large.addNumber(bulk.begin(), bulk.end())` により 10,000 要素を1回で追加 (`ex01/main.cpp:37`)。

## Ex02: Mutated abomination

### MutantStack class
- **Description**: `std::stack` を継承し全メンバ関数を提供、イテレータを持ち、subject 例の操作がイテレータで可能か。OCF か。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module08/ex02 && make && ./mutantstack; make fclean
  ```
- **Observation**:
  - `class MutantStack : public std::stack<T, Container>` (`ex02/MutantStack.hpp:9-10`) で std::stack を継承し、push/pop/top/size 等の全メンバを継承的に提供。
  - `iterator`/`const_iterator` を `container_type::iterator` から typedef し (`ex02/MutantStack.hpp:13-16`)、`begin()`/`end()` を protected メンバ `c`(基底の内部コンテナ) から実装 (`ex02/MutantStack.hpp:36-50`)。
  - subject 例の操作を実測で再現: push/top(17)/pop/size(1)/iterator 走査/`std::stack<int> s(mstack)` への変換すべて成功 (`ex02/main.cpp`)。
  - OCF 4 メンバ完備 (`ex02/MutantStack.hpp:19-33`)。

### Better tests
- **Description**: subject 例より多いテストが main にあるか。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module08/ex02 && make && ./mutantstack; make fclean
  ```
- **Observation**: subject 例に加え、const MutantStack の const_iterator 走査、`std::stack` への変換、`std::list` を土台にした `MutantStack<int, std::list<int> >` の走査を実施 (`ex02/main.cpp:24-41`)。subject「別コンテナに置換しても同じ出力」の趣旨を list-backed で確認。

## 破壊的テスト結果 (rubric)
- ex00: 空コンテナ検索で `std::out_of_range` を throw、未発見も throw。クラッシュなし (`ex00_empty`, `ex00/main.cpp`)。
- ex01: 容量超過 (単一/範囲) で `std::length_error`、要素 0/1 で `std::logic_error`。範囲追加失敗後も単一追加で回復可 (`span_overflow`)。
- ex02: 空スタックで `begin()==end()`、コピー/代入の独立性を確認 (`ex02_extra`)。
- SIGSEGV/SIGABRT/SIGBUS を誘発した入力はなし。

## メモリ検査
- **Description**: ヒープ確保と解放。全 exercise が STL コンテナ経由でヒープを使用。
- **Status**: PASS
- **採用方針**: macOS Memory Check Policy **方針 B (`leaks --atExit`)**。対象は stdin を読まず自走完了する専用テスト。サンドボックス内では `leaks` が task port 制約でハングするため escalated 権限で実行、再実行は 1 秒未満で完了しハングなし。
- **Evidence/Test Command** (対象プロセスを検査可能な権限で実行):
  ```sh
  cd ~/42_cpp/Module08/ex00 && make && leaks --atExit -- ./easyfind; make fclean
  cd ~/42_cpp/Module08/ex01 && make && leaks --atExit -- ./span; make fclean
  cd ~/42_cpp/Module08/ex02 && make && leaks --atExit -- ./mutantstack; make fclean
  ```
- **Observation**: ex00 `0 leaks for 0 total leaked bytes` (191 nodes)、ex01 `0 leaks` (191 nodes)、ex02 `0 leaks` (189 nodes)。追加ハーネス `span_overflow` も 0 leaks。

## 仕様との乖離
- 機能的な subject 乖離は検出されず。ex01 は subject 例と一致 (2/14)、10,000 要素要件も満たす。

## 修正要求
1. (任意) `ex01` の `addNumber(InputIterator, InputIterator)` は容量チェック前に `std::vector<int> pending(first, last)` へ全要素をコピーする (`ex01/Span.hpp:28`)。巨大な不正範囲でも一旦全コピーしてから throw するため、`std::distance` で個数を先に確認してから挿入する方が無駄がない (機能上は問題なし)。
2. (任意) `ex01` の `shortestSpan`/`longestSpan` は `unsigned int` を返す (`ex01/Span.hpp:37-38`)。overflow 安全 (下記 追加厳格レビュー参照) だが subject 例は `int` 前提。設計意図を defense で説明できるようにしておくこと。

## 追加厳格レビュー (rubric 非依存 / 別観点)

> rubric に無い観点から、設計・堅牢性・移植性・const 正しさ・例外安全を厳格に監査した所見。いずれも FAIL 事由ではなく、設計判断の確認・改善提案。

### [設計/堅牢性] ex01: span の戻り値型 `unsigned int` と整数オーバーフロー対策 — 妥当
- **観点**: `INT_MAX - INT_MIN` は `int` では表現不可能 (4294967295)。多くの実装が `int` 減算で UB/オーバーフローを起こす。
- **検証**: `distanceBetween` は符号処理で桁溢れを回避 (`ex01/Span.cpp:29-43`)。`{INT_MIN, 0, INT_MAX}` で `longestSpan()=4294967295`, `shortestSpan()=2147483647` を確認 (`span_overflow`)。
  ```sh
  mkdir -p /tmp/Module08_review && cd ~/42_cpp/Module08 && c++ -Wall -Wextra -Werror -std=c++98 -Iex01 ~/42_cpp/Module08/outputs/artifacts/tests/span_overflow.cpp ex01/Span.cpp -o /tmp/Module08_review/span_overflow && /tmp/Module08_review/span_overflow
  ```
- **評価**: `unsigned int` 戻り + `distanceBetween` の桁溢れ対策は、多くの実装が見落とす整数オーバーフローに正しく対処した**優れた堅牢性**。ただし subject 例は `int` を暗黙前提とするため、「なぜ unsigned にしたか」を説明できると良い (Notes [INFO])。

### [例外安全] ex01: `operator=` の copy-then-swap — 妥当
- **観点**: 代入中の例外で自己破壊しないか。
- **検証**: `operator=` は一時 `copied` を作ってから `swap` する (`ex01/Span.cpp:16-24`)。例外は copy 段階に限定され、swap は no-throw のため強い例外安全性に近い。自己代入ガードもあり。
- **評価**: 適切。

### [移植性] 例外ヘッダの明示 include — 良好 (Module06 の教訓に対応)
- **観点**: Module06 で `std::bad_cast` を `<typeinfo>` 非 include で使い、採点環境でコンパイル失敗した事例があった。
- **検証**: `easyfind.hpp:5` と `Span.hpp:5` は `<stdexcept>` を明示 include。`std::out_of_range`/`length_error`/`logic_error` は同ヘッダ由来で推移的インクルードに依存しない。
  ```sh
  grep -n "stdexcept" ~/42_cpp/Module08/ex00/easyfind.hpp ~/42_cpp/Module08/ex01/Span.hpp
  ```
- **評価**: 移植性の観点で良好。Module06 のような環境依存ビルド失敗リスクはない。

### [設計] ex02: protected メンバ `c` への依存 — 標準保証されており妥当
- **観点**: `begin()/end()` が `this->c` (基底 `std::stack` の内部コンテナ) に依存する (`ex02/MutantStack.hpp:37,41,45,49`)。
- **評価**: `std::stack` の下位コンテナが `protected` メンバ `c` であることは C++ 標準が規定しており、継承クラスからのアクセスは**移植的に安全**。MutantStack の定石実装。

### [const 正しさ] 全 exercise — 良好
- easyfind は const/非 const overload (`ex00/easyfind.hpp`)、Span の span 系は const メンバ (`ex01/Span.hpp:37-38`)、MutantStack は const `begin/end` を提供 (`ex02/MutantStack.hpp:44-49`)。const オブジェクトからの読み取りが全て可能 (`ex00/main.cpp:28-34`, `ex02/main.cpp:24-29` で実証)。

### [API 完全性] ex02: reverse_iterator 非提供 — 要件外だが言及
- **観点**: `MutantStack` は `begin/end` のみで `rbegin/rend`(reverse_iterator) を提供しない。
- **評価**: subject/rubric は forward イテレータのみ要求しており **要件外 (FAIL ではない)**。実用上 reverse も欲しければ `container_type::reverse_iterator` を typedef して `c.rbegin()/c.rend()` を返す拡張が可能、という補足に留まる。

### [軽微] ex01: `addNumber` 範囲版の事前コピー — 修正要求1と同旨
- 前掲の修正要求1。巨大不正範囲での二重確保は非効率だが機能・安全性に問題なし。

## Notes (severity-tagged)
- **[INFO] ex01 の span 系戻り値は `unsigned int`。** `ex01/Span.hpp:37-38`。整数オーバーフロー対策として妥当だが subject 例は int 前提。仕様違反ではない。
- **[INFO] ex01 範囲 addNumber は容量チェック前に全要素を pending へコピー。** `ex01/Span.hpp:28-34`。機能・安全性に問題なし、効率面の改善余地のみ。
- **[INFO] ex02 は protected メンバ `c` に依存。** `ex02/MutantStack.hpp:37` 他。C++ 標準保証で移植的に安全。
- **[INFO] Makefile は必須フラグに `-pedantic` を追加。** `ex00/Makefile:4` 他。警告 0 でビルド可能。問題なし。
- **[GOOD] 例外ヘッダ `<stdexcept>` を明示 include。** Module06 の推移的インクルード依存問題を回避できている。

## レビュイーへの説明要求リスト

> 以下は AI が自動生成した質問候補であり、defense で人間レビュアーが選別・拡張する前提。

### Critical Questions (Top 5)
1. ex00 で const/非 const の easyfind overload を分ける理由と、それぞれの戻り値型 (`iterator`/`const_iterator`) の意味を説明してください (`ex00/easyfind.hpp:9-26`)。
2. ex01 で `shortestSpan` を「ソート後の隣接差の最小」で求める理由 (最小2数の差では不十分な例) を説明してください (`ex01/Span.cpp:55-65`)。
3. ex01 で span 系を `unsigned int` 戻りにし、`distanceBetween` で桁溢れ対策した意図 (`INT_MIN`〜`INT_MAX` の距離) を説明してください (`ex01/Span.cpp:29-43`)。
4. ex02 で `std::stack` の protected メンバ `c` を使ってイテレータを実装できる理由 (標準保証) を説明してください (`ex02/MutantStack.hpp:37`)。
5. ex02 で MutantStack を別コンテナ (`std::list`) に置換しても同じ走査結果になる理由を説明してください (`ex02/main.cpp:34-41`)。

### Supplementary Questions
- ex01 範囲 addNumber を事前コピーせず `std::distance` で個数確認してから挿入する実装に変える利点を説明できるか。
- ex02 に reverse_iterator を追加する場合の実装方針を説明できるか。
- Module 08 で STL コンテナ・アルゴリズムが「解禁」される理由と、Module 07 までとの設計思想の違いを説明できるか。
- ex01 の `operator=` を copy-then-swap にした例外安全上の利点を説明できるか。

## Disclosure
42 subject Chapter III (AI Instructions) を尊重し、本レポートは学習・peer learning・人間による defense を代替しない補助的レビューである。

Reviewer: Agentic Reviewer (AI-generated, advisory only)
Final pass/fail decision: requires human reviewer confirmation
