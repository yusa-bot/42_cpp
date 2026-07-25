#ifndef BUREAUCRAT_HPP
# define BUREAUCRAT_HPP

# include <string>
# include <iostream>
# include <exception>

class Form;

// Key Point: 例外を呼び出し側で扱う流れ

class Bureaucrat {

private:
    const std::string    _name;
    int                    _grade;

// これらはmainなどclass外部から呼び出せる必要がある
public:
    Bureaucrat();
    Bureaucrat(const Bureaucrat& src);
    Bureaucrat& operator=(const Bureaucrat& rhs);
    ~Bureaucrat();

    Bureaucrat(const std::string& name, int grade);

    // 外部にBureaucrat内部の文字列への参照を公開しないようにするため、copy返し
    std::string getName() const;
    int getGrade() const;

    void incrementGrade();
    void decrementGrade();

    void signForm(Form& form) const;

    // std::exceptionを継承 -> catch (std::exception& e) で捕捉できる
    // virtualは意図的に継承先でも明示的に記載
    // throw(): 例外指定
    class GradeTooHighException : public std::exception {
    public:
        virtual const char* what() const throw();
    };

    class GradeTooLowException : public std::exception {
    public:
        virtual const char* what() const throw();
    };
};

std::ostream& operator<<(std::ostream& os, const Bureaucrat& rhs);

#endif
