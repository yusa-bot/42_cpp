#ifndef SHRUBBERY_CREATION_FORM_HPP
# define SHRUBBERY_CREATION_FORM_HPP

# include "AForm.hpp"
# include <string>

class ShrubberyCreationForm : public AForm {
private:
    const std::string _target;

protected:
    virtual void executeAction() const;

public:
    ShrubberyCreationForm(const std::string& target);
    ShrubberyCreationForm(const ShrubberyCreationForm& src);
    ShrubberyCreationForm& operator=(const ShrubberyCreationForm& rhs);
    virtual ~ShrubberyCreationForm();

    class FileOpenException : public std::exception {
    public:
        virtual const char* what() const throw();
    };
};

#endif
