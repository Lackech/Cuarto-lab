

#ifndef EXCMAPEOINVALIDO_H
#define EXCMAPEOINVALIDO_H

#include <exception>
#include <iostream>
#include <string>
using namespace std;

class ExcMapeoInvalido : public invalid_argument {
public:
    ExcMapeoInvalido();
    ExcMapeoInvalido(int tpo);
    string what();
private:

    int tipo;

};

#endif /* EXCMAPEOINVALIDO_H */

