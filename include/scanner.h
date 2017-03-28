#ifndef SCANNER_H
#define SCANNER_H

#define TOKENLEN 256
#include <stdio.h>


extern int curLine ;

typedef enum
{
	intDecl, floatDecl, assignmentOP, printOP, plusOP, minusOP, mulOP, divOP, variable, intConst, floatConst, EOFsymbol   
	
} TokenType ;


typedef struct Token
{
	char data[TOKENLEN] ;
	int size ;
	TokenType type ;
	
	int line ; // indicate the number of line it appears, for erro reporting
	
} Token ;



void err_mesg(char*,int) ;

Token Scanner(FILE*) ; 
Token digitGetter(FILE*, char) ;
Token alphaGetter(FILE*, char) ;
int isOperation(TokenType) ;
int isSymbol(TokenType) ;
int percendence(TokenType) ;


#endif