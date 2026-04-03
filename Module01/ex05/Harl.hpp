#ifndef HARL_HPP
# define HARL_HPP

# include <string>

// 関数ポインタ

class Harl {

private:
    void debug( void );
    void info( void );
    void warning( void );
    void error( void );

public:
    Harl();
    ~Harl();

    // パラメータとして渡されたレベルに応じて上記の4つのメンバー関数を呼び出す
    void complain( std::string level );
};

#endif
