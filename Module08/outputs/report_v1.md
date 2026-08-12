# CPP Module 08 レビューレポート（公式 Intra Rubric + 厳格監査）

## Environment Metadata
- Compiler: `Apple clang version 21.0.0 (clang-2100.1.1.101)`
- Memory checker: `/usr/bin/leaks` (Report Version 4.0)
- `pdftotext`: `pdftotext version 26.03.0`
- レビュー対象: `~/42_cpp/Module08`
- Subject: `08/docs/cpp-module-08.v10_1.en.subject.pdf` (Version 10.1)
- Rubric: `docs/Intra Projects CPP Module 08 Edit.html`
- 過去レポート: `~/42_cpp/Module08/outputs/report_v1.md`（更新前の同ファイル）

## Subject / Rubric 全要件一覧

### Subject — 一般ルール
1. `c++ -Wall -Wextra -Werror` でコンパイルし、`-std=c++98`を追加しても成功すること。
2. 指定されたディレクトリ名・ファイル名・クラス名・関数名を使用すること。
3. 外部ライブラリ、C++11以降の機能、Boost、`*printf`、`*alloc`、`free`を使用しないこと。
4. 明示的に許可されない限り、`using namespace`と`friend`を使用しないこと。
5. Module 08ではSTLコンテナ・アルゴリズムが許可され、適切な箇所では可能な限り使用すること。
6. Module 02〜09のクラスは、明示的な例外を除きOrthodox Canonical Form（OCF）に従うこと。
7. テンプレート以外の関数実装をヘッダに置かないこと。
8. ヘッダは単独で使用でき、二重includeを防止すること。
9. メモリリークを発生させないこと。
10. C/C++プロジェクト標準に従い、Makefileに`all`、`clean`、`fclean`、`re`を用意すること。

### Subject — Exercise 00: Easy find
1. `Makefile`、`main.cpp`、`easyfind.h`または`easyfind.hpp`を提出すること。`easyfind.tpp`は任意。
2. 整数コンテナを受け取る関数テンプレート`easyfind(T, int)`を実装すること。
3. 指定整数の最初の出現を検索すること。
4. 見つからない場合は例外を投げるか、エラー値を返すこと。
5. 独自のテストを提出すること。
6. 連想コンテナへの対応は不要。

### Subject — Exercise 01: Span
1. `Makefile`、`main.cpp`、`Span.h`または`Span.hpp`、`Span.cpp`を提出すること。
2. `Span`は最大`N`個の整数を格納し、`N`はコンストラクタに渡す`unsigned int`であること。
3. `addNumber()`は整数を1個追加し、満杯なら例外を投げること。
4. `shortestSpan()`と`longestSpan()`は格納値間の最短・最長距離を返すこと。
5. 格納数が0または1の場合、両span関数は例外を投げること。
6. サンプル`{6,3,17,9,11}`で`2`と`14`を得ること。
7. 少なくとも10,000個の数でテストすること。
8. iterator rangeを使い、1回の呼び出しで複数の数を追加するメンバ関数を実装すること。
9. 一般ルールに従い`Span`をOCFで実装すること。

### Subject — Exercise 02: Mutated abomination
1. `Makefile`、`main.cpp`、`MutantStack.h`または`MutantStack.hpp`を提出すること。`MutantStack.tpp`は任意。
2. `MutantStack`を`std::stack`を用いて実装すること。
3. `std::stack`の全メンバ関数に加えてiteratorを提供すること。
4. iterator走査と`std::stack`へのコピー構築を含むsubjectサンプルの操作を実行できること。
5. 別の走査可能コンテナで同等の操作を行った場合、同等の要素出力になること。
6. 独自のテストを提出すること。
7. 一般ルールに従い`MutantStack`をOCFで実装すること。

### 公式Intra rubric — 記載順
1. `Preliminary tests` / `Prerequisites`
2. `Exercise 00: Easy find` / `Makefile`
3. `Exercise 00: Easy find` / `Template function`
4. `Exercise 01: Span` / `Makefile`
5. `Exercise 01: Span` / `Class and member functions`
6. `Exercise 01: Span` / `Improved addNumber function`
7. `Exercise 02: Mutated abomination` / `Makefile`
8. `Exercise 02: Mutated abomination` / `MutantStack class`
9. `Exercise 02: Mutated abomination` / `Better tests`
10. 人間によるdefenseで確認するフラグ: `Can’t support / explain code`。

## 【判定】: PASS

