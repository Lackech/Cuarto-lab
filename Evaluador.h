

#ifndef EVALUADOR_H
#define	EVALUADOR_H
#include <string>
#include <iostream>
#include <memory>
using namespace std;

#include "Polinomio.h"
#include "Mapeo.h"

template <typename T>

class Evaluador {
public:
    Evaluador(Mapeo < T >& m);
    //virtual ~Evaluador();


    // REQ: exp esté bien construida y que existan los nombres de polinomios en el Mapeo.
    // RET: un polinomio con el resultado de evaluar exp.
    // EFE: evaluar exp y genera un nuevo polinomio.
    T& evaluar(const string& exp);



    //private:

    int prioridades(string h);

    // REQ: exp esté bien construida y que existan los nombre de polinomios en el Mapeo.
    // RET: una hilera.
    // EFE: construye correctamente una nueva hilera con la equivalente expresión posfija para expInfija.
    string& aPosfija(const string& expInfija);




    // REQ: expPosfija esté bien construida y que existan los nombre de polinomios en el Mapeo.
    // RET: un nuevo polinomio.
    // EFE: construye un nuevo polinomio que cumple la IVA y corresponde a la evaluación correcta de expPosfija.
    T& evaluarPosfija(const string& expPosfija);
    
private:
    Mapeo < T >& mapeo;
    typedef stack<string> Pila;
    static shared_ptr<string> posfija;
    static shared_ptr< T > resObj;
};

//template <typename T>
//shared_ptr<T> Evaluador::resObj(new T);
//shared_ptr<string> Evaluador::posfija(new string);

template <typename T>
Evaluador < T >::Evaluador(Mapeo < T >& m) : mapeo(m) {
}

//Evaluador::~Evaluador() {
//}

template <typename T>
T& Evaluador < T >::evaluar(const string& exp) {
    string prueba;
    string desdeMain = exp;
    prueba = aPosfija(desdeMain);
    Polinomio poli(evaluarPosfija(prueba));
    *resObj = poli;
    return *resObj;
}

template <typename T>
string& Evaluador < T >::aPosfija(const string& expInfija) {

    int posP;
    Pila operadores;
    string caracTemp;
    string h = expInfija;
    int pos = 0;
    string res = "";
    string temp = "";
    posP = h.find_first_of("p");

    //recorrer la string 
    h = h + ")";
    operadores.push("(");
    while (!operadores.empty()) {
        caracTemp = h[pos];

        //caso que sea un parentesis izquierdo
        if (caracTemp == "(") {
            operadores.push(caracTemp);
            pos++;
        }            //caso que sea un operador como *,+,-
        else if (caracTemp == "*" || caracTemp == "+" || caracTemp == "-") {
            while (prioridades(operadores.top()) >= prioridades(caracTemp)) { // puede existir un error
                res = res + operadores.top();
                operadores.pop();
            }
            operadores.push(caracTemp);
            pos++;
        }            //caso de parentesis derecho
        else if (caracTemp == ")") {
            temp = operadores.top();
            while (temp != "(") {
                res = res + temp;
                operadores.pop();
                temp = operadores.top();
            }
            operadores.pop();
            pos++;
        }            //caso que sea un p#
        else {


            if (caracTemp == "p" && pos != posP) {
                res = res + ",";
            }
            res = res + caracTemp;
            pos++;
        }
    }
    *posfija = res;
    return *posfija;
}

//define prioridades a los operadores

template <typename T>
int Evaluador < T >::prioridades(string op) {
    int pri = 100;
    if (op == "*") {
        pri = 4;
    }
    if (op == "+" || op == "-") {
        pri = 3;
    }
    if (op == ")") {
        pri = 2;
    }
    if (op == "(") {
        pri = 1;
    }
    return pri;
}

template <typename T>
T& Evaluador < T >::evaluarPosfija(const string& expPosfija) {

    string caracTemp;
    int pos = 0;
    stack < T > evaluando;
    string heval = expPosfija;
    string posActual;

    //agregar el caracter / al final
    heval = heval + ")";

    //recorre la string hasta que se termina
    while (heval.at(pos) != ')') {
        if (heval.at(pos) != '+' && heval.at(pos) != '-' && heval.at(pos) != '*') {
            caracTemp = heval.at(pos);
            pos++;
            posActual = heval.at(pos);
        }

        //mete en una string el nombre completo del polinomio
        while (posActual.compare(",") != 0 && posActual != "+" && posActual != "-" && posActual != "*") {
            caracTemp = caracTemp + heval.at(pos);
            pos++;
            posActual = heval.at(pos);
        }

        //significa que hay un polinomio adelante
        if (heval[pos] == ',') {
            evaluando.push(mapeo.obtObjeto(caracTemp));
            pos++;
            caracTemp = "";
        } else {
            evaluando.push(mapeo.obtObjeto(caracTemp));
            if (heval[pos] == '+') {
                T t1(evaluando.top());
                evaluando.pop();
                T t2(evaluando.top());
                evaluando.pop();
                T tRes(t2 + t1);
                evaluando.push(tRes);
                pos++;
                if (heval[pos] == ',') {
                    pos++;
                }
            }
            if (heval[pos] == '-') {
                T t1(evaluando.top());
                evaluando.pop();
                T t2(evaluando.top());
                evaluando.pop();
                T tRes(t2 - t1);
                evaluando.push(tRes);
                pos++;
                if (heval[pos] == ',') {
                    pos++;
                }
            }
            if (heval[pos] == '*') {
                T t1(evaluando.top());
                evaluando.pop();
                T t2(evaluando.top());
                evaluando.pop();
                T tRes(t2 * t1);
                evaluando.push(tRes);
                pos++;
                if (heval[pos] == ',') {
                    pos++;
                }
            }
        }
    }
    *resObj = evaluando.top();
    return *resObj;

}


#endif	/* EVALUADOR_H */

