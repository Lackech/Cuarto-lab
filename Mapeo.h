

#ifndef MAPEO_H
#define        MAPEO_H

#include <iostream>
#include <map>
#include <algorithm>
using namespace std;

#include "Polinomio.h"
#include "ExcMapeoInvalido.h"

class Mapeo {
public:

    Mapeo();
    virtual ~Mapeo();

    // REQ: que nombre sea del tipo "p1", "p2", etc; y que poli cumpla la IVA correspondiente.
    // EFE: guarda en *this a poli asociado al nombre.
    void asgPolinomio(const string& nombre, const Polinomio& poli)throw (ExcMapeoInvalido&);

    // REQ: que nombre haya sido previamente guardado con asgPolinomio a un polinomio.
    // RET: el polinomio previamente guardado con el nombre dado.
    // EFE: busca el polinomio previamente guardado bajo el nombre dado.
    Polinomio& obtPolinomio(const string& nombre)throw (ExcMapeoInvalido&);

    // RET: true si el nombre ha sido previamente asignado a algún polinomio, false en caso contrario.
    bool verPolinomio(const string& nombre);

private:
    typedef map<string, Polinomio> Tabla;
    Tabla nva;


};

#endif          /* MAPEO_H */
