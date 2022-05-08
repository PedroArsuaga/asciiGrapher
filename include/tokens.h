#ifndef _TOKENS_H
#define _TOKENS_H

#include <string>
#include <vector>


class token{
    public:
        std::string st;
        enum Type {Operator,Number,LeftPar,RightPar,Function,Variable};
        Type type;
        int precedence;
        token(){st = "aaaa";};
        token(Type t, std::string s, int prec = -1,bool b = false,bool r = false ) : type {t}, st {s}, precedence {prec}, binary {b} , isRightAssociative {r} {};
        bool binary;
        bool isRightAssociative; 
        token(const token&old){
            st = old.st;
            type = old.type;
            precedence = old.precedence;
            binary = old.binary;
            isRightAssociative = old.isRightAssociative;
        }

};

//Convierte un string en un vector de tokens
std::vector<token> strToToken(std::string expr);




#endif