

#include "ExcMapeoInvalido.h"

ExcMapeoInvalido::ExcMapeoInvalido() : invalid_argument("Mapeo invalido: "), tipo(0) {
}

ExcMapeoInvalido::ExcMapeoInvalido(int tpo) : invalid_argument("Excepcion de mapeo: "), tipo(tpo) {
}

string ExcMapeoInvalido::what() {
    string err(invalid_argument::what());
    if (tipo == 0) { // tipo == 0
        err = err + "El nombre a guardar debe tener el siguiente formato: p#";
    } else if (tipo == 1) { // tipo == 1
        err = err + "No existe un polinomio asociado a ese nombre";
    }
    return err;
}



