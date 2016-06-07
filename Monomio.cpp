

#include "Monomio.h"

shared_ptr<Monomio> Monomio::rs_ptr = 0;

Monomio::Monomio() : coef(0.0), expX(0), expY(0) {
}

Monomio::Monomio(const Monomio& orig) : coef(orig.coef), expX(orig.expX), expY(orig.expY) {

}

Monomio::~Monomio() {
}

Monomio::Monomio(const string& expmono) throw (ExcMonomioInvalido&) {
    string expmono_crr = quitarBlancos(expmono); // expresión para corregir elimnando blancos y otros caracteres inválidos.
    if (expmono_crr.length() == 0 || caracterInvalido(expmono_crr) || caracterDemas(expmono_crr)) // lanza excepción en caso de hilera vacía y caracter inválido
        throw ExcMonomioInvalido(3);
    string sCoef = "";
    string sExpX = "";
    string sExpY = "";
    int cntExpRgl = 8;
    string hExpRgl[] = {"[xX]{2,}|[yY]{2,}|[.]{2,}|[-+]{2,}|\\(\\)", // 0 -> hilera inválida
        "[-+]?[xX]|[-+]?[yY]|[-+]?[xX][yY]", // 1 -> no coef no exps
        "[-+]?[xX]\\(\\d+\\)|[-+]?[yY]\\(\\d+\\)|[-+]?[xX]\\(\\d+\\)[yY]|[-+]?[xX][yY]\\(\\d+\\)", // 2 -> no coef un exp 
        "[-+]?[xX]\\(\\d+\\)[yY]\\(\\d+\\)", // 3 -> no coef dos exps
        "[-+]?\\d+|[-+]?\\d+\\.\\d+", // 4 -> coef no x, no y, no exps
        "[-+]?\\d+[xX]|[-+]?\\d+[yY]|[-+]?\\d+\\.\\d+[xX]|[-+]?\\d+\\.\\d+[yY]|[-+]?\\d+[xX][yY]|[-+]?\\d+[xX][yY]|[-+]?\\d+\\.\\d+[xX][yY]|[-+]?\\d+\\.\\d+[xX][yY]", // 5 -> coef no exps
        "[-+]?\\d+[xX]\\(\\d+\\)|[-+]?\\d+\\.\\d+[xX]\\(\\d+\\)|[-+]?\\d+[yY]\\(\\d+\\)|[-+]?\\d+\\.\\d+[yY]\\(\\d+\\)|[-+]?\\d+[xX]\\(\\d+\\)[yY]|[-+]?\\d+\\.\\d+[xX]\\(\\d+\\)[yY]|[-+]?\\d+[xX][yY]\\(\\d+\\)|[-+]?\\d+\\.\\d+[xX][yY]\\(\\d+\\)", // 6 -> coef un exp
        "[-+]?\\d+[xX]\\(\\d+\\)[yY]\\(\\d+\\)|[-+]?\\d+\\.\\d+[xX]\\(\\d+\\)[yY]\\(\\d+\\)"}; // 7 -> coef dos exps
    regex expRgl[cntExpRgl];

    for (int i = 0; i < cntExpRgl; i++) // se inicializa arreglo de expresiones regulares
        expRgl[i].assign(hExpRgl[i]);

    for (int i = 0; i < cntExpRgl; i++) { // se identifica la expresión regular que aplica para procesar la hilera
        if (regex_match(expmono_crr, expRgl[i]))
            switch (i) {
                case 0:
                {
                    throw ExcMonomioInvalido(3);
                }
                case 1: // 1 -> no coef no exps
                {
                    if (expmono_crr[0] == '-')
                        sCoef = "-1.0";
                    else sCoef = "1.0";
                    regex expRegDosLetras("[-+]?[xX][yY]");
                    if (regex_match(expmono_crr, expRegDosLetras)) {
                        sExpX = "1";
                        sExpY = "1";
                    } else if (expmono_crr.find('x') != string::npos || expmono_crr.find('X') != string::npos) {
                        sExpX = "1";
                        sExpY = "0";
                    } else if (expmono_crr.find('y') != string::npos || expmono_crr.find('Y') != string::npos) {
                        sExpX = "0";
                        sExpY = "1";
                    }
                    break;
                }
                case 2: // 2 -> no coef un exp
                {
                    if (expmono_crr[0] == '-')
                        sCoef = "-1.0";
                    else sCoef = "1.0";
                    regex expRegDosLetras("[-+]?[xX]\\(\\d+\\)[yY]|[-+]?[xX][yY]\\(\\d+\\)");
                    if (regex_match(expmono_crr, expRegDosLetras))
                        if (expmono_crr.find("x(") != string::npos || expmono_crr.find("X(") != string::npos) {
                            sExpX = extraerExp('x', 'X', expmono_crr);
                            sExpY = "1";
                        } else {
                            sExpX = "1";
                            sExpY = extraerExp('y', 'Y', expmono_crr);
                        } else if (expmono_crr.find("x(") != string::npos || expmono_crr.find("X(") != string::npos) {
                        sExpX = extraerExp('x', 'X', expmono_crr);
                        sExpY = "0";
                    } else {
                        sExpX = "0";
                        sExpY = extraerExp('y', 'Y', expmono_crr) + ")";
                    }
                    break;
                }
                case 3: // 3 -> no coef dos exps
                    if (expmono_crr[0] == '-')
                        sCoef = "-1.0";
                    else sCoef = "1.0";
                    sExpX = extraerExp('x', 'X', expmono_crr);
                    sExpY = extraerExp('y', 'Y', expmono_crr);
                    break;
                case 4: // 4 -> coef no x, no y, no exps
                    sCoef = extraerCoef(expmono_crr);
                    sExpX = "0";
                    sExpY = "0";
                    break;
                case 5: // 5 -> coef no exps
                {
                    sCoef = extraerCoef(expmono_crr);
                    regex expRegDosLetras("[-+]?\\d+[xX][yY]|[-+]?\\d+[xX][yY]|[-+]?\\d+\\.\\d+[xX][yY]|[-+]?\\d+\\.\\d+[xX][yY]");
                    if (regex_match(expmono_crr, expRegDosLetras)) {
                        sExpX = "1";
                        sExpY = "1";
                    } else if (expmono_crr.find('x') != string::npos || expmono_crr.find('X') != string::npos) {
                        sExpX = "1";
                        sExpY = "0";
                    } else if (expmono_crr.find('y') != string::npos || expmono_crr.find('Y') != string::npos) {
                        sExpX = "0";
                        sExpY = "1";
                    }
                    break;
                }
                case 6: // 6 -> coef un exp
                {
                    sCoef = extraerCoef(expmono_crr);
                    regex expRegDosLetras("[-+]?\\d+[xX]\\(\\d+\\)[yY]|[-+]?\\d+\\.\\d+[xX]\\(\\d+\\)[yY]|[-+]?\\d+[xX][yY]\\(\\d+\\)|[-+]?\\d+\\.\\d+[xX][yY]\\(\\d+\\)");
                    if (regex_match(expmono_crr, expRegDosLetras))
                        if (expmono_crr.find("x(") != string::npos || expmono_crr.find("X(") != string::npos) {
                            sExpX = extraerExp('x', 'X', expmono_crr);
                            sExpY = "1";
                        } else {
                            sExpX = "1";
                            sExpY = extraerExp('y', 'Y', expmono_crr);
                        } else if (expmono_crr.find("x(") != string::npos || expmono_crr.find("X(") != string::npos) {
                        sExpX = extraerExp('x', 'X', expmono_crr);
                        sExpY = "0";
                    } else {
                        sExpX = "0";
                        sExpY = extraerExp('y', 'Y', expmono_crr) + ")";
                    }
                    break;
                }
                case 7: // 7 -> coef dos exps
                    sCoef = extraerCoef(expmono_crr);
                    sExpX = extraerExp('x', 'X', expmono_crr);
                    sExpY = extraerExp('y', 'Y', expmono_crr);
                    break;
            }
    };

    coef = atof(sCoef.c_str());
    expX = atoi(sExpX.c_str());
    expY = atoi(sExpY.c_str());

    if (coef == 0.0) {
        expX = 0;
        expY = 0;
    }
}

