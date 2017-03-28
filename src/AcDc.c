#include <stdlib.h>
#include "../include/symtable.h"
#include "../include/generator.h"

#define TARGET "target.dc"




void AcDcCompiler(FILE* source, FILE* target)
{	
	/* Scanning & Parsing */
	Program parseTree = Parser( source ) ;
	
	/* Symbol table building */
	SymTable symTable = symTableBuilder( &parseTree ) ;
	
	/* Semantic analysis: type checking */
	typeChecker( &parseTree, &symTable ) ;
	
	/* Code generation */
	codeGenerator( target, &parseTree, &symTable ) ;
	
	fclose( source ) ;
	fclose( target ) ;	
}



int main(int argc, char* argv[])
{
	FILE *source, *target ;
	
	if( argc == 2 || argc == 3 )
	{	
		if( (source = fopen( argv[1], "r" )) == NULL ) 
		{
			fprintf(stderr, "AcDc: error: %s: No such file or directory\n", argv[1]) ;
			exit(0) ;
		}
		
		if( argc == 2 )	
			target = fopen( TARGET, "w" ) ;
		else
			target = fopen( argv[2], "w" ) ;
	}	
	else 
	{	
		fprintf(stderr, "Usage: %s [source] [target]\n", argv[0]) ;	
		exit(0) ;
	}	
	
	AcDcCompiler( source, target ) ;
	
	return 0 ;
}