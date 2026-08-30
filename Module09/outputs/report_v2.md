# C++ Module 09 Review Report v2 (Rubric Based)

## 【判定】: FAIL

更新後の候補判定である。前回FAILだったex01のstderr出力は解消した。一方、ex02はrubricが明記するnon-interface classのOCFを内部helper型が依然満たさないため、総合候補はFAILとなる。最終判定は人間レビュアーがdefenseで確認する必要がある。

## 禁止関数チェック: PASS

`*printf`, `*alloc`, `free`, `using namespace`, `friend`, 外部ライブラリ、C++11以降の構文は検出されず、全ソースは`-std=c++98 -Wall -Wextra -Werror`でビルドできた。Module 09ではSTL containerと`<algorithm>`が許可される (subject Chapter II, pp.3-4; Chapter III, p.6)。

Evidence:

```sh
rg -n --glob '*.{cpp,hpp}' 'printf|malloc|calloc|realloc|free[[:space:]]*\(|using[[:space:]]+namespace|\bfriend\b|nullptr|\boverride\b|\bfinal\b|constexpr|std::move[[:space:]]*\(|decltype|#include[[:space:]]*<(chrono|thread|mutex|atomic|random|regex|initializer_list|unordered_[^>]*)>' ~/42_cpp/Module09/ex00 ~/42_cpp/Module09/ex01 ~/42_cpp/Module09/ex02 || true
mkdir -p ~/42_cpp/Module09/outputs/tmp
c++ -Wall -Wextra -Werror -std=c++98 ~/42_cpp/Module09/ex00/main.cpp ~/42_cpp/Module09/ex00/BitcoinExchange.cpp -o ~/42_cpp/Module09/outputs/tmp/btc_v2_std98
c++ -Wall -Wextra -Werror -std=c++98 ~/42_cpp/Module09/ex01/main.cpp ~/42_cpp/Module09/ex01/RPN.cpp -o ~/42_cpp/Module09/outputs/tmp/RPN_v2_std98
c++ -Wall -Wextra -Werror -std=c++98 ~/42_cpp/Module09/ex02/main.cpp ~/42_cpp/Module09/ex02/PmergeMe.cpp -o ~/42_cpp/Module09/outputs/tmp/PmergeMe_v2_std98
```

## Preliminary tests

### Prerequisites

- **Description**: 全`ex0*`の`.cpp/.hpp/Makefile`、C++98、必須警告フラグ、relink、header内実装、include guard、header自己完結性、禁止API、containerのexercise間再利用を再検査した。Module 05固有の`Bureaucrat/Form/AForm/Intern/makeForm`、抽象Form、Form実行条件、ファイル生成失敗はModule 09 subject pp.9-15の提出物・機能に存在せず、本Moduleの評価要件ではない。
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

- **Observation**: 必須9ソース/headersと3 Makefileが存在した。3 headerの単独構文検査は無出力・exit 0。各exerciseの`make`と`make re`は成功し、連続2回目の`make`は`Nothing to be done for 'all'.`でrelinkしなかった。containerはex00=`std::map` (`ex00/BitcoinExchange.hpp:4,9`)、ex01=`std::stack<int,std::list<int> >` (`ex01/RPN.hpp:4-5,10`)、ex02=`std::vector/std::deque` (`ex02/PmergeMe.hpp:4-5,12-14`) で、前exerciseのcontainerを再利用していない。各Makefileは`CXX = c++`、必須flags、`all/clean/fclean/re`を持つ (`ex00/Makefile:1-29`, `ex01/Makefile:1-29`, `ex02/Makefile:1-27`)。

## Exercise 00: Bitcoin Exchange

### Code review

- **Description**: Makefile、最低1 container、OCF、database読込、日付検証、直近下位date lookupを再監査した。
- **Status**: PASS
- **Evidence/Test Command**:

