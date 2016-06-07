

#ifndef EXCPOLINOMIOINVALIDO_H
#define EXCPOLINOMIOINVALIDO_H

#include <exception>
#include <iostream>
#include <string>
using namespace std;

#include "ExcMonomioInvalido.h"

class ExcPolinomioInvalido : public invalid_argument {
public:
    ExcPolinomioInvalido();
    ExcPolinomioInvalido(int);
    string what();
    //virtual ~ExcPolinomioInvalido();
private:
    int tipo;
};

#endif /* EXCPOLINOMIOINVALIDO_H */

