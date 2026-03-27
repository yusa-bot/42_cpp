#include "Account.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>

// -----------------------------------------------------------------------------
// static変数のみ、クラス外で1度だけ初期化する。 // 最初に静的変数を初期化する
// -----------------------------------------------------------------------------

int Account::_nbAccounts = 0;
int Account::_totalAmount = 0;
int	Account::_totalNbDeposits = 0;
int	Account::_totalNbWithdrawals = 0;

// -----------------------------------------------------------------------------
// Static関数
// -----------------------------------------------------------------------------

// public (Staticデータを外部から操作する用のため)
int Account::getNbAccounts(void) { return _nbAccounts; }
int Account::getTotalAmount(void) { return _totalAmount; }
int Account::getNbDeposits(void) { return _totalNbDeposits; }
int Account::getNbWithdrawals(void) { return _totalNbWithdrawals; }

// private (クラス内部の関数から呼び出され、値を保護して整合性を保つため)
void Account::_displayTimestamp(void) {

    // エポックからの経過秒数取得 -> 現地時間に変換
    std::time_t t = std::time(NULL);
    std::tm* now = std::localtime(&t);

    std::cout << "["
              << (now->tm_year + 1900)
              << std::setfill('0') << std::setw(2) << (now->tm_mon + 1)
              << std::setfill('0') << std::setw(2) << now->tm_mday
              << "_"
              << std::setfill('0') << std::setw(2) << now->tm_hour
              << std::setfill('0') << std::setw(2) << now->tm_min
              << std::setfill('0') << std::setw(2) << now->tm_sec
              << "] ";
}

// public (Staticデータを外部から操作する用のため)
void Account::displayAccountsInfos(void) {

    // タイムスタンプ表示
    _displayTimestamp();

    std::cout << "accounts:" << getNbAccounts() << ";"
              << "total:" << getTotalAmount() << ";"
              << "deposits:" << getNbDeposits() << ";"
              << "withdrawals:" << getNbWithdrawals() << std::endl;
}

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

// コンストラクタ
// 各インスタンスごとのprivate変数を初期化
Account::Account(int initial_deposit) {

    // 代入
    _accountIndex = _nbAccounts;
    _amount = initial_deposit;
    _nbDeposits = 0;
    _nbWithdrawals = 0;

    // 更新
    _nbAccounts++;
    _totalAmount += initial_deposit;

    // log記載　例: [19920104_091532] index:0;amount:42;created
    _displayTimestamp();
    std::cout << "index:" << _accountIndex << ";"
              << "amount:" << _amount << ";"
              << "created" << std::endl;
}

// デストラクタ LIFO
Account::~Account(void) {
    // log記載　例: [19920104_091532] index:0;amount:47;closed
    _displayTimestamp();
    std::cout << "index:" << _accountIndex << ";"
              << "amount:" << _amount << ";"
              << "closed" << std::endl;
}

// -----------------------------------------------------------------------------
// インスタンス関数 (Instance Functions)
// -----------------------------------------------------------------------------

// 各口座のステータス表示
// 例: [19920104_091532] index:0;amount:42;deposits:0;withdrawals:0
void Account::displayStatus(void) const {
    _displayTimestamp();
    std::cout << "index:" << _accountIndex << ";"
              << "amount:" << _amount << ";"
              << "deposits:" << _nbDeposits << ";"
              << "withdrawals:" << _nbWithdrawals << std::endl;
}

// 入金処理
void Account::makeDeposit(int deposit) {

    // log記載 例: [19920104_091532] index:0;p_amount:42;deposit:5;amount:47;nb_deposits:1 (更新前後)
    _displayTimestamp();
    std::cout << "index:" << _accountIndex << ";"
              << "p_amount:" << _amount << ";"
              << "deposit:" << deposit << ";";

    // 各インスタンス変数 更新
    _amount += deposit;
    _nbDeposits++;

    // クラス全体 更新
    _totalAmount += deposit;
    _totalNbDeposits++;

    // log記載
    std::cout << "amount:" << _amount << ";"
              << "nb_deposits:" << _nbDeposits << std::endl;
}

// 出金処理
// ログ例 (成功): [19920104_091532] index:1;p_amount:819;withdrawal:34;amount:785;nb_withdrawals:1 (更新前後)
// ログ例 (拒否): [19920104_091532] index:0;p_amount:47;withdrawal:refused
bool Account::makeWithdrawal(int withdrawal) {

    // log記載
    _displayTimestamp();
    std::cout << "index:" << _accountIndex << ";"
              << "p_amount:" << _amount << ";";

    // 残高が足りない
    if (_amount < withdrawal) {
        std::cout << "withdrawal:refused" << std::endl;
        return false;
    } else {
        std::cout << "withdrawal:" << withdrawal << ";";

        // 各インスタンス変数 更新
        _amount -= withdrawal;
        _nbWithdrawals++;

        // クラス全体 更新
        _totalAmount -= withdrawal;
        _totalNbWithdrawals++;

        std::cout << "amount:" << _amount << ";"
                  << "nb_withdrawals:" << _nbWithdrawals << std::endl;
        return true;
    }
}

// 各インスタンスごとの_amount
int Account::checkAmount(void) const {
    return _amount;
}