```sh
rg -n 'CXX|CXXFLAGS|^all:|^clean:|^fclean:|^re:' ~/42_cpp/Module09/ex00/Makefile
rg -n '#[[:space:]]*include[[:space:]]*<map>|std::map|lower_bound' ~/42_cpp/Module09/ex00/BitcoinExchange.hpp ~/42_cpp/Module09/ex00/BitcoinExchange.cpp
rg -n 'BitcoinExchange\(|operator=|~BitcoinExchange' ~/42_cpp/Module09/ex00/BitcoinExchange.hpp ~/42_cpp/Module09/ex00/BitcoinExchange.cpp
wc -l ~/42_cpp/Module09/ex00/data.csv
awk -F, 'NR==1{next} prev>=$1{print "non-increasing:" NR ":" prev ">=" $1; bad=1} {prev=$1} END{print bad?"order=FAIL":"order=PASS"}' ~/42_cpp/Module09/ex00/data.csv
```

- **Observation**: `std::map<std::string,double>`を用い (`ex00/BitcoinExchange.hpp:9`)、`lower_bound`後にexact matchでなければ一つ戻して直近の下位日付を選ぶ (`ex00/BitcoinExchange.cpp:177-189`)。主要classはOCF 4要素を明示する (`ex00/BitcoinExchange.hpp:14-17`)。databaseは1613行で検査出力は`order=PASS`だった。container選択理由の口頭説明は`Critical Questions`に残した。

### Error handle

- **Description**: 空ファイル、欠損ファイル、不正日付、閏日、database範囲前、0/1000境界、1000超、負数、NaN/Infinity相当、壊れた行、エラー後の処理継続を再検証した。
- **Status**: PASS
- **Evidence/Test Command**:

```sh
mkdir -p ~/42_cpp/Module09/outputs/tmp
make -C ~/42_cpp/Module09/ex00
python3 ~/42_cpp/Module09/outputs/artifacts/tests/test_btc.py ~/42_cpp/Module09/ex00/btc ~/42_cpp/Module09/ex00 ~/42_cpp/Module09/outputs/tmp
make -C ~/42_cpp/Module09/ex00 fclean
```

- **Observation**: harnessは`PASS: btc boundary/error/continuation tests`、exit 0。`2012-02-29`は下位日のrate 4.98で計算し、`1900-02-29`、1000.001、-0.001、壊れた行を拒否した。エラー後の末尾正常行も処理した。空ファイルはexit 0・出力なし、欠損ファイルと引数不足は非0終了。headerなし1行入力は`INFO: no-header exit=0 stdout='' stderr=''`で、旧WARNは継続した (`ex00/BitcoinExchange.cpp:147-150`)。

### Main usage

- **Description**: subject例と同内容の`input.txt`でexact date、非存在dateの直近下位rate、結果表示を再検証した。
- **Status**: PASS
- **Evidence/Test Command**:

```sh
make -C ~/42_cpp/Module09/ex00
(cd ~/42_cpp/Module09/ex00 && ./btc input.txt)
make -C ~/42_cpp/Module09/ex00 fclean
```

- **Observation**: subject p.10の9結果と一致し、`2011-01-03 => 3 = 0.9`、`2011-01-09 => 1 = 0.32`、負数/不正日付/過大値の各エラーを順番どおり出力した。`data.csv`の`2011-01-01,0.3`と`2011-01-07,0.32` (`ex00/data.csv:245,247`) に対し、非存在日では直近下位rateを使用した。

## Exercise 01: Reverse Polish Notation

### Code review

- **Description**: Makefile、最低1 container、前exerciseと異なるcontainer、OCF、operator dispatch、0除算、overflow例外を再監査した。
- **Status**: PASS
- **Evidence/Test Command**:

```sh
rg -n 'CXX|CXXFLAGS|^all:|^clean:|^fclean:|^re:' ~/42_cpp/Module09/ex01/Makefile
rg -n '#[[:space:]]*include[[:space:]]*<(list|stack)>|std::stack|std::list|RPN\(|operator=|~RPN|division by zero|integer overflow' ~/42_cpp/Module09/ex01/RPN.hpp ~/42_cpp/Module09/ex01/RPN.cpp
```

