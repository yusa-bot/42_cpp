# C++ Module 09 Review Report (Rubric Based)

## 【判定】: FAIL

候補判定である。ex01 は異常時の出力先が subject の標準エラー要件に反し、ex02 は rubric が要求する non-interface class の OCF を内部 helper 型が満たさない。最終判定は人間レビュアーが defense で確認する必要がある。

## 禁止関数チェック: PASS

`*printf`, `*alloc`, `free`, `using namespace`, `friend`, 外部ライブラリ、C++11 以降の構文は検出されず、全ソースは `-std=c++98 -Wall -Wextra -Werror` でビルドできた。Module 09 では STL container と `<algorithm>` は許可される (subject Chapter II, pp.3-4; Chapter III, p.6)。

Evidence:

```sh
rg -n --glob '*.{cpp,hpp}' 'printf|malloc|calloc|realloc|free[[:space:]]*\(|using[[:space:]]+namespace|\bfriend\b|nullptr|\boverride\b|\bfinal\b|constexpr|std::move[[:space:]]*\(|decltype|#include[[:space:]]*<(chrono|thread|mutex|atomic|random|regex|initializer_list|unordered_[^>]*)>' ~/42_cpp/Module09/ex00 ~/42_cpp/Module09/ex01 ~/42_cpp/Module09/ex02 || true
mkdir -p ~/42_cpp/Module09/outputs/tmp
c++ -Wall -Wextra -Werror -std=c++98 ~/42_cpp/Module09/ex00/main.cpp ~/42_cpp/Module09/ex00/BitcoinExchange.cpp -o ~/42_cpp/Module09/outputs/tmp/btc_std98
c++ -Wall -Wextra -Werror -std=c++98 ~/42_cpp/Module09/ex01/main.cpp ~/42_cpp/Module09/ex01/RPN.cpp -o ~/42_cpp/Module09/outputs/tmp/RPN_std98
c++ -Wall -Wextra -Werror -std=c++98 ~/42_cpp/Module09/ex02/main.cpp ~/42_cpp/Module09/ex02/PmergeMe.cpp -o ~/42_cpp/Module09/outputs/tmp/PmergeMe_std98
```

## Preliminary tests

### Prerequisites

- **Description**: 提出ファイル、全 `ex0*` の全 `.cpp/.hpp/Makefile`、C++98、必須警告フラグ、relink、header 内実装、include guard、header 自己完結性、禁止 API、container の exercise 間再利用を検査した。Module 05 固有の `Bureaucrat/Form/AForm/Intern/makeForm`、抽象 Form、Form 実行条件、ファイル生成失敗は Module 09 subject pp.9-15 の提出物・機能に存在しないため、Module 09 の評価要件ではない。
- **Status**: PASS
- **Evidence/Test Command**:

```sh
find ~/42_cpp/Module09/ex00 ~/42_cpp/Module09/ex01 ~/42_cpp/Module09/ex02 -maxdepth 1 -type f \( -name '*.cpp' -o -name '*.hpp' -o -name 'Makefile' \) -print | sort
rg -n --glob '*.{cpp,hpp}' 'Bureaucrat|AForm|Form|Intern|makeForm|executeForm|=[[:space:]]*0[[:space:]]*;' ~/42_cpp/Module09/ex00 ~/42_cpp/Module09/ex01 ~/42_cpp/Module09/ex02 || true
rg -n --glob '*.hpp' '\)[[:space:]]*(const[[:space:]]*)?\{' ~/42_cpp/Module09/ex00 ~/42_cpp/Module09/ex01 ~/42_cpp/Module09/ex02 || true
rg -n '^#[[:space:]]*(ifndef|define|endif)' ~/42_cpp/Module09/ex00/BitcoinExchange.hpp ~/42_cpp/Module09/ex01/RPN.hpp ~/42_cpp/Module09/ex02/PmergeMe.hpp
c++ -Wall -Wextra -Werror -std=c++98 -fsyntax-only -x c++ ~/42_cpp/Module09/ex00/BitcoinExchange.hpp
c++ -Wall -Wextra -Werror -std=c++98 -fsyntax-only -x c++ ~/42_cpp/Module09/ex01/RPN.hpp
c++ -Wall -Wextra -Werror -std=c++98 -fsyntax-only -x c++ ~/42_cpp/Module09/ex02/PmergeMe.hpp
make -C ~/42_cpp/Module09/ex00 fclean
make -C ~/42_cpp/Module09/ex00
make -C ~/42_cpp/Module09/ex00
make -C ~/42_cpp/Module09/ex00 re
make -C ~/42_cpp/Module09/ex01 fclean
make -C ~/42_cpp/Module09/ex01
make -C ~/42_cpp/Module09/ex01
make -C ~/42_cpp/Module09/ex01 re
make -C ~/42_cpp/Module09/ex02 fclean
make -C ~/42_cpp/Module09/ex02
make -C ~/42_cpp/Module09/ex02
make -C ~/42_cpp/Module09/ex02 re
make -C ~/42_cpp/Module09/ex00 fclean
make -C ~/42_cpp/Module09/ex01 fclean
make -C ~/42_cpp/Module09/ex02 fclean
```

