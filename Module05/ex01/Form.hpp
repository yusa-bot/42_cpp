#ifndef FORM_HPP
# define FORM_HPP

# include <string>
# include <exception>
# include <iosfwd>

class Bureaucrat;

class Form {
private:
    const std::string _name;
    bool              _signed;
    const int         _gradeToSign;
    const int         _gradeToExecute;

public:
    Form();
    Form(const Form& src);
    Form& operator=(const Form& rhs);
    ~Form();

    Form(const std::string& name, int gradeToSign, int gradeToExecute);

    std::string getName() const;
    bool getIsSigned() const;
    // 必要なgrade
    int getGradeToSign() const;
    int getGradeToExecute() const;

    void beSigned(const Bureaucrat& bureaucrat);

    class GradeTooHighException : public std::exception {
    public:
        virtual const char* what() const throw();
    };

    class GradeTooLowException : public std::exception {
    public:
        virtual const char* what() const throw();
    };
};

std::ostream& operator<<(std::ostream& os, const Form& rhs);

#endif
