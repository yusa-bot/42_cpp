#ifndef FILEREPLACER_HPP
# define FILEREPLACER_HPP

# include <string>

// replace再実装. cppの関数の使い方を知る

class FileReplacer {

private:
    std::string _filename;
    std::string _s1;
    std::string _s2;

    // 内部で使用する置換ロジック（ヘルパー関数）
    std::string _replaceLine(std::string line) const;

public:
    FileReplacer(std::string filename, std::string s1, std::string s2);
    ~FileReplacer();

    // 実際にファイル操作を行うメインメソッド
    int execute() const;
};

#endif