9個のYes/No rubric項目はすべてPASS。必須提出ファイル10個はすべてレビュー対象Gitリポジトリに追跡されている。3 exerciseはいずれも提出Makefileのフラグで警告なくコンパイル・正常実行でき、適切なSTLアルゴリズム／コンテナを使用し、機能要件を満たす。`Can’t support / explain code`フラグは静的・自動レビューでは判定できないため、人間によるdefense確認を要する。

## 禁止関数チェック: PASS
- **Description**: 禁止されたC関数、外部／新言語機能、`using namespace`、`friend`を検査し、Module 08でのSTL使用が許可範囲かつ適切かを確認する。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  rg -n 'printf|malloc|calloc|realloc|free\s*\(|using\s+namespace|\bfriend\b|nullptr|\boverride\b|\bfinal\b|constexpr|std::move|decltype|unordered_|#include\s*<(chrono|thread|mutex|atomic|random|regex|tuple|array|forward_list)>' ~/42_cpp/Module08/ex00 ~/42_cpp/Module08/ex01 ~/42_cpp/Module08/ex02
  grep -nE 'std::find|std::sort|std::min_element|std::max_element|std::vector|std::stack' ~/42_cpp/Module08/ex00/easyfind.hpp ~/42_cpp/Module08/ex01/Span.hpp ~/42_cpp/Module08/ex01/Span.cpp ~/42_cpp/Module08/ex02/MutantStack.hpp
  ```
- **Observation**: 禁止featureのscanは該当0件。`std::find`、`std::sort`、`std::min_element`、`std::max_element`、`std::vector`、`std::stack`を適切なexerciseで使用しており、Module 08固有のSTL要件に合致する。

## Preliminary tests

### Prerequisites
- **Description**: 必須ファイルがGit追跡されていること、必須フラグによるC++98コンパイル、ヘッダの自己完結性とinclude guard、OCF、非テンプレート実装の配置、メモリ挙動を確認する。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  git -C ~/42_cpp ls-files Module08/ex00 Module08/ex01 Module08/ex02 | sort
  c++ -Wall -Wextra -Werror -std=c++98 -fsyntax-only -x c++ ~/42_cpp/Module08/ex00/easyfind.hpp
  c++ -Wall -Wextra -Werror -std=c++98 -fsyntax-only -x c++ ~/42_cpp/Module08/ex01/Span.hpp
  c++ -Wall -Wextra -Werror -std=c++98 -fsyntax-only -x c++ ~/42_cpp/Module08/ex02/MutantStack.hpp
  grep -nE '^#ifndef|^# define|^#endif' ~/42_cpp/Module08/ex00/easyfind.hpp ~/42_cpp/Module08/ex01/Span.hpp ~/42_cpp/Module08/ex02/MutantStack.hpp
  grep -nE 'Span\(|operator=|~Span' ~/42_cpp/Module08/ex01/Span.hpp
  grep -nE 'MutantStack\(|operator=|~MutantStack' ~/42_cpp/Module08/ex02/MutantStack.hpp
  mkdir -p /tmp/Module08_review
  c++ -Wall -Wextra -Werror -std=c++98 -I~/42_cpp/Module08/ex00 -I~/42_cpp/Module08/ex01 -I~/42_cpp/Module08/ex02 ~/42_cpp/Module08/outputs/artifacts/tests/memory_hold.cpp ~/42_cpp/Module08/ex01/Span.cpp -o /tmp/Module08_review/memory_hold
  /bin/zsh -lc '(/tmp/Module08_review/memory_hold & pid=$!; sleep 1; leaks "$pid"; leaks_rc=$?; wait "$pid"; prog_rc=$?; echo "leaks_exit=$leaks_rc program_exit=$prog_rc")'
  ```
- **Observation**: 必須提出ファイル10個すべてをGitが追跡している。3ヘッダはいずれも単独コンパイルでき、include guardを備える。`Span`と`MutantStack`にはdefault constructor、copy constructor、copy assignment、destructorがある。`Span`の非テンプレート実装は`Span.cpp`にあり、ヘッダ内の関数本体はテンプレートである。メモリ検査は方針A（PID注入）を採用し、保存済み`memory_hold.cpp`を意図的に8秒間生存させて実行中に`leaks`を接続した。結果は`188 nodes malloced for 107 KB`、`0 leaks for 0 total leaked bytes`で、`leaks`とプログラムはいずれも終了コード0だった。

## Exercise 00: Easy find

