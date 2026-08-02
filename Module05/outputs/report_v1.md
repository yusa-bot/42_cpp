# CPP Module 05 Review Report (Rubric Based)

> レビュー対象: `cpp-module--ayusa/Module05` (ex00〜ex03)  
> ワークスペースルート: `~/workspace/42tokyo/cpp-module`  
> 以降の Evidence/Test Command はワークスペースルートからそのまま再実行可能。

## Environment Metadata
- OS: `Darwin MacBook-Pro.local 25.5.0 Darwin Kernel Version 25.5.0: Tue Jun 9 22:28:34 PDT 2026; root:xnu-12377.121.10~1/RELEASE_ARM64_T6041 arm64`
- Compiler: `Apple clang version 21.0.0 (clang-2100.1.1.101)`
- Memory checker: `leaks: Search through a process for leaked memory.` (`valgrind` は未インストール)
- pdftotext: `pdftotext version 26.03.0`
- Document parsing: `pdftotext -layout 05/docs/en.subject.pdf /tmp/m05_subject.txt`

## 【判定】: PASS

ex00〜ex03 の必須ファイルが存在し、全 exercise が `-Wall -Wextra -Werror -std=c++98` で警告なくビルド・実行できた。rubric の Bureaucrat / Form / Forms that actually do something / Intern / Good dispatching を全て満たし、禁止関数・禁止 STL・C++11 機能の使用はない。ex03 の動的確保について `leaks` で 0 leaks を確認した。人間レビュアーによる最終確認を前提とした総合 PASS 候補。

