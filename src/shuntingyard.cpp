#include <stdio.h>
#include <iostream>
#include <string>
#include <stack>
#include <deque>
#include <vector>
#include <cmath> //pow
#include <cctype> //isdigit
#include <cassert>
#include "../include/shuntingyard.h"
#include "../include/tokens.h"

using namespace std;

//Devuelve la precedencia del operador ingresado
// + = - > * > / > f(x)
int precedence(char op){
    switch(op){
        case '^':
            return 4;
            break;
        case '*':
            return 3;
            break;
        case '/':
            return 3;
            break;
        case '-':
            return 1;
            break;
        case '+':
            return 1;
    };
    return 0;
};

//Devuelve el resultado de realizar la operación op con x e y. En el resultado x va primero. 
float evalBin(float x, float y,char op){
    float res;

    switch(op){
        case '+': res = x+y; break;
        case '-': res = x-y; break;
        case '/': res = x/y; break;
        case '*': res = x*y; break;
        case '^': res = pow(x,y); break;
    };
    return res;
};
//Devuelve f(x);
float funcEval(float x, string f){
    float res;
    if(f == "cos"){
        res = cos(x);
    }else if(f == "sin"){
        res = sin(x);
    }else if(f == "tan"){
        res = tan(x);
    }else if(f == "exp"){
        res = exp(x);
    }else if(f == "log"){
        res = log(x);
    }else{
        res = sqrt(x);
    };
    return res;
};


//Evalúa si una expresión matemática está balanceada
bool isBalanced(string expr){
    stack<char> st;
    char x;
    for(int i = 0; i< expr.length();i++){
        if((expr.at(i) == '(') || (expr.at(i) == '[') || (expr.at(i) == '{')){  //si es un paréntesis izq. entonces se pone en el stack
            st.push(expr.at(i));
            continue;
        };
        switch(expr.at(i)){     //revisar paréntesis derecho
            case ')':
                x = st.top();
                st.pop();    
                if(x != '(') return false;
                break;
            case ']':
                x = st.top();
                st.pop(); 
                if(x != '[') return false;
                break;

            case '}':
                x = st.top();
                st.pop(); 
                if(x != '{') return false;
                break;;
        };  
    };
    return st.empty();  //
};


//Algoritmo
deque<token> ShuntingYard(vector<token> expr){
   deque<token> q;
   stack<token> s;

   for(int i = 0; i < expr.size(); i++){
       switch(expr.at(i).type){
           case token::Type::Number:    //NÚMERO
                q.push_back(expr.at(i));
                break;
            case token::Type::Operator: //OPERADOR
                if(!expr.at(i).isRightAssociative){ //+,-,*,/
                    while( !s.empty() && (expr.at(i).precedence <= s.top().precedence) ){
                        q.push_back(s.top());
                        s.pop();
                    };
                    s.push(expr.at(i));
                }else{ //^
                    while( !s.empty() && expr.at(i).precedence < s.top().precedence){
                        q.push_back(s.top());
                        s.pop();
                    };
                    s.push(expr.at(i));
                };
                break;
            case token::Type::LeftPar: //PARÉNTESIS IZQ
                s.push(expr.at(i));
                break;
            case token::Type::RightPar: //PARÉNTESIS DER
                while( !s.empty() && (s.top().type != token::Type::LeftPar)){
                    q.push_back(s.top());
                    s.pop();
                };
                s.pop();
                if(!s.empty() && (s.top().type == token::Type::Function)){
                    q.push_back(s.top());
                    s.pop();
                };
                break;
            case token::Type::Function: //FUNCIÓN
                s.push(expr.at(i));
                break;
            case token::Type::Variable: //VARIABLE
                q.push_back(expr.at(i));
                break;
            default:
                break;
       };
   };
   while(!s.empty()){
       q.push_back(s.top());
       s.pop();
   };
   return q;
    
};



//Constructor del árbol
exprTree::exprTree(deque<token> postfix){
    stack<node *> s;
    node * newNode;
    for(int i = 0; i < postfix.size(); i++){
        token current = postfix.at(i);
        switch(current.type){
            case token::Type::Variable:
            case token::Type::Number:
                newNode = new node(current);
                break;
            case token::Type::Operator: //ab- = a - b
                newNode = new node(current);
                assert(!s.empty());
                newNode -> right = s.top();
                s.pop();
                assert(!s.empty());
                newNode -> left = s.top();
                s.pop();
                break;
            case token::Type::Function:
                assert(!s.empty());
                newNode = new node(current);
                newNode -> left = s.top();
                s.pop();
                break;
        }; //switch
        s.push(newNode);
    };//for
    root = s.top();
};
//Imprimir árbol en orden
void exprTree::printTree(node &n){
    if(n.left != NULL) printTree(*n.left);
    cout << n.data.st << " ";
    if(n.right != NULL) printTree(*n.right);
};
//Evaluación del árbol
float node::eval(float t){
    float res;
    token r = data;
    switch(r.type){
        case token::Type::Variable:
            res = t;
            break;
        case token::Type::Number:
            res = stof(r.st);
            break;
        case token::Type::Operator: 
            if(r.binary){
                //cout << "AAA";
                float a = left -> eval(t);
                //cout << "BBB";
                float b = right ->  eval(t);
                res = evalBin(a,b,r.st[0]);
            };
            break;
        case token::Type::Function:
        //cout << "CCC";
            res = funcEval(left -> eval(t),r.st);
            break;
    };//switch
    return res;
};