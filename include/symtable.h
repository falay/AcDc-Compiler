#ifndef SYMTABLE_H
#define SYMTABLE_H


#include "parser.h"
#define BUFLEN 128

typedef struct SymTable
{
	Declaration* IDs ; 
	int size ;
	int capacity ;
	
} SymTable ;


SymTable symTableBuilder(Program*) ;
void tableAdder(Declaration, SymTable*) ;
void tableInitializer(SymTable*,int) ;

void typeChecker(Program*, SymTable*) ;
DataType identifierFinder(char[VARLEN], SymTable*, int) ;
DataType generailizer(Expression*, Expression*) ;
void expressionChecker(Expression*, SymTable*, int) ;
void typeConverter(Expression*, DataType) ;
void assignChecker(Statement, SymTable*) ;


#endif

