#include "../include/symtable.h"
#include <string.h>

SymTable symTableBuilder(Program* parseTree)
{
	SymTable symtable ;
	
	tableInitializer( &symtable, 1 ) ;
	
	Declarations* declarations = parseTree->decls ;
	while( declarations )
	{
		tableAdder( declarations->decl, &symtable ) ;
		declarations = declarations->decls ;
	}	
	
	return symtable ;
}


void tableInitializer(SymTable* symtable, int capacity)
{
	symtable->IDs = (Declaration*)malloc( sizeof(Declaration)*capacity ) ;
	symtable->capacity = capacity ;
	symtable->size = 0 ;
}


void tableAdder(Declaration decl, SymTable* symtable)
{	
	/* Check if duplicated */
	for(int i=0; i<symtable->size; i++)
	{
		if( strcmp(symtable->IDs[i].identifier, decl.identifier) == 0 )
		{	
			char errMesg[BUFLEN] ;
			sprintf( errMesg, "error: redeclaration of %s", decl.identifier ) ;
			err_mesg( errMesg, decl.line ) ;
		}	
	}	
	
	if( symtable->size == symtable->capacity )
	{
		symtable->capacity <<= 1 ;
		symtable->IDs = (Declaration*)realloc( symtable->IDs, sizeof(Declaration)*symtable->capacity ) ;
	}	
	
	symtable->IDs[symtable->size++] = decl ;	
}


void typeChecker(Program* parseTree, SymTable* symTable)
{
	Statements* statements = parseTree->stmts ;
	
	while( statements )
	{
		switch( statements->stmt.type )
		{
			case Assignment:
				assignChecker( statements->stmt, symTable ) ;	
				break ;
				
			case Print:
				identifierFinder( statements->stmt.stmt.identifier, symTable, statements->stmt.line ) ;
				break ;
				
			default:
				fprintf(stderr, "invalid statement\n") ;
				exit(0) ;
		} ;
		
		statements = statements->stmts ;
	}	
}



void assignChecker(Statement stmt, SymTable* symTable)
{
	expressionChecker( stmt.stmt.assign.expr, symTable, stmt.line ) ;
	DataType lhsType = identifierFinder( stmt.stmt.assign.identifier, symTable, stmt.line ) ;
	DataType rhsType = stmt.stmt.assign.expr->type ;
		
	if( lhsType == floatType && rhsType == intType )
		typeConverter( stmt.stmt.assign.expr, floatType ) ;
	
	else if( lhsType == intType && rhsType == floatType )
		err_mesg("error: cannot convert float to int", stmt.line) ;
}


void typeConverter(Expression* expr, DataType type)
{
	if( expr->type == floatType && type == intType )
	{
		fprintf(stderr, "invalid conversion\n") ;
		exit(0) ;
	}	
	else if( expr->type == intType && type == floatType )
	{
		Expression* oldExpr = (Expression*)malloc( sizeof(Expression) ) ;
		oldExpr->left = expr->left ;
		oldExpr->right = expr->right ;
		oldExpr->value = expr->value ;
		oldExpr->type = floatType ;
		
		expr->value.type = int2float ;
		expr->value.valueNode.operation = CONVERT ;
		expr->type = intType ;
		expr->left = oldExpr ;
		expr->right = NULL ;
	}	
}


void expressionChecker(Expression* expr, SymTable* symTable, int line)
{
	if( expr->left == NULL && expr->right == NULL )
	{
		switch( expr->value.type )
		{
			case idNode:
				expr->type = identifierFinder( expr->value.valueNode.identifier, symTable, line ) ;
				break ;
				
			case intNode:
				expr->type = intType ;
				break ;
				
			case floatNode:
				expr->type = floatType ;
				break ;
			
			default:	
				fprintf(stderr, "error: invalid type\n") ;
				exit(0) ;	
		} ;
	}
	
	else
	{
		expressionChecker( expr->left, symTable, line ) ;
		expressionChecker( expr->right, symTable, line ) ;
		DataType type = generailizer( expr->left, expr->right ) ;
		typeConverter( expr->left, type ) ;
		typeConverter( expr->right, type ) ;
	
		expr->type = type ;
	}	
}


DataType generailizer(Expression* left, Expression* right)
{
	if( left->type == intType && right->type == intType )
		return intType ;
	return floatType ;
}


DataType identifierFinder(char ID[VARLEN], SymTable* symTable, int line)
{
	for(int i=0; i<symTable->size; i++)
	{
		if( strcmp(ID, symTable->IDs[i].identifier) == 0 )
			return symTable->IDs[i].type ;
	}	
	
	char errMesg[BUFLEN] ;
	sprintf( errMesg, "error: %s undeclared", ID ) ;
	err_mesg( errMesg, line ) ;
}

