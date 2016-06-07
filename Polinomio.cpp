

#include "Polinomio.h"
#include "Evaluador.h"
#include "Monomio.h"
#include <string.h>
#include <stdio.h>



shared_ptr<Polinomio> Polinomio::pol_ptr(nullptr);

Polinomio::Polinomio() {

    inicio = shared_ptr<NdoPol>(new NdoPol());
}

Polinomio::Polinomio(const Polinomio& orig) { //Constructor de copias 
    shared_ptr<NdoPol> p = orig.inicio;
    inicio = shared_ptr<NdoPol>(new NdoPol(p->mono));
    p = p->sgt;
    shared_ptr<NdoPol> ultimo = inicio;
    while (p != nullptr) {
        ultimo->sgt = shared_ptr<NdoPol>(new NdoPol(p->mono));
        p = p->sgt;
        ultimo = ultimo->sgt;
    }
}

Polinomio::Polinomio(const string& expoli) throw (ExcPolinomioInvalido&) { //Constructor que ordena y simplifica la expresion
    string h = expoli;
    string temp;
    bool unico = true;
    string valor;
    int posTemp;
    int pos;

    //manda excepcion
    if (expoli.length() == 0 || caracterInvalido(expoli)) {
        throw ExcPolinomioInvalido(2);
    }

    pos = h.find_first_of("+-");

    while (pos != -1) { //mientras hayan signos en el string
        valor = h[pos];
        if (pos != 0) { //si no hay sino en el primer monomio
            temp = h.substr(0, (h.find_first_of("+-")));
            Monomio m(temp);

            insertMonoOrd(m);

            h.erase(0, (h.find_first_of("+-")));
        } else {
            if (valor == "+") {
                posTemp = h.find_first_of("+-", pos + 1);
                if (posTemp != -1) { //es positivo, esta en el medio
                    temp = h.substr(pos + 1, posTemp - 1);
                    Monomio m(temp);

                    insertMonoOrd(m);

                    h.erase(0, posTemp);
                } else { //es positivo, esta en el final
                    temp = h.substr(pos + 1);
                    Monomio m(temp);

                    insertMonoOrd(m);

                    h.erase();
                    unico = false;
                }
            } else { //es negativo
                posTemp = h.find_first_of("+-", pos + 1);
                if (posTemp != -1) { //es negativo, esta en el medio
                    temp = h.substr(0, (h.find_first_of("+-", pos + 1)));
                    Monomio m(temp);

                    insertMonoOrd(m);

                    h.erase(0, h.find_first_of("+-", pos + 1));
                } else { //es positivo, esta en el final
                    Monomio m(h);

                    insertMonoOrd(m);

                    h.erase();
                    unico = false;
                }
            }
        }
        pos = h.find_first_of("+-");
        temp.erase();

    }

    if (pos == -1 && unico) {
        Monomio m(h);
        insertMonoOrd(m);
    }
}

bool Polinomio::insertMonoOrd(Monomio m) {
    bool rsl = false;

    shared_ptr<NdoPol> p = inicio;
    shared_ptr<NdoPol> ant = nullptr;
    shared_ptr<NdoPol> actual = nullptr;
    if (p == nullptr) {
        inicio = shared_ptr<NdoPol>(new NdoPol(m));
    } else {
        if (p->mono.semejante(m)) {
            p->mono.asgCoef(p->mono.obtCoef() + m.obtCoef());
        } else {
            if (p->mono.obtExpX() < m.obtExpX() || (p->mono.obtExpX() == m.obtExpX() && p->mono.obtExpY() < m.obtExpY())) {
                p = shared_ptr<NdoPol>(new NdoPol(m));
                p->sgt = inicio;
                inicio = p;
            } else {
                if (p->sgt == nullptr) {
                    ant = p;
                    p = shared_ptr<NdoPol>(new NdoPol(m));
                    ant->sgt = p;
                } else {
                    while (p != nullptr) {
                        ant = p;
                        p = p->sgt;
                        actual = p;
                        if (p->mono.semejante(m)) {
                            p->mono.asgCoef(p->mono.obtCoef() + m.obtCoef());
                            p = nullptr;
                            rsl = false;
                        } else {
                            if (p->mono.obtExpX() < m.obtExpX() || (p->mono.obtExpX() == m.obtExpX() && p->mono.obtExpY() < m.obtExpY())) {
                                p = shared_ptr<NdoPol>(new NdoPol(m));
                                p->sgt = actual;
                                ant->sgt = p;
                                p = nullptr;
                                rsl = false;
                            } else {

                                if (p->sgt == nullptr) {
                                    ant = p;
                                    p = nullptr;
                                }
                                rsl = true;
                            }
                        }
                    }
                }
                if (rsl) {
                    p = shared_ptr<NdoPol>(new NdoPol(m));
                    ant->sgt = p;
                }
            }
        }
    }
}