### Makefile
- **Description**: Makefileが適切なcompiler/flagsでビルドし、`all`、`clean`、`fclean`、`re`が動作することを確認する。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  make -C ~/42_cpp/Module08/ex00 fclean
  make -C ~/42_cpp/Module08/ex00
  ~/42_cpp/Module08/ex00/easyfind
  make -C ~/42_cpp/Module08/ex00 re
  make -C ~/42_cpp/Module08/ex00 fclean
  find ~/42_cpp/Module08/ex00 -maxdepth 1 \( -name '*.o' -o -name '*.d' -o -name easyfind \) -print
  ```
- **Observation**: Makefileは`CXX = c++`、`CXXFLAGS = -Wall -Wextra -Werror -pedantic -std=c++98`を使用する（`ex00/Makefile:3-4`）。`make`と`make re`は成功し、実行ファイルは終了コード0。`fclean`後に実行ファイル、`.o`、`.d`は残らない。

### Template function
- **Description**: テンプレート`easyfind(T, int)`、STL algorithmによる最初の一致の検索、未発見処理、十分なテストを確認する。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  grep -n 'std::find' ~/42_cpp/Module08/ex00/easyfind.hpp
  mkdir -p /tmp/Module08_review
  c++ -Wall -Wextra -Werror -std=c++98 -I~/42_cpp/Module08/ex00 ~/42_cpp/Module08/outputs/artifacts/tests/ex00_empty.cpp -o /tmp/Module08_review/ex00_empty
  /tmp/Module08_review/ex00_empty
  c++ -Wall -Wextra -Werror -std=c++98 -I~/42_cpp/Module08/ex00 ~/42_cpp/Module08/outputs/artifacts/tests/ex00_const_container_ayusa.cpp -o /tmp/Module08_review/ex00_const_container_ayusa
  /tmp/Module08_review/ex00_const_container_ayusa
  ```
- **Observation**: mutable版とconst版はいずれも`std::find`を使用し（`easyfind.hpp:9-25`）、一致iteratorを返し、未発見なら`std::out_of_range`を投げる。提出`main.cpp`は`vector`、const `vector`、`list`、`deque`、重複時の最初の一致、未発見をテストする。空コンテナharnessは期待どおり例外を報告し、constコンテナharnessはコンパイル・実行に成功して`compiled: const container OK`を出力した。

## Exercise 01: Span

### Makefile
- **Description**: Makefileが適切なcompiler/flagsでビルドし、`all`、`clean`、`fclean`、`re`が動作することを確認する。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  make -C ~/42_cpp/Module08/ex01 fclean
  make -C ~/42_cpp/Module08/ex01
  ~/42_cpp/Module08/ex01/span
  make -C ~/42_cpp/Module08/ex01 re
  make -C ~/42_cpp/Module08/ex01 fclean
  find ~/42_cpp/Module08/ex01 -maxdepth 1 \( -name '*.o' -o -name '*.d' -o -name span \) -print
  ```
- **Observation**: Makefileは`CXX = c++`、`CXXFLAGS = -Wall -Wextra -Werror -pedantic -std=c++98`を使用する（`ex01/Makefile:3-4`）。`make`と`make re`は成功し、実行ファイルは終了コード0。`fclean`後に実行ファイル、`.o`、`.d`は残らない。

### Class and member functions
- **Description**: 容量制限、単一要素追加、両span計算と例外、STL algorithm使用、sample結果、OCF、10,000要素テストを確認する。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  grep -nE 'std::sort|std::min_element|std::max_element' ~/42_cpp/Module08/ex01/Span.cpp
  make -C ~/42_cpp/Module08/ex01
  ~/42_cpp/Module08/ex01/span
  make -C ~/42_cpp/Module08/ex01 fclean
  mkdir -p /tmp/Module08_review
  c++ -Wall -Wextra -Werror -std=c++98 -I~/42_cpp/Module08/ex01 ~/42_cpp/Module08/outputs/artifacts/tests/span_overflow.cpp ~/42_cpp/Module08/ex01/Span.cpp -o /tmp/Module08_review/span_overflow
  /tmp/Module08_review/span_overflow
  ```
