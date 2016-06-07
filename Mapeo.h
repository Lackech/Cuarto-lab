

#ifndef MAPEO_H
#define MAPEO_H

#include <iostream>
#include <map>
#include <algorithm>
#include "Polinomio.h"
#include "ExcMapeoInvalido.h"

using namespace std;



template <typename T>
class Mapeo {
public:

    Mapeo();
    //virtual ~Mapeo();

    // REQ: que nombre sea del tipo "p1", "p2", etc; y que poli cumpla la IVA correspondiente.
    // EFE: guarda en *this a poli asociado al nombre.
    void asgObjeto(const string& nombre, T& objGuardar)throw (ExcMapeoInvalido&);

    // REQ: que nombre haya sido previamente guardado con asgPolinomio a un polinomio.
    // RET: el polinomio previamente guardado con el nombre dado.
    // EFE: busca el polinomio previamente guardado bajo el nombre dado.
    T& obtObjeto(const string& nombre)throw (ExcMapeoInvalido&); //Puede ser que se tenga que devolver un tipo T&

    // RET: true si el nombre ha sido previamente asignado a algún polinomio, false en caso contrario.
    bool verObjeto(const string& nombre);

private:
    typedef map<string, T> Tabla;
    Tabla nva;
};

template <typename T>
Mapeo < T >::Mapeo() {

}

//Mapeo::~Mapeo() {
//}


template <typename T>
void Mapeo < T >::asgObjeto(const string& nombre, T& objGuardar)throw (ExcMapeoInvalido&) {
    typedef pair <string, T> par;
    string h = nombre;
    if (h.at(0) != 'p') {
        throw ExcMapeoInvalido(0); //si el nombre es diferente de p#
    } else {
        nva.insert(par(nombre, objGuardar));
    }
}


template <typename T>
T& Mapeo < T >::obtObjeto(const string& nombre)throw (ExcMapeoInvalido&) {
    Tabla::iterator it = nva.begin();
    it = nva.find(nombre);
    if (it == nva.end()) {
        throw ExcMapeoInvalido(1); //si no existe el nombre en mapeo
    } else
        return it->second;
}


template <typename T>
bool Mapeo < T >::verObjeto(const string& nombre) {
    Tabla::iterator it = nva.begin();
    
    bool encontrado = false;
    it = nva.find(nombre);
    if (it != nva.end()) {
        encontrado = true;
    }
    return encontrado;
}

#endif          /* MAPEO_H */
