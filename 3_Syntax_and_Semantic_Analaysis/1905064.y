

%{
#include <bits/stdc++.h>
#include <typeinfo>

#include "1905064_HelperFunctions.h"
#include "1905064_SymbolTable.h"
#include "1905064_SymbolTableChecks.h"
#include "1905064_OutputStream.h"
#include "1905064_ErrorChecks.h"

using namespace std;

// global variables
ofstream logout;
ofstream errout;
ofstream parsetree;
SymbolTable *symbolTable=new SymbolTable(11);
SymbolInfo *currentParameterList=new SymbolInfo();
SymbolInfo *currentScope=new SymbolInfo("global","SCOPE"); //for matching function return type......

// external variable
extern int line_count;
extern int error_count;

extern FILE *yyin;





void yyerror(string $$){
	// logout<<"Error at line "<<global_line_count<<": "<<$$<<"\n"<<endl;
	// errout<<"Error at line "<<global_line_count<<": "<<$$<<"\n"<<endl;
    // error_count++;
}

int yyparse(void);
int yylex(void);

%}



%union{
    SymbolInfo* symbolInfo; 
	
}

%token<symbolInfo> IF ELSE LOWER_THAN_ELSE FOR WHILE DO BREAK CHAR DOUBLE RETURN SWITCH CASE DEFAULT CONTINUE PRINTLN INCOP DECOP ASSIGNOP NOT LPAREN RPAREN LCURL RCURL LSQUARE RSQUARE COMMA SEMICOLON INT FLOAT VOID CONST_INT CONST_FLOAT ID ADDOP MULOP RELOP LOGICOP CONST_CHAR MAIN BITOP
%type<symbolInfo> start program unit func_declaration func_definition parameter_list compound_statement var_declaration type_specifier declaration_list statements statement expression_statement variable expression logic_expression rel_expression simple_expression term unary_expression factor argument_list arguments enter_Scope 


%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
start : program{
		grammarRuleLog(line_count,"start","program",logout);
		$$ = new SymbolInfo("","start");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		$$->printParseTree(parsetree,0);
	}
	;

program : program unit {
		grammarRuleLog(line_count,"program","program unit",logout);
		$$ = new SymbolInfo("","program");
		$$->set_name(concatElements({$1,$2}));
		$$->addChild({$1,$2});
		
		
	}
	| unit {
		grammarRuleLog(line_count,"program","unit",logout);
		$$ = new SymbolInfo("","program");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		
	}
	;
	
unit : var_declaration {
		grammarRuleLog(line_count,"unit","var_declaration",logout);
		$$ = new SymbolInfo("","unit");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		
	}
	| func_declaration {
		grammarRuleLog(line_count,"unit","func_declaration",logout);
		$$ = new SymbolInfo("","unit");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		
	}
	| func_definition{
		grammarRuleLog(line_count,"unit","func_definition",logout);
		$$ = new SymbolInfo("","unit");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
	}
	/* | error {
		yyclearin;
		yyerrok;
		errorPrint("Syntax error at unit",errout);
		$$ = new SymbolInfo("","unit");
		$$->set_name("");
		$$->addChild({});
		
	} */
	;
     
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON {
		grammarRuleLog(line_count,"func_declaration","type_specifier ID LPAREN parameter_list RPAREN SEMICOLON",logout);
		$$ = new SymbolInfo("","func_declaration");
		$$->set_name(concatElements({$1,$2,$3,$4,$5,$6}));
		$2->setAsFunctionDeclaration();
		$2->setParameterArray($4->getParameterArray());
		$2->setVarType($1->getVarType());
		currentParameterList->setParameterArray({});
		declarationInsertion(symbolTable,$2,errout);
		$$->addChild({$1,$2,$3,$4,$5,$6});
		

		
	} 
	/* | type_specifier ID LPAREN error RPAREN SEMICOLON {
		grammarRuleLog(line_count,"func_declaration","type_specifier ID LPAREN parameter_list RPAREN SEMICOLON",logout);
		$$ = new SymbolInfo("","func_declaration");
		$$->set_name(concatElements({$1,$2,$3,$5,$6}));
		$2->setAsFunctionDeclaration();
		// $2->setParameterArray($4->getParameterArray());
		$2->setVarType($1->getVarType());
		currentParameterList->setParameterArray({});

		declarationInsertion(symbolTable,$2,errout);
		$$->addChild({$1,$2,$3,$5,$6});
		
	} */
	| type_specifier ID LPAREN RPAREN SEMICOLON {
		grammarRuleLog(line_count,"func_declaration","type_specifier ID LPAREN RPAREN SEMICOLON",logout);
		$$ = new SymbolInfo("","func_declaration");
		$$->set_name(concatElements({$1,$2,$3,$4,$5}));
		$2->setAsFunctionDeclaration();
		$2->setVarType($1->getVarType());

		declarationInsertion(symbolTable,$2,errout);
		$$->addChild({$1,$2,$3,$4,$5});
		
	}
	;
		 