- **Observation**: 容量は`unsigned int`で保持され、constructorでreserveされる（`Span.cpp:8-10`）。`addNumber(int)`は満杯なら例外を投げる（`Span.cpp:45-49`）。sampleはshortest `2`、longest `14`を出力した。`shortestSpan()`はsort後に全隣接差を調べ（`Span.cpp:51-65`）、単に最小の2値を引く実装ではない。`longestSpan()`は`std::min_element`と`std::max_element`を使用する（`Span.cpp:68-76`）。要素数0/1では例外を投げる。提出`main.cpp:32-41`はちょうど10,000個をテストする。境界harnessではlongestが`UINT_MAX`、shortestが`INT_MAX`となり、容量超過rangeの失敗後もオブジェクトを再利用できた。

### Improved addNumber function
- **Description**: iterator rangeから1回の呼び出しで複数値を追加し、容量超過を拒否するメンバ関数を確認する。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  sed -n '24,35p' ~/42_cpp/Module08/ex01/Span.hpp
  mkdir -p /tmp/Module08_review
  c++ -Wall -Wextra -Werror -std=c++98 -I~/42_cpp/Module08/ex01 ~/42_cpp/Module08/outputs/artifacts/tests/ex01_range_input.cpp ~/42_cpp/Module08/ex01/Span.cpp -o /tmp/Module08_review/ex01_range_input
  /tmp/Module08_review/ex01_range_input
  ```
- **Observation**: `template <typename InputIterator> void addNumber(InputIterator first, InputIterator last)`が`Span.hpp:26-35`に実装され、追加候補数を確認した後にrangeを1回で挿入する。10,000要素テストでも使用される（`main.cpp:37`）。single-passの`std::istream_iterator<int>` rangeでshortest `1`、longest `8`を得た。容量超過rangeは追加先を変更する前に例外となり、その後`10`と`20`を追加してshortest `10`を得た。

## Exercise 02: Mutated abomination

### Makefile
- **Description**: Makefileが適切なcompiler/flagsでビルドし、`all`、`clean`、`fclean`、`re`が動作することを確認する。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  make -C ~/42_cpp/Module08/ex02 fclean
  make -C ~/42_cpp/Module08/ex02
  ~/42_cpp/Module08/ex02/mutantstack
  make -C ~/42_cpp/Module08/ex02 re
  make -C ~/42_cpp/Module08/ex02 fclean
  find ~/42_cpp/Module08/ex02 -maxdepth 1 \( -name '*.o' -o -name '*.d' -o -name mutantstack \) -print
  ```
- **Observation**: Makefileは`CXX = c++`、`CXXFLAGS = -Wall -Wextra -Werror -pedantic -std=c++98`を使用する（`ex02/Makefile:3-4`）。`make`と`make re`は成功し、実行ファイルは終了コード0。`fclean`後に実行ファイル、`.o`、`.d`は残らない。

### MutantStack class
- **Description**: `MutantStack`が`std::stack`操作に加えてiteratorを提供し、subject sampleの操作を実行できることを確認する。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  sed -n '9,50p' ~/42_cpp/Module08/ex02/MutantStack.hpp
  make -C ~/42_cpp/Module08/ex02
  ~/42_cpp/Module08/ex02/mutantstack
  make -C ~/42_cpp/Module08/ex02 fclean
  mkdir -p /tmp/Module08_review
  c++ -Wall -Wextra -Werror -std=c++98 -I~/42_cpp/Module08/ex02 ~/42_cpp/Module08/outputs/artifacts/tests/ex02_extra.cpp -o /tmp/Module08_review/ex02_extra
  /tmp/Module08_review/ex02_extra
  c++ -Wall -Wextra -Werror -std=c++98 -I~/42_cpp/Module08/ex02 ~/42_cpp/Module08/outputs/artifacts/tests/ex02_compare_list.cpp -o /tmp/Module08_review/ex02_compare_list
  /tmp/Module08_review/ex02_compare_list
  ```
- **Observation**: `MutantStack<T, Container>`は`std::stack<T, Container>`をpublic継承するため（`MutantStack.hpp:9-10`）、stack操作をすべて利用できる。protectedなunderlying container `c`を介し、mutable/const iterator typedefと`begin()/end()`を公開する（`MutantStack.hpp:13-49`）。subjectの`push`、`top`、`pop`、`size`、iterator走査、`MutantStack<int>`から`std::stack<int>`への構築はすべて成功した。追加harnessで空rangeとcopy/assignmentの独立性を確認し、比較harnessでは`MutantStack<int>`と`std::list<int>`に同じ操作列を適用して、同一の`5 3 5 737 0`を出力し終了コード0となった。

### Better tests
- **Description**: `main()`がsubject例より多くの挙動をテストすることを確認する。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  nl -ba ~/42_cpp/Module08/ex02/main.cpp
  make -C ~/42_cpp/Module08/ex02
  ~/42_cpp/Module08/ex02/mutantstack
  make -C ~/42_cpp/Module08/ex02 fclean
  ```
