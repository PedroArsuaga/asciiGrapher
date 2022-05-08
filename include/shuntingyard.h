//Algoritmo para convertir de infix a notación polaca inversa

#ifndef _SHUNTINGYARD_H
#define _SHUNTINGYARD_H

#include <string>
#include <deque>
#include <vector>
#include "tokens.h"

class node{
    public:
        token data;
        node *left; node *right;
        node(){
            left = right = NULL;
        };
        node(token info){
            data = info;
            left = right = NULL;
        };
        float eval(float t);//Devuelve el resultado de evaluar el árbol, usando t como variable

};

class exprTree{
    public:
        node * root;
        exprTree(){root = NULL;};
        exprTree(std::deque<token> postfix);
        void printTree(node &n);//Imprime el árbol en orden
        //float eval(node &n,float t);//Devuelve el resultado de evaluar el árbol, usando t como variable
};



//Devuelve la precedencia del operador ingresado
// + = - > * > / > f(x)
int precedence(std::string op);

//Devuelve el resultado de realizar la operación op con x e y. En el resultado x va primero. 
float evalBin(float x, float y,char op);

//Devuelve el resultado de realizar la operación op con x.
float evalUn(float x,char op);

//Evalúa si una expresión matemática está balanceada
bool isBalanced(std::string exp);



//Algoritmo
std::deque<token> ShuntingYard(std::vector<token> expr);

//Evaluar expresión
float evalSY(std::deque<token> expr);



#endif