func_definition : type_specifier ID LPAREN parameter_list RPAREN  { insertFunctionDefinition(symbolTable,currentParameterList,$2,$1,errout); currentScope=$2;} compound_statement {currentScope=new SymbolInfo("global","SCOPE");}{
		grammarRuleLog(line_count,"func_definition","type_specifier ID LPAREN parameter_list RPAREN compound_statement",logout);
		$$ = new SymbolInfo("","func_definition");
		$$->set_name(concatElements({$1,$2,$3,$4,$5,$7}));
		$$->addChild({$1,$2,$3,$4,$5,$7});
		
	}
	/* | type_specifier ID LPAREN error RPAREN  { insertFunctionDefinition(symbolTable,currentParameterList,$2,$1,errout); currentScope=$2;} compound_statement {currentScope=new SymbolInfo("global","SCOPE");}{
		grammarRuleLog(line_count,"func_definition","type_specifier ID LPAREN parameter_list RPAREN compound_statement",logout);
		$$ = new SymbolInfo("","func_definition");
		$$->set_name(concatElements({$1,$2,$3,$5,$7}));
		errorPrint("Syntax error at func_definition",errout);
		$$->addChild({$1,$2,$3,$5,$7});
		
	} */
	| type_specifier ID LPAREN RPAREN { insertFunctionDefinition(symbolTable,currentParameterList,$2,$1,errout); currentScope=$2;} compound_statement {currentScope=new SymbolInfo("global","SCOPE");}{
		grammarRuleLog(line_count,"func_definition","type_specifier ID LPAREN RPAREN compound_statement",logout);
		$$ = new SymbolInfo("","func_definition");
		$$->set_name(concatElements({$1,$2,$3,$4,$6}));
		$$->addChild({$1,$2,$3,$4,$6});
		
	}
	;				

parameter_list : parameter_list COMMA type_specifier ID {
		grammarRuleLog(line_count,"parameter_list","parameter_list COMMA type_specifier ID",logout);
		$$ = new SymbolInfo("","parameter_list");
		$$->set_name(concatElements({$1,$2,$3,$4}));
		$$->addChild({$1,$2,$3,$4});
		$$->setParameterArray($1->getParameterArray());
		$4->setVarType($3->getVarType());
		//errout<<$4->getVarType()<<" "<<$4->get_name()<<endl;
		$$->addParameter($4->get_name(),$3->getVarType());
		isVoidTypeSpecifier($3,$4,errout);
		currentParameterList->setParameterArray($$->getParameterArray());
		
	} 
	| parameter_list COMMA type_specifier {
		grammarRuleLog(line_count,"parameter_list","parameter_list COMMA type_specifier",logout);
		$$ = new SymbolInfo("","parameter_list");
		$$->set_name(concatElements({$1,$2,$3}));
		$$->addChild({$1,$2,$3});
		
	
		$$->setParameterArray($1->getParameterArray());
		$$->addParameter("",$3->getVarType());
		SymbolInfo* temp=new SymbolInfo("","");
		isVoidTypeSpecifier($3,temp,errout);
		currentParameterList->setParameterArray($$->getParameterArray());
		
	}
	| type_specifier ID {
		grammarRuleLog(line_count,"parameter_list","type_specifier ID",logout);
		$$ = new SymbolInfo("","parameter_list");
		$$->set_name(concatElements({$1,$2}));
		$$->addChild({$1,$2});
		
		$2->setVarType($1->getVarType());
		$$->addParameter($2->get_name(),$1->getVarType());
		isVoidTypeSpecifier($1,$2,errout);
		currentParameterList->setParameterArray($$->getParameterArray());
		
	}
	| type_specifier {
		grammarRuleLog(line_count,"parameter_list","type_specifier",logout);
		$$ = new SymbolInfo("","parameter_list");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		$$->addParameter("",$1->getVarType());
		SymbolInfo* temp=new SymbolInfo("","");
		isVoidTypeSpecifier($1,temp,errout);
		currentParameterList->setParameterArray($$->getParameterArray());
		
	}
	;

