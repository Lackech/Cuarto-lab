

#ifndef EXCMONOMIOINVALIDO_H
#define	EXCMONOMIOINVALIDO_H

#include <exception>
#include <iostream>
#include <string>
using namespace std;

class ExcMonomioInvalido : public invalid_argument {
public:
    ExcMonomioInvalido();
    ExcMonomioInvalido(int);
    string what();

private:
    // 0 = división por monomio cero, 1 = NO semejantes, 2 = exponente inválido en el divisor, 3 = expresión inválida
    int tipo;
};

#endif	/* EXCMONOMIOINVALIDO_H */