- **Observation**: `std::stack<int,std::list<int> >`を用い (`ex01/RPN.hpp:10`)、ex00の`std::map`を再利用していない。主要classはOCF 4要素を明示 (`ex01/RPN.hpp:17-20`)。operand数、0除算、`int`範囲を例外化している (`ex01/RPN.cpp:39-69`)。container選択理由の口頭説明は`Critical Questions`に残した。

### Main usage

- **Description**: 正常RPN式、引数なし、空式、0除算、operand不足/余剰、括弧、2桁数、小数を検査し、subject p.11のstderr要件を再確認した。
- **Status**: PASS
- **Evidence/Test Command**:

```sh
mkdir -p ~/42_cpp/Module09/outputs/tmp
make -C ~/42_cpp/Module09/ex01
python3 ~/42_cpp/Module09/outputs/artifacts/tests/test_rpn.py ~/42_cpp/Module09/ex01/RPN
~/42_cpp/Module09/ex01/RPN '(1 + 1)' > ~/42_cpp/Module09/outputs/tmp/rpn_v2_error_stdout.txt 2> ~/42_cpp/Module09/outputs/tmp/rpn_v2_error_stderr.txt
wc -c ~/42_cpp/Module09/outputs/tmp/rpn_v2_error_stdout.txt ~/42_cpp/Module09/outputs/tmp/rpn_v2_error_stderr.txt
sed -n '1,5p' ~/42_cpp/Module09/outputs/tmp/rpn_v2_error_stderr.txt
make -C ~/42_cpp/Module09/ex01 fclean
```

- **Observation**: harnessは`PASS: RPN arithmetic and stderr error-channel tests`、exit 0。8種類の異常入力はすべて非0終了、stderr出力あり、stdout空となった。括弧入力の実測はexit 1、stdout 0 bytes、stderr 21 bytes、stderr=`Error: invalid token`。usageとcatchは`std::cerr`へ修正済み (`ex01/main.cpp:7,15`) で、v1のFAILは解消した。

### Usage advanced

- **Description**: rubric記載の3複合式を含む正常式で演算順序と結果を再検証した。
- **Status**: PASS
- **Evidence/Test Command**:

```sh
make -C ~/42_cpp/Module09/ex01
~/42_cpp/Module09/ex01/RPN '8 9 * 9 - 9 - 9 - 4 - 1 +'
~/42_cpp/Module09/ex01/RPN '9 8 * 4 * 4 / 2 + 9 - 8 - 8 - 1 - 6 -'
~/42_cpp/Module09/ex01/RPN '1 2 * 2 / 2 + 5 * 6 - 1 3 * - 4 5 * * 8 /'
make -C ~/42_cpp/Module09/ex01 fclean
```

- **Observation**: 実出力は順に`42`, `42`, `15`でrubricの期待値と一致した。追加正常ケース`1 2 * 2 / 2 * 2 4 - +`、単一数`9`、負の計算結果`7 8 -`も`test_rpn.py`で一致した。

## Exercise 02: PmergeMe

### Code review

- **Description**: Makefile、2 container、過去exerciseと異なるcontainer、Ford-Johnsonのcontainer別実装、Jacobsthal挿入順、全non-interface classのOCFを再監査した。
- **Status**: FAIL
- **Evidence/Test Command**:

```sh
rg -n 'CXX|CXXFLAGS|^all:|^clean:|^fclean:|^re:' ~/42_cpp/Module09/ex02/Makefile
rg -n '#[[:space:]]*include[[:space:]]*<(vector|deque)>|std::vector|std::deque|fordJohnsonVector|fordJohnsonDeque|makeInsertionOrder|lower_bound' ~/42_cpp/Module09/ex02/PmergeMe.hpp ~/42_cpp/Module09/ex02/PmergeMe.cpp
rg -n '^(struct|class)[[:space:]]|^[[:space:]]*(explicit[[:space:]]+)?(Element|Pair|Pending|ElementPointerLess)[[:space:]]*\(|operator=' ~/42_cpp/Module09/ex02/PmergeMe.cpp
```