compound_statement : LCURL enter_Scope statements RCURL {
		grammarRuleLog(line_count,"compound_statement","LCURL statements RCURL",logout);
		$$ = new SymbolInfo("","compound_statement");
		$$->set_name(concatElements({$1,$3,$4}));
		$$->addChild({$1,$3,$4});
		
		symbolTable->print_all(logout);
		symbolTable->exit_scope();
	}
	/* | LCURL enter_Scope error RCURL {
		grammarRuleLog(line_count,"compound_statement","LCURL statements RCURL",logout);
		$$ = new SymbolInfo("","compound_statement");
		$$->set_name(concatElements({$1,$3}));
		$$->addChild({$1,$3});
		
		symbolTable->print_all(logout);
		symbolTable->exit_scope();
	} */
	| LCURL enter_Scope RCURL {
		grammarRuleLog(line_count,"compound_statement","LCURL RCURL",logout);
		$$ = new SymbolInfo("","compound_statement");
		$$->set_name(concatElements({$1,$3}));
		$$->addChild({$1,$3});
		
		symbolTable->print_all(logout);
		symbolTable->exit_scope();
	}
	;
 		    
var_declaration : type_specifier declaration_list SEMICOLON {
		grammarRuleLog(line_count,"var_declaration","type_specifier declaration_list SEMICOLON",logout);
		$$ = new SymbolInfo("","var_declaration");
		$$->set_name(concatElements({$1,$2,$3}));
		$$->addChild({$1,$2,$3});
		
		insertVarDeclaration(symbolTable,$1,$2,errout);
		
	}
	/* | type_specifier error SEMICOLON {
		grammarRuleLog(line_count,"var_declaration","type_specifier error SEMICOLON",logout);
		$$ = new SymbolInfo("","var_declaration");
		$$->set_name("");
		$$->addChild({});
		
		errorPrint("Syntax error at var_declaration",errout);
		
	} */
	;
 		 
type_specifier	: INT {
		grammarRuleLog(line_count,"type_specifier","INT",logout);
		$$ = new SymbolInfo("","type_specifier");
		$$->set_name(concatElements({$1}));
		$$->setVarType($1->getVarType());
		$$->addChild({$1});
		
	}
	| FLOAT {
		grammarRuleLog(line_count,"type_specifier","FLOAT",logout);
		$$ = new SymbolInfo("","type_specifier");
		$$->set_name(concatElements({$1}));
		$$->setVarType($1->getVarType());
		$$->addChild({$1});
		
	}
	| VOID {
		grammarRuleLog(line_count,"type_specifier","VOID",logout);
		$$ = new SymbolInfo("","type_specifier");
		$$->set_name(concatElements({$1}));
		$$->setVarType($1->getVarType());
		$$->addChild({$1});
		
	}
	;
 		