Polinomio::~Polinomio() { //Destructor
}

Polinomio & Polinomio::operator+(const Polinomio & p) const {

    if (pol_ptr != nullptr)
        pol_ptr.reset();
    pol_ptr = shared_ptr<Polinomio>(new Polinomio());
    shared_ptr<NdoPol> a, b, ultimo;
    a = inicio;
    b = p.inicio;
    ultimo = nullptr;
    while (a != nullptr && b != nullptr) {
        if (a->mono.obtExpX() > b->mono.obtExpX()) { //toma en cuenta caso de que el unico monomio que haya sea el cero
            if (ultimo == nullptr) {
                pol_ptr->inicio = shared_ptr<NdoPol>(new NdoPol(a->mono));
                ultimo = pol_ptr->inicio;
            } else {
                ultimo->sgt = shared_ptr<NdoPol>(new NdoPol(a->mono));
                ultimo = ultimo->sgt;
            }
            a = a->sgt;
        } else if (a->mono.obtExpX() < b->mono.obtExpX()) {
            if (ultimo == nullptr) {
                pol_ptr->inicio = shared_ptr<NdoPol>(new NdoPol(b->mono));
                ultimo = pol_ptr->inicio;
            } else {
                ultimo->sgt = shared_ptr<NdoPol>(new NdoPol(b->mono));
                ultimo = ultimo->sgt;
            }
            b = b->sgt;
        } else { //unico caso que queda es que sean iguales
            if (a->mono.obtExpY() > b->mono.obtExpY()) {
                if (ultimo == nullptr) {
                    pol_ptr->inicio = shared_ptr<NdoPol>(new NdoPol(a->mono));
                    ultimo = pol_ptr->inicio;
                } else {
                    ultimo->sgt = shared_ptr<NdoPol>(new NdoPol(a->mono));
                    ultimo = ultimo->sgt;
                }
                a = a->sgt;
            } else if (a->mono.obtExpY() < b->mono.obtExpY()) {
                if (ultimo == nullptr) {
                    pol_ptr->inicio = shared_ptr<NdoPol>(new NdoPol(b->mono));
                    ultimo = pol_ptr->inicio;
                } else {
                    ultimo->sgt = shared_ptr<NdoPol>(new NdoPol(b->mono));
                    ultimo = ultimo->sgt;
                }
                b = b->sgt;
            } else {
                if (ultimo == nullptr) {
                    pol_ptr->inicio = shared_ptr<NdoPol>(new NdoPol(a->mono + b->mono));
                    ultimo = pol_ptr->inicio;
                } else {
                    ultimo->sgt = shared_ptr<NdoPol>(new NdoPol(a->mono + b->mono));
                    ultimo = ultimo->sgt;
                }
                a = a->sgt;
                b = b->sgt;
            }
        }

    }
    if (a != nullptr) {
        while (a != nullptr) {
            ultimo->sgt = shared_ptr<NdoPol>(new NdoPol(a->mono));
            ultimo = ultimo->sgt;
            a = a->sgt;
        }
    }
    if (b != nullptr) {
        while (b != nullptr) {
            ultimo->sgt = shared_ptr<NdoPol>(new NdoPol(b->mono));
            ultimo = ultimo->sgt;
            b = b->sgt;
        }
    }


    return *pol_ptr;

}

