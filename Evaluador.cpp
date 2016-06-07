

#include "Evaluador.h"
#include <string>
using namespace std;

shared_ptr<Polinomio> Evaluador::resPol(new Polinomio);
shared_ptr<string> Evaluador::posfija(new string);

Evaluador::Evaluador(Mapeo& m) : mapeo(m) {
}

Evaluador::~Evaluador() {
}

Polinomio& Evaluador::evaluar(const string& exp) {
    string prueba;
    string desdeMain = exp;
    prueba = aPosfija(desdeMain);
    Polinomio poli(evaluarPosfija(prueba));
    *resPol = poli;
    return *resPol;
}

string& Evaluador::aPosfija(const string& expInfija) {

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
        }
            //caso que sea un operador como *,+,-
        else if (caracTemp == "*" || caracTemp == "+" || caracTemp == "-") {
            while (prioridades(operadores.top()) >= prioridades(caracTemp)) { // puede existir un error
                res = res + operadores.top();
                operadores.pop();
            }
            operadores.push(caracTemp);
            pos++;
        }
            //caso de parentesis derecho
        else if (caracTemp == ")") {
            temp = operadores.top();
            while (temp != "(") {
                res = res + temp;
                operadores.pop();
                temp = operadores.top();
            }
            operadores.pop();
            pos++;
        }
            //caso que sea un p#
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

int Evaluador::prioridades(string op) {
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

Polinomio& Evaluador::evaluarPosfija(const string& expPosfija) {

    string caracTemp;
    int pos = 0;
    stack<Polinomio> evaluando;
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
            evaluando.push(mapeo.obtPolinomio(caracTemp));
            pos++;
            caracTemp = "";
        } else {
            evaluando.push(mapeo.obtPolinomio(caracTemp));
            if (heval[pos] == '+') {
                Polinomio poli1(evaluando.top());
                evaluando.pop();
                Polinomio poli2(evaluando.top());
                evaluando.pop();
                Polinomio polires(poli2 + poli1);
                evaluando.push(polires);
                pos++;
                if (heval[pos] == ',') {
                    pos++;
                }
            }
            if (heval[pos] == '-') {
                Polinomio poli1(evaluando.top());
                evaluando.pop();
                Polinomio poli2(evaluando.top());
                evaluando.pop();
                Polinomio polires(poli2 - poli1);
                evaluando.push(polires);
                pos++;
                if (heval[pos] == ',') {
                    pos++;
                }
            }
            if (heval[pos] == '*') {
                Polinomio poli1(evaluando.top());
                evaluando.pop();
                Polinomio poli2(evaluando.top());
                evaluando.pop();
                Polinomio polires(poli2 * poli1);
                evaluando.push(polires);
                pos++;
                if (heval[pos] == ',') {
                    pos++;
                }
            }
        }
    }
    *resPol = evaluando.top();
    return *resPol;

}