- **Observation**: 必須9ソース/headers と3 Makefile が存在した。3 header の単独構文検査は無出力・exit 0。各 exercise の最初の `make` と `make re` は成功し、連続2回目の `make` は `Nothing to be done for 'all'.` となりrelinkしなかった。各 Makefile は `CXX = c++`、`-Wall -Wextra -Werror -std=c++98`、`all/clean/fclean/re` を持つ (`ex00/Makefile:1-29`, `ex01/Makefile:1-29`, `ex02/Makefile:1-27`)。container は ex00=`std::map` (`ex00/BitcoinExchange.hpp:4,9`)、ex01=`std::stack` (`ex01/RPN.hpp:4,9`)、ex02=`std::vector/std::deque` (`ex02/PmergeMe.hpp:4-5,28-30`) で、前 exercise の container を再利用していない。

## Exercise 00: Bitcoin Exchange

### Code review

- **Description**: 通常の Makefile、最低1 container、OCF、database 読み込み、日付検証、lower-date lookup を監査した。
- **Status**: PASS
- **Evidence/Test Command**:

```sh
rg -n 'CXX|CXXFLAGS|^all:|^clean:|^fclean:|^re:' ~/42_cpp/Module09/ex00/Makefile
rg -n '#[[:space:]]*include[[:space:]]*<map>|std::map|lower_bound' ~/42_cpp/Module09/ex00/BitcoinExchange.hpp ~/42_cpp/Module09/ex00/BitcoinExchange.cpp
rg -n 'BitcoinExchange\(|operator=|~BitcoinExchange' ~/42_cpp/Module09/ex00/BitcoinExchange.hpp ~/42_cpp/Module09/ex00/BitcoinExchange.cpp
awk -F, 'NR==1{next} prev>=$1{print "non-increasing:" NR ":" prev ">=" $1; bad=1} {prev=$1} END{print bad?"order=FAIL":"order=PASS"}' ~/42_cpp/Module09/ex00/data.csv
```

- **Observation**: `std::map<std::string,double>` を用い (`ex00/BitcoinExchange.hpp:9`)、`lower_bound` 後に exact match でなければ一つ戻して直近の下位日付を選ぶ (`ex00/BitcoinExchange.cpp:177-189`)。主要 class は default/copy/copy assignment/destructor を明示 (`ex00/BitcoinExchange.hpp:14-17`)。database は1613行で、検査出力は `order=PASS` だった。container 選択理由の口頭説明は `Critical Questions` に残した。

### Error handle

- **Description**: 空ファイル、欠損ファイル、不正日付、閏日、database 範囲前の日付、0/1000境界、1000超、負数、NaN/Infinity相当、壊れた行、エラー後の処理継続を検証した。
- **Status**: PASS
- **Evidence/Test Command**:

