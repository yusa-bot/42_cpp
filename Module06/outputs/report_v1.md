# CPP Module 06 Review Report (Rubric Based)
## 【判定】: PASS

ex00〜ex02 の必須ファイルが存在し、全 exercise が `-Wall -Wextra -Werror -pedantic -std=c++98` で警告なくビルド・実行できた。rubric の Scalar conversion / Retyping of raw data / Real type identification を全て満たす。禁止関数・禁止 STL・C++11 機能・`<typeinfo>` の使用はなく、ex02 の動的確保は 0 leaks。人間レビュアーの最終確認を前提とした総合 PASS 候補。

## 禁止関数チェック: PASS
- **Description**: `*printf` / `*alloc` / `free()` / `using namespace` / `friend` / STL コンテナ・`<algorithm>` / C++11 機能 / `<typeinfo>`・`typeid`(ex02 で明示禁止) の使用有無を検査。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module06
  grep -rEn "printf|[mcr]alloc|free\(|using[[:space:]]+namespace|friend" ex00 ex01 ex02 --include="*.cpp" --include="*.hpp"
  grep -rEn "#include[[:space:]]*<(vector|list|map|set|deque|algorithm|queue|stack|array|unordered_|tuple)" ex00 ex01 ex02 --include="*.cpp" --include="*.hpp"
  grep -rEn "nullptr|\boverride\b|\bfinal\b|constexpr|std::move|decltype|<chrono>|<thread>|<mutex>|<atomic>|<random>|<regex>|<cstdint>" ex00 ex01 ex02 --include="*.cpp" --include="*.hpp"
  grep -rEn "typeinfo|typeid" ex00 ex01 ex02 --include="*.cpp" --include="*.hpp"
  ```
- **Observation**: 4 コマンドとも一致 0 件。subject Chapter II の Allowed/Forbidden (C++11/Boost、`*printf/*alloc/free`、`using namespace`、`friend`、Module 08 未満の STL) および ex02 の `<typeinfo>` 禁止に抵触しない。

## Preliminary tests

### Prerequisites
- **Description**: `c++ -Wall -Wextra -Werror` かつ C++98 でビルドできるか。各 exercise の `make`, `make re`, `make fclean` を検証。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module06/ex00 && make && make re && make fclean
  cd ~/42_cpp/Module06/ex01 && make && make re && make fclean
  cd ~/42_cpp/Module06/ex02 && make && make re && make fclean
  for h in ex00/ScalarConverter.hpp ex01/Data.hpp ex01/Serializer.hpp ex02/Base.hpp ex02/A.hpp ex02/B.hpp ex02/C.hpp; do cd ~/42_cpp/Module06 && c++ -Wall -Wextra -Werror -std=c++98 -fsyntax-only -x c++ "$h" && echo "OK $h"; done
  ```
- **Observation**: 全 exercise でビルド exit 0・警告 0。各 Makefile は CXX=`c++`、CXXFLAGS=`-Wall -Wextra -Werror -pedantic -std=c++98` (`ex00/Makefile:2-3`, `ex01/Makefile:3-4`, `ex02/Makefile:3-4`)、`re` は `fclean all` (`ex00/Makefile:24` 他)。ヘッダ 7 本すべて `-fsyntax-only` で単独コンパイル成功 (self-contained + include guard)。

### Forbidden Function
- **Description**: Forbidden Function フラグ対象の有無。
- **Status**: PASS (該当なし)
- **Evidence/Test Command**: 上記「禁止関数チェック」の全コマンド。
- **Observation**: 該当 0 件。

## Exercise 00: Conversion of scalar types

### Scalar conversion
- **Description**: private constructor + static method のクラスか、`static_cast` を使うか (promotion に限り implicit cast 可)、subject どおり char/int/float/double へ変換し表示するか。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module06/ex00 && make
  ./convert 0; ./convert nan; ./convert 42.0f
  ./convert "'c'"; ./convert -42; ./convert 42; ./convert +inf; ./convert -inff; ./convert nanf
  ./convert 2147483648; ./convert 127; ./convert 128; ./convert 3.14; ./convert 1e40; ./convert abc; ./convert ""
  make fclean
  # 非インスタンス化 (private ctor) の確認 (コンパイル失敗が期待):
  mkdir -p /tmp/Module06_review && printf '#include "ScalarConverter.hpp"\nint main(){ ScalarConverter s; (void)s; return 0; }\n' > /tmp/Module06_review/inst_sc.cpp && cd ~/42_cpp/Module06 && c++ -std=c++98 -Iex00 /tmp/Module06_review/inst_sc.cpp ex00/ScalarConverter.cpp -o /tmp/Module06_review/inst_sc
  ```
- **Observation**:
  - クラスは全 OCF メンバを private 化し (`ex00/ScalarConverter.hpp:13-17`)、public は `static void convert(const std::string&)` のみ (`ex00/ScalarConverter.hpp:20`)。上記 inst_sc は `error: calling a private constructor of class 'ScalarConverter'` でコンパイル失敗 → ユーザはインスタンス化不可。
  - 型変換は `static_cast` を使用 (`ex00/ScalarConverter.cpp:97,104,123` 等)。implicit cast は数値昇格の範囲。
  - subject の 3 例と文字単位一致: `0`→`char: Non displayable/int: 0/float: 0.0f/double: 0.0`、`nan`→`char/int: impossible/float: nanf/double: nan`、`42.0f`→`char: '*'/int: 42/float: 42.0f/double: 42.0`。
  - pseudo-literal: `+inf/-inf/+inff/-inff/nan/nanf` を正しく処理し float 側は `...ff/nanf`、double 側は `...inf/nan` を出力 (`ex00/ScalarConverter.cpp:12-26,107-137`)。
  - 境界/異常: `2147483648`・`128` は int が overflow/範囲外で `impossible`、char は範囲外/非表示で `impossible`/`Non displayable`、`127`(DEL) は `Non displayable`、`1e40` は float `impossible`・double 表示可、`abc`/空文字/`42abc` は 4 型とも `impossible`。全て期待どおり。

## Exercise 01: Serialization

### Retyping of raw data
- **Description**: private constructor + static method のクラスか、`reinterpret_cast` が 2 回使われるか、非空 Data 構造体を作り serialize→deserialize で元ポインタと一致するか。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module06/ex01 && make && ./serializer; echo run=$?; make fclean
  grep -rn "reinterpret_cast" ~/42_cpp/Module06/ex01/Serializer.cpp
  mkdir -p /tmp/Module06_review && printf '#include "Serializer.hpp"\nint main(){ Serializer s; (void)s; return 0; }\n' > /tmp/Module06_review/inst_se.cpp && cd ~/42_cpp/Module06 && c++ -std=c++98 -Iex01 /tmp/Module06_review/inst_se.cpp ex01/Serializer.cpp -o /tmp/Module06_review/inst_se
  ```
- **Observation**:
  - OCF メンバを private 化 (`ex01/Serializer.hpp:12-16`)、public は `static uintptr_t serialize(Data*)` と `static Data* deserialize(uintptr_t)` (`ex01/Serializer.hpp:19-20`)。inst_se は `error: calling a private constructor of class 'Serializer'` でコンパイル失敗。
  - `reinterpret_cast` は `serialize`(`ex01/Serializer.cpp:5`) と `deserialize`(`ex01/Serializer.cpp:10`) の 2 回で使用 (rubric "should be used twice" と一致)。
  - Data は非空 (`int id; std::string name;`、`ex01/Data.hpp:6-10`)。
  - 実行で `same pointer: true` かつ `data: 42, Serializer test` を出力し (`ex01/main.cpp:14-20`)、deserialize 結果が元ポインタと一致 (run exit 0)。

## Exercise 02: Identify real type

### Real type identification
- **Description**: Base は public virtual destructor のみ、A/B/C が public 継承、`generate()`/`identify(Base*)`/`identify(Base&)` を実装、`dynamic_cast` で実型判定、`identify(Base*)` は NULL チェック、`identify(Base&)` は try/catch、`<typeinfo>` 不使用を検査。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module06/ex02 && make && ./identify; ./identify; make fclean
  grep -rn "dynamic_cast" ~/42_cpp/Module06/ex02/Base.cpp
  grep -rEn "typeinfo|typeid" ~/42_cpp/Module06/ex02
  mkdir -p /tmp/Module06_review && cd ~/42_cpp/Module06/ex02 && c++ -Wall -Wextra -Werror -std=c++98 -I. ~/42_cpp/Module06/outputs/artifacts/tests/edge_ex02.cpp Base.cpp -o /tmp/Module06_review/edge_ex02 && /tmp/Module06_review/edge_ex02; echo exit=$?
  ```
- **Observation**:
  - Base は public virtual destructor のみ (`ex02/Base.hpp:6-9`)。A/B/C は空クラスで public 継承 (`ex02/A.hpp:6`, `ex02/B.hpp:6`, `ex02/C.hpp:6`)。
  - `generate()` は `rand()%3` で A/B/C を new して Base* を返す (`ex02/Base.cpp:14-29`)。
  - `identify(Base*)` は `dynamic_cast<T*>(p) != NULL` で判定し NULL チェックを内包 (`ex02/Base.cpp:31-38`)。`identify(Base&)` は `dynamic_cast<T&>` を try/catch (`std::bad_cast`) で判定し、内部でポインタ不使用 (`ex02/Base.cpp:40-58`)。
  - `<typeinfo>`/`typeid` は一致 0 件。`dynamic_cast` は pointer 版 3 箇所・reference 版 3 箇所で使用。
  - 実行で generate/A/B/C すべて両 identify が実型を正しく出力。edge_ex02 で `identify((Base*)NULL)` がクラッシュせず (`survived NULL pointer`, exit 0)、generate+delete のループも正常。

## 破壊的テスト結果
- ScalarConverter: `2147483648`/`128`(int overflow・範囲外)、`127`(DEL 非表示)、`1e40`(float 範囲外)、`abc`/空文字/`42abc`(解析不能)で `impossible`/`Non displayable` を正しく出力しクラッシュなし (ex00 の Evidence 参照)。
- ScalarConverter/Serializer: private ctor によりユーザ側インスタンス化がコンパイル時に拒否される (`inst_sc.cpp`/`inst_se.cpp`)。
- ex02: `identify((Base*)NULL)` が SIGSEGV を起こさず処理継続 (`edge_ex02`)。
- SIGSEGV/SIGABRT/SIGBUS を誘発した入力はなし。

## メモリ検査
- **Description**: ヒープ確保と解放を検査。`new` は ex02 のみ (`ex02/Base.cpp:23,25,27`)。ex00/ex01 は動的確保なし。
- **Status**: PASS
- **採用方針**: macOS Memory Check Policy **方針 B (`leaks --atExit`)**。
- **方針 B の条件確認**:
  1. 対象 `./identify` と保存済み `edge_ex02` harness は stdin を読まず自走完了する専用テスト。
  2. 本 macOS サンドボックス内では `leaks` が task port 制約でハングするため、escalated(非サンドボックス)権限で実行。再実行は 1 秒未満で完了しハングなし。
- **Evidence/Test Command** (対象プロセスを検査可能な権限で実行):
  ```sh
  cd ~/42_cpp/Module06/ex02 && make && leaks --atExit -- ./identify; make fclean
  mkdir -p /tmp/Module06_review && cd ~/42_cpp/Module06/ex02 && c++ -Wall -Wextra -Werror -std=c++98 -I. ~/42_cpp/Module06/outputs/artifacts/tests/edge_ex02.cpp Base.cpp -o /tmp/Module06_review/edge_ex02 && leaks --atExit -- /tmp/Module06_review/edge_ex02
  ```
- **Observation**: `./identify`: `Process 75399: 190 nodes malloced for 31 KB` / `0 leaks for 0 total leaked bytes.`。`edge_ex02`: `Process 75433: 190 nodes malloced for 31 KB` / `0 leaks`。generate() の `new` は main で `delete generated` (`ex02/main.cpp:20`)、harness でも各反復で delete。

## 仕様との乖離
- 機能的な subject 乖離は検出されず。ex00 の出力は subject 例と文字単位一致し、rubric の「outputs に過度に厳しくしない」但し書きの範囲でも問題なし。
- ex02 の `identify(Base*)` は p が A/B/C いずれにも該当しない(NULL 含む)場合に何も出力しない。subject は A/B/C の表示のみを要求しており、仕様乖離ではない (Notes [INFO] 参照)。

## 修正要求
1. (任意・堅牢性) `ex01` の `uintptr_t` は `<stdint.h>` から取得している (`ex01/Serializer.hpp:5`)。C++98 に `<cstdint>` は無いため実務上妥当だが、C++ ヘッダ体裁を統一したい場合は `<cstddef>`/`<cstdint>` 事情を defense で説明できるようにしておくこと。挙動・移植性に問題はなく FAIL 事由ではない。
2. (任意) `ex02` の乱数 seed は `srand(time(NULL))` を初回一回のみ設定するため (`ex02/Base.cpp:15-20`)、同一秒内の連続実行で generate() の結果が変わらない。評価時に多様性を見せたい場合はseed 方針を説明できるようにしておくこと。

## Notes (severity-tagged)
- **[INFO] `ex01` は `uintptr_t` を `<stdint.h>` から取得。** `ex01/Serializer.hpp:5`。subject が `uintptr_t` を明示要求しており (subject Chapter VI)、C 由来ヘッダの使用は許容範囲。`<cstdint>` は grep で 0 件 (C++11 ヘッダ不使用)。仕様違反ではない。
- **[INFO] `ex02` の `generate()` は初回のみ seed。** `ex02/Base.cpp:15-20`。subject は "Feel free to use anything you like for the random choice" と明記しており、同一秒連続実行で同結果になっても仕様違反ではない。
- **[INFO] `ex02` の `identify(Base*)` は該当なし/NULL 時に無出力。** `ex02/Base.cpp:31-38`。subject は A/B/C の表示のみ要求。NULL でもクラッシュしないことを `edge_ex02` で確認済み。
- **[INFO] Makefile は必須フラグに `-pedantic` を追加。** `ex00/Makefile:3` 他。必須フラグを失わず警告 0 でビルド可能。問題なし。

## Prior Report Reconciliation

N/A (過去レポートなし)。レビュー開始時、`~/42_cpp/Module06/outputs` は存在せず、`ls` は `No such file or directory`。本レビューで outputs と artifacts/tests を新規作成した。

## クリーンアップ確認
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module06 && for d in ex00 ex01 ex02; do (cd "$d" && make fclean); done
  git -C /Users/fujiki/workspace/42tokyo/cpp-module status --short ~/42_cpp/Module06
  ```
- **Observation**: 各 exercise の binary / `.o` / `.d` を除去。レビュー対象既存ソースに変更なし。新規生成物は `~/42_cpp/Module06/outputs/` 以下の report と artifacts/tests のみ (`git status --short` は `?? outputs/` のみ)。

## レビュイーへの説明要求リスト

> 以下は AI が自動生成した質問候補であり、defense で人間レビュアーが選別・拡張する前提。

### Critical Questions (Top 5)
1. ex00 で literal の型判定 (char/int/float/double、pseudo-literal) をどの順序・条件で行っているか、`isCharLiteral`/`parseNumber` の分岐を説明してください (`ex00/ScalarConverter.cpp:28-51,139-158`)。
2. ex00 で char が `impossible`/`Non displayable`/表示の 3 分岐になる条件と、int/float の overflow 判定基準を説明してください (`ex00/ScalarConverter.cpp:91-125`)。
3. ex01 で `reinterpret_cast` を選び `static_cast` を使わない理由、および `uintptr_t` を介する意義を説明してください (`ex01/Serializer.cpp:4-11`)。
4. ex02 で `identify(Base*)` は NULL チェック、`identify(Base&)` は try/catch(`std::bad_cast`) と、判定手段を変える理由を説明してください (`ex02/Base.cpp:31-58`)。
5. ex02 で Base に virtual destructor が必要な理由と、`dynamic_cast` が成立するための多態性条件を説明してください (`ex02/Base.hpp:6-9`)。

### Supplementary Questions
- ex00 の float/double 整形 (`setprecision(7)`/`(15)` と `.0` 付与) をどう決めたか (`ex00/ScalarConverter.cpp:67-89`)。
- ex01 の `<stdint.h>` 使用と C++98 標準の関係を説明できるか。
- ex02 の乱数 seed を初回一回に限定した理由と、複数生成時の分布への影響。
- ex00 でクォート無し 1 文字 (例: `a`) を char literal として扱う設計判断の根拠 (`ex00/ScalarConverter.cpp:33-36`)。

## Disclosure
42 subject Chapter III (AI Instructions) を尊重し、本レポートは学習・peer learning・人間による defense を代替しない補助的レビューである。

Reviewer: Agentic Reviewer (AI-generated, advisory only)  
Final pass/fail decision: requires human reviewer confirmation