Polinomio & Polinomio::operator-(const Polinomio & p) const {

    if (pol_ptr != nullptr)
        pol_ptr.reset();
    pol_ptr = shared_ptr<Polinomio>(new Polinomio());
    shared_ptr<NdoPol> a, b, ultimo;
    Monomio menos("-1");
    a = inicio;
    b = p.inicio;
    ultimo = nullptr;
    while (a != nullptr && b != nullptr) {
        if (a->mono.obtExpX() > b->mono.obtExpX()) { //toma en cuenta caso de que el unico monomio que haya sea el cero
            if (ultimo == nullptr) {
                pol_ptr->inicio = shared_ptr<NdoPol>(new NdoPol(a->mono));
                ultimo = pol_ptr->inicio;
            } else {
                ultimo->sgt = shared_ptr<NdoPol>(new NdoPol(a->mono));
                ultimo = ultimo->sgt;
            }
            a = a->sgt;
        } else if (a->mono.obtExpX() < b->mono.obtExpX()) {
            if (ultimo == nullptr) {

                pol_ptr->inicio = shared_ptr<NdoPol>(new NdoPol(b->mono * menos));
                ultimo = pol_ptr->inicio;
            } else {

                ultimo->sgt = shared_ptr<NdoPol>(new NdoPol(b->mono * menos));
                ultimo = ultimo->sgt;
            }
            b = b->sgt;
        } else { //unico caso que queda es que sean iguales
            if (a->mono.obtExpY() > b->mono.obtExpY()) {
                if (ultimo == nullptr) {
                    pol_ptr->inicio = shared_ptr<NdoPol>(new NdoPol(a->mono));
                    ultimo = pol_ptr->inicio;
                } else {
                    ultimo->sgt = shared_ptr<NdoPol>(new NdoPol(a->mono));
                    ultimo = ultimo->sgt;
                }
                a = a->sgt;
            } else if (a->mono.obtExpY() < b->mono.obtExpY()) {
                if (ultimo == nullptr) {

                    pol_ptr->inicio = shared_ptr<NdoPol>(new NdoPol(b->mono * menos));
                    ultimo = pol_ptr->inicio;
                } else {

                    ultimo->sgt = shared_ptr<NdoPol>(new NdoPol(b->mono * menos));
                    ultimo = ultimo->sgt;
                }
                b = b->sgt;
            } else {
                if (ultimo == nullptr) {
                    pol_ptr->inicio = shared_ptr<NdoPol>(new NdoPol(a->mono - b->mono));
                    ultimo = pol_ptr->inicio;
                    if (ultimo->mono.obtCoef() == 0) {
                        ultimo->mono = ultimo->mono * menos;
                    }
                } else {
                    ultimo->sgt = shared_ptr<NdoPol>(new NdoPol(a->mono - b->mono));
                    ultimo = ultimo->sgt;
                    if (ultimo->mono.obtCoef() == 0) {
                        ultimo->mono = ultimo->mono * menos;
                    }
                }
                a = a->sgt;
                b = b->sgt;
            }
        }

    }
    if (a != nullptr) {
        while (a != nullptr) {
            ultimo->sgt = shared_ptr<NdoPol>(new NdoPol(a->mono));
            ultimo = ultimo->sgt;
            a = a->sgt;
        }
    }
    if (b != nullptr) {
        while (b != nullptr) {
            ultimo->sgt = shared_ptr<NdoPol>(new NdoPol(b->mono * menos));
            ultimo = ultimo->sgt;
            b = b->sgt;
        }
    }


    return *pol_ptr;

}