## 禁止関数チェック: PASS
- **Description**: `*printf` / `*alloc` / `free()` / `using namespace` / `friend` / STL コンテナ・`<algorithm>` / C++11 機能の使用有無を検査。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module05
  grep -rEn "printf|[mcr]alloc|free\(|using[[:space:]]+namespace|friend" ex00 ex01 ex02 ex03 --include="*.cpp" --include="*.hpp"
  grep -rEn "#include[[:space:]]*<(vector|list|map|set|deque|algorithm|queue|stack|array|unordered_|tuple)" ex00 ex01 ex02 ex03 --include="*.cpp" --include="*.hpp"
  grep -rEn "nullptr|\boverride\b|\bfinal\b|constexpr|std::move|decltype|<chrono>|<thread>|<mutex>|<atomic>|<random>|<regex>" ex00 ex01 ex02 ex03 --include="*.cpp" --include="*.hpp"
  ```
- **Observation**: 3 コマンドとも一致 0 件。subject の Allowed/Forbidden (抽出テキスト 82〜104 行: C++11/Boost、`*printf/*alloc/free`、`using namespace`、`friend`、Module 08 未満の STL containers/algorithms を禁止) に抵触しない。

## Preliminary tests

### Prerequisites
- **Description**: `c++ -Wall -Wextra -Werror` かつ C++98 でビルドできるか。各 exercise の `make`, `make re`, `make fclean` を検証。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module05/ex00 && make && ./Bureaucrat >/dev/null; echo run=$?; make re && make fclean
  cd ~/42_cpp/Module05/ex01 && make && ./Form >/dev/null; echo run=$?; make re && make fclean
  cd ~/42_cpp/Module05/ex02 && make && ./AForm >/dev/null; echo run=$?; rm -f garden_shrubbery; make re && make fclean
  cd ~/42_cpp/Module05/ex03 && make && ./Intern >/dev/null; echo run=$?; rm -f garden_shrubbery; make re && make fclean
  ```
- **Observation**: 全 exercise でビルドと実行は exit 0。各 Makefile の CXX は `c++`、CXXFLAGS は `-Wall -Wextra -Werror -pedantic -std=c++98 -O3` (`ex00/Makefile:2-3`, `ex01/Makefile:2-3`, `ex02/Makefile:2-3`, `ex03/Makefile:2-3`)。`re` は `fclean all` (`ex00/Makefile:25`, `ex01/Makefile:25`, `ex02/Makefile:26`, `ex03/Makefile:26`)。コンパイル警告 0。

### Forbidden Function
- **Description**: rubric の Forbidden Function フラグ対象があるか。
- **Status**: PASS
- **Evidence/Test Command**: 上記「禁止関数チェック」の全コマンド。
- **Observation**: 該当 0 件。Forbidden Function フラグ対象外。

## Ex00: Mommy, when I grow up, I want to be a bureaucrat!

### Bureaucrat
- **Description**: Makefile、Bureaucrat の定数名、grade 1〜150、範囲外例外、accessor、increment/decrement、`std::exception` 捕捉、`operator<<`、OCF を rubric 順に検査。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module05/ex00 && make && ./Bureaucrat; make fclean
  ```
- **Observation**:
  - 定数 name は `ex00/Bureaucrat.hpp:17`、grade は `ex00/Bureaucrat.hpp:18`。
  - コンストラクタは grade `<1` で GradeTooHigh、`>150` で GradeTooLow を throw (`ex00/Bureaucrat.cpp:24-30`)。main は grade 0 / 151 をテスト (`ex00/main.cpp:40-55`)。
  - increment は grade を 1 減らし、grade 1 からの操作を拒否 (`ex00/Bureaucrat.cpp:44-48`)。decrement は 1 増やし、grade 150 からの操作を拒否 (`ex00/Bureaucrat.cpp:50-54`)。実行出力で 2→1、149→150 と各境界例外を確認。
  - 例外は `public std::exception` 派生で `what() const throw()` (`ex00/Bureaucrat.hpp:39-47`, `ex00/Bureaucrat.cpp:60-66`)。実際に `catch (std::exception& e)` で捕捉 (`ex00/main.cpp:19,36,45,53`)。
  - `operator<<` は `std::ostream&` を返し、subject の `<name>, bureaucrat grade <grade>.` 形式 (`ex00/Bureaucrat.cpp:75-78`)。
  - OCF 4 メンバは `ex00/Bureaucrat.hpp:22-25`。main に copy/assign テストあり (`ex00/main.cpp:57-64`)。

## Ex01: Form up, maggots!

### Form
- **Description**: Form の全属性、const/private、grade 制約、accessor/`operator<<`、`beSigned`、`Bureaucrat::signForm`、OCF を検査。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module05/ex01 && make && ./Form; make fclean
  ```
- **Observation**:
  - 全属性は private (`ex01/Form.hpp:11-15`)。name と grade 2 個は const、signed は bool。signed 初期値 false (`ex01/Form.cpp:9-11,28-29`)。
  - grade `<1` で GradeTooHigh、`>150` で GradeTooLow (`ex01/Form.cpp:30-33`)。main で 0/151 をテスト (`ex01/main.cpp:33-48`)。
  - accessor は `ex01/Form.cpp:40-54`、完全な状態を出す `operator<<` は `ex01/Form.cpp:78-84`。
  - `beSigned` は bureaucrat grade が required grade より数値的に大きい場合のみ拒否し、それ以外を署名済みにする (`ex01/Form.cpp:56-60`)。
  - `signForm` は `beSigned` を呼び、成功/失敗理由を出力 (`ex01/Bureaucrat.cpp:56-64`)。実行で両経路を確認。
  - Form の OCF 4 メンバは `ex01/Form.hpp:18-21`、Bureaucrat も OCF 維持 (`ex01/Bureaucrat.hpp:20-23`)。

## Ex02: No, you need form 28B, not 28C...

### Forms that actually do something
- **Description**: AForm の抽象性、3 具体 Form の grade/name/action/target-only constructor、execute 条件、Bureaucrat::executeForm、例外処理、OCF を検査。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module05/ex02 && make && ./AForm; rm -f garden_shrubbery; make fclean
  mkdir -p /tmp/Module05_review && cd ~/42_cpp/Module05/ex02 && c++ -Wall -Wextra -Werror -std=c++98 -I. ~/42_cpp/Module05/outputs/artifacts/tests/boundary_ex02.cpp Bureaucrat.cpp AForm.cpp ShrubberyCreationForm.cpp RobotomyRequestForm.cpp PresidentialPardonForm.cpp -o /tmp/Module05_review/boundary_ex02 && cd /tmp/Module05_review && ./boundary_ex02
  cd ~/42_cpp/Module05/ex02 && c++ -Wall -Wextra -Werror -std=c++98 -I. ~/42_cpp/Module05/outputs/artifacts/tests/abstract_test.cpp AForm.cpp -o /tmp/Module05_review/abstract_test
  ```
- **Observation**:
  - AForm は `virtual void executeAction() const = 0` (`ex02/AForm.hpp:20`) を持つ。abstract_test は `error: variable type 'AForm' is an abstract class` と `unimplemented pure virtual method 'executeAction'` で意図どおりコンパイル失敗。
  - grade は Shrubbery 145/137 (`ex02/ShrubberyCreationForm.cpp:8-10`)、Robotomy 72/45 (`ex02/RobotomyRequestForm.cpp:25-27`)、Presidential 25/5 (`ex02/PresidentialPardonForm.cpp:8-10`)。subject Chapter VI と一致。各 public constructor は target 1 引数 (`ex02/*Form.hpp:15`)。
  - Shrubbery は `<target>_shrubbery` に木を出力 (`ex02/ShrubberyCreationForm.cpp:29-42`)。Robotomy は drilling 音と rand()%2 の 50% 成否 (`ex02/RobotomyRequestForm.cpp:8-18,46-52`)。Presidential は Zaphod Beeblebrox による恩赦を表示 (`ex02/PresidentialPardonForm.cpp:29-33`)。
  - `AForm::execute` は未署名を先に拒否し、次に executor grade を確認してから派生 action を呼ぶ (`ex02/AForm.cpp:62-68`)。rubric が認める「base checks then derived action」方式。
  - `Bureaucrat::executeForm(const AForm&) const` は成功/失敗を明示 (`ex02/Bureaucrat.cpp:66-75`)。
  - boundary_ex02 実測: grade 1/150 生成可、grade 1 increment と 150 decrement は例外、sign grade==145 で成功、未署名 execute は `form is not signed`、exec grade==137 で成功、138 で `form grade is too low`。
  - AForm destructor は virtual (`ex02/AForm.hpp:26`)。AForm/Bureaucrat/全具体 Form に OCF 4 メンバあり。

## Ex03: At least this beats coffee-making

### Intern
- **Description**: Intern クラス、`makeForm(formName,target)`、3 種の生成、未知名のエラー/NULL、所有権解放、OCF を検査。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module05/ex03 && make && ./Intern; rm -f garden_shrubbery; make fclean
  mkdir -p /tmp/Module05_review && cd ~/42_cpp/Module05/ex03 && c++ -Wall -Wextra -Werror -std=c++98 -I. ~/42_cpp/Module05/outputs/artifacts/tests/intern_ex03.cpp Bureaucrat.cpp AForm.cpp ShrubberyCreationForm.cpp RobotomyRequestForm.cpp PresidentialPardonForm.cpp Intern.cpp -o /tmp/Module05_review/intern_ex03 && /tmp/Module05_review/intern_ex03
  ```
- **Observation**:
  - 3 正常名で対応 form を生成し `Intern creates <form>` を出力 (`ex03/Intern.cpp:42-63`)。ハーネスで返却値 NON-NULL と各 grade 145/137, 72/45, 25/5 を確認。
  - 未知名 (`coffee request`) と空文字は `Intern cannot create coffee request` (空名では `Intern cannot create `) を出力し NULL を返す (`ex03/Intern.cpp:65-66`)。main は NULL チェック後に利用 (`ex03/main.cpp:22-26,34,40`)。
  - 成功時に返された 3 pointer は `ex03/main.cpp:45-47` で delete。基底 destructor は virtual (`ex03/AForm.hpp:26`)。
  - Intern の OCF 4 メンバは `ex03/Intern.hpp:15-18`。

### Good dispatching
- **Description**: `makeForm()` が if/elseif の羅列でなく、rubric が求める関数ポインタ配列型 dispatch を使うか。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  sed -n '42,67p' ~/42_cpp/Module05/ex03/Intern.cpp
  grep -nE "else[[:space:]]+if|elseif" ~/42_cpp/Module05/ex03/Intern.cpp
  ```
- **Observation**: メンバ関数ポインタ型 `FormCreator`、form 名配列、creator 配列を同一 index で dispatch (`ex03/Intern.cpp:44-62`)。`else if`/`elseif` は一致 0 件。rubric の “array of pointers to member functions” を直接満たす。

## 破壊的テスト結果
- grade 上限/下限: `boundary_ex02.cpp` で 1/150 は有効、grade 1 increment と grade 150 decrement は例外。クラッシュなし。
- 署名境界: Shrubbery の sign grade==145 は成功。
- execute 境界: exec grade==137 は成功、138 は拒否。
- 未署名 form: `form is not signed` を throw し `executeForm` が捕捉。
- 不明 Form 名/空名: `Intern::makeForm` は NULL を返しクラッシュしない。
- ファイル生成失敗: 存在しないディレクトリを target にした Shrubbery は `FileOpenException` を throwし、executeForm が捕捉して exit 0。
  - **Evidence/Test Command**:
    ```sh
    mkdir -p /tmp/Module05_review && cd ~/42_cpp/Module05/ex02 && c++ -Wall -Wextra -Werror -std=c++98 -I. ~/42_cpp/Module05/outputs/artifacts/tests/filefail_ex02.cpp Bureaucrat.cpp AForm.cpp ShrubberyCreationForm.cpp RobotomyRequestForm.cpp PresidentialPardonForm.cpp -o /tmp/Module05_review/filefail_ex02 && /tmp/Module05_review/filefail_ex02; echo exit=$?
    ```
  - **Observation**: `chief couldn't execute ShrubberyCreationForm because could not open shrubbery file`、`survived file-open failure without crash`、`exit=0`。
- SIGSEGV/SIGABRT/SIGBUS を生じた入力はなし。

## メモリ検査
- **Description**: ヒープ確保と解放、未知名を含む Intern 経路を検査。
- **Status**: PASS
- **採用方針**: macOS Memory Check Policy **方針 B (`leaks --atExit`)**。
- **方針 B の条件確認**:
  1. 対象 `./Intern` と保存済み `intern_ex03` harness は stdin を読まず、sleep なしで自走完了する専用テスト。
  2. サンドボックス内では task port 制約により `leaks` がハングしたため停止し、escalated(非サンドボックス)権限で再実行。再実行は 1 秒未満で完了しハングしなかった。
- **Evidence/Test Command** (macOS では対象プロセスを検査可能な権限で実行):
  ```sh
  cd ~/42_cpp/Module05/ex03 && make && leaks --atExit -- ./Intern; rm -f garden_shrubbery; make fclean
  mkdir -p /tmp/Module05_review && cd ~/42_cpp/Module05/ex03 && c++ -Wall -Wextra -Werror -std=c++98 -I. ~/42_cpp/Module05/outputs/artifacts/tests/intern_ex03.cpp Bureaucrat.cpp AForm.cpp ShrubberyCreationForm.cpp RobotomyRequestForm.cpp PresidentialPardonForm.cpp Intern.cpp -o /tmp/Module05_review/intern_ex03 && leaks --atExit -- /tmp/Module05_review/intern_ex03
  ```
- **Observation**: `./Intern`: `Process 57683: 190 nodes malloced for 31 KB` / `0 leaks for 0 total leaked bytes.`。harness: `Process 57688: 189 nodes malloced for 31 KB` / `0 leaks for 0 total leaked bytes.`。ソース上の `new` は `ex03/Intern.cpp:31,35,39` のみで、main は成功返却 pointer を `ex03/main.cpp:45-47` で delete。

## 仕様との乖離
- subject に対する機能的な仕様乖離は検出されなかった。
- subject の出力例は “something like” であり、実装の `couldn't`、form class name、理由文は明示的かつ要件の範囲内。

## 修正要求
1. `ex02/Makefile` と `ex03/Makefile` の `fclean` に `$(RM) -f *_shrubbery` を追加し、プログラムの副生成物も掃除することを推奨。現状は実行後の `garden_shrubbery` が `make fclean` 後も残る。
2. 防御的改善として Shrubbery target のパス検証を検討すること。`ex02/ShrubberyCreationForm.cpp:30` と `ex03/ShrubberyCreationForm.cpp:30` は target をそのままパス接頭辞として使い、絶対パスや `../` による作業ディレクトリ外書き込みを許す。subject の必須要件ではないため FAIL 事由とはしない。

## Notes (severity-tagged)
- **[INFO] const メンバを持つクラスの copy assignment は mutable state のみコピー。** Bureaucrat は `_grade` (`ex00/Bureaucrat.cpp:13-19`)、Form/AForm は `_signed` (`ex01/Form.cpp:18-22`, `ex02/AForm.cpp:18-22`) のみ。OCF メンバ自体は存在し、const 属性を代入できない C++ の制約に沿う。仕様違反ではない。
- **[INFO] Makefile は必須フラグに `-pedantic -O3` を追加。** `ex00/Makefile:3` 等。必須フラグを失わず、全ビルド警告 0 のため問題なし。
- **[INFO] Robotomy は初回のみ `srand(time(NULL))`。** `ex02/RobotomyRequestForm.cpp:8-18`, `ex03/RobotomyRequestForm.cpp:8-18`。`rand()%2` により各実行が 2 分岐で、subject の 50% 要件に沿う。

## Prior Report Reconciliation

N/A (過去レポートなし)。レビュー開始時、`cpp-module--ayusa/Module05/outputs` は存在せず、`ls` は `No such file or directory`。本レビューで outputs と artifacts/tests を新規作成した。

## クリーンアップ確認
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module05 && for d in ex00 ex01 ex02 ex03; do (cd "$d" && make fclean); done
  find ~/42_cpp/Module05/ex00 ~/42_cpp/Module05/ex01 ~/42_cpp/Module05/ex02 ~/42_cpp/Module05/ex03 -name "*_shrubbery" -delete
  git -C ~/workspace/42tokyo/cpp-module status --short cpp-module--ayusa/Module05
  ```
- **Observation**: 各 exercise の binary / `.o` / `.d` / shrubbery 副生成物を除去。レビュー対象既存ソースに変更なし。新規生成物は `cpp-module--ayusa/Module05/outputs/` 以下の report と artifacts/tests のみ。

## レビュイーへの説明要求リスト

> 以下は AI が自動生成した質問候補であり、defense で人間レビュアーが選別・拡張する前提。

### Critical Questions (Top 5)
1. `AForm::execute` を基底で条件確認し、protected 純粋仮想 `executeAction()` で派生 action を呼ぶ設計の利点を説明してください (`ex02/AForm.cpp:62-68`)。
2. `Intern::makeForm` のメンバ関数ポインタ配列と `(this->*creators[i])(target)` の型・呼出し構文を説明してください (`ex03/Intern.cpp:42-64`)。
3. copy assignment が const name/grade を変更せず mutable state のみ代入する場合、コピー構築との意味の差を説明してください (`ex00/Bureaucrat.cpp:10-19`, `ex02/AForm.cpp:13-22`)。
4. grade 1 が最高という規則に対して、`bureaucrat.getGrade() > requiredGrade` が権限不足を表す理由と equality 境界を説明してください (`ex02/AForm.cpp:56-66`)。
5. `Intern::makeForm` が返す raw `AForm*` の所有権、virtual destructor の必要性、NULL 返却時の呼出側責任を説明してください (`ex03/Intern.cpp:30-40,65-66`, `ex03/main.cpp:22-47`)。

### Supplementary Questions
- `make fclean` で shrubbery 副生成物を消す最小修正を説明してください。
- Shrubbery target に絶対パスや `../` が含まれる場合の挙動と、防御する場合の方針を説明してください。
- 例外クラスを OCF 対象外にできる subject 上の根拠を説明してください (subject Chapter IV, PDF p.8)。
- Robotomy の seed を static 初回一回にした理由を説明してください (`ex03/RobotomyRequestForm.cpp:8-18`)。

## Disclosure
42 subject Chapter III (AI Instructions) を尊重し、本レポートは学習・peer learning・人間による defense を代替しない補助的レビューである。

Reviewer: Agentic Reviewer (AI-generated, advisory only)  
Final pass/fail decision: requires human reviewer confirmation