- **Observation**: vector/deque用の再帰本体は別実装 (`ex02/PmergeMe.cpp:155-204,229-276`) で、pairing、再帰、Jacobsthal順、paired-largeまでの二分挿入を行う。しかしrubricは「any non-interface class is not in orthodox canonical class form, do not grade this exercise」と明記する (`docs/CPP Module 09 correction.html:397-405`)。内部class型`Element`, `Pair`, `Pending`, `ElementPointerLess` (`ex02/PmergeMe.cpp:17-59`) はOCF 4要素を宣言しておらず、`Element`と`Pending`は引数付きconstructorしかない (`ex02/PmergeMe.cpp:22-24,36-39`)。v1のCRITICALは未解消で、本項目はFAIL。

### Main usage

- **Description**: 1要素、整列済み、逆順、奇数/偶数、重複、`INT_MAX`、固定例、決定的random、3000相異整数、不正値、NULLを検査し、After列を独立oracleと比較した。
- **Status**: PASS
- **Evidence/Test Command**:

```sh
mkdir -p ~/42_cpp/Module09/outputs/tmp
make -C ~/42_cpp/Module09/ex02
python3 ~/42_cpp/Module09/outputs/artifacts/tests/test_pmerge.py ~/42_cpp/Module09/ex02/PmergeMe
c++ -Wall -Wextra -Werror -std=c++98 -I~/42_cpp/Module09/ex02 ~/42_cpp/Module09/outputs/artifacts/tests/null_ex02.cpp ~/42_cpp/Module09/ex02/PmergeMe.cpp -o ~/42_cpp/Module09/outputs/tmp/null_v2_ex02
~/42_cpp/Module09/outputs/tmp/null_v2_ex02
~/42_cpp/Module09/ex02/PmergeMe 3 5 9 7 4
make -C ~/42_cpp/Module09/ex02 fclean
```

- **Observation**: harnessは`PASS: 200 oracle runs, including 3000 unique integers and invalid inputs`、exit 0。3000逆順・相異値では`vector=677.00000 us`, `deque=984.00000 us`で、4行形式、両container名、非0精度を確認した。固定例は`Before: 3 5 9 7 4`, `After: 3 4 5 7 9`。不正値は非0終了かつstderrへ出力。NULL harnessは`caught: invalid positive integer`、exit 0で、guard (`ex02/PmergeMe.cpp:103-105`) が機能した。

## メモリ検査方針と結果

macOS Memory Check Policyの**方針A (PID注入方式)**を採用した。各専用harnessはdefault/copy/copy assignmentと主要処理を実行し、対象objectのdestructor完了後に`sleep(4)`で生存する。`sleep(1)`後に同一shell内でPIDを`leaks`へ渡した。

Evidence:

```sh
mkdir -p ~/42_cpp/Module09/outputs/tmp ~/42_cpp/Module09/outputs/artifacts/logs
c++ -Wall -Wextra -Werror -std=c++98 -I~/42_cpp/Module09/ex00 ~/42_cpp/Module09/outputs/artifacts/tests/memory_ex00.cpp ~/42_cpp/Module09/ex00/BitcoinExchange.cpp -o ~/42_cpp/Module09/outputs/tmp/memory_v2_ex00
c++ -Wall -Wextra -Werror -std=c++98 -I~/42_cpp/Module09/ex01 ~/42_cpp/Module09/outputs/artifacts/tests/memory_ex01.cpp ~/42_cpp/Module09/ex01/RPN.cpp -o ~/42_cpp/Module09/outputs/tmp/memory_v2_ex01
c++ -Wall -Wextra -Werror -std=c++98 -I~/42_cpp/Module09/ex02 ~/42_cpp/Module09/outputs/artifacts/tests/memory_ex02.cpp ~/42_cpp/Module09/ex02/PmergeMe.cpp -o ~/42_cpp/Module09/outputs/tmp/memory_v2_ex02
(cd ~/42_cpp/Module09/ex00 && (../outputs/tmp/memory_v2_ex00 >/dev/null & pid=$!; sleep 1; leaks "$pid" > ../outputs/artifacts/logs/leaks_v2_ex00.txt 2>&1; leaks_status=$?; wait "$pid"; exit "$leaks_status"))
(~/42_cpp/Module09/outputs/tmp/memory_v2_ex01 >/dev/null & pid=$!; sleep 1; leaks "$pid" > ~/42_cpp/Module09/outputs/artifacts/logs/leaks_v2_ex01.txt 2>&1; leaks_status=$?; wait "$pid"; exit "$leaks_status")
(~/42_cpp/Module09/outputs/tmp/memory_v2_ex02 >/dev/null & pid=$!; sleep 1; leaks "$pid" > ~/42_cpp/Module09/outputs/artifacts/logs/leaks_v2_ex02.txt 2>&1; leaks_status=$?; wait "$pid"; exit "$leaks_status")
rg -n 'Process|total leaked bytes' ~/42_cpp/Module09/outputs/artifacts/logs/leaks_v2_ex00.txt ~/42_cpp/Module09/outputs/artifacts/logs/leaks_v2_ex01.txt ~/42_cpp/Module09/outputs/artifacts/logs/leaks_v2_ex02.txt
```