- **Observation**: subjectの操作列に加え、`main.cpp:24-29`はconst `MutantStack`を`const_iterator`で走査し、`main.cpp:31-32`は`std::stack`への変換／copyを確認し、`main.cpp:34-41`はlist-backed `MutantStack`を生成して走査する。sampleより明確に広いテストである。

## 破壊的テスト結果
- ex00: 空コンテナと未発見値では`std::out_of_range`となり、クラッシュしない（`outputs/artifacts/tests/ex00_empty.cpp`、`ex00/main.cpp:22-26`）。
- ex01: 要素数0/1、単一追加の容量超過、range容量超過で例外となる。失敗したrange追加後も再利用でき、`INT_MIN`から`INT_MAX`までの距離をsigned overflow UBなしで処理する（`outputs/artifacts/tests/span_overflow.cpp`）。
- ex01: single-passの`std::istream_iterator` rangeが動作し、range容量超過時に`Span`内容は部分変更されない（`outputs/artifacts/tests/ex01_range_input.cpp`）。
- ex02: 空stackで`begin()==end()`となり、copy/assignment後のstackは独立する（`outputs/artifacts/tests/ex02_extra.cpp`）。MutantStackとlistに同じ操作列を適用すると同一のserialized outputを得る（`outputs/artifacts/tests/ex02_compare_list.cpp`）。
- rubricに関係するクラッシュ、誤出力、状態破壊は観測されなかった。

## 仕様との乖離
- subjectまたは公式Intra rubricからの機能的な乖離は検出されなかった。
- 提出テストの出力label（例: `shortest:`）はsubjectの最小sampleと異なるが、subjectは固定UIの完全一致ではなく期待値を示しているため、数値結果`2`と`14`が正しい本実装は要件を満たす。

## Notes (severity-tagged)
- **[GOOD] `Span(unsigned int)`は`explicit`に修正済み。** `Span.hpp:19`。負のコンパイルテスト`outputs/artifacts/tests/ex01_span_implicit_harm.cpp`は、`Span s = 5`と`report(5)`の双方がコンパイルエラーとなることを確認した。
- **[INFO] range overloadは容量検証前に一時`std::vector<int>`を生成する。** `Span.hpp:28-34`。single-pass input iteratorに対応し、挿入のatomicityを得られる一方、rangeを一時的に複製し、拒否されるrangeでもallocationが起き得る。
- **[INFO] spanメソッドは`unsigned int`を返す。** `Span.hpp:37-38`。非負距離と整合し、レビュー環境では`INT_MAX - INT_MIN`相当も表現できる。subjectは戻り値型を指定していない。
- **[INFO] `MutantStack`はprotectedな`std::stack::c`へアクセスする。** `MutantStack.hpp:36-49`。このexerciseにおける通常かつportableな実装方法である。
- **[INFO] 公式rubricの`Can’t support / explain code`はdefense専用フラグである。** 自動レビューでは本人の理解を確認できないため、人間レビュアーが下記の説明質問を行う必要がある。
- **[GOOD]** Required files are tracked, headers are self-contained, OCF is present, and STL is used appropriately throughout.

## Prior Report Reconciliation

参照した過去レポート: `~/42_cpp/Module08/outputs/report_v1.md`（更新前の内容）

