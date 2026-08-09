# CPP Module 05 Review Report v2 — 新 rubric 版 + 追加厳格レビュー

## 新旧 rubric の主な差分 (本レポートで重点検証する箇所)
1. **[新設・足切り]** Prerequisites に「ヘッダ内の関数実装禁止 (テンプレート関数は例外)」。
2. **[構造変更・足切り]** 各 Exercise に独立 `Makefile` 項目 (「フラグ不備ならその exercise の評価を打ち切り」)。
3. **[厳格化]** Ex02: 「抽象 `AForm` を継承し、`AForm::execute(Bureaucrat const&)` を overload」「`Bureaucrat::executeForm(AForm const&)`」を明記 (旧の "2 方式いずれも可" 文は削除)。
4. **[緩和]** Good dispatching: 「if/elseif/else 以外の解法ならよい」(旧は "メンバ関数ポインタ配列" を実質必須)。

## 【判定】: PASS

新 rubric の追加ゲート (ヘッダ実装禁止 / Makefile 足切り / Ex02 AForm 継承) をすべて満たし、Good dispatching も充足。旧 rubric で PASS だった機能要件も維持。総合 PASS 候補。

## 禁止関数チェック: PASS
- 旧 rubric でレビュー済み・省略 (該当 0 件)。新 rubric の Forbidden Function 条件 (`*alloc/*printf/free`, `using namespace`, `friend`, 外部ライブラリ / C++98 以外) は旧と同一。
- **Evidence**: `grep -rEn "printf|[mcr]alloc|free\(|using[[:space:]]+namespace|friend" ~/42_cpp/Module05/ex0{0,1,2,3} --include=*.cpp --include=*.hpp` → 0 件。

## Preliminary tests

