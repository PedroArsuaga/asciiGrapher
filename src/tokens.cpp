#include <stdio.h>
#include <iostream>
#include "../include/tokens.h"

using namespace std;


//Convierte un string en un vector de tokens
std::vector<token> strToToken(std::string expr){
    std::vector<token> tokens;
    token::Type t;
    for(int i = 0; i < expr.length(); i++){
        if(expr.at(i) != ' '){
            if( isdigit( expr.at(i) ) ){ //NÚMERO
                if( ((i > 0) && !isdigit(expr.at(i-1))) || (i == 0) ){
                    t = token::Type::Number;
                    string num;
                    int j = i;
                    while( (j < expr.length()) && isdigit(expr.at(j))){
                        num.push_back(expr.at(j));
                        j++;
                    };
                    tokens.push_back(token {t,num,-1,false});
                };
            }else if(((i > 0) && isalpha(expr.at(i))  && !isalpha(expr.at(i-1))) || (  (i == 0) && isalpha(expr.at(i)) )){ //LETRAS
                string func;
                int j = i;
                while( (j < expr.length()) && isalpha(expr.at(j))){
                    func.push_back(expr.at(j));
                    j++;
                };
                if(func.length() > 1){
                    t = token::Type::Function;
                }else{
                    t = token::Type::Variable;
                };
                tokens.push_back(token {t,func,-1,false});
            }else{//OPERADOR
                int pr = -1; //precedencia
                bool b = true; //es binario?
                bool r = true;
                switch(expr.at(i)){
                    case '(':   t = token::Type::LeftPar; break;
                    case ')':   t = token::Type::RightPar; break;
                    case '+':   t = token::Type::Operator; pr = 1; break;
                    case '-':   t = token::Type::Operator; pr = 1; break;
                    case '*':   t = token::Type::Operator; pr = 2; break;
                    case '/':   t = token::Type::Operator; pr = 2; break;
                    case '^':   t = token::Type::Operator; pr = 3; r = true; break;
                };
                if(!isalpha(expr.at(i))) tokens.push_back(token {t,string(1,expr.at(i)),pr,b});
            };
        };
    };//for
    return tokens;
};

