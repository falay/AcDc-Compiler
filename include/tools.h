#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include "scanner.h"

typedef struct Expression Expression ;


typedef struct vector
{
	Token* data ;
	int size ;
	int capacity ;
	
} vector ;


typedef struct exprStack
{
	Expression** node ;
	int size ;
	int capacity ;
	
} exprStack ;


/* token vector */
void vectorInitializer(vector*, int) ;
void vectorAppender(vector*, Token) ;
void vectorReleaser(vector*) ;

/* token stack */
void stackPusher(vector*, Token) ;
void stackPopper(vector*) ;
int isEmpty(vector*) ;
Token top(vector*) ;

/* expression tree stack */
void exprStackInitializer(exprStack*, int) ;
void exprStackPusher(exprStack*, Expression*) ;
Expression* exprStackPopper(exprStack*) ;
void exprStackReleaser(exprStack*) ;



#endif