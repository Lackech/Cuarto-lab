

#include "Mapeo.h"
#include <map>

Mapeo::Mapeo() {

}

Mapeo::~Mapeo() {
}

void Mapeo::asgPolinomio(const string& nombre, const Polinomio& poli)throw (ExcMapeoInvalido&) {
    typedef pair <string, Polinomio> par;
    string h = nombre;
    if (h.at(0) != 'p') {
        throw ExcMapeoInvalido(0); //si el nombre es diferente de p#
    } else {
        nva.insert(par(nombre, poli));
    }
}

Polinomio& Mapeo::obtPolinomio(const string& nombre)throw (ExcMapeoInvalido&) {
    Tabla::iterator it = nva.begin();
    it = nva.find(nombre);
    if (it == nva.end()) {
        throw ExcMapeoInvalido(1); //si no existe el nombre en mapeo
    } else
        return it->second;
}

bool Mapeo::verPolinomio(const string& nombre) {
    Tabla::iterator it = nva.begin();
    bool encontrado = false;
    it = nva.find(nombre);
    if (it != nva.end()) {
        encontrado = true;
    }
    return encontrado;
}