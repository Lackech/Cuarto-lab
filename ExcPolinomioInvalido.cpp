

#include "ExcPolinomioInvalido.h"

ExcPolinomioInvalido::ExcPolinomioInvalido() : invalid_argument("Polinomio inválido: "), tipo(0) {

}

ExcPolinomioInvalido::ExcPolinomioInvalido(int te) : invalid_argument("Excepcion de polinomio: "), tipo(te) {
}

string ExcPolinomioInvalido::what() {
    string err(invalid_argument::what());
    if (tipo == 0) { // tipo == 0
        err = err + "división por polinomio cero";
    } else if (tipo == 1) { // tipo == 1
        err = err + "caso de 0^0";
    } else if (tipo == 2) { // tipo == 2
        err = err + "hilera inválida";
    }
    return err;
}

//ExcPolinomioInvalido::~ExcPolinomioInvalido() {
//}

