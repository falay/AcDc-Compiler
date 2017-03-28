#include "../include/scanner.h"
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>


int curLine = 1 ;

void err_mesg(char* mesg, int line)
{
	fprintf( stderr, "Line %d: %s\n", line, mesg ) ;
	exit(0) ;
}


Token alphaGetter(FILE* source, char alpha)
{
	int index = 0 ;
	Token token ;
	while( isalpha( alpha ) )
	{
		token.data[index++] = alpha ;
		alpha = fgetc( source ) ;
	}	
		
	ungetc( alpha, source ) ;
	token.data[index] = '\0' ;
	token.line = curLine ;
	token.size = index ;
	
	/* declaration or variable */	
	if( index == 1 )
	{
		switch( token.data[0] )
		{
			case 'i':
				token.type = intDecl ;
				return token ;
				
			case 'f':
				token.type = floatDecl ;
				return token ;
				
			case 'p':
				token.type = printOP ;
				return token ;
				
			default:
				token.type = variable ;
				return token ;
		} ;
	}	
	
	/* variable */
	else
	{
		token.type = variable ;
		return token ;
	}	
}



Token digitGetter(FILE* source, char num)
{
	int index = 0 ;
	Token token ;
	while( isdigit( num ) )
	{	
		token.data[index++] = num ;
		num = fgetc( source ) ;
	}	
	
	/* Integer */
	if( num != '.' )
	{
		ungetc( num, source ) ;
		
		token.data[index] = '\0' ;
		token.size = index ;
		token.type = intConst ;
		token.line = curLine ;
		
		return token ;
	}	
	
	/* Floating number */
	token.data[index++] = '.' ;
	num = fgetc( source ) ;
	
	if( !isdigit( num ) )
		err_mesg("syntax error: invalid digit", curLine) ;

	while( isdigit( num ) )
	{
		token.data[index++] = num ;
		num = fgetc( source ) ;
	}	
	
	ungetc( num, source ) ;
	token.data[index] = '\0' ;
	token.size = index ;
	token.line = curLine ;
	token.type = floatConst ;
	
	return token ;
}


Token Scanner(FILE* source)
{
	Token token ;
	
	while( !feof( source ) )
	{
		char Char = fgetc( source ) ;
		
		if( isspace( Char ) )
		{	
			if( Char == '\n' ) curLine ++ ;
			continue ;
		}
		
		if( isdigit( Char ) )
			return digitGetter( source, Char ) ;
		
		if( isalpha( Char ) )
			return alphaGetter( source, Char ) ;
		
		token.data[0] = Char ;
		token.data[1] = '\0' ;
		token.size = 1 ;
		token.line = curLine ;
		switch( Char )
		{
			case '+':
				token.type = plusOP ;
				return token ;
			
			case '-':
				token.type = minusOP ;
				return token ;
				
			case '*':
				token.type = mulOP ;
				return token ;
				
			case '/':
				token.type = divOP ;
				return token ;
				
			case '=':
				token.type = assignmentOP ;
				return token ;
				
			case EOF:
				token.type = EOFsymbol ;
				token.data[0] = '\0' ;
				token.size = 0 ;
				return token ;
				
			default:
				err_mesg("syntax error: invalid token", curLine) ;	
		} ;	
	}
	
	token.type = EOFsymbol ;
	token.data[0] = '\0' ;
	token.size = 0 ;
	
	return token ;	
}


int isOperation(TokenType type)
{
	return type == plusOP || type == minusOP || type == mulOP || type == divOP ;
}


int isSymbol(TokenType type)
{
	return type == variable || type == intConst || type == floatConst ;
}


int percendence(TokenType type)
{
	switch( type )
	{
		case plusOP:
		case minusOP:
			return 0 ;
		
		case mulOP:
		case divOP:
			return 1 ;
			
		default:
			fprintf(stderr, "invalid type") ;
			exit(0) ;
	} ;
} 