Monomio& Monomio::operator+(const Monomio& m) const throw (ExcMonomioInvalido&) {
    if (rs_ptr != 0)
        rs_ptr.reset();
    rs_ptr = shared_ptr<Monomio>(new Monomio());
    Monomio& rs = *rs_ptr;
    if (!this->semejante(m))
        throw ExcMonomioInvalido(1);
    else {
        if (!(this->esCero())&&!(m.esCero())) {
            rs.coef = this->coef + m.coef;
            rs.expX = this->expX;
            rs.expY = this->expY;
        } else {
            if (this->esCero())
                rs = m;
            else rs = *this;
        }
        if (rs.coef == 0.0) {
            rs.expX = 0;
            rs.expY = 0;
        }
    }
    return rs; // se usa atributo static
}

Monomio& Monomio::operator-(const Monomio& m) const throw (ExcMonomioInvalido&) {
    if (rs_ptr != 0)
        rs_ptr.reset();
    rs_ptr = shared_ptr<Monomio>(new Monomio());
    Monomio& rs = *rs_ptr;
    if (!this->semejante(m))
        throw ExcMonomioInvalido(1);
    else {
        if (!(this->esCero())&&!(m.esCero())) {
            rs.coef = this->coef - m.coef;
            rs.expX = this->expX;
            rs.expY = this->expY;
        } else {
            if (this->esCero()) {
                rs = m;
                rs.coef = -rs.coef;
            } else rs = *this;
        }
        if (rs.coef == 0.0) {
            rs.expX = 0;
            rs.expY = 0;
        }
    }
    return rs; // se usa atributo static
}

Monomio& Monomio::operator*(const Monomio& m) const {
    if (rs_ptr != 0)
        rs_ptr.reset();
    rs_ptr = shared_ptr<Monomio>(new Monomio());
    Monomio& rs = *rs_ptr;
    rs.coef = this->coef * m.coef;
    rs.expX = this->expX + m.expX;
    rs.expY = this->expY + m.expY;
    if (rs.coef == 0.0) {
        rs.expX = 0;
        rs.expY = 0;
    }
    return rs; // se usa atributo static
}