```sh
mkdir -p ~/42_cpp/Module09/outputs/tmp
make -C ~/42_cpp/Module09/ex00
python3 ~/42_cpp/Module09/outputs/artifacts/tests/test_btc.py ~/42_cpp/Module09/ex00/btc ~/42_cpp/Module09/ex00 ~/42_cpp/Module09/outputs/tmp
make -C ~/42_cpp/Module09/ex00 fclean
```

- **Observation**: harness は `PASS: btc boundary/error/continuation tests`、exit 0。`2012-02-29` は下位日の `2012-02-28` rate 4.98で計算し、`1900-02-29`、1000.001、-0.001、壊れた行を拒否した。エラー行の後に置いた `2011-01-04 | 2` も処理され、同じ正常行の出力が合計2回となった。空ファイルはexit 0・出力なし、欠損ファイルと引数不足は非0終了だった。入力先頭行を無条件に捨てる挙動は別途WARNとした (`ex00/BitcoinExchange.cpp:147-150`)。

### Main usage

- **Description**: subject 添付例と同内容の `input.txt` を用い、exact date と存在しない date の直近下位 rate、結果表示を検証した。
- **Status**: PASS
- **Evidence/Test Command**:

```sh
make -C ~/42_cpp/Module09/ex00
(cd ~/42_cpp/Module09/ex00 && ./btc input.txt)
make -C ~/42_cpp/Module09/ex00 fclean
```

- **Observation**: subject p.10 の9結果と一致し、`2011-01-03 => 3 = 0.9`、`2011-01-09 => 1 = 0.32`、負数/不正日付/過大値の各エラーを順番どおり出力した。`data.csv` の `2011-01-01,0.3` と `2011-01-07,0.32` (`ex00/data.csv:245,247`) に対し、非存在日 `2011-01-03` と `2011-01-09` は直近下位 rate を使用した。

## Exercise 01: Reverse Polish Notation

### Code review

- **Description**: 通常の Makefile、最低1 container、前 exercise と異なる container、OCF、operator dispatch、0除算とoverflow例外を監査した。
- **Status**: PASS
- **Evidence/Test Command**:

```sh
rg -n 'CXX|CXXFLAGS|^all:|^clean:|^fclean:|^re:' ~/42_cpp/Module09/ex01/Makefile
rg -n '#[[:space:]]*include[[:space:]]*<stack>|std::stack|RPN\(|operator=|~RPN|division by zero|integer overflow' ~/42_cpp/Module09/ex01/RPN.hpp ~/42_cpp/Module09/ex01/RPN.cpp
```

- **Observation**: `std::stack<int>` を用い (`ex01/RPN.hpp:9`)、ex00 の `std::map` を再利用していない。主要 class はOCF 4要素を明示 (`ex01/RPN.hpp:16-19`)。演算前のoperand数、0除算、`int` 範囲を例外化している (`ex01/RPN.cpp:39-69`)。container 選択理由の口頭説明は `Critical Questions` に残した。

### Main usage

- **Description**: 正常な整数RPN式と、引数なし、空式、0除算、operand不足/余剰、括弧、2桁数、小数を検査し、subject p.11 の「error message should be displayed on the standard error」を確認した。
- **Status**: FAIL
- **Evidence/Test Command**:

```sh
mkdir -p ~/42_cpp/Module09/outputs/tmp
make -C ~/42_cpp/Module09/ex01
python3 ~/42_cpp/Module09/outputs/artifacts/tests/test_rpn.py ~/42_cpp/Module09/ex01/RPN
~/42_cpp/Module09/ex01/RPN '(1 + 1)' > ~/42_cpp/Module09/outputs/tmp/rpn_error_stdout.txt 2> ~/42_cpp/Module09/outputs/tmp/rpn_error_stderr.txt
wc -c ~/42_cpp/Module09/outputs/tmp/rpn_error_stdout.txt ~/42_cpp/Module09/outputs/tmp/rpn_error_stderr.txt
sed -n '1,5p' ~/42_cpp/Module09/outputs/tmp/rpn_error_stdout.txt
make -C ~/42_cpp/Module09/ex01 fclean
```

