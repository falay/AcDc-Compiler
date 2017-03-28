#ifndef GENERATOR_H
#define GENERATOR_H

#include "parser.h"
#include "symtable.h"

#define REGLEN 26


typedef struct Data
{
	DataType type ;
	char identifier[VARLEN] ;
	union
	{
		int iValue ;
		float fValue ;
		
	} value ;
	
} Data ;


Data regList[REGLEN] ; // One register maps to a variable.


void codeGenerator(FILE*, Program*, SymTable*) ;
void regMapper(SymTable*) ;
char regFinder(char[VARLEN]) ;
char popHandler() ;
void exprGenerator(FILE*, Expression*) ;


#endif