#ifndef PARSER_H
#define PARSER_H

#define VARLEN 256

#include <stdio.h>
#include "scanner.h"
#include "tools.h"

/* defined types */

typedef enum 
{
	intType, floatType, nonType
	
} DataType ;


typedef enum
{
	Print, Assignment 
	
} StmtType ;


typedef enum
{
	idNode, intNode, floatNode, plusNode, minusNode, mulNode, divNode, int2float
	
} ValueType ;


typedef enum
{
	PLUS, MINUS, MUL, DIV, CONVERT
	
} OperationType ;



/* parsing structures */


typedef struct Declaration
{
	DataType type ;
	char identifier[VARLEN] ;
	int line ;
	
} Declaration ;


typedef struct Declarations
{
	struct Declaration decl ;
	struct Declarations* decls ; 
	
} Declarations ;



/* The node in the expression tree */
typedef struct Value
{
	ValueType type ;
	union
	{
		char identifier[VARLEN] ;
		OperationType operation ;
		int intValue ;
		float floatValue ;
		
	} valueNode ;
	
} Value ;


/* The structure of the expression tree */
typedef struct Expression
{
	struct Expression* left ;
	struct Expression* right ;
	Value value ;	
	DataType type ;
	
} Expression ;


typedef struct AssignmentStmt
{
	char identifier[VARLEN] ; /* A for A = expr */
	Expression* expr ;

} AssignmentStmt ;


typedef struct Statement
{
	StmtType type ; /* print or assignment */
	int line ;
	
	union
	{
		char identifier[VARLEN] ; // for print
		AssignmentStmt assign ;   // for assignment
		
	} stmt ;
	
} Statement ;


typedef struct Statements
{
	struct Statement stmt ;
	struct Statements* stmts ;
	
} Statements ;



/* The main parse tree */
typedef struct Program
{
	Declarations* decls ;
	Statements*	  stmts ;
	
} Program ;


/* subroutines */

Program Parser(FILE*) ;
Declarations* declsParser(FILE*) ;
Declaration declParser(FILE*, Token) ;
Declarations* declsNodeMaker(Declaration, Declarations*) ;
Statements* stmtsParser(FILE*) ;
Statements* stmtsNodeMaker(Statement, Statements*) ;
void varNamer(char ID[VARLEN], char data[TOKENLEN]) ;
Statement printParser(FILE*) ;
Statement assignmentParser(FILE*, Token) ;
void ungeter(Token, FILE*) ;
void exprParser(FILE*, vector*) ;
void postOrderConverter(vector*, vector*) ;
Expression* treeGenerator(vector*) ;
int isInteger(float) ;
int isDigit(ValueType) ;

#endif