- **Observation**: 正常式は正答したが、harness は8種類すべてで `error was not written to stderr` と `error polluted stdout` を報告してexit 1。括弧入力では program exit 1、stdout 21 bytes、stderr 0 bytes、stdout は `Error: invalid token` だった。原因は引数エラーと例外の双方で `std::cout` を使う実装 (`ex01/main.cpp:7,15`) であり、subject p.11 の標準エラー要件に反する。

### Usage advanced

- **Description**: rubric 記載の3つの複合式を含む正常式で演算順序と結果を検証した。
- **Status**: PASS
- **Evidence/Test Command**:

```sh
make -C ~/42_cpp/Module09/ex01
~/42_cpp/Module09/ex01/RPN '8 9 * 9 - 9 - 9 - 4 - 1 +'
~/42_cpp/Module09/ex01/RPN '9 8 * 4 * 4 / 2 + 9 - 8 - 8 - 1 - 6 -'
~/42_cpp/Module09/ex01/RPN '1 2 * 2 / 2 + 5 * 6 - 1 3 * - 4 5 * * 8 /'
make -C ~/42_cpp/Module09/ex01 fclean
```

- **Observation**: 実出力は順に `42`, `42`, `15` で rubric の期待値と一致した。`test_rpn.py` の追加正常ケース `1 2 * 2 / 2 * 2 4 - +`, 単一数 `9`, 負の計算結果 `7 8 -` も期待値と一致した。

## Exercise 02: PmergeMe

### Code review

- **Description**: 通常の Makefile、2 container、過去 exercise と異なる container、Ford-Johnson merge-insert sort のcontainer別実装、Jacobsthal挿入順、全non-interface classのOCFを監査した。
- **Status**: FAIL
- **Evidence/Test Command**:

```sh
rg -n 'CXX|CXXFLAGS|^all:|^clean:|^fclean:|^re:' ~/42_cpp/Module09/ex02/Makefile
rg -n '#[[:space:]]*include[[:space:]]*<(vector|deque)>|std::vector|std::deque|fordJohnsonVector|fordJohnsonDeque|makeInsertionOrder|lower_bound' ~/42_cpp/Module09/ex02/PmergeMe.hpp ~/42_cpp/Module09/ex02/PmergeMe.cpp
rg -n '^(struct|class)[[:space:]]|^[[:space:]]*(explicit[[:space:]]+)?(Element|Pair|Pending|ElementPointerLess)[[:space:]]*\(|operator=' ~/42_cpp/Module09/ex02/PmergeMe.cpp
```

- **Observation**: vector と deque 用の再帰本体は別実装 (`ex02/PmergeMe.cpp:155-204,229-276`) で、pairing、再帰、Jacobsthal順、paired-largeまでの二分挿入を行う。しかし rubric `Exercise 02: PmergeMe` 冒頭は「any non-interface class is not in orthodox canonical class form, do not grade this exercise」と要求する (`docs/CPP Module 09 correction.html:397-405`)。内部 class 型である `Element`, `Pair`, `Pending`, `ElementPointerLess` (`ex02/PmergeMe.cpp:17-59`) はOCF 4要素を宣言しておらず、特に `Element` と `Pending` は引数付きconstructorしかない (`ex02/PmergeMe.cpp:22-24,36-39`)。このため実行結果が正しくても本rubric項目はFAIL。

### Main usage

- **Description**: 1要素、整列済み、逆順、奇数/偶数、重複、`INT_MAX`、固定例、決定的random、3000個の相異なる整数、不正値、NULLを検査し、After列を独立oracleと比較した。
- **Status**: PASS
- **Evidence/Test Command**:

