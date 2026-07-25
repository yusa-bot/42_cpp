#ifndef AFORM_HPP
# define AFORM_HPP

# include <string>
# include <exception>
# include <iosfwd>

class Bureaucrat;

// Abstract base class
class AForm {
private:
    const std::string _name;
    bool              _signed;
    const int         _gradeToSign;
    const int         _gradeToExecute;

// ** 具体的な処理だけを派生クラスへ任せる
protected:
    virtual void executeAction() const = 0;

public:
    AForm();
    AForm(const AForm& src);
    AForm& operator=(const AForm& rhs);
    virtual ~AForm();

    AForm(const std::string& name, int gradeToSign, int gradeToExecute);

    std::string getName() const;
    bool getIsSigned() const;
    // 必要なgrade
    int getGradeToSign() const;
    int getGradeToExecute() const;

    void beSigned(const Bureaucrat& bureaucrat);

    // ** 署名・grade の共通チェックを AForm::execute() が担当
    // -> 各派生クラスは、条件判定を重複して書かずに、自分固有の処理だけを実装
    void execute(const Bureaucrat& executor) const;

    class GradeTooHighException : public std::exception {
    public:
        virtual const char* what() const throw();
    };

    class GradeTooLowException : public std::exception {
    public:
        virtual const char* what() const throw();
    };

    // **
    class FormNotSignedException : public std::exception {
    public:
        virtual const char* what() const throw();
    };
};

std::ostream& operator<<(std::ostream& os, const AForm& rhs);

#endif