declaration_list : declaration_list COMMA ID {
		grammarRuleLog(line_count,"declaration_list","declaration_list COMMA ID",logout);
		$$ = new SymbolInfo("","declaration_list");
		$$->set_name(concatElements({$1,$2,$3}));
		$$->setDeclarationList($1->getDeclarationList());
		$$->addDeclaration($3->get_name(),false);
		$$->addChild({$1,$2,$3});
		
	}
	| declaration_list COMMA ID LSQUARE CONST_INT RSQUARE {
		grammarRuleLog(line_count,"declaration_list","declaration_list COMMA ID LSQUARE CONST_INT RSQUARE",logout);
		$$ = new SymbolInfo("","declaration_list");
		$$->set_name(concatElements({$1,$2,$3,$4,$5,$6}));
		$$->addChild({$1,$2,$3,$4,$5,$6});
		
		$$->setDeclarationList($1->getDeclarationList());
		$3->setAsArray();
		$$->addDeclaration($3->get_name(),true);
		
	}
	| ID {
		grammarRuleLog(line_count,"declaration_list","ID",logout);
		$$ = new SymbolInfo("","declaration_list");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		$$->addDeclaration($1->get_name(),false);
		
	}
	| ID LSQUARE CONST_INT RSQUARE {
		grammarRuleLog(line_count,"declaration_list","ID LSQUARE CONST_INT RSQUARE",logout);
		$$ = new SymbolInfo("","declaration_list");
		$$->set_name(concatElements({$1,$2,$3,$4}));
		
		$$->addChild({$1,$2,$3,$4});
		$1->setAsArray();
		$$->addDeclaration($1->get_name(),true);
		
	}
	;
 		  
statements : statement {
		grammarRuleLog(line_count,"statements","statement",logout);
		$$ = new SymbolInfo("","statements");
		$$->set_name(concatElements({$1}));
		
		$$->addChild({$1});
		
	}
	| statements statement {
		grammarRuleLog(line_count,"statements","statements statement",logout);
		$$ = new SymbolInfo("","statements");
		$$->set_name(concatElements({$1,$2}));
		$$->addChild({$1,$2});
		
	}
	;
	   
statement : var_declaration {
		grammarRuleLog(line_count,"statement","var_declaration",logout);
		$$ = new SymbolInfo("","statement");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
	}
	| expression_statement {
		grammarRuleLog(line_count,"statement","expression_statement",logout);
		$$ = new SymbolInfo("","statement");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
	}
	| compound_statement {
		grammarRuleLog(line_count,"statement","compound_statement",logout);
		$$ = new SymbolInfo("","statement");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
	}
	| FOR LPAREN expression_statement expression_statement expression RPAREN statement {
		grammarRuleLog(line_count,"statement","FOR LPAREN expression_statement expression_statement expression RPAREN statement",logout);
		$$ = new SymbolInfo("","statement");
		$$->set_name(concatElements({$1,$2,$3,$4,$5,$6,$7}));
		$$->addChild({$1,$2,$3,$4,$5,$6,$7});
		
	}
	| IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE {
		grammarRuleLog(line_count,"statement","IF LPAREN expression RPAREN statement",logout);
		$$ = new SymbolInfo("","statement");
		$$->set_name(concatElements({$1,$2,$3,$4,$5}));
		$$->addChild({$1,$2,$3,$4,$5});
		
	}
	| IF LPAREN expression RPAREN statement ELSE statement {
		grammarRuleLog(line_count,"statement","IF LPAREN expression RPAREN statement ELSE statement",logout);
		$$ = new SymbolInfo("","statement");
		$$->set_name(concatElements({$1,$2,$3,$4,$5,$6,$7}));
		$$->addChild({$1,$2,$3,$4,$5,$6,$7});
		
	}
	| WHILE LPAREN expression RPAREN statement {
		grammarRuleLog(line_count,"statement","WHILE LPAREN expression RPAREN statement",logout);
		$$ = new SymbolInfo("","statement");
		$$->set_name(concatElements({$1,$2,$3,$4,$5}));
		$$->addChild({$1,$2,$3,$4,$5});
		
	}
	| PRINTLN LPAREN ID RPAREN SEMICOLON {
		grammarRuleLog(line_count,"statement","PRINTLN LPAREN ID RPAREN SEMICOLON",logout);
		$$ = new SymbolInfo("","statement");
		$$->set_name(concatElements({$1,$2,$3,$4,$5}));
		$$->addChild({$1,$2,$3,$4,$5});
		
		if( symbolTable->lookUp($3->get_name())==NULL ){
			errorPrint("Undeclared variable",errout);
		}
		
	}
	| RETURN expression SEMICOLON {
		grammarRuleLog(line_count,"statement","RETURN expression SEMICOLON",logout);
		$$ = new SymbolInfo("","statement");
		$$->set_name(concatElements({$1,$2,$3}));
		$$->addChild({$1,$2,$3});


		if($2->getVarType()!=currentScope->getVarType())
			errorPrint("Return type mismatch of function",errout);
		
	}
	;
	  
expression_statement : SEMICOLON {
		grammarRuleLog(line_count,"expression_statement","SEMICOLON",logout);
		$$ = new SymbolInfo("","expression_statement");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
	}			
	| expression SEMICOLON {
		grammarRuleLog(line_count,"expression_statement","expression SEMICOLON",logout);
		$$ = new SymbolInfo("","expression_statement");
		$$->set_name(concatElements({$1,$2}));
		$$->setVarType($1->getVarType());
		$$->addChild({$1,$2});
		
	} 
	/* | error SEMICOLON {
		yyclearin;
		yyerrok;
		errorPrint("Syntax error at expression statement",errout);
		$$ = new SymbolInfo("","expression_statement");
		$$->addChild({});
		
	}  */
	;
	  
variable : ID {
		grammarRuleLog(line_count,"variable","ID",logout);

		$$ = new SymbolInfo("","variable");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});

		variableCheck(symbolTable, $$, $1, errout);
		
	}
	| ID LSQUARE expression RSQUARE {
		grammarRuleLog(line_count,"variable","ID LSQUARE expression RSQUARE",logout);

		$$ = new SymbolInfo("","variable");
		$$->set_name(concatElements({$1,$2,$3,$4}));
		$$->addChild({$1,$2,$3,$4});
		
		arrayCheck(symbolTable, $$, $1, $3, errout);
			
	}
	;

expression : logic_expression {
		grammarRuleLog(line_count,"expression","logic_expression",logout);
		$$ = new SymbolInfo("","expression");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		$$->setVarType($1->getVarType());
		
	}
	| variable ASSIGNOP logic_expression {
		grammarRuleLog(line_count,"expression","variable ASSIGNOP logic_expression",logout);
		$$ = new SymbolInfo("","expression");
		$$->set_name(concatElements({$1,$2,$3}));
		$$->addChild({$1,$2,$3});
		
		ASSIGN_checks($3, $1,errout);
		
	}
	;
		
logic_expression : rel_expression {
		grammarRuleLog(line_count,"logic_expression","rel_expression",logout);
		$$ = new SymbolInfo("","logic_expression");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		$$->setVarType($1->getVarType());
		if($1->isArray())
			$$->setAsArray();
		
	}	
	| rel_expression LOGICOP rel_expression {
		grammarRuleLog(line_count,"logic_expression","rel_expression LOGICOP rel_expression",logout);
		$$ = new SymbolInfo("","logic_expression");
		$$->set_name(concatElements({$1,$2,$3}));
		$$->addChild({$1,$2,$3});
		
		$$->setVarType("INT");
		
	}
	;
			
rel_expression : simple_expression {
		grammarRuleLog(line_count,"rel_expression","simple_expression",logout);
		$$ = new SymbolInfo("","rel_expression");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		$$->setVarType($1->getVarType());
		if($1->isArray())
			$$->setAsArray();
		
	}
	| simple_expression RELOP simple_expression	{
		grammarRuleLog(line_count,"rel_expression","simple_expression RELOP simple_expression",logout);
		$$ = new SymbolInfo("","rel_expression");
		$$->set_name(concatElements({$1,$2,$3}));
		$$->addChild({$1,$2,$3});
		
		if( $1->getVarType() == "VOID" or $3->getVarType() == "VOID" ){
			errorPrint("Void cannot be used in expression",errout);
			$$->setVarType("error");
		}else{
			$$->setVarType("INT");
		}
		
	}
	;
				