```sh
mkdir -p ~/42_cpp/Module09/outputs/tmp
make -C ~/42_cpp/Module09/ex02
python3 ~/42_cpp/Module09/outputs/artifacts/tests/test_pmerge.py ~/42_cpp/Module09/ex02/PmergeMe
c++ -Wall -Wextra -Werror -std=c++98 -I~/42_cpp/Module09/ex02 ~/42_cpp/Module09/outputs/artifacts/tests/null_ex02.cpp ~/42_cpp/Module09/ex02/PmergeMe.cpp -o ~/42_cpp/Module09/outputs/tmp/null_ex02
~/42_cpp/Module09/outputs/tmp/null_ex02
~/42_cpp/Module09/ex02/PmergeMe 3 5 9 7 4
make -C ~/42_cpp/Module09/ex02 fclean
```

- **Observation**: harness は `PASS: 200 oracle runs, including 3000 unique integers and invalid inputs`、exit 0。3000逆順・相異値では `vector=6439.00000 us`, `deque=7935.00000 us` を観測し、4行形式、両container名、非0精度を確認した。固定例は `Before: 3 5 9 7 4`, `After: 3 4 5 7 9`。0、負数、`INT_MAX+1`、`+1`、小数、空文字、引数なしは非0終了かつstderrにエラー。直接NULLを渡すharnessは `caught: invalid positive integer`、exit 0で、`parsePositiveInteger` のNULL guard (`ex02/PmergeMe.cpp:103-105`) が機能した。

## メモリ検査方針と結果

macOS Memory Check Policy の **方針A (PID注入方式)** を採用した。各専用harnessは対象classのdefault/copy/copy assignmentと主要処理を実行し、対象オブジェクトのscopeを抜けてdestructorを完了してから `sleep(4)` で生存する。`sleep(1)` 後に同一shell内でPIDを `leaks` へ渡した。

Evidence:

```sh
mkdir -p ~/42_cpp/Module09/outputs/tmp ~/42_cpp/Module09/outputs/artifacts/logs
c++ -Wall -Wextra -Werror -std=c++98 -I~/42_cpp/Module09/ex00 ~/42_cpp/Module09/outputs/artifacts/tests/memory_ex00.cpp ~/42_cpp/Module09/ex00/BitcoinExchange.cpp -o ~/42_cpp/Module09/outputs/tmp/memory_ex00
c++ -Wall -Wextra -Werror -std=c++98 -I~/42_cpp/Module09/ex01 ~/42_cpp/Module09/outputs/artifacts/tests/memory_ex01.cpp ~/42_cpp/Module09/ex01/RPN.cpp -o ~/42_cpp/Module09/outputs/tmp/memory_ex01
c++ -Wall -Wextra -Werror -std=c++98 -I~/42_cpp/Module09/ex02 ~/42_cpp/Module09/outputs/artifacts/tests/memory_ex02.cpp ~/42_cpp/Module09/ex02/PmergeMe.cpp -o ~/42_cpp/Module09/outputs/tmp/memory_ex02
(cd ~/42_cpp/Module09/ex00 && (../outputs/tmp/memory_ex00 >/dev/null & pid=$!; sleep 1; leaks "$pid" > ../outputs/artifacts/logs/leaks_ex00.txt 2>&1; leaks_status=$?; wait "$pid"; exit "$leaks_status"))
(~/42_cpp/Module09/outputs/tmp/memory_ex01 >/dev/null & pid=$!; sleep 1; leaks "$pid" > ~/42_cpp/Module09/outputs/artifacts/logs/leaks_ex01.txt 2>&1; leaks_status=$?; wait "$pid"; exit "$leaks_status")
(~/42_cpp/Module09/outputs/tmp/memory_ex02 >/dev/null & pid=$!; sleep 1; leaks "$pid" > ~/42_cpp/Module09/outputs/artifacts/logs/leaks_ex02.txt 2>&1; leaks_status=$?; wait "$pid"; exit "$leaks_status")
rg -n 'Process|total leaked bytes' ~/42_cpp/Module09/outputs/artifacts/logs/leaks_ex00.txt ~/42_cpp/Module09/outputs/artifacts/logs/leaks_ex01.txt ~/42_cpp/Module09/outputs/artifacts/logs/leaks_ex02.txt
```

