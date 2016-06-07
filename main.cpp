

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string.h>


#include "Evaluador.h"
#include "Mapeo.h"
#include "Monomio.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    //Excepciones de polinomio
    
    cout <<"-----------Excepciones de polinomio------------" << endl;
    //Hilera invalida    
    try{
        Polinomio polimal1("3.5x(4)y(3)+4.1x(2)y(1)+5.2x(0)y(0)?");
        cout << polimal1.aHil() << endl;
    }catch (ExcPolinomioInvalido e){
        cout << e.what() << endl;
    }
    
    //Caso 0(0)
    double x = 0;
    double y = 1.5;
    Polinomio polimal2("1.5x(3)y(2)-3x(0)y(1)");
    try{
        double result = polimal2.eval(x,y);
        cout << result << endl;
    }catch(ExcPolinomioInvalido e){
        cout << e.what() << endl;
    }
    
    //Division entre cero
    Polinomio polimal3;
    try{
        Polinomio rsl(polimal2/polimal3);
    }catch(ExcPolinomioInvalido e){
        cout << e.what() << endl;
    }
    
     

    //Excepciones de mapeo
    
    cout << "\n" << endl;
    cout <<"-----------Excepciones de mapeo------------" << endl;
    Mapeo mappba;
    
    //guardar un polinomio con un nombre incorrecto
    try{
        Polinomio poli1("3x(5)y(2)+5.5x(3)y(2)-4.3x(2)y-2xy");
        mappba.asgObjeto("op1",poli1);               
    }catch(ExcMapeoInvalido e){
        cout << e.what() << endl;
    }
    
    //buscar un nombre que no existe
    try{
        cout << mappba.obtObjeto("p2").aHil() << endl;
    }catch(ExcMapeoInvalido e){
        cout << e.what() << endl;
    }
    
    


    //se lee el archivo que contiene las operaciones y las realiza
    cout << "\n" << endl;
    cout <<"-----------Lectura del archivo de operaciones------------" << endl;
    Mapeo map;
    Evaluador eva(map);
    string nomArch = "datos.txt";
    string expmono = "";
    string nombre;
    string poli;
    int cont = 1;
    
    
    ifstream archivoLineasEntrada(nomArch.c_str(), ios::in);
    if (!archivoLineasEntrada) { // operador ! sobrecargado
        cerr << "No se pudo abrir el archivo de entrada" << endl;
        exit(1);
    }
    getline(archivoLineasEntrada, expmono); // se lee la primera línea
    while (expmono != "") {
        
        try{
            nombre = expmono.substr(0, expmono.find("="));
            poli = expmono.substr(expmono.find("=") + 1);        
        }catch (ExcPolinomioInvalido e){
            cout << e.what() << endl;
        }
        
        try{        
            map.asgObjeto(nombre,poli);    
            cout << "El polinomio " << nombre << " es: " << map.obtObjeto(nombre).aHil() << endl;
        }catch(ExcMapeoInvalido e){
            cout << e.what() << endl;
        }
        getline(archivoLineasEntrada, expmono);
    }

    getline(archivoLineasEntrada, expmono);
    
    while (!archivoLineasEntrada.eof()) {
        if(expmono != ""){
            Polinomio res(eva.evaluar(expmono));
            cout << "El resultado de " << expmono << " es: " << endl;
            cout << res.aHil() << endl;
        }
        getline(archivoLineasEntrada, expmono); // se lee la siguiente línea
    }

    return 0;
}

