

#include "ExcMonomioInvalido.h"

ExcMonomioInvalido::ExcMonomioInvalido() : invalid_argument("Monomio inválido:"), tipo(0) {
}

ExcMonomioInvalido::ExcMonomioInvalido(int te) : invalid_argument("Excepcion de monomio:"), tipo(te) {
}

string ExcMonomioInvalido::what() {
    string err(invalid_argument::what());
    if (tipo == 0) { // tipo == 0
        err = err + "división por monomio cero";
    } else if (tipo == 1) { // tipo == 1
        err = err + "suma o resta con monomios no semejantes";
    } else if (tipo == 2) { // tipo == 2
        err = err + "división con divisor que tiene exponentes mayores al dividendo";
    } else if (tipo == 3) { // tipo == 3
        err = err + "expresión inválida para construir monomio";
    } else if (tipo == 4) { // tipo == 4
        err = err + "cero a la cero se invalida";
    }
    return err;
}

