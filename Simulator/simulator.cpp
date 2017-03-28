#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <cmath> 
using namespace std ;

#define REGLEN 26


/* This is a dc assembly code simulator */
/* Registers: a~z 26 registers */

bool isInteger(double num)
{
	return num == ceil(num) ;
}

void Simulator(string fileName)
{
	ifstream infile( fileName );
	string line ;
	
	stack<double> stackMachine ;
	double registers[REGLEN] ;
	
	while( getline(infile, line) )
	{		
		if( line == "0 k" || line == "5 k" )
			continue ;
		
		else if( isdigit(line[0]) )
			stackMachine.push( stof( line ) ) ;	
		
		else if( line[0] == 's' )
		{
			registers[ line[1]-'a' ] = stackMachine.top() ;
			stackMachine.pop() ;
		}	
		
		else if( line[0] == 'l' )
			stackMachine.push( registers[ line[1]-'a' ] ) ;
			
		else if( line[0] == 'p' )
		{	
			double printNum = stackMachine.top() ;
	
			if( isInteger(printNum) )
				cout << (int)printNum << endl ;
			else
				cout << printNum << endl ;
		}
		
		else // operation
		{
			double second = stackMachine.top() ;
			stackMachine.pop() ;
			double first = stackMachine.top() ;
			stackMachine.pop() ;			
			double result ;
			
			switch( line[0] )
			{
				case '+':
					result = first + second ;
					break ;
				
				case '-':
					result = first - second ;
					break ;
					
				case '*':
					result = first * second ;
					break ;
					
				case '/':
					result = first / second ;
					break ;
				
				default:
					cerr << "error code gen\n" ;
					exit(0) ;
			} ;

			stackMachine.push( result ) ;	
		}	
	}	
	
}


int main(int argc, char* argv[])
{
	if( argc == 2 )
		Simulator( argv[1] ) ;
	else
		fprintf(stderr, "Usage: %s [dc target file]\n", argv[0]) ;		
}