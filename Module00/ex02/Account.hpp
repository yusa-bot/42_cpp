// ************************************************************************** //
//                                                                            //
//                Account.hpp for GlobalBanksters United                //
//                Created on  : Thu Nov 20 19:43:15 1989                      //
//                Last update : Wed Jan 04 14:54:06 1992                      //
//                Made by : Brad "Buddy" McLane <bm@gbu.com>                  //
//                                                                            //
// ************************************************************************** //


#pragma once
#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

// ************************************************************************** //
//                               Account Class                                //
// ************************************************************************** //

class Account {

private:

	// <static: インスタンスに依存せずクラス全体で共有> -> 0初期化
	// private: データだから ***ペア***
	static int	_nbAccounts; // 総口座数
	static int	_totalAmount; // 全口座の預金総額
	static int	_totalNbDeposits; // 全口座の累計入金回数
	static int	_totalNbWithdrawals; // 全口座の累計出金回数

	// private: 値を保護して整合性を保つ & 外部からは使わない関数
	static void	_displayTimestamp( void );

	// <各インスタンスごと>
	int				_accountIndex;
	int				_amount;
	int				_nbDeposits;
	int				_nbWithdrawals;

	Account( void ); // 引数なしでのインスタンス化を禁止


public:

	typedef Account		t;

	// <static: インスタンスに依存せずクラス全体で共有>
	// public: 関数だから (getter) ***ペア***
	static int	getNbAccounts( void );
	static int	getTotalAmount( void );
	static int	getNbDeposits( void );
	static int	getNbWithdrawals( void );
	// 銀行全体の総情報
	static void	displayAccountsInfos( void );

	// インスタンスメンバ(関数) - 個々の口座の情報を管理する部分
	// コンストラクタ - 新しい口座を作成, インデックスを割り当て、全体の口座数と総額を更新
	Account( int initial_deposit );
	// デストラクタ
	~Account( void );

	// 入出金処理
	void	makeDeposit( int deposit );
	bool	makeWithdrawal( int withdrawal );
	// check
	int		checkAmount( void ) const;
	void	displayStatus( void ) const;

};

// ************************************************************************** //
// vim: set ts=4 sw=4 tw=80 noexpandtab:                                      //
// -*- indent-tabs-mode:t;                                                   -*-
// -*- mode: c++-mode;                                                       -*-
// -*- fill-column: 75; comment-column: 75;                                  -*-
// ************************************************************************** //


#endif
