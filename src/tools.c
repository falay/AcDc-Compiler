#include "../include/tools.h"


void vectorInitializer(vector* symVec, int capacity)
{
	symVec->data = (Token*)malloc( sizeof(Token)*capacity ) ;
	symVec->size = 0 ;
	symVec->capacity = capacity ;
}


void vectorAppender(vector* symVec, Token token)
{
	if( symVec->size == symVec->capacity )
	{	
		symVec->capacity <<= 1 ;
		symVec->data = (Token*)realloc( symVec->data, sizeof(Token)*symVec->capacity ) ;
	}
	
	symVec->data[symVec->size++] = token ;
}


void vectorReleaser(vector* symVec)
{
	free( symVec->data ) ;
	symVec->data = NULL ;
	symVec->size = symVec->capacity = 0 ;
}


void stackPusher(vector* stack, Token token) 
{
	vectorAppender( stack, token ) ;
}


void stackPopper(vector* stack) 
{
	if( isEmpty(stack) )
		fprintf(stderr, "error: pop from an empty stack\n") ;
	else
		stack->size -- ;	
}


int isEmpty(vector* stack) 
{
	return stack->size == 0 ;
}


Token top(vector* stack)
{
	return stack->data[stack->size-1] ;
}

void exprStackInitializer(exprStack* stack, int capacity) 
{
	stack->node = (Expression**)malloc( sizeof(Expression*)*capacity ) ;
	stack->size = 0 ;
	stack->capacity = capacity ;	 
}


void exprStackPusher(exprStack* stack, Expression* expr) 
{
	if( stack->size == stack->capacity )
	{
		stack->capacity <<= 1 ;
		stack->node = (Expression**)realloc( stack->node, sizeof(Expression*)*stack->capacity ) ;
	}	
	
	stack->node[stack->size++] = expr ;
}

Expression* exprStackPopper(exprStack* stack) 
{
	if( stack->size == 0 )
	{
		fprintf(stderr, "pop from an empty stack\n") ;
		exit(0) ;
	}	
	
	return stack->node[--stack->size] ;
}


void exprStackReleaser(exprStack* stack)
{
	free( stack->node ) ;
	stack->size = stack->capacity = 0 ;
} 





