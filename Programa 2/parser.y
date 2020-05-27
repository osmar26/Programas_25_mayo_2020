/*Fecha: 23/mayo/2020*/
/*Autores: Ana Belem Méndez Cabrera*/
/*Descripción: Archivo que genera nuestro analizador léxico,*/
/*             ambigüedad,recursividad,factorización*/
/*             terminales encontrados en nuestra gramática.*/

%{
	#include <stdio.h>
    #include <string.h>
    void yyerror(char *s);
    extern int yylex();
%}

%union{
    /*aqui se guardan las variables que se usaran como atributos*/
    /**/
    char exp[50];
}

/*Los símbolos terminales de la gramática PARA Bison SON tokens y deben declararse en la sección de definiciones.*/ 
%token DEF 
%token ESTRUCT
%token ENTERO REAL DREAL SIN CAR
%token CARACTER CADENA NUM ID
%token IF THEN
%token DO WHILE SEGUN
%token TRUE FALSE
%token INICIO TERMINAR END
%token CASO PRED
%token WRITE READ
%token DEV
%token COMA DOSP PYC 
%right IGUAL 
%left OR
%left AND
%left IDENTICO DIFERENTE
%left MENORQUE MENORIGUAL MAYORQUE MAYORIGUAL
%left MAS MENOS
%left MUL DIV MODULO
%right NOT
%left PUNTO
%left LCOR RCOR
%left LPAR RPAR
%nonassoc SITEMP
%nonassoc SINO

/*No terminales van declarados en minuscula*/
%type<exp> programa declaraciones tipo_registro tipo base tipo_arreglo lista_var funciones argumentos lista_arg arg tipo_arg param_arr sentencias sentencia casos predeterminado e_bool relacional expresion variable variable_comp dato_est_sim arreglo parametros lista_param
%start programa

%%
programa:         declaraciones funciones ;

declaraciones:    tipo lista_var PYC declaraciones {}
                | tipo_registro lista_var PYC declaraciones {}
                | {};

tipo_registro:    ESTRUCT INICIO declaraciones END {};

tipo:             base tipo_arreglo {};

base:             ENTERO{} | REAL{} | DREAL{} | CAR{} | SIN {};

tipo_arreglo:     LPAR NUM RPAR tipo_arreglo{} | {};

lista_var:        lista_var COMA ID{} | ID{} ;

funciones:        DEF tipo ID LPAR argumentos RPAR INICIO declaraciones sentencias END funciones {}| {};
                    
argumentos:       lista_arg{} | SIN{} ;

lista_arg:        lista_arg COMA arg | arg ;

arg:              tipo_arg ID ;

tipo_arg:         base param_arr ;

param_arr:        LPAR RPAR param_arr{} | {};

sentencias:       sentencias sentencia {} | sentencia {};

sentencia:        IF e_bool THEN sentencia %prec SITEMP END{}                                                               
                  | IF e_bool THEN sentencia SINO sentencia SINO sentencia END{}
                  | WHILE e_bool DO sentencia END{}
                  | DO sentencia WHILE e_bool PYC {}
                  | SEGUN LPAR variable RPAR DO casos predeterminado END{}
                  | variable IGUAL expresion PYC{}
                  | WRITE expresion PYC {}
                  | READ variable PYC{}
                  | DEV PYC{}
                  | DEV expresion PYC {}
                  | TERMINAR PYC {}
                  | INICIO sentencias END {};

casos:            CASO NUM DOSP sentencia casos{} | CASO NUM DOSP sentencia{};

predeterminado:   PRED DOSP sentencia{} | {};

e_bool:           e_bool OR e_bool
                | e_bool AND e_bool
                | NOT e_bool{}
                | relacional
                | TRUE{}
                | FALSE{} ;

relacional:       relacional MENORQUE relacional
                | relacional MAYORQUE relacional
                | relacional MENORIGUAL relacional
                | relacional MAYORIGUAL relacional
                | relacional IDENTICO relacional 
                | relacional DIFERENTE relacional
                | expresion ;

expresion:        expresion MAS expresion {}
                | expresion MENOS expresion{}
                | expresion MUL expresion {}
                | expresion DIV expresion {}
                | expresion MODULO expresion {}
                | LPAR expresion RPAR {}
                | variable{}
                | NUM{}
                | CADENA{}
                | CARACTER {};
                

variable:         ID variable_comp {};

variable_comp:    dato_est_sim{} | arreglo{} | LPAR parametros RPAR{} ;

dato_est_sim:     dato_est_sim PUNTO ID{} | {};

arreglo:           LPAR expresion RPAR{}
                 | arreglo LPAR expresion RPAR {};

parametros:        lista_param{} | {};

lista_param:       lista_param COMA expresion 
                  | expresion ;
                
%%
void yyerror(char *s){
    printf("%s\n", s);
}