実測結果はex00/ex01/ex02の全てが`0 leaks for 0 total leaked bytes` (`outputs/artifacts/logs/leaks_v2_ex00.txt:26`, `leaks_v2_ex01.txt:26`, `leaks_v2_ex02.txt:26`)。各logはrestricted processのreadonly memory表示制限を1行目で通知したが、malloc node走査とleak集計を25-26行目で完了した。

## 仕様との乖離

1. ex02 rubricのnon-interface class OCF要件 (`docs/CPP Module 09 correction.html:397-405`) に対し、内部helper class型4つがOCF宣言を持たない (`ex02/PmergeMe.cpp:17-59`)。

ex01のstderr乖離は解消した。`ex01/main.cpp:7,15`は`std::cerr`を使用し、`test_rpn.py`は8異常入力でstdout空・stderr非空を確認してexit 0だった。

## 破壊的テスト結果

| 対象 | 入力・条件 | 実測 |
|---|---|---|
| ex00 | 空、欠損、閏日/非閏日、database前、0、1000、1000.001、負数、NaN/Infinity相当、壊れた行 | crashなし。エラー後も末尾まで処理。harness exit 0 |
| ex00 | headerなしで`2011-01-03 | 3`の1行のみ | exit 0、stdout/stderrとも空。先頭行を無条件破棄 (`ex00/BitcoinExchange.cpp:147-150`) |
| ex01 | 引数なし、空、0除算、operand不足/余剰、括弧、2桁、小数 | 全て非0終了、stdout空、stderrにエラー。harness exit 0 |
| ex02 | 200 oracle runs、3000相異整数、重複、境界、不正値 | 全sort一致。不正値はstderr。harness exit 0 |
| ex02 | `argv[1] == NULL` direct harness | `caught: invalid positive integer`、exit 0 |
| 全exercise | destructor後PID注入`leaks` | 各0 leaked bytes |

## Notes (severity-tagged)

- [CRITICAL] ex02の内部helper class型はrubricが明示するnon-interface class OCF要件を満たさない。根拠: `docs/CPP Module 09 correction.html:397-405`, `ex02/PmergeMe.cpp:17-59`。
- [WARN] ex00は入力の先頭行を内容確認せず破棄するため、headerなし1行入力はexit 0・無出力になる。根拠: `ex00/BitcoinExchange.cpp:147-150`, `test_btc.py`実測`INFO: no-header exit=0 stdout='' stderr=''`。subject例のheader付き入力は正常なためrubricのError handleはPASSとした。
- [INFO] v1のex01 stderr CRITICALは解消した。根拠: `ex01/main.cpp:7,15`と`test_rpn.py` exit 0、括弧入力のstdout 0 bytes/stderr 21 bytes。
- [INFO] macOS方針Aのleaks集計は3 exerciseとも0 bytes。根拠: `outputs/artifacts/logs/leaks_v2_ex00.txt:26`, `leaks_v2_ex01.txt:26`, `leaks_v2_ex02.txt:26`。
- [INFO] container選択、Ford-Johnson、container間の時間差の口頭説明は自動実行できないため、人間defense向け質問候補として残した。