| 旧指摘・所見 | 旧重大度 | 新分類 | 根拠 |
|---|---|---|---|
| 総合判定 PASS | PASS | Carried over | Official Intra rubricの全9項目を再検証し全てPASS。 |
| 禁止関数/C++11機能なし、STL使用は適切 | PASS | Carried over | 禁止featureの`rg`は0件。STL algorithm/container使用箇所を再確認。 |
| ex00 `easyfind`は`std::find`を使用しconst版も提供 | PASS/GOOD | Carried over | `easyfind.hpp:9-25`、空・const container harnessの実測。 |
| ex01 sample、容量、例外、10,000件、STL algorithm | PASS | Carried over | `span`実行と`span_overflow.cpp`を再実測。 |
| ex01 iterator-range版`addNumber` | PASS | Carried over | Official rubricでも独立項目。`Span.hpp:26-35`と`ex01_range_input.cpp`で確認。 |
| ex02 iterator、const iterator、list-backed test | PASS | Carried over | `MutantStack.hpp:9-49`、submitted main、`ex02_extra.cpp`で確認。 |
| Makefileは問題なし | INFO | Carried over and promoted into rubric sections | Official Intra rubricには各exerciseの独立Makefile項目があるため、3項目として明示し全てPASS。 |
| range版`addNumber`はtemporary vectorを使うため効率改善余地 | INFO/任意修正 | Carried over | `Span.hpp:28-34`。input iterator対応とatomicityの利点も併記し、FAIL扱いしない。 |
| span戻り値`unsigned int` | INFO | Carried over | `Span.hpp:37-38`とportable boundary harnessで`UINT_MAX`まで確認。 |
| copy-then-swapは妥当 | GOOD | Carried over | `Span.cpp:16-24`。新レポートではrubric項目内のOCF/設計確認へ統合。 |
| `<stdexcept>`明示include | GOOD | Carried over | `easyfind.hpp:5`、`Span.hpp:5`およびheader単独compile成功。 |
| protected `c`使用は妥当 | INFO | Carried over | `MutantStack.hpp:36-49`。 |
| reverse iterator非提供 | 要件外 | Dropped from修正要求 | Subject/rubricはforward iteratorによるsample操作のみを要求し、欠陥・修正要求ではないため。 |
| memory check 0 leaks | PASS | Carried over with updated evidence | 保存済み8秒harnessにPID injection方式で`leaks`を接続し、0 leaks、両exit 0を確認。 |
| `Span(unsigned int)`の暗黙変換リスク | INFO | Resolved by evidence | `Span.hpp:19`は`explicit Span(unsigned int capacity)`。`ex01_span_implicit_harm.cpp`は`Span s = 5`で`no viable conversion`、`report(5)`で`no matching function`となり、暗黙変換が拒否されることを確認。 |

## 修正要求
1. **任意・トレードオフあり**: range版`addNumber`の一時vectorはinput iterator対応とstrong/atomic behaviorを与える。一時コピーのメモリコストを下げるならforward iterator用に`std::distance`で事前検証する別設計が可能だが、single-pass input iteratorを二度走査してはならない (`Span.hpp:26-35`; `outputs/artifacts/tests/ex01_range_input.cpp`)。

## レビュイーへの説明要求リスト

> 以下はAIが自動生成した質問候補であり、defenseで人間レビュアーが選別・拡張する前提です。回答できない場合の扱いは公式rubricの`Can’t support / explain code`フラグに従います。

### Critical Questions (Top 5)
1. ex00で`std::find`を使用する理由、返されるiteratorが「最初の一致」を指す理由、未発見時に`end()`をどう扱っているか説明してください (`easyfind.hpp:9-25`)。
2. ex01で`shortestSpan()`をソート後の隣接要素だけ比較すればよい理由と、「最小の2数を引くだけ」では誤る例を説明してください (`Span.cpp:51-65`)。
3. ex01のrange版`addNumber`が一時vectorを作る理由、容量超過時に既存`Span`が変更されない理由、input iteratorを二度走査する設計の問題を説明してください (`Span.hpp:26-35`)。
4. ex01の`distanceBetween()`が`INT_MIN`を含む計算で`-(first + 1) + 1`としている理由と、単純な`second - first`の危険性を説明してください (`Span.cpp:29-43`)。
5. ex02で`std::stack`自体はiteratorを公開しないのに、派生`MutantStack`が`this->c.begin()`を使える理由と、underlying containerを`std::list`へ交換できる理由を説明してください (`MutantStack.hpp:9-49`)。

### Supplementary Questions
- const/非constの`easyfind` overloadで戻り値が`iterator`/`const_iterator`に分かれる理由は何ですか。
- `Span`のcopy assignmentでtemporary vectorを先に作ってからswapする例外安全上の利点は何ですか (`Span.cpp:16-24`)。
- `Span(unsigned int)`に`explicit`を付けたことで、`Span s = 5`や`report(5)`が拒否される仕組みを説明してください。
- `MutantStack`のcopy/assignmentがunderlying containerも独立してコピーする理由を説明してください。
- Module 08でSTLの利用が許可されるだけでなく推奨・要求される理由をsubjectのmodule-specific rulesに沿って説明してください。

## Disclosure
42 subject Chapter IV (AI Instructions) を尊重し、本レポートは学習・peer learning・人間によるdefenseを代替しない補助的レビューです。

Reviewer: Agentic Reviewer (AI-generated, advisory only)
Final pass/fail decision: requires human reviewer confirmation