Monomio& Monomio::operator/(const Monomio& m) const throw (ExcMonomioInvalido&) {
    if (rs_ptr != 0)
        rs_ptr.reset();
    rs_ptr = shared_ptr<Monomio>(new Monomio());
    Monomio& rs = *rs_ptr;
    if (m.esCero()) {
        throw ExcMonomioInvalido(0);
    } else {
        rs.coef = coef / m.coef;
        rs.expX = expX - m.expX;
        rs.expY = expY - m.expY;
        if ((rs.expX < 0) || (rs.expY < 0))
            throw ExcMonomioInvalido(2);
        if (rs.coef == 0.0) {
            rs.expX = 0;
            rs.expY = 0;
        }
    }
    return rs; // se usa atributo static
}

void Monomio::asgCoef(double c) {
    coef = c;
}

void Monomio::asgExpX(int ne) {
    expX = ne;
}

void Monomio::asgExpY(int ne) {
    expY = ne;
}

double Monomio::obtCoef() const {
    return coef;
}

int Monomio::obtExpX() const {
    return expX;
}

int Monomio::obtExpY() const {
    return expY;
}

bool Monomio::semejante(const Monomio & m) const {
    return (expX == m.expX) && (expY == m.expY);
}

bool Monomio::verInv() {
    bool rs = true;
    if (coef == 0)
        rs = (expX == 0) && (expY == 0);
    return rs;
}

string Monomio::aHil() {
    stringstream rs;
    rs << std::setprecision(2) << std::fixed << coef << "X(" << expX << ")Y(" << expY << ")";
    return rs.str();
}

bool Monomio::esCero() const {
    return (coef == 0);
}

string Monomio::extraerExp(char min, char may, string b) {
    string rs;
    int pLetra = b.find(min);
    int pExpIni = 0;
    int pExpFnl = 0;
    if (pLetra == string::npos) {
        pLetra = b.find(may);
    };
    pExpIni = pLetra + 2; // +1 es el (, +2 es el primer dígito del exponente
    pExpFnl = pLetra + 2; // posición del primer caracter del exponente
    while (b[pExpFnl] != ')') pExpFnl++;
    rs = b.substr(pExpIni, pExpFnl - pExpIni); // se extraen los dígitos del expo
    return rs;
}

string Monomio::extraerCoef(string b) {
    string rs;
    int pCoefIni = 0;
    int pCoefFnl = 0;
    while (b[pCoefFnl] != 'x' && b[pCoefFnl] != 'X' && b[pCoefFnl] != 'y' && b[pCoefFnl] != 'Y') pCoefFnl++;
    rs = b.substr(pCoefIni, pCoefFnl - pCoefIni);
    return rs;
}

double Monomio::eval(double vX, double vY) throw (ExcMonomioInvalido&) {
    double rs = 0.0;
    if (((expX == 0)&&(vX == 0.0)) || ((expY == 0)&&(vY == 0.0)))
        throw ExcMonomioInvalido(4);
    else rs = coef * pow(vX, expX) * pow(vY, expY);
    return rs;
}

bool Monomio::operator<(const Monomio& m) const {
    return ((expX < m.expX) || (expX == m.expX)&&(expY < m.expY));
}

string Monomio::quitarBlancos(string x) {
    string rsl = "";
    for (int i = 0; i < x.length(); i++) {
        if (x[i] != ' ')
            rsl = rsl + x[i];
    }
    return rsl;
}

bool Monomio::caracterInvalido(string x) {
    string validos = "-+0123456789.xXyY()";
    bool rsl = false;
    int i = 0;
    while (!rsl && i < x.length()) {
        if (validos.find(x[i]) == string::npos)
            rsl = true;
        i++;
    }
    return rsl;
}

bool Monomio::caracterDemas(string x) {
    int i = 0; // contador de posición
    int cX = 0; // contador de x o X
    int cY = 0; // contador de y o Y
    int cPI = 0; // contador de paréntesis izquierdos
    int cPD = 0; // contador de paréntesis derechos
    int cP = 0; // contador de puntos
    int cS = 0; // contador de signos
    int xLng = x.length();
    while ((i < xLng) && !(cX > 1) && !(cY > 1) && !(cPI > 2) && !(cPD > 2) && !(cP > 1) && !(cS > 1)) {
        switch (x[i]) {
            case 'x': cX++;
                break;
            case 'X': cX++;
                break;
            case 'y': cY++;
                break;
            case 'Y': cY++;
                break;
            case '(': cPI++;
                break;
            case ')': cPD++;
                break;
            case '.': cP++;
                break;
            case '+': cS++;
                break;
            case '-': cS++;
                break;
        };
        i++;
    }
    return (cX > 1) || (cY > 1) || (cPI > 2) || (cPD > 2) || (cP > 1) || (cS > 1) || (cPI != cPD);
}