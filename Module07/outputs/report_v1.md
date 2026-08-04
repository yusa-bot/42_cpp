# CPP Module 07 Review Report (Rubric Based)
## 【判定】: PASS

ex00〜ex02 の必須ファイルが存在し、全 exercise が `-Wall -Wextra -Werror -pedantic -std=c++98` で警告なくビルド・実行できた。rubric の Simple/Complex types / Iter / Array (Constructors, Access) を全て満たす。禁止関数・禁止 STL・C++11 機能の使用はなく、テンプレートはヘッダに定義、ex02 は `new[]` を使用し 0 leaks。人間レビュアーの最終確認を前提とした総合 PASS 候補。

## 禁止関数チェック: PASS
- **Description**: `*printf` / `*alloc` / `free()` / `using namespace` / `friend` / STL コンテナ・`<algorithm>` / C++11 機能の使用有無を検査。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module07
  grep -rEn "printf|[mcr]alloc|free\(|using[[:space:]]+namespace|friend" ex00 ex01 ex02 --include="*.cpp" --include="*.hpp"
  grep -rEn "#include[[:space:]]*<(vector|list|map|set|deque|algorithm|queue|stack|array|unordered_|tuple)" ex00 ex01 ex02 --include="*.cpp" --include="*.hpp"
  grep -rEn "nullptr|\boverride\b|\bfinal\b|constexpr|std::move|decltype|<chrono>|<thread>|<array>|<cstdint>" ex00 ex01 ex02 --include="*.cpp" --include="*.hpp"
  ```
- **Observation**: 3 コマンドとも一致 0 件。subject Chapter II の Allowed/Forbidden (C++11/Boost、`*printf/*alloc/free`、`using namespace`、`friend`、Module 08 未満の STL) に抵触しない。

## Preliminary tests

### Prerequisites
- **Description**: `c++ -Wall -Wextra -Werror` かつ C++98 でビルドできるか。各 exercise の `make`, `make re`, `make fclean`、およびヘッダ単独コンパイルを検証。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module07/ex00 && make && make re && make fclean
  cd ~/42_cpp/Module07/ex01 && make && make re && make fclean
  cd ~/42_cpp/Module07/ex02 && make && make re && make fclean
  cd ~/42_cpp/Module07 && for h in ex00/whatever.hpp ex01/iter.hpp ex02/Array.hpp; do c++ -Wall -Wextra -Werror -std=c++98 -fsyntax-only -x c++ "$h" && echo "OK $h"; done
  ```
- **Observation**: 全 exercise でビルド exit 0・警告 0。各 Makefile は CXX=`c++`、CXXFLAGS=`-Wall -Wextra -Werror -pedantic -std=c++98` (`ex00/Makefile:3-4`, `ex01/Makefile:3-4`, `ex02/Makefile:3-4`)、`re` は `fclean all`。ヘッダ 3 本すべて `-fsyntax-only` で単独コンパイル成功 (self-contained + include guard)。テンプレートはすべてヘッダ内に定義 (subject「Templates must be defined in the header files.」に準拠)。

### Forbidden Function
- **Description**: Forbidden Function フラグ対象の有無。
- **Status**: PASS (該当なし)
- **Evidence/Test Command**: 上記「禁止関数チェック」の全コマンド。
- **Observation**: 該当 0 件。

## Exercise 00: Start with a few functions

### Simple types
- **Description**: `swap`/`min`/`max` の 3 関数テンプレートが単純型 (int, std::string) で subject の期待出力どおり動くか。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module07/ex00 && make && ./whatever; make fclean
  ```
- **Observation**: 実行結果が subject Chapter IV の期待出力と一致:
  ```
  a = 3, b = 2
  min(a, b) = 2
  max(a, b) = 3
  c = chaine2, d = chaine1
  min(c, d) = chaine1
  max(c, d) = chaine2
  ```
  `swap` は参照 2 引数を入れ替え (`ex00/whatever.hpp:6-12`)、`min`/`max` は `<`/`>` で比較し等値時は第 2 引数を返す実装 (`ex00/whatever.hpp:14-22`)。

### Complex types
- **Description**: 3 関数が複合型 (比較演算子を備えた自作クラス、std::string) でも動くか。また「等値なら 2 番目を返す」仕様の確認。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  mkdir -p /tmp/Module07_review && cd ~/42_cpp/Module07 && c++ -Wall -Wextra -Werror -std=c++98 -Iex00 ~/42_cpp/Module07/outputs/artifacts/tests/ex00_complex.cpp -o /tmp/Module07_review/ex00_complex && /tmp/Module07_review/ex00_complex
  ```
- **Observation**: 自作クラス `Val` で `swap` が入れ替え成功 (a.v=2, b.v=1)、等値要素で `min`/`max` とも第 2 引数 (id=2) を返し subject 仕様「If they are equal, it returns the second one」に合致。std::string でも `min=abc / max=abd` と正しい。

## Exercise 01: Iter

### Does it work?
- **Description**: `iter(array, length, function)` が任意型配列で動作し、第 3 引数の関数が const 参照・非 const 参照のどちらでも受けられるか (subject「support both const and non-const elements」)。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module07/ex01 && make && ./iter; make fclean
  ```
- **Observation**: `iter` は `template <typename T, typename F> void iter(T *array, const std::size_t length, F function)` (`ex01/iter.hpp:9-13`)。`T` が要素の const 性を吸収するため const/非 const 両対応。main の実行結果:
  ```
  2 3 4      # 非const配列に increment(int&) を適用 → {1,2,3}→{2,3,4} を print
  4 5 6      # const 配列 readOnlyNumbers に print(const int&)
  template
  iter       # std::string 配列に print(const std::string&)
  ```
  非 const 要素の変更 (`increment(int&)`)、const 配列の読み取り (`print(const T&)`)、複数型 (int/std::string) すべて成功。

## Exercise 02: Array

> rubric 前提: 「If the inner allocation of the actual array does not come from a use of new[], don't grade this exercise.」→ `new[]` 使用を確認。「arrays of both simple and complex types」→ int と std::string で検証。

### Constructors
- **Description**: 空配列と `unsigned int n` 指定の配列を生成できるか。`new[]` による確保、デフォルト初期化、深いコピー/代入の独立性。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module07/ex02 && make && ./array; make fclean
  grep -n "new T\[" ~/42_cpp/Module07/ex02/Array.hpp
  mkdir -p /tmp/Module07_review && cd ~/42_cpp/Module07 && c++ -Wall -Wextra -Werror -std=c++98 -Iex02 ~/42_cpp/Module07/outputs/artifacts/tests/ex02_deep.cpp -o /tmp/Module07_review/ex02_deep && /tmp/Module07_review/ex02_deep
  ```
- **Observation**:
  - 空配列 `Array<int> empty;` は `_elements(0), _size(0)` (`ex02/Array.hpp:15`)、サイズ指定は `new T[n]()` で確保 (`ex02/Array.hpp:17-20`)。`new T[]` は 2 箇所 (`ex02/Array.hpp:19,37`) で使用し rubric の new[] 要件を満たす。事前確保 (preventive allocation) はなし。
  - `new T[n]()` の値初期化により int 要素は 0 (実行の `default value: 0`)、subject の Tip (`new int()` → 0) に沿う。
  - 深いコピー独立性: copy ctor で `b[0]=999` にしても `a[0]=10` のまま、operator= で `c[1]=777` にしても `a[1]=20` のまま (`ex02_deep` 出力)。copy ctor は operator= に委譲 (`ex02/Array.hpp:22-24`)、operator= は new→try/catch コピー→旧 delete[] の順で例外安全 (`ex02/Array.hpp:30-51`)。
  - 自己代入ガードあり (`ex02/Array.hpp:33`)、サイズ縮小代入 (`big = empty` → size 0) も正しく更新。
  - OCF 4 メンバ完備 (`ex02/Array.hpp:15,17/22,26,30`)。複合型 std::string でも生成・代入・表示が正常 (main の `hello array`)。

### Access
- **Description**: `operator[]` による読み書き (const インスタンスでは読み取りのみ)、範囲外アクセスで `std::exception` を投げるか。
- **Status**: PASS
- **Evidence/Test Command**: 上記 Constructors の `./array` および `ex02_deep` 実行 (同じ Evidence)。
- **Observation**:
  - 非 const 版 `T& operator[](unsigned int)` で書き込み (`numbers[0]=42`)、const 版 `const T& operator[](unsigned int) const` で const インスタンスの読み取り (`readOnly[0]` → 42) (`ex02/Array.hpp:53-63`)。
  - 範囲外は `std::out_of_range`(= `std::exception` 派生) を throw: 空配列 `empty[0]`、`a[3]`、main の `numbers[3]` すべてで捕捉 (`exception: Array index is out of bounds`)。境界 `a[2]` は正常アクセス。
  - `_elements` が NULL の空配列でも、`index >= _size` 判定が先に効くため未割り当てメモリへアクセスしない (subject「must never access non-allocated memory」に準拠)。

## 破壊的テスト結果
- ex00: 複合型 `Val` での swap/min/max、等値時の「第 2 引数を返す」挙動を確認 (`ex00_complex`)。クラッシュなし。
- ex02: 深いコピー/代入の独立性、自己代入、空配列 `[0]`・境界 `[3]` での `std::out_of_range`、サイズ縮小代入を確認 (`ex02_deep`)。クラッシュなし。
- SIGSEGV/SIGABRT/SIGBUS を誘発した入力はなし。

## メモリ検査
- **Description**: ヒープ確保と解放を検査。`new[]` は ex02 のみ (`ex02/Array.hpp:19,37`)。ex00/ex01 は動的確保なし。
- **Status**: PASS
- **採用方針**: macOS Memory Check Policy **方針 B (`leaks --atExit`)**。
- **方針 B の条件確認**:
  1. 対象 `./array` と保存済み `ex02_deep` harness は stdin を読まず自走完了する専用テスト。
  2. 本 macOS サンドボックス内では `leaks` が task port 制約でハングするため escalated(非サンドボックス)権限で実行。再実行は 1 秒未満で完了しハングなし。
- **Evidence/Test Command** (対象プロセスを検査可能な権限で実行):
  ```sh
  cd ~/42_cpp/Module07/ex02 && make && leaks --atExit -- ./array; make fclean
  mkdir -p /tmp/Module07_review && cd ~/42_cpp/Module07 && c++ -Wall -Wextra -Werror -std=c++98 -Iex02 ~/42_cpp/Module07/outputs/artifacts/tests/ex02_deep.cpp -o /tmp/Module07_review/ex02_deep && leaks --atExit -- /tmp/Module07_review/ex02_deep
  ```
- **Observation**: `./array`: `Process 5561: 191 nodes malloced for 32 KB` / `0 leaks for 0 total leaked bytes.`。`ex02_deep`: `Process 5586: 191 nodes malloced for 32 KB` / `0 leaks`。`new[]` は全経路で `delete[]` と対称 (`ex02/Array.hpp:27,42,47`)。

## 仕様との乖離
- 機能的な subject 乖離は検出されず。ex00 の出力は subject 例と文字単位一致。ex02 は new[] 必須・空配列・n 指定・深いコピー・範囲外例外・size() をすべて満たす。

## 修正要求
1. (任意・堅牢性) `ex00` の `min`/`max` は `const T&` を返す (`ex00/whatever.hpp:15,20`)。lvalue 同士では問題ないが、一時オブジェクト同士 (`::min(T(1), T(2))`) を渡すと戻り参照が dangling になりうる。subject の用法 (lvalue) では実害なく FAIL 事由ではないが、defense で戻り値の寿命について説明できるようにしておくこと。
2. (任意) `ex00` の `swap`/`min`/`max` は汎用名のグローバル関数テンプレート。main は `::swap` と修飾して呼んでおり衝突しないが (`ex00/main.cpp:10,18`)、非修飾呼び出しでは ADL により `std::swap` 等と競合しうる点を理解しておくこと。
3. (任意・堅牢性) `ex02` の `Array(unsigned int n)` は単一引数の変換コンストラクタで、`explicit` 未指定のため `unsigned int`/`int` から `Array<T>` への暗黙変換を許す (`ex02/Array.hpp:17`)。`explicit Array(unsigned int n)` とマークすると、`Array<int> a = 5;` や関数実引数 `f(7)` のような意図しない暗黙変換をコンパイル時に禁止でき、より堅牢になる。subject/rubric は `explicit` を要求しておらず、`ex02/main.cpp` も暗黙変換に依存していない (生成は直接初期化・コピー・代入のみ、`ex02/main.cpp:9-47`) ため FAIL 事由ではなく、後付けしても既存コードは壊れない。検証: `printf '%s\n' '#include "Array.hpp"' 'int main(){ Array<int> a = 5; (void)a; return 0; }' > /tmp/imp.cpp && c++ -Wall -Wextra -Werror -std=c++98 -I/Users/fujiki/workspace/42tokyo/cpp-module/cpp-module--ayusa/Module07/ex02 /tmp/imp.cpp -o /tmp/imp` は現状コンパイル成功 (暗黙変換が通る) が、`explicit` 付与版では `error: no viable conversion from 'int' to 'Array<int>'` となる。

## Notes (severity-tagged)
- **[INFO] `ex00` の `min`/`max` は `const T&` 戻り。** `ex00/whatever.hpp:15-22`。lvalue 用法では安全。一時オブジェクトを渡すと dangling の可能性があるが subject 用法では発生しない。仕様違反ではない。
- **[INFO] `ex02` の `operator[]` は `unsigned int index`。** `ex02/Array.hpp:53,59`。subject は index 型を指定しておらず、負値は符号なしに変換され `>= _size` で例外化するため安全。fugu 版 (int index + 負値チェック) とは実装方針が異なるがいずれも妥当。
- **[INFO] `ex02` の `Array(unsigned int n)` は `explicit` 未指定の変換コンストラクタ。** `ex02/Array.hpp:17`。`unsigned int`/`int` → `Array<T>` の暗黙変換を許す (実測: `Array<int> a = 5;` がコンパイル成功)。subject/rubric は `explicit` を要求せず、`ex02/main.cpp` も暗黙変換に非依存のため仕様違反ではないが、`explicit` 付与で意図しない変換を防げる (修正要求 3)。
- **[INFO] `ex02` は `new T[n]()` で値初期化。** `ex02/Array.hpp:19,37`。scalar は 0 初期化され subject Tip に沿う。空配列は `_elements=NULL` で `new[]` を呼ばない (preventive allocation 回避) が、rubric の「allocation は new[] 由来」要件は n>0 経路で満たす。
- **[INFO] Makefile は必須フラグに `-pedantic` を追加。** `ex00/Makefile:4` 他。必須フラグを失わず警告 0 でビルド可能。問題なし。

## Prior Report Reconciliation

N/A (過去レポートなし)。レビュー開始時、`cpp-module--ayusa/Module07/outputs` は存在せず、`ls` は `No such file or directory`。本レビューで outputs と artifacts/tests を新規作成した。

## クリーンアップ確認
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module07 && for d in ex00 ex01 ex02; do (cd "$d" && make fclean); done
  git -C /Users/fujiki/workspace/42tokyo/cpp-module status --short cpp-module--ayusa/Module07
  ```
- **Observation**: 各 exercise の binary / `.o` / `.d` を除去。レビュー対象既存ソースに変更なし。新規生成物は `cpp-module--ayusa/Module07/outputs/` 以下の report と artifacts/tests のみ (`git status --short` は `?? outputs/` のみ)。

## レビュイーへの説明要求リスト

> 以下は AI が自動生成した質問候補であり、defense で人間レビュアーが選別・拡張する前提。

### Critical Questions (Top 5)
1. ex00 で `min`/`max` を `const T&` 戻りにした設計と、一時オブジェクトを渡した場合の戻り参照の寿命について説明してください (`ex00/whatever.hpp:15-22`)。
2. ex01 で `iter` が const 要素と非 const 要素の両方に対応できる仕組み (`T` による const 吸収と `F` の受け方) を説明してください (`ex01/iter.hpp:9-13`)。
3. ex02 の `operator=` が new→コピー→旧 delete[] の順である理由 (例外安全性) と、自己代入ガードの意義を説明してください (`ex02/Array.hpp:30-51`)。
4. ex02 で `new T[n]()`(値初期化) と `new T[n]`(デフォルト初期化) の違いを理解しているか、subject の Tip との関係を説明してください (`ex02/Array.hpp:19`)。
5. ex02 の `operator[]` を const/非 const の 2 版で用意する理由と、空配列 (`_elements=NULL`) で未割り当てメモリにアクセスしない根拠を説明してください (`ex02/Array.hpp:53-63`)。

### Supplementary Questions
- テンプレート定義をヘッダに置く必要がある理由 (実体化のタイミング) を説明できるか。
- ex00 の汎用名テンプレートを非修飾呼び出しした場合の ADL による std:: との競合可能性を理解しているか。
- ex02 の copy ctor を operator= に委譲する設計の利点と注意点 (初期化順序) を説明できるか。
- ex02 を `.tpp` に分離しなかった判断 (単一ヘッダ内定義) の是非を説明できるか。

## Disclosure
42 subject Chapter III (AI Instructions) を尊重し、本レポートは学習・peer learning・人間による defense を代替しない補助的レビューである。

Reviewer: Agentic Reviewer (AI-generated, advisory only)  
Final pass/fail decision: requires human reviewer confirmation