CRITICAL: 1件 / WARN: 1件 / INFO: 3件

## Prior Report Reconciliation

参照した過去レポート: `~/42_cpp/Module09/outputs/report_v1.md`

| 旧指摘 (要約) | 旧重大度/区分 | 新分類 | 根拠 |
|---|---|---|---|
| ex01異常時メッセージがstderrに出ない | CRITICAL Note | Resolved by evidence | `ex01/main.cpp:7,15`は`std::cerr`。`test_rpn.py` exit 0、括弧入力stdout 0 bytes/stderr 21 bytes |
| ex02内部helper class型のOCF欠落 | CRITICAL Note | Carried over | `ex02/PmergeMe.cpp:17-59`に4型が残り、OCF 4要素なし。rubric `docs/CPP Module 09 correction.html:397-405` |
| ex00入力先頭行の無条件破棄 | WARN Note | Carried over | `ex00/BitcoinExchange.cpp:147-150`。`test_btc.py`でheaderなし入力はexit 0・無出力 |
| 方針Aのleaksが全exercise 0 bytes | INFO Note | Carried over | v2再測定も`leaks_v2_ex00.txt:26`, `leaks_v2_ex01.txt:26`, `leaks_v2_ex02.txt:26`で0 bytes |
| 口頭説明は人間defenseで確認 | INFO Note | Carried over | 自動レビューにはレビュイーとの対話経路がないため、質問候補を維持 |
| ex01 stderr要件の仕様乖離 | 仕様との乖離 | Resolved by evidence | subject p.11に対し`ex01/main.cpp:7,15`は`std::cerr`、8異常入力でstderr出力を確認 |
| ex02 helper型OCFのrubric乖離 | 仕様との乖離 | Carried over | `ex02/PmergeMe.cpp:17-59`とrubric `docs/CPP Module 09 correction.html:397-405` |
| ex01を`std::cerr`へ変更する修正要求 | 修正要求1 | Resolved by evidence | `ex01/main.cpp:7,15`で実装済み、stderr channel test PASS |
| ex02 helper型をOCF化する修正要求 | 修正要求2 | Carried over | `Element/Pair/Pending/ElementPointerLess`にOCF宣言なし (`ex02/PmergeMe.cpp:17-59`) |
| ex00 headerを検証またはdata処理する修正要求 | 修正要求3 | Carried over | 先頭`getline`後に検証せず次の行からloop (`ex00/BitcoinExchange.cpp:147-150`) |
| ex00のmap選択と`lower_bound`説明 | Critical Question 1 | Carried over | 実装箇所が継続 (`ex00/BitcoinExchange.cpp:177-189`) |
| ex01のstack選択とoperand順序説明 | Critical Question 2 | Carried over | stack実装とpop順が継続 (`ex01/RPN.hpp:10`, `ex01/RPN.cpp:39-46`) |
| ex01がstdoutを使う理由と修正方法 | Critical Question 3 | Resolved by evidence | stdout使用は解消され`std::cerr`へ変更 (`ex01/main.cpp:7,15`) |
| ex02のFord-Johnson構成説明 | Critical Question 4 | Carried over | pairing/Jacobsthal/二分挿入が継続 (`ex02/PmergeMe.cpp:42-101,155-204,229-276`) |
| ex02 helper structのOCF適合根拠 | Critical Question 5 | Carried over | OCF欠落が継続 (`ex02/PmergeMe.cpp:17-59`) |
| vector/dequeの時間差説明 | Supplementary Question 1 | Carried over | v2実測677.00000 us対984.00000 us、説明はdefense事項 |
| 重複時のpaired-large制約説明 | Supplementary Question 2 | Carried over | equal処理が継続 (`ex02/PmergeMe.cpp:45-52,198-201,270-273`) |
| ex00 headerなし挙動の仕様解釈 | Supplementary Question 3 | Carried over | headerなし入力の無出力を再現 (`ex00/BitcoinExchange.cpp:147-150`) |
| ex01の`long`中間計算の移植性説明 | Supplementary Question 4 | Carried over | `long`演算と`int`範囲checkが継続 (`ex01/RPN.cpp:51-69`) |