Polinomio & Polinomio::operator*(const Polinomio & p) const {

    if (pol_ptr != nullptr)
        pol_ptr.reset();
    pol_ptr = shared_ptr<Polinomio>(new Polinomio());
    string h = pol_ptr->aHil();
    shared_ptr<NdoPol> a, b, ultimo;
    a = inicio;
    b = p.inicio;
    ultimo = nullptr;

    if (a == nullptr || b == nullptr) {
        //pol_ptr->inicio = shared_ptr<NdoPol>(new NdoPol());
    } else {
        while (a != nullptr) { // variable independiente
            while (b != nullptr) { //variable dependiente

                if (ultimo == nullptr) {
                    Monomio m(a->mono * b->mono);


                    pol_ptr->inicio = shared_ptr<NdoPol>(new NdoPol(m));

                    ultimo = pol_ptr->inicio;
                } else {
                    Monomio m(a->mono * b->mono);
                    pol_ptr->insertMonoOrd(m);

                }
                b = b->sgt;

            }
            b = p.inicio;
            a = a->sgt;
        }
    }

    return *pol_ptr;

}

Polinomio & Polinomio::operator/(const Polinomio & p)const throw (ExcPolinomioInvalido&) {
    if (pol_ptr != nullptr)
        pol_ptr.reset();
    pol_ptr = shared_ptr<Polinomio>(new Polinomio());
    shared_ptr<NdoPol> a, b;
    a = inicio;
    b = p.inicio;
    if (b->mono.obtCoef() == 0)
        throw ExcPolinomioInvalido(0);
    return *pol_ptr;

}

int Polinomio::verGrado() {

    shared_ptr<NdoPol> p = inicio;
    int mayor;
    if (inicio->mono.obtExpX() >= inicio->mono.obtExpY()) {
        mayor = inicio->mono.obtExpX();
    } else {
        mayor = inicio->mono.obtExpY();
    }
    return mayor;
}

double Polinomio::eval(double x, double y)throw (ExcPolinomioInvalido&) {
    double res = 0.0;
    double coefTemp = 0.0;
    int expXTemp = 0;
    int expYTemp = 0;
    double valor1 = 0.0;
    double valor2 = 0.0;
    shared_ptr<NdoPol> p = inicio;
    while (p != nullptr) {
        coefTemp = p->mono.obtCoef();
        expXTemp = p->mono.obtExpX();
        expYTemp = p->mono.obtExpY();
        valor1 = pow(x, expXTemp);
        valor2 = pow(y, expYTemp);
        if ((x == 0 && expXTemp == 0) || (y == 0 && expYTemp == 0)) {
            throw ExcPolinomioInvalido(1);
        }
        res = res + (coefTemp * valor1 * valor2);
        p = p->sgt;
    }


    stringstream stream;
    stream << fixed << setprecision(2) << res;
    string s = stream.str();
    res = atof(s.c_str());
    return res;
}

string Polinomio::aHil() {
    stringstream fs;
    shared_ptr<NdoPol> p = inicio;
    while (p != nullptr) {
        if (p != inicio && p->mono.obtCoef() >= 0) {
            fs << "+";
        }
        fs << p->mono.aHil();

        p = p->sgt;
    }
    return fs.str();

}

bool Polinomio::verInv() {
    bool rs = true;
    shared_ptr<NdoPol> p = inicio;
    while (p->sgt != nullptr) {
        if (!p->mono.verInv() || p->mono.obtExpX() < p->sgt->mono.obtExpX() || (p->mono.obtExpX() == p->sgt->mono.obtExpX() && p->mono.obtExpY() < p->sgt->mono.obtExpY())) {
            rs = false;
        }
        p = p->sgt;
    }
    return rs;
}

bool Polinomio::caracterInvalido(string x) {
    string validos = "-+*/0123456789.xXyY()";
    bool rsl = false;
    int i = 0;
    while (!rsl && i < x.length()) {
        if (validos.find(x[i]) == string::npos)
            rsl = true;
        i++;
    }
    return rsl;
}