実測結果は ex00=`0 leaks for 0 total leaked bytes`, ex01=`0 leaks for 0 total leaked bytes`, ex02=`0 leaks for 0 total leaked bytes` (`outputs/artifacts/logs/leaks_ex00.txt:26`, `leaks_ex01.txt:26`, `leaks_ex02.txt:26`)。macOS は各log先頭で restricted process のreadonly memory表示制限を通知したが、malloc node走査とleak集計は完了した (`leaks_ex00.txt:1,25-26`, `leaks_ex01.txt:1,25-26`, `leaks_ex02.txt:1,25-26`)。

## 仕様との乖離

1. ex01 は異常時メッセージを標準エラーへ出す subject p.11 の要件に対し、引数エラーとcatchの両方で `std::cout` を使う (`ex01/main.cpp:7,15`)。異常8入力すべてでstderrが空だった (`outputs/artifacts/tests/test_rpn.py` の上記実行結果)。
2. ex02 rubric のnon-interface class OCF要件 (`docs/CPP Module 09 correction.html:397-405`) に対し、内部helper class型4つがOCF宣言を持たない (`ex02/PmergeMe.cpp:17-59`)。

## 破壊的テスト結果

| 対象 | 入力・条件 | 実測 |
|---|---|---|
| ex00 | 空、欠損、閏日/非閏日、database前、0、1000、1000.001、負数、NaN/Infinity相当、壊れた行 | crashなし。エラー後も末尾まで処理。harness exit 0 |
| ex00 | headerなしで `2011-01-03 | 3` の1行のみ | exit 0、stdout/stderrとも空。先頭行を無条件破棄 (`ex00/BitcoinExchange.cpp:147-150`) |
| ex01 | 引数なし、空、0除算、operand不足/余剰、括弧、2桁、小数 | 全て非0終了だがstderr 0 bytes、stdoutにエラー。harness exit 1 |
| ex02 | 200 oracle runs、3000相異整数、重複、境界、不正値 | 全sort一致。不正値はstderr。harness exit 0 |
| ex02 | `argv[1] == NULL` direct harness | `caught: invalid positive integer`、exit 0 |
| 全exercise | destructor後PID注入 `leaks` | 各0 leaked bytes |

## Notes (severity-tagged)

- [CRITICAL] ex01 は subject のstderr要件を満たさない。根拠: `ex01/main.cpp:7,15`、`test_rpn.py` は8異常入力でstderr空を再現。
- [CRITICAL] ex02 の内部helper class型はrubricが明示するnon-interface class OCF要件を満たさない。根拠: `docs/CPP Module 09 correction.html:397-405`, `ex02/PmergeMe.cpp:17-59`。
- [WARN] ex00 は入力の先頭行を内容確認せず破棄するため、headerなし1行入力はexit 0・無出力になる。根拠: `ex00/BitcoinExchange.cpp:147-150`, `test_btc.py` 実測 `INFO: no-header exit=0 stdout='' stderr=''`。subject例のheader付き入力は正常に処理できるためrubricのError handleはPASSとした。
- [INFO] macOS方針Aのleaks集計は3 exerciseとも0 bytes。restricted process の表示制限通知は各logの1行目、集計完了は25-26行目に残した。
- [INFO] 口頭説明を必要とするcontainer選択、Ford-Johnson説明、container間の時間差説明は自動実行できないため、人間defense向け質問候補として下記に残した。

CRITICAL: 2件 / WARN: 1件 / INFO: 2件

## Prior Report Reconciliation

N/A (過去レポートなし)

Step 0時点で `~/42_cpp/Module09/outputs` 内に既存ファイルはなかった。

## 修正要求

1. ex01 のusage/error catchを `std::cerr` へ変更し、正常結果だけをstdoutへ残す (`ex01/main.cpp:7,15`; subject p.11)。
2. ex02 の `Element`, `Pair`, `Pending`, `ElementPointerLess` をrubric要求どおりOCFにするか、helper class型を不要にする設計へ変更する (`ex02/PmergeMe.cpp:17-59`; rubric `docs/CPP Module 09 correction.html:397-405`)。
3. ex00 の先頭行を `date | value` headerとして検証し、不正headerを明示的に拒否するか、headerがなければその行をdataとして処理する (`ex00/BitcoinExchange.cpp:147-150`)。