DroppedおよびDowngradedに分類した旧指摘はない。

## 修正要求

1. ex02の`Element`, `Pair`, `Pending`, `ElementPointerLess`をrubric要求どおりOCFにするか、helper class型を不要にする設計へ変更する (`ex02/PmergeMe.cpp:17-59`; `docs/CPP Module 09 correction.html:397-405`)。
2. ex00の先頭行を`date | value` headerとして検証し、不正headerを明示的に拒否するか、headerがなければその行をdataとして処理する (`ex00/BitcoinExchange.cpp:147-150`)。

## Cleanup Verification

```sh
make -C ~/42_cpp/Module09/ex00 fclean
make -C ~/42_cpp/Module09/ex01 fclean
make -C ~/42_cpp/Module09/ex02 fclean
find ~/42_cpp/Module09/ex00 ~/42_cpp/Module09/ex01 ~/42_cpp/Module09/ex02 -maxdepth 1 -type f \( -name '*.o' -o -name '*.d' -o -name 'btc' -o -name 'RPN' -o -name 'PmergeMe' -o -name '*_shrubbery' \) -print
find ~/42_cpp/Module09/ex00 ~/42_cpp/Module09/ex01 ~/42_cpp/Module09/ex02 -maxdepth 1 -type f -perm -111 -print
git status --short -- ~/42_cpp/Module09
```

3つの`make fclean`は成功し、後続2つの`find`とtarget限定`git status`は無出力だった。workspace全体では`~/42_cpp/`自体がuntracked表示される。実体検査ではbinary、`.o`, `.d`, `*_shrubbery`は`outputs/`外に残っていない。review生成物は`outputs/tmp/`と`outputs/artifacts/`に限定し、既存`.cpp/.hpp/Makefile/data.csv/input.txt`は変更していない。

## レビュイーへの説明要求リスト

以下はAIが自動生成した質問候補であり、defenseで人間レビュアーが選別・拡張する。

### Critical Questions (Top 5)

1. ex00で時系列rate検索に`std::map`を選んだ理由と、`lower_bound`が未登録日に直近下位日付を選ぶ分岐を説明してください (`ex00/BitcoinExchange.cpp:177-189`)。
2. ex01で`std::stack<int,std::list<int> >`を選んだ理由、defaultの`deque`でなく`list`をunderlying containerにした理由、right/left operandをpopする順序を説明してください (`ex01/RPN.hpp:4-5,10`, `ex01/RPN.cpp:39-64`)。
3. ex02のpairing、large chain再帰、Jacobsthal insertion order、paired-largeを上限にしたbinary insertionがFord-Johnsonを構成する理由を説明してください (`ex02/PmergeMe.cpp:42-101,155-204,229-276`)。
4. ex02の内部`struct`もC++ではclass型であることを踏まえ、rubricのnon-interface class OCF要件に対する適合根拠、または修正方針を説明してください (`ex02/PmergeMe.cpp:17-59`)。
5. 3000件でvector 677.00000 us、deque 984.00000 usとなった差を、格納方式、iterator/reference安定性、main-chain insertion costに関連付けて説明してください (`test_pmerge.py`実測)。

### Supplementary Questions

- ex02で重複を許容する方針と、equal valueでもpaired-large制約が壊れない理由を説明してください (`ex02/PmergeMe.cpp:45-52,198-201,270-273`)。
- ex00の入力先頭行がheaderでない場合の現在の挙動と、仕様解釈を説明してください (`ex00/BitcoinExchange.cpp:147-150`)。
- ex01の演算overflowを`long`で中間計算してから`int`範囲確認する設計の移植性を説明してください (`ex01/RPN.cpp:51-69`)。

## Disclosure

42 subject Chapter IVのAI Instructionsを尊重し、本レポートは学習、本人の説明、peer reviewを代替しない。コード理解と最終判定は必ず人間が確認する。

Reviewer: Agentic Reviewer (AI-generated, advisory only)
Final pass/fail decision: requires human reviewer confirmation
