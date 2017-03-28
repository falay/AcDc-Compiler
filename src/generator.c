#include "../include/generator.h"
#include "../include/symtable.h"
#include <string.h>



/* Code generation */
// we have 26 registers, namely, a~z registers.


void codeGenerator(FILE* target, Program* parseTree, SymTable* symtable)
{
	Statements* Stmts = parseTree->stmts ;
		
	regMapper( symtable ) ;
	
	while( Stmts )
	{
		switch( Stmts->stmt.type )
		{
			case Print:
				fprintf( target, "l%c\np\n", regFinder(Stmts->stmt.stmt.identifier) ) ;
				fprintf( target, "s%c\n", popHandler() ) ;
				break ;
				
			case Assignment:
				exprGenerator( target, Stmts->stmt.stmt.assign.expr ) ;
				fprintf( target, "s%c\n0 k\n", regFinder(Stmts->stmt.stmt.assign.identifier) ) ;
				break ;
		} ;
		
		Stmts = Stmts->stmts ;
	}	
}


void regMapper(SymTable* symtable)
{
	if( symtable->size > 26 )
	{
		fprintf(stderr, "error: support at most 26 variables\n") ;
		exit(0) ;
	}	
	
	int i ;
	for(i=0; i<symtable->size; i++)
	{
		varNamer( regList[i].identifier, symtable->IDs[i].identifier ) ;
		regList[i].type = symtable->IDs[i].type ;
	}	
	for(; i<REGLEN; i++)
		regList[i].type = nonType ;
}


char regFinder(char identifier[VARLEN])
{
	for(int i=0; i<REGLEN; i++)
	{		
		if( strcmp(regList[i].identifier, identifier) == 0 )	
			return 'a' + i ;
	}		
}

char popHandler()
{
	for(int i=0; i<REGLEN; i++)
	{
		if( regList[i].type == nonType )
			return 'a' + i ;
 	}	
}


void exprGenerator(FILE* target, Expression* expr)
{
	if( expr->left == NULL && expr->right == NULL )
	{
		switch( expr->value.type )
		{
			case idNode:
				fprintf( target, "l%c\n", regFinder(expr->value.valueNode.identifier) ) ;
				break ;
				
			case intNode:
				fprintf( target, "%d\n", expr->value.valueNode.intValue ) ;
				break ;
			
			case floatNode:
				fprintf( target, "%f\n", expr->value.valueNode.floatValue ) ;
				break ;			
		} ;
		
		return ;
	}		
	
	exprGenerator( target, expr->left ) ;
	
	/* type conversion */
	if( expr->right == NULL )
		fprintf( target, "5 k\n" ) ;
	else
	{
		exprGenerator( target, expr->right ) ;
		switch( expr->value.type )
		{
			case plusNode:
				fprintf( target, "+\n" ) ;
				break ;
				
			case minusNode:
				fprintf( target, "-\n" ) ;
				break ;
				
			case mulNode:
				fprintf( target, "*\n" ) ;
				break ;
				
			case divNode:
				fprintf( target, "/\n" ) ;
				break ;			
		} ;
	}	
}

