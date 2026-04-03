#include "FileReplacer.hpp"
#include <iostream>
#include <fstream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

FileReplacer::FileReplacer(std::string filename, std::string s1, std::string s2)
    : _filename(filename), _s1(s1), _s2(s2) {}

FileReplacer::~FileReplacer() {}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

// resultに置き換え後を追加
std::string FileReplacer::_replaceLine(std::string line) const {
    if (_s1.empty()) return line;

    std::string result;
    size_t start = 0;
    size_t found;

    // lineに_s1がある間
    while ((found = line.find(_s1, start)) != std::string::npos) {
        result.append(line, start, found - start); // _s1までを追加
        result.append(_s2); // _s2を追加

        // whileの最後の探索での長さを追加
        start = found + _s1.length(); // -> _s1まで+_s2 で次へ
    }

    result.append(line, start, std::string::npos);
    return result;
}

// ifstream, ofstreamを作成 -> ifsからfileを一行づつ読み取り -> replaceしてofsに書き込み
int FileReplacer::execute() const {
    // input file stream: 引数をC言語形式の文字列に変換し、file streamを作成
    std::ifstream ifs(_filename.c_str());

    if (!ifs.is_open()) {
        std::cout << "Error: Cannot open file " << _filename << std::endl;
        return 1;
    }

    // output file stream: 引数をC言語形式の文字列に変換し、file streamを作成
    std::ofstream ofs((_filename + ".replace").c_str());

    if (!ofs.is_open()) {
        std::cout << "Error: Cannot create output file." << std::endl;
        return 1;
    }

    // ifsからfileを一行づつ読み取り -> replaceしてofsに書き込み
    std::string line;
    while (std::getline(ifs, line)) {
        ofs << _replaceLine(line);
        if (!ifs.eof()) ofs << std::endl;
    }
    return 0;
}
