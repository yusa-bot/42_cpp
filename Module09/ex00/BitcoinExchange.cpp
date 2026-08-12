#include "BitcoinExchange.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

// -----------------------------------------------------------------------------
// namespace
// -----------------------------------------------------------------------------

// これらの関数はBitcoinExchangeオブジェクトが不要なため、関数をこの.cpp以外から見えなくする
namespace {

// 閏年
bool isLeapYear(int year) {
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

int daysInMonth(int year, int month) {
    static const int days[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    if (month == 2 && isLeapYear(year))
        return 29;
    return days[month - 1];
}

bool allDigits(const std::string& text, std::size_t start, std::size_t count) {
    for (std::size_t index = start; index < start + count; ++index) {
        if (text[index] < '0' || text[index] > '9')
            return false;
    }
    return true;
}

int numberAt(const std::string& text, std::size_t start, std::size_t count) {
    int value = 0;
    for (std::size_t index = start; index < start + count; ++index)
        value = value * 10 + (text[index] - '0');
    return value;
}

std::string trim(const std::string& text) {
    const std::string::size_type first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
        return "";
    const std::string::size_type last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

bool isValidDate(const std::string& date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;
    if (!allDigits(date, 0, 4)
        || !allDigits(date, 5, 2)
        || !allDigits(date, 8, 2))
        return false;

    const int year = numberAt(date, 0, 4);
    const int month = numberAt(date, 5, 2);
    const int day = numberAt(date, 8, 2);
    if (year < 1 || month < 1 || month > 12)
        return false;
    return day >= 1 && day <= daysInMonth(year, month);
}

bool parseValue(const std::string& text, double& value) {
    std::istringstream stream(text); // 数値として読み取り
    char extra;

    if (!(stream >> value) || (stream >> extra)) // doubleに変換 -> 数値以外をextraで検出
        return false;
    return value == value; // NaN検査
}

}

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

BitcoinExchange::BitcoinExchange() {
    loadDatabase("data.csv");
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
    : _rates(other._rates) {
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other)
        _rates = other._rates;
    return *this;
}

BitcoinExchange::~BitcoinExchange() {
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

void BitcoinExchange::loadDatabase(const std::string& filename) {
	std::ifstream file(filename.c_str()); // std::ifstreamコンストラクタはファイル名をconst char*として受け取る
    std::string line;

    if (!file.is_open())
        throw std::runtime_error("could not open database file");
    if (!std::getline(file, line))
        throw std::runtime_error("database file is empty");
    if (trim(line) != "date,exchange_rate")
        throw std::runtime_error("invalid database header");

    while (std::getline(file, line)) {
        if (trim(line).empty())
            continue;

        const std::string::size_type separator = line.find(',');
        if (separator == std::string::npos
            || line.find(',', separator + 1) != std::string::npos)
            throw std::runtime_error("invalid database line: " + line);

        const std::string date = trim(line.substr(0, separator));
        const std::string rateText = trim(line.substr(separator + 1));
        double rate;

        if (!isValidDate(date) || !parseValue(rateText, rate) || rate < 0)
            throw std::runtime_error("invalid database line: " + line);
        _rates[date] = rate;
    }
    if (_rates.empty())
        throw std::runtime_error("database contains no exchange rates");
}

void BitcoinExchange::processFile(const std::string& filename) const {

	// ----- parse -----
	std::ifstream file(filename.c_str());
    std::string line;

    if (!file.is_open())
        throw std::runtime_error("could not open input file");
    if (!std::getline(file, line))
        return;

    while (std::getline(file, line)) {
        const std::string originalLine = line;
        const std::string::size_type separator = line.find('|');
        if (separator == std::string::npos
            || line.find('|', separator + 1) != std::string::npos) {
            std::cout << "Error: bad input => " << originalLine << std::endl;
            continue;
        }

        const std::string date = trim(line.substr(0, separator));
        const std::string valueText = trim(line.substr(separator + 1));
        double value;

        if (!isValidDate(date) || !parseValue(valueText, value)) {
            std::cout << "Error: bad input => " << originalLine << std::endl;
            continue;
        }
        if (value < 0) {
            std::cout << "Error: not a positive number." << std::endl;
            continue;
        }
        if (value > 1000) {
            std::cout << "Error: too large a number." << std::endl;
            continue;
        }

		// -----  -----
        std::map<std::string, double>::const_iterator rateIt =
            _rates.lower_bound(date);
		// iterator: first(key), second(value)

		// --rateItするか?
        if (rateIt == _rates.end() || rateIt->first != date) {
            if (rateIt == _rates.begin()) {
                std::cout << "Error: no exchange rate available for date."
                          << std::endl;
                continue;
            }
            --rateIt;
        }

        std::cout << date << " => " << valueText << " = "
                  << value * rateIt->second << std::endl;
    }
}
