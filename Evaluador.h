

#ifndef EVALUADOR_H
#define	EVALUADOR_H
#include <string>
#include <iostream>
#include <memory>
using namespace std;

#include "Polinomio.h"
#include "Mapeo.h"

class Evaluador {
public:
    Evaluador(Mapeo& m);
    virtual ~Evaluador();


    // REQ: exp esté bien construida y que existan los nombres de polinomios en el Mapeo.
    // RET: un polinomio con el resultado de evaluar exp.
    // EFE: evaluar exp y genera un nuevo polinomio.
    Polinomio& evaluar(const string& exp);



    //private:

    int prioridades(string h);

    // REQ: exp esté bien construida y que existan los nombre de polinomios en el Mapeo.
    // RET: una hilera.
    // EFE: construye correctamente una nueva hilera con la equivalente expresión posfija para expInfija.
    string& aPosfija(const string& expInfija);




    // REQ: expPosfija esté bien construida y que existan los nombre de polinomios en el Mapeo.
    // RET: un nuevo polinomio.
    // EFE: construye un nuevo polinomio que cumple la IVA y corresponde a la evaluación correcta de expPosfija.
    Polinomio& evaluarPosfija(const string& expPosfija);
private:
    Mapeo& mapeo;
    typedef stack<string> Pila;
    static shared_ptr<string> posfija;
    static shared_ptr<Polinomio> resPol;
};

#endif	/* EVALUADOR_H */