## Cleanup Verification

```sh
make -C ~/42_cpp/Module09/ex00 fclean
make -C ~/42_cpp/Module09/ex01 fclean
make -C ~/42_cpp/Module09/ex02 fclean
find ~/42_cpp/Module09/ex00 ~/42_cpp/Module09/ex01 ~/42_cpp/Module09/ex02 -maxdepth 1 -type f \( -name '*.o' -o -name '*.d' -o -name 'btc' -o -name 'RPN' -o -name 'PmergeMe' -o -name '*_shrubbery' \) -print
find ~/42_cpp/Module09/ex00 ~/42_cpp/Module09/ex01 ~/42_cpp/Module09/ex02 -maxdepth 1 -type f -perm -111 -print
git status --short -- ~/42_cpp/Module09
```

3つの `make fclean` は成功し、後続2つの `find` とtarget限定 `git status` は無出力だった。workspace全体では `~/42_cpp/` 自体がuntrackedとしてまとめて表示されるため、Gitは既存sourceとreview生成物を個別差分化できない。ファイル実体検査では実行binary、`.o`, `.d`, `*_shrubbery` は `outputs/` 外に残っていない。review用生成物は `outputs/tmp/` と `outputs/artifacts/` に限定した。対象の既存 `.cpp/.hpp/Makefile/data.csv/input.txt` は変更していない。

## レビュイーへの説明要求リスト

以下はAIが自動生成した質問候補であり、defenseで人間レビュアーが選別・拡張する。

### Critical Questions (Top 5)

1. ex00 で時系列rate検索に `std::map` を選んだ理由と、`lower_bound` がdatabase未登録日に直近の下位日付を返すまでの分岐を説明してください (`ex00/BitcoinExchange.cpp:177-189`)。
2. ex01 で `std::stack` を選んだ理由と、operatorを読んだときにright/left operandを取り出す順序を説明してください (`ex01/RPN.cpp:39-64`)。
3. ex01 のsubjectがエラーをstderrへ要求するのに `std::cout` を使っている理由、stdout/stderr分離を最小変更で直す方法を説明してください (`ex01/main.cpp:7,15`)。
4. ex02 のpairing、large chain再帰、Jacobsthal insertion order、paired-largeを上限にしたbinary insertionがFord-Johnsonを構成する理由を説明してください (`ex02/PmergeMe.cpp:64-101,155-204,229-276`)。
5. ex02 の内部 `struct` もC++ではclass型であることを踏まえ、rubricのnon-interface class OCF要件に対して `Element/Pair/Pending/ElementPointerLess` が適合すると考える根拠、または修正方針を説明してください (`ex02/PmergeMe.cpp:17-59`)。

### Supplementary Questions

- ex02 でvectorとdequeの処理時間に差が出る理由を、要素格納、iterator/reference安定性、main-chain insertion costに関連付けて説明してください。
- ex02 で重複を許容する方針と、equal valueでもpaired-large制約が壊れない理由を説明してください (`ex02/PmergeMe.cpp:45-52,198-201,270-273`)。
- ex00 の入力先頭行がheaderでない場合の現在の挙動と、どの仕様解釈で処理方針を決めたか説明してください (`ex00/BitcoinExchange.cpp:147-150`)。
- ex01 の演算overflowを `long` で中間計算してから `int` 範囲確認する設計の移植性を説明してください (`ex01/RPN.cpp:51-69`)。

## Disclosure

42 subject Chapter IV のAI Instructionsを尊重し、本レポートは学習、本人の説明、peer reviewを代替しない。コード理解と最終判定は必ず人間が確認する。

Reviewer: Agentic Reviewer (AI-generated, advisory only)
Final pass/fail decision: requires human reviewer confirmation