simple_expression : term {
		grammarRuleLog(line_count,"simple_expression","term",logout);
		$$ = new SymbolInfo("","simple_expression");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		$$->setVarType($1->getVarType());
		if($1->isArray())
			$$->setAsArray();
		
	}
	| simple_expression ADDOP term {
		grammarRuleLog(line_count,"simple_expression","simple_expression ADDOP term",logout);
		$$ = new SymbolInfo("","simple_expression");
		$$->set_name(concatElements({$1,$2,$3}));
		$$->addChild({$1,$2,$3});
		
		$$->setVarType( typeCast($1,$3) );
		if( $3->getVarType() == "VOID" ){
			errorPrint("Void cannot be used in expression",errout);
		}
		
	}
	;
					
term :	unary_expression {
		grammarRuleLog(line_count,"term","unary_expression",logout);
		$$ = new SymbolInfo("","term");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		$$->setVarType($1->getVarType());
		if($1->isArray())
			$$->setAsArray();
		
	}
	|  term MULOP unary_expression {
		grammarRuleLog(line_count,"term","term MULOP unary_expression",logout);
		$$ = new SymbolInfo("","term");
		$$->set_name(concatElements({$1,$2,$3}));
		$$->addChild({$1,$2,$3});
		
		MULOP_check($$, $2, $1, $3,errout);
		
	}
	;

unary_expression : ADDOP unary_expression {
		grammarRuleLog(line_count,"unary_expression","ADDOP unary_expression",logout);
		$$ = new SymbolInfo("","unary_expression");
		$$->set_name(concatElements({$1,$2}));
		$$->addChild({$1,$2});
		
		if( $2->getVarType() == "VOID" ){
			errorPrint("Void cannot be used in expression",errout);
			$$->setVarType("error");
		}else{
			$$->setVarType($2->getVarType());
		}
		
	} 
	| NOT unary_expression {
		grammarRuleLog(line_count,"unary_expression","NOT unary_expression",logout);
		$$ = new SymbolInfo("","unary_expression");
		$$->set_name(concatElements({$1,$2}));
		$$->addChild({$1,$2});
		
		if( $2->getVarType() == "VOID" ){
			errorPrint("Void cannot be used in expression",errout);
			$$->setVarType("error");
		}else{
			$$->setVarType("INT");
		}
		
	}
	| factor {
		grammarRuleLog(line_count,"unary_expression","factor",logout);
		$$ = new SymbolInfo("","unary_expression");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		$$->setVarType($1->getVarType());
		//errout<<$$->getVarType()<<" "<<$$->get_name()<<endl;
		if($1->isArray())
			$$->setAsArray();
		
	}
	;
	
factor : variable {
		grammarRuleLog(line_count,"factor","variable",logout);
		$$ = new SymbolInfo("","factor");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		$$->setVarType($1->getVarType());
		if($1->isArray())
			$$->setAsArray();
		
	}
	| ID LPAREN argument_list RPAREN {
		grammarRuleLog(line_count,"factor","ID LPAREN argument_list RPAREN",logout);
		$$ = new SymbolInfo("","factor");
		$$->set_name(concatElements({$1,$2,$3,$4}));
		$$->addChild({$1,$2,$3,$4});
		functionCallCheck(symbolTable,$$,$1,$3,errout);
		//errout<<$1->getVarType()<<" "<<$1->get_name()<<endl;
	}
			
	| LPAREN expression RPAREN {
		grammarRuleLog(line_count,"factor","LPAREN expression RPAREN",logout);
		$$ = new SymbolInfo("","factor");
		$$->set_name(concatElements({$1,$2,$3}));
		$$->addChild({$1,$2,$3});
		
		$$->setVarType($2->getVarType());
		
	}
	| CONST_INT {
		grammarRuleLog(line_count,"factor","CONST_INT",logout);
		$$ = new SymbolInfo("","factor");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		$$->setVarType("INT");
		
	}
	| CONST_FLOAT {
		grammarRuleLog(line_count,"factor","CONST_FLOAT",logout);
		$$ = new SymbolInfo("","factor");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		$$->setVarType("FLOAT");
		
	}
	| variable INCOP {
		grammarRuleLog(line_count,"factor","variable INCOP",logout);
		$$ = new SymbolInfo("","factor");
		$$->set_name(concatElements({$1,$2}));
		$$->addChild({$1,$2});
		
		if( $1->getVarType() == "VOID" ){
			errorPrint("Void cannot be used in expression",errout);
			$$->setVarType("error");
		}else{
			$$->setVarType($1->getVarType());
		}
		
		
	}
	| variable DECOP {
		grammarRuleLog(line_count,"factor","variable DECOP",logout);
		$$ = new SymbolInfo("","factor");
		$$->set_name(concatElements({$1,$2}));
		$$->addChild({$1,$2});
		
		if( $1->getVarType() == "VOID" ){
			errorPrint("Void cannot be used in expression",errout);
			$$->setVarType("error");
		}else{
			$$->setVarType($1->getVarType());
		}
		
	}
	;