### Prerequisites
- **Description**: 必須フラグ (`-Wall -Wextra -Werror`) + C++98 ビルド (旧レビュー済み・省略) に加え、**新設ゲート「ヘッダ内の非テンプレート関数実装禁止」** を検証。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module05
  grep -rnE "\)\s*(const)?\s*\{" ex00 ex01 ex02 ex03 --include="*.hpp"
  ```
- **Observation**: ヘッダ内に関数本体 (`) {` 形式) は **0 件**。全メンバ実装は `.cpp` に分離し、`.hpp` は class 宣言 + 例外クラス宣言のみ (Bureaucrat.hpp/Form.hpp/AForm.hpp/Intern.hpp)。テンプレート実装も無い (Module05 に template なし)。→ 新ゲート適合。ビルド (`-std=c++98` 含む) は旧レビュー済み・省略。

## Exercise 00: Mommy, when I grow up, I want to be a bureaucrat!

### Makefile
- **Status**: PASS
- **Evidence/Test Command**: `cd ~/42_cpp/Module05/ex00 && make && make fclean`
- **Observation**: `c++` + `-Wall -Wextra -Werror -pedantic -std=c++98` でビルド成功 (exit 0)。新 rubric の Makefile 足切りをクリア。

### Bureaucrat
- **Status**: PASS (旧 rubric でレビュー済み・省略)
- **Observation**: 定数 name / grade 1–150 / 範囲外例外 / accessor / increment・decrement と例外 / `std::exception` 派生 / `operator<<` は旧 rubric レビューで PASS 済み。新 rubric の Bureaucrat 項目は旧と実質同一のため詳細省略。

## Exercise 01: Form up, maggots!

### Makefile
- **Status**: PASS
- **Evidence/Test Command**: `cd ~/42_cpp/Module05/ex01 && make && make fclean`
- **Observation**: ビルド成功 (exit 0)。

### Form
- **Status**: PASS (旧 rubric でレビュー済み・省略)
- **Observation**: private/const 属性、grade 制約、accessor、`operator<<`、`beSigned()`、`Bureaucrat::signForm()` は旧レビューで PASS 済み。新 rubric は旧と同一のため詳細省略。

## Exercise 02: No, you need form 28B, not 28C...

### Makefile
- **Status**: PASS
- **Evidence/Test Command**: `cd ~/42_cpp/Module05/ex02 && make && ./AForm >/dev/null; rm -f *_shrubbery; make fclean`
- **Observation**: ビルド成功 (exit 0)。

### Forms that actually do something
- **Description**: **[新 rubric 厳格化]** 「抽象 `AForm` を継承」「`AForm::execute(Bureaucrat const&)` を overload」「`Bureaucrat::executeForm(AForm const&)`」を重点検証 (grade/name/action/target1個は旧レビュー済み・省略)。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  cd ~/42_cpp/Module05
  grep -n "= 0\|void execute\|public AForm\|virtual void executeAction" ex02/AForm.hpp ex02/ShrubberyCreationForm.hpp
  grep -n "executeForm" ex02/Bureaucrat.hpp
  ```
- **Observation**:
  - `AForm` は `virtual void executeAction() const = 0;` (`ex02/AForm.hpp:20`) を持つ**抽象クラス**。3 具体 Form は `: public AForm` で継承 (`ex02/ShrubberyCreationForm.hpp:7` 他)。→ 「抽象 AForm 継承」要件を満たす。
  - execute の実装方式: `AForm::execute(const Bureaucrat&) const` (`ex02/AForm.hpp:40`) が署名/権限チェックを行い、protected 純粋仮想 `executeAction()` に委譲する **NVI 方式**。派生は `executeAction` を override (`ex02/*Form.hpp:12`)。**subject Chapter VI が「基底でチェックしてから派生の別メソッドを呼ぶ方式」を明示的に許可**しているため、新 rubric の「AForm::execute を overload すべき (should)」の意図 (抽象 AForm 経由で execute が機能する) を満たす。詳細は下記「追加厳格レビュー」で論点化。
  - `Bureaucrat::executeForm(const AForm& form) const` (`ex02/Bureaucrat.hpp:38`) — 新 rubric の `AForm const &` 型に一致。
  - 抽象性 (`AForm a(...)` がコンパイル不可) / grade・action・境界は旧レビュー済み・省略。

## Exercise 03: At least this beats coffee-making

### Makefile
- **Status**: PASS
- **Evidence/Test Command**: `cd ~/42_cpp/Module05/ex03 && make && ./Intern >/dev/null; rm -f *_shrubbery; make fclean`
- **Observation**: ビルド成功 (exit 0)。

### Intern
- **Status**: PASS (旧 rubric でレビュー済み・省略)
- **Observation**: `makeForm()` の 3 種生成 / 未知名で NULL・エラー出力は旧レビューで PASS 済み。新 rubric の Intern 項目は旧と同一のため詳細省略。

### Good dispatching
- **Description**: **[新 rubric 緩和]** 「if/elseif/else 以外の解法」であることを検証。
- **Status**: PASS
- **Evidence/Test Command**:
  ```sh
  sed -n '42,64p' ~/42_cpp/Module05/ex03/Intern.cpp
  ```
- **Observation**: `typedef AForm* (Intern::*FormCreator)(const std::string&) const;` によるメンバ関数ポインタ配列 + form 名配列 + `for` ループで dispatch (`ex03/Intern.cpp:44-63`)。フォーム種別ごとの if/elseif チェーンではない (照合の `if` は配列走査の一致判定のみ)。→ 新 rubric「if/else 以外の解法」も、旧 rubric「メンバ関数ポインタ配列」も両方満たす。

## メモリ検査
- **Status**: PASS
- **採用方針**: 方針 B (`leaks --atExit`, escalated)。`new` は ex03 のみ。
- **Evidence/Test Command**: `cd ~/42_cpp/Module05/ex03 && make && leaks --atExit -- ./Intern; rm -f *_shrubbery; make fclean`
- **Observation**: `Process ...: 0 leaks for 0 total leaked bytes.` (190 nodes)。ex00-ex02 は動的確保なし (旧レビュー済み)。

## 追加厳格レビュー (rubric 非依存 / 別観点)

### [設計/新 rubric 表現との整合] AForm::execute の NVI 方式 — 妥当だが説明推奨
- 新 rubric は「should overload the abstract `AForm::execute`」と表現するが、本実装は `execute` を**非仮想**にし、純粋仮想は `executeAction`。派生が override するのは `executeAction`。
- これは **subject Chapter VI が明示許可**する「基底でチェック→派生の別メソッド呼び出し」方式 (NVI) であり、AForm は `executeAction` により抽象・継承・execute 機能をすべて満たす。→ **FAIL ではない**。ただし新 rubric の文言 (execute を overload) と実装 (executeAction を override) の対応を defense で説明できると安全。

### [堅牢性/セキュリティ] ShrubberyCreationForm の任意パス書き込み
- `executeAction` は target をそのままファイル名接頭辞に使う (`ex02/ShrubberyCreationForm.cpp:30`)。target に絶対/相対パスを与えると**作業ディレクトリ外にファイル生成可能**。
- **Evidence/Test Command**:
  ```sh
  mkdir -p /tmp/Module05_v2/sub && cd ~/42_cpp/Module05/ex02 && c++ -Wall -Wextra -Werror -std=c++98 -I. ~/42_cpp/Module05/outputs/artifacts/tests/traversal_ex02.cpp Bureaucrat.cpp AForm.cpp ShrubberyCreationForm.cpp RobotomyRequestForm.cpp PresidentialPardonForm.cpp -o /tmp/Module05_v2/traversal && /tmp/Module05_v2/traversal && ls /tmp/Module05_v2/sub/evil_shrubbery; make fclean
  ```
- **Observation**: `/tmp/Module05_v2/sub/evil_shrubbery` が生成される。subject は "working directory" を想定しており仕様違反ではないが、防御的にはファイル名のサニタイズが望ましい。Severity: INFO。

### [設計] copy assignment が const メンバを複製しない — 妥当
- `Bureaucrat::operator=`/`Form`/`AForm::operator=` は const name/grade を代入せず mutable state のみ複製 (旧レビュー既出)。const メンバゆえの妥当な設計。Severity: INFO。

## Notes (severity-tagged)
- **[WARN] `make fclean` が副生成物 `*_shrubbery` を削除しない。** (旧 rubric レビューからの carry over) `ex02/Makefile` の fclean は `$(RM) $(NAME)` のみ。実測: `./AForm` 実行後 `garden_shrubbery` が残り、`make fclean` 後も残存。repo には未 commit。新 rubric に副生成物削除の明文要件はないが堅牢性上の指摘。
- **[INFO]** AForm は NVI (execute 非仮想 + executeAction 純粋仮想)。新 rubric 文言との対応は説明推奨 (上記)。
- **[INFO]** ShrubberyCreationForm の任意パス書き込み (上記)。
- **[INFO]** copy assignment は const メンバ非複製 (上記)。

## Prior Report Reconciliation

| 旧指摘 (要約) | 旧重大度 | 新分類 | 根拠 |
|---|---|---|---|
| `make fclean` が `*_shrubbery` を削除しない | WARN | Carried over | 本レビューで再現 (`./AForm`→`garden_shrubbery` 残存、`fclean` 後も残存) |
| copy assignment が const メンバを複製しない | INFO | Carried over | `ex00/Bureaucrat.cpp` の `operator=` は `_grade` のみ複製 |
| ShrubberyCreationForm の任意パス書き込み | INFO | Carried over | `traversal_ex02.cpp` で `/tmp/.../evil_shrubbery` 生成を確認 |
| ex03 dispatch がメンバ関数ポインタ配列 (旧 Good dispatching 合致) | INFO | Resolved by evidence | 新 rubric では「if/else 以外」で足り、引き続き充足 (`ex03/Intern.cpp:44-63`) |

## レビュイーへの説明要求リスト

### Critical Questions (Top 5)
1. Ex02 で `AForm::execute` を非仮想にし `executeAction` を純粋仮想にした NVI 設計と、新 rubric「AForm::execute を overload」の対応関係を説明してください (`ex02/AForm.hpp:20,40`)。
2. ShrubberyCreationForm の target に相対/絶対パスを与えた場合の書き込み先と、防御方針を説明してください (`ex02/ShrubberyCreationForm.cpp:30`)。
3. `Intern::makeForm` のメンバ関数ポインタ配列 dispatch の仕組みを説明してください (`ex03/Intern.cpp:44-63`)。
4. copy assignment が const メンバ (name/grade) を複製しない影響を説明してください。
5. `makeForm` が返す `AForm*` の所有権 (誰が delete するか) と未知名 NULL 契約を説明してください。

### Supplementary Questions
- `make fclean` で `*_shrubbery` を削除する最小修正。
- 例外クラスを OCF 対象外にできる subject 上の根拠。

## Disclosure
42 subject Chapter III を尊重し、本レポートは学習・peer learning・人間 defense を代替しない補助的レビューである。

Reviewer: Agentic Reviewer (AI-generated, advisory only)
Final pass/fail decision: requires human reviewer confirmation
