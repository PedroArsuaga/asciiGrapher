#include <stdio.h>
#include <iostream>
#include "../include/curses.h"
#include <cmath>
#include <stdlib.h>
#include <string>
#include "../include/shuntingyard.h"


#define ANCHO 160
#define ALTURA 80
#define LARGO_STRING 30

#define GREEN 0
#define RED 1
#define WHITE 2

using namespace std;

void configTerminal(){ //Config. inicial
    initscr();
    resize_term(ALTURA,ANCHO);
    refresh();
    
    keypad(stdscr,true);
    refresh();
};
void drawAxis(WINDOW *win){//Ejes
    wclear(win);
    mvwhline(win,getmaxy(win) / 2 ,0,'-',getmaxx(win));
    mvwvline(win,0,getmaxx(win) / 2,'|',getmaxy(win));
    wrefresh(win);
};
void drawMenu(WINDOW *win){//Menú principal
    mvwaddstr(win,2,getmaxx(win)/4,"CURVAS PARAMÉTRICAS \n 1.Dibujar curva \n 2.Elegir color \n 3. Salir");
};
void drawCurve(WINDOW *canvas, WINDOW *info, char *coordx, char *coordy,int color,int t0, int tf,int scale){
    string x(coordx);
    string y(coordy);
    int width = getmaxx(info);
    int canvasW = getmaxx(canvas);
    int canvasH = getmaxy(canvas);
    mvwaddstr(info,16,width/4,"Evaluando parentesis... ");
    if( isBalanced(x) && isBalanced(y)){
        attron(COLOR_PAIR(GREEN)); mvwaddstr(info,16,(width/4) + 25, "OK"); attroff(COLOR_PAIR(GREEN));
        exprTree X(ShuntingYard(strToToken(coordx)));
        exprTree Y(ShuntingYard(strToToken(coordy)));
        attron(COLOR_PAIR(color));
        mvwaddstr(info,17,width/4,"Dibujando... ");
        for(int t = t0; t <= tf; t++){
            mvwaddch(canvas,  round(-(Y.root -> eval(t/100))*scale + (canvasH/2))  , round( (X.root -> eval(t/100))*scale + (canvasW/2))  ,'#');
        };//for
        attroff(COLOR_PAIR(color));
        attron(COLOR_PAIR(GREEN)); mvwaddstr(info,17,(width/4) + 25, "OK"); attroff(COLOR_PAIR(GREEN));
        wrefresh(canvas);
    }else{
        attron(COLOR_PAIR(RED)); mvwaddstr(info,16,(width/4) + 25, "ERROR"); attroff(COLOR_PAIR(RED));
    }//if else
};
void drawLine(WINDOW *canvas, int x0, int y0, int xf, int yf){
    int x,y; float t;
    int canvasW = getmaxx(canvas);
    int canvasH = getmaxy(canvas);
    for(int i = 0; i <= 10; i++){
        t = double(i)/10;
        x = round(xf*t + x0*(1 - t));
        y = round(yf*t + y0*(1 - t));
        mvwaddch(canvas,-y + (canvasH / 2), x + (canvasW / 2),'#');
    };
    wrefresh(canvas);
};


char * coordx,  *coordy;
int menuWidth;
bool quit = false;
int colorCurva = WHITE;

int main(){

    init_pair(GREEN,COLOR_GREEN,COLOR_BLACK);
    init_pair(RED,COLOR_RED,COLOR_BLACK);
    init_pair(WHITE,COLOR_WHITE,COLOR_BLACK);
    configTerminal();
    //GRÁFICO
    WINDOW *graph = newwin(ALTURA-20,ANCHO/2,5,10);
    drawAxis(graph);
    box(graph,0,0);
    wrefresh(graph);
    //ENTRADA
    WINDOW *input = newwin(ALTURA-20,40,5,ANCHO - 50);
    wrefresh(input);
    menuWidth = getmaxx(input);

    coordx = new char[LARGO_STRING];
    coordy = new char[LARGO_STRING];

   


    //LOOP PRINCIPAL
    while(!quit){
        drawMenu(input);
        box(input,0,0);
        wrefresh(input);
        noecho();
        curs_set(0);
        char c = mvwgetch(input,5,getmaxx(input)/2);
        switch(c){
            case '1'://DIBUJAR 
                echo(); curs_set(1);
                mvwaddstr(input,6,menuWidth/4,"Ingresar coordenada horizontal:"); box(input,0,0);  
                mvwgetstr(input,7,menuWidth/4,coordx);
                mvwaddstr(input,8,menuWidth/4,"Ingresar coordenada vertical:"); box(input,0,0);
                mvwgetstr(input,9,menuWidth/4,coordy);
                mvwaddstr(input,12,menuWidth,"Curva ingresada:");
                mvwaddstr(input,13,menuWidth/4,"x(t) ="); mvwaddstr(input,13, (menuWidth/4) + 7, coordx);
                mvwaddstr(input,14,menuWidth/4,"y(t) ="); mvwaddstr(input,14, (menuWidth/4) + 7, coordy);
                box(input,0,0);
                drawCurve(graph,input,coordx,coordy,WHITE,0,200*314,1);
                noecho(); curs_set(0);
                break;
            case '2'://COLORES
                break;
            case '3'://SALIR
                quit = true;
                break;
        };//switch
    };//while
    







    getchar();
    endwin();
    cout << "Finalizado" << endl;

    return 0;
};