argument_list : arguments {
		grammarRuleLog(line_count,"argument_list","arguments",logout);
		$$ = new SymbolInfo("","argument_list");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		$$->setParameterArray($1->getParameterArray());
		
	}
	/* | arguments error {
		grammarRuleLog(line_count,"argument_list","arguments",logout);
		yyclearin;
		yyerrok;
		errorPrint("Syntax error at arguments",errout);
		$$ = new SymbolInfo("","argument_list");
		$$->set_name(concatElements({$1}));
		$$->addChild({$1});
		
		$$->setParameterArray($1->getParameterArray());
		
	} */
	| {
		grammarRuleLog(line_count,"argument_list","",logout);
		$$ = new SymbolInfo("","argument_list");
		$$->addChild({});
		
	}
	;
	
arguments : arguments COMMA logic_expression {
		grammarRuleLog(line_count,"arguments","arguments COMMA logic_expression",logout);
		$$ = new SymbolInfo("","arguments");
		$$->set_name(concatElements({$1,$2,$3}));
		$$->addChild({$1,$2,$3});
		
		$$->setParameterArray($1->getParameterArray());
		$$->addParameter($3->get_name(),$3->getVarType());
		
	}
	| logic_expression {
		grammarRuleLog(line_count,"arguments","logic_expression",logout);
		$$ = new SymbolInfo("","arguments");
		$$->set_name(concatElements({$1}));
		
		$$->addChild({$1});
		$$->addParameter($1->get_name(),$1->getVarType());
		
	}
	;

enter_Scope : {

	symbolTable->enter_scope();
	for(auto parameterInfo:currentParameterList->getParameterArray()){
		if( parameterInfo->getName() == "" ) continue;
		if( parameterInfo->getType() == "VOID" )
			parameterInfo->setType("error");
		SymbolInfo* s=parameterToSymbol(parameterInfo);
		declarationInsertion(symbolTable,s,errout);

	}
	currentParameterList->setParameterArray({});
}


%%
int main(int argc,char *argv[]){
	FILE *fp;

	if((fp=fopen(argv[1],"r"))==NULL){
		printf("Cannot Open Input File.\n");
		exit(1);
	}

	logout.open("log.txt");
	errout.open("error.txt");
	parsetree.open("tree.txt");

	yyin=fp;
	yyparse();
	
	

	logout<<"Total Lines: "<<line_count<<endl;
    logout<<"Total Errors: "<<error_count<<endl;

	fclose(yyin);
 	logout.close();
	errout.close();
	parsetree.close();
	return 0;

	/* #!/bin/bash

yacc -d -y 1905064.y
echo 'Generated the parser C file as well the header file'
g++ -w -c -o y.o y.tab.c
echo 'Generated the parser object file'
flex 1905064.l
echo 'Generated the scanner C file'
g++ -w -c -o l.o lex.yy.c
# if the above command doesn't work try g++ -fpermissive -w -c -o l.o lex.yy.c
echo 'Generated the scanner object file'
g++  1905064_SymbolTable.cpp 1905064_SymbolTableChecks.cpp 1905064_HelperFunctions.cpp 1905064_OutputStream.cpp 1905064_ErrorChecks.cpp -c
g++  1905064_SymbolTable.o 1905064_SymbolTableChecks.o 1905064_HelperFunctions.o 1905064_OutputStream.o 1905064_ErrorChecks.o y.o l.o -lfl -o er 
echo 'All ready, running'
./er sserror.c */
}