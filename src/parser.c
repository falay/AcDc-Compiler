#include "../include/parser.h"
#include <stdlib.h>
#include <math.h>

Program Parser(FILE* source)
{
	Program parseTree ;
	
	parseTree.decls = declsParser( source ) ;
	parseTree.stmts = stmtsParser( source ) ;
	
	return parseTree ;
}

Declarations* declsParser(FILE* source)
{
	Token token = Scanner( source ) ;
	Declarations* decls ;
	Declaration decl ;
	switch( token.type )
	{
		case intDecl :
		case floatDecl :
			decl = declParser( source, token ) ;
			decls = declsParser( source ) ;
			return declsNodeMaker( decl, decls ) ;
		
		case printOP :
		case variable :
			ungeter( token, source ) ;
			return NULL ;
		
		case EOFsymbol :
			return NULL ;
		
		default:
			err_mesg("Line %d: syntax error: Expect a declaration", token.line) ;		
	} ;	
}



Declaration declParser(FILE* source, Token token)
{
	Token varToken = Scanner( source ) ;
	if( varToken.type != variable )
		err_mesg("syntax error: expect a variable name", varToken.line) ;
	
	Declaration declNode ;
	switch( token.type )
	{
		case intDecl:
			declNode.type = intType ;
			break ;
			
		default:
			declNode.type = floatType ;
			break ;
	} ;
	
	varNamer( declNode.identifier, varToken.data ) ;
	declNode.line = token.line ;
	
	return declNode ;
}


Declarations* declsNodeMaker(Declaration decl, Declarations* decls)
{
	Declarations* root = (Declarations*)malloc( sizeof(Declarations) ) ;
	root->decl = decl ;
	root->decls = decls ;
	
	return root ;
}


Statements* stmtsParser(FILE* source)
{
	Token token = Scanner( source ) ;

	Statement singleStmt ;	
	switch( token.type )
	{
		case printOP:
			singleStmt = printParser( source ) ;
			break ;
		
		case variable:
			singleStmt = assignmentParser( source, token ) ;
			break ;
	
		case EOFsymbol :
			return NULL ;
			
		default:
			err_mesg("syntax error: invalid statement", token.line) ;	
	} ;	 
	
	Statements* totalStmts = stmtsParser( source ) ;
	
	return stmtsNodeMaker( singleStmt, totalStmts ) ;
}


Statements* stmtsNodeMaker(Statement stmt, Statements* stmts)
{
	Statements* root = (Statements*)malloc( sizeof(Statements) ) ;
	root->stmt = stmt ;
	root->stmts = stmts ;

	return root ;	
}


void varNamer(char ID[VARLEN], char data[TOKENLEN])
{
	int index ;
	for(index=0; data[index]!='\0'; index++)
		ID[index] = data[index] ;
	
	ID[index] = '\0' ;
}


Statement printParser(FILE* source)
{
	Token token = Scanner( source ) ;
	
	if( token.type != variable )
		err_mesg("syntax error: invalid print statement", token.line) ;		
	
	Statement printStmt ;
	
	varNamer( printStmt.stmt.identifier, token.data ) ;
	printStmt.type = Print ;
	printStmt.line = token.line ;
	
	return printStmt ;
}




void ungeter(Token token, FILE* source)
{
	for(int i=token.size-1; i>=0; i--)
		ungetc( token.data[i], source ) ; 
}


void exprParser(FILE* source, vector* symVec)
{
	int counter = 0 ; /* 0 denotes variable or digit, 1 denotes operation */
	Token nextToken = Scanner( source ) ;
	while( counter == 0 || isOperation(nextToken.type) )
	{
		switch( counter )
		{
			case 0:
			{	
				if( !isSymbol(nextToken.type) )
					err_mesg("syntax error: invalid assignment", nextToken.line) ;
				
				vectorAppender( symVec, nextToken ) ;
				break ;
			}
			default:
			{
				if( !isOperation(nextToken.type) )
					err_mesg("syntax error: invalid assignment", nextToken.line) ;
			
				vectorAppender( symVec, nextToken ) ;
				break ;
			}		
		} ;	
		
		counter = (counter+1) % 2 ;
		nextToken = Scanner( source ) ;
	}	
	
	ungeter( nextToken, source ) ;	
}



Expression* exprTreeGenerator(vector* symVec)
{
	vector postOrder ;
	vectorInitializer( &postOrder, 1 ) ;
	postOrderConverter( symVec, &postOrder ) ;	
	
	return treeGenerator( &postOrder ) ;
}


Expression* exprNodeMaker(Token token)
{
	Expression* node = (Expression*)malloc( sizeof(Expression) ) ;
	node->left = node->right = NULL ;
	
	switch( token.type )
	{
		case variable:
			node->value.type = idNode ;
			varNamer( node->value.valueNode.identifier, token.data ) ;
			break ;

		case intConst:
			node->value.type = intNode ;
			node->value.valueNode.intValue = atoi( token.data ) ;
			break ;
		
		case floatConst:
			node->value.type = floatNode ;
			node->value.valueNode.floatValue = atof( token.data ) ;
			break ;
			
		case plusOP: 
			node->value.type = plusNode ;
			node->value.valueNode.operation = PLUS ;
			break ;
			
		case minusOP: 
			node->value.type = minusNode ;
			node->value.valueNode.operation = MINUS ;
			break ;
			
		case mulOP:
			node->value.type = mulNode ;
			node->value.valueNode.operation = MUL ;
			break ;
			
		case divOP:
			node->value.type = divNode ;
			node->value.valueNode.operation = DIV ;
			break ;
		
		default:
			fprintf(stderr, "invalid type") ;
			exit(0) ;
	} ;
	
	return node ;	
}


Expression* treeGenerator(vector* postOrder)
{
	exprStack stack ;
	exprStackInitializer( &stack, 1 ) ;

	for(int i=0; i<postOrder->size; i++)
	{
		if( isSymbol(postOrder->data[i].type) )
			exprStackPusher( &stack, exprNodeMaker(postOrder->data[i]) ) ;
		else
		{
			Expression* right = exprStackPopper( &stack ) ;
			Expression* left  = exprStackPopper( &stack ) ;
			Expression* root  = exprNodeMaker(postOrder->data[i]) ;
			root->left = left ;
			root->right = right ;
			
			exprStackPusher( &stack, root ) ;
		}	
	}	
	
	return stack.node[0] ;
}


void postOrderConverter(vector* symVec, vector* postOrder)
{
	vector stack ;
	vectorInitializer( &stack, 1 ) ;
	
	for(int i=0; i<symVec->size; i++)
	{
		if( isSymbol(symVec->data[i].type) )
			vectorAppender( postOrder, symVec->data[i] ) ;
		else
		{
			while( !isEmpty(&stack) && (percendence(symVec->data[i].type) <= percendence(top(&stack).type)) )
			{
				vectorAppender( postOrder, top(&stack) ) ;
				stackPopper( &stack ) ;
			}	
			stackPusher( &stack, symVec->data[i] ) ;
		}		
	}	
	for(int i=stack.size-1; i>=0; i--)
		vectorAppender( postOrder, stack.data[i] ) ;
	
	vectorReleaser( symVec ) ;
	vectorReleaser( &stack ) ;	
}

/*
void inOrder(Expression* node)
{
	if( node )
	{
		inOrder(node->left) ;
		
		switch( node->value.type )
		{
			case idNode:
				printf("%s ", node->value.valueNode.identifier) ;
				break ;
			
			case intNode:
				printf("%d ", node->value.valueNode.intValue) ;
				break ;
			
			case floatNode:
				printf("%f ", node->value.valueNode.floatValue) ;
				break ;
				
			case plusNode:
				printf("+ ") ;
				break ;
			
			case minusNode:
				printf("- ") ;
				break ;			
			case mulNode:
				printf("* ") ;
				break ;			
			case divNode:
				printf("/ ") ;
				break ;				
		} ;
		
		inOrder(node->right) ;
	}	
}
*/

int isInteger(float num)
{
	return ceilf(num) == num ;
}

int isDigit(ValueType type)
{
	return type == intNode || type == floatNode ;
}


Expression* constFolder(Expression* node)
{
	if( node == NULL || (node->left == NULL && node->right == NULL) )
		return node ;
	
	Expression* leftResult 	= constFolder(node->left) ;
	Expression* rightResult = constFolder(node->right) ;
	
	node->left  = leftResult ;
	node->right = rightResult ;
	
	if( !isDigit(leftResult->value.type) || !isDigit(rightResult->value.type) )
		return node ;	
	
	float leftNum = (leftResult->value.type == intNode)? leftResult->value.valueNode.intValue : leftResult->value.valueNode.floatValue ;
	float rightNum = (rightResult->value.type == intNode)? rightResult->value.valueNode.intValue : rightResult->value.valueNode.floatValue ;
	
	free( leftResult ) ;
	free( rightResult ) ;
	
	float opResult ;
	Expression* opNode = (Expression*)malloc( sizeof(Expression) ) ;
	opNode->left = opNode->right = NULL ;
	switch( node->value.valueNode.operation )
	{
		case PLUS:
			opResult = leftNum + rightNum ;	
			break ;
			
		case MINUS: 
			opResult = leftNum - rightNum ;
			break ;
		
		case MUL: 
			opResult = leftNum * rightNum ;
			break ;
			
		case DIV:
			opResult = leftNum / rightNum ;
			break ;
			
		default:
			fprintf(stderr, "invalid operation") ;
			exit(0) ;
	} ;
	
	if( isInteger(opResult) )
	{
		opNode->value.type = intNode ;
		opNode->value.valueNode.intValue = opResult ;
	}	
	else
	{
		opNode->value.type = floatNode ;
		opNode->value.valueNode.floatValue = opResult ;		
	}	
	
	free(node) ;
	
	return opNode ;
}


Statement assignmentParser(FILE* source, Token token)
{
	Token assignToken = Scanner( source ) ;
	
	if( assignToken.type != assignmentOP )
		err_mesg("syntax error: expect an assignment symbol", assignToken.line) ;
	
	Statement assignStmt ;
	assignStmt.type = Assignment ;
	varNamer( assignStmt.stmt.assign.identifier, token.data ) ;	
	assignStmt.line = token.line ;
	
	/* expression parsing */
	vector symVec ;
	vectorInitializer( &symVec, 1 ) ;
	exprParser( source, &symVec ) ;
	
	assignStmt.stmt.assign.expr = constFolder( exprTreeGenerator( &symVec ) ) ;
			
	return assignStmt ;
}
