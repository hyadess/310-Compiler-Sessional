%option noyywrap
%x SINGLE_LINE_STRING
%x MULTI_LINE_STRING
%x SINGLE_LINE_COMMENT
%x MULTI_LINE_COMMENT

%{
#include<cstdio>
#include<cstdlib>
#include<string>
#include<vector>
#include<bits/stdc++.h>
#include "1905064_SymbolTable.h"
#include "1905064_OutputStream.h"
#include "1905064_ICG_GeneratorFunctions.h"
#include "y.tab.h"
using namespace std;

int line_count=1;
int error_count=0;
extern ofstream logout;
extern ofstream errout;



%}

WHITESPACE [ \t\f\r\v]+ 
LETTER [a-zA-Z]
DIGIT [0-9]
NEWLINE [\r]?\n
ALPHANEUMERIC_AND_DASH [_0-9a-zA-Z]
CHARACTERS [ !-~]{-}['\\]
SPECIAL_CHARACTERS \\[abefntrv0\\"'?]


FOR_IDENTIFIER [_a-zA-Z]{ALPHANEUMERIC_AND_DASH}*
FOR_INTEGER {DIGIT}+
FOR_FLOAT ({DIGIT}+\.?{DIGIT}*|\.{DIGIT}+)([Ee][+-]?{DIGIT}+)?
FOR_CHARACTER_LITERALS \'({CHARACTERS}|{SPECIAL_CHARACTERS})\' 




FOR_TOO_MANY_DECIMAL_POINTS ((({DIGIT}+\.{DIGIT}*|\.{DIGIT}+)([Ee][+-]?{DIGIT}+)?)\.[0-9\.Ee+-]*(\.)*)

FOR_ILLFORMED_NUMBER ([0-9]*(\.[0-9]*)?[Ee][+-]?[0-9]*[0-9\.Ee]*[+-]?(\.)*)

FOR_INVALID_ID_SUFFIX_NUM_PREFIX {DIGIT}+{ALPHANEUMERIC_AND_DASH}+

FOR_MULTICHAR_CONST_CHAR '({CHARACTERS}|{SPECIAL_CHARACTERS})({CHARACTERS}|{SPECIAL_CHARACTERS})+'

FOR_UNFINISHED_CONST_CHAR \'({CHARACTERS}|{SPECIAL_CHARACTERS})|\'

FOR_EMPTY_CONST_CHAR \'\'

FOR_UNRECOGNIZED_CHAR .


%%

{NEWLINE} {line_count++;}



"if" {
	SymbolInfo *s= new  SymbolInfo(yytext, "IF");
	s->setAsToken();
	s->setLineNo(line_count);
	yylval.symbolInfo=s;
	lexemeLog(line_count,"IF",yytext,logout);
	return IF;
	}
"else" {
	SymbolInfo *s= new  SymbolInfo(yytext, "ELSE");
	s->setAsToken();
	s->setLineNo(line_count);
	yylval.symbolInfo=s;
	lexemeLog(line_count,"ELSE",yytext,logout);
	return ELSE;
	}
"for" {
	SymbolInfo *s= new  SymbolInfo(yytext, "FOR");
	s->setAsToken();
	s->setLineNo(line_count);
	yylval.symbolInfo=s;
	lexemeLog(line_count,"FOR",yytext,logout);
	return FOR;
	}
"while" {
	SymbolInfo *s= new  SymbolInfo(yytext, "WHILE");
	s->setAsToken();
	s->setLineNo(line_count);
	yylval.symbolInfo=s;
	lexemeLog(line_count,"WHILE",yytext,logout);
	return WHILE;
	}
"do" {
	SymbolInfo *s= new  SymbolInfo(yytext, "DO");
	s->setAsToken();
	s->setLineNo(line_count);
	yylval.symbolInfo=s;
	lexemeLog(line_count,"DO",yytext,logout);
	return DO;
	}
"int" {
	SymbolInfo *s= new  SymbolInfo(yytext, "INT");
	s->setAsToken();
	s->setLineNo(line_count);
	s->setVarType("INT");
	yylval.symbolInfo=s;
	lexemeLog(line_count,"INT",yytext,logout);
	return INT;
	}
"float" {
	SymbolInfo *s= new  SymbolInfo(yytext, "FLOAT");
	s->setAsToken();
	s->setLineNo(line_count);
	s->setVarType("FLOAT");
	yylval.symbolInfo=s;
	lexemeLog(line_count,"FLOAT",yytext,logout);
	return FLOAT;
	}
"double" {
	SymbolInfo *s= new  SymbolInfo(yytext, "DOUBLE");
	s->setAsToken();
	s->setLineNo(line_count);
	s->setVarType("DOUBLE");
	yylval.symbolInfo=s;
	lexemeLog(line_count,"DOUBLE",yytext,logout);
	return DOUBLE;
	}
"char" {
	SymbolInfo *s= new  SymbolInfo(yytext, "CHAR");
	s->setAsToken();
	s->setLineNo(line_count);
	s->setVarType("CHAR");
	yylval.symbolInfo=s;
	lexemeLog(line_count,"CHAR",yytext,logout);
	return CHAR;
	}
"return" {
	SymbolInfo *s= new  SymbolInfo(yytext, "RETURN");
	s->setAsToken();
	s->setLineNo(line_count);
	yylval.symbolInfo=s;
	lexemeLog(line_count,"RETURN",yytext,logout);
	return RETURN;
	}
"void" {
	SymbolInfo *s= new  SymbolInfo(yytext, "VOID");
	s->setAsToken();
	s->setLineNo(line_count);
	s->setVarType("VOID");
	yylval.symbolInfo=s;
	lexemeLog(line_count,"VOID",yytext,logout);
	return VOID;
	}

"break" {
	SymbolInfo *s= new  SymbolInfo(yytext, "BREAK");
	s->setAsToken();
	s->setLineNo(line_count);
	yylval.symbolInfo=s;
	lexemeLog(line_count,"BREAK",yytext,logout);
	return BREAK;
	}
"continue" {
	SymbolInfo *s= new  SymbolInfo(yytext, "CONTINUE");
	s->setAsToken();
	s->setLineNo(line_count);
	yylval.symbolInfo=s;
	lexemeLog(line_count,"CONTINUE",yytext,logout);
	return CONTINUE;
	}
"default" {
	SymbolInfo *s= new  SymbolInfo(yytext, "DEFAULT");
	s->setAsToken();
	s->setLineNo(line_count);
	yylval.symbolInfo=s;
	lexemeLog(line_count,"DEFAULT",yytext,logout);
	return DEFAULT;
	}
"println"	{
	SymbolInfo *s= new  SymbolInfo(yytext, "PRINTLN");
	s->setAsToken();
	s->setLineNo(line_count);
	yylval.symbolInfo=s;
	lexemeLog(line_count,"PRINTLN",yytext,logout);
	return PRINTLN;
	}


"+"|"-"     {
				
				SymbolInfo *s= new  SymbolInfo(yytext, "ADDOP");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"ADDOP",yytext,logout);
				return ADDOP;
			}
"*"|"/"|"%"	{
				SymbolInfo *s= new  SymbolInfo(yytext, "MULOP");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"MULOP",yytext,logout);
				return MULOP;
			}
"++"	   	{
				SymbolInfo *s= new  SymbolInfo(yytext, "INCOP");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"INCOP",yytext,logout);
				return INCOP;
			}
"--"	   	{
				SymbolInfo *s= new  SymbolInfo(yytext, "DECOP");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"DECOP",yytext,logout);
				return DECOP;
			}
"<"|"<="|">"|">="|"=="|"!=" 	{
				SymbolInfo *s= new  SymbolInfo(yytext, "RELOP");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"RELOP",yytext,logout);
				return RELOP;
			}
"="         {
				SymbolInfo *s= new  SymbolInfo(yytext, "ASSIGNOP");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"ASSIGNOP",yytext,logout);
				return ASSIGNOP;
			}
"&&"|"||"   {
				SymbolInfo *s= new  SymbolInfo(yytext, "LOGICOP");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"LOGICOP",yytext,logout);
				return LOGICOP;
			}
"&"|"|"|"^"|"<<"|">>"		{
				SymbolInfo *s= new  SymbolInfo(yytext, "BITOP");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"BITOP",yytext,logout);
				return BITOP;
			}		
"!"         {
				SymbolInfo *s= new  SymbolInfo(yytext, "NOT");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"NOT",yytext,logout);
				return NOT;
			}
"("         {
				SymbolInfo *s= new  SymbolInfo(yytext, "LPAREN");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"LPAREN",yytext,logout);
				return LPAREN;
			}
")"         {
				SymbolInfo *s= new  SymbolInfo(yytext, "RPAREN");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"RPAREN",yytext,logout);
				return RPAREN;
			}
"{"         {
				SymbolInfo *s= new  SymbolInfo(yytext, "LCURL");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"LCURL",yytext,logout);
				return LCURL;
			}
"}"         {
				SymbolInfo *s= new  SymbolInfo(yytext, "RCURL");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"RCURL",yytext,logout);
				return RCURL;
			}
"["         {
				SymbolInfo *s= new  SymbolInfo(yytext, "LSQUARE");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"LSQUARE",yytext,logout);
				return LSQUARE;
			}
"]"         {
				SymbolInfo *s= new  SymbolInfo(yytext, "RSQUARE");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"RSQUARE",yytext,logout);
				return RSQUARE;
			}
","         {
				SymbolInfo *s= new  SymbolInfo(yytext, "COMMA");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"COMMA",yytext,logout);
				return COMMA;
			}
";"         {
				SymbolInfo *s= new  SymbolInfo(yytext, "SEMICOLON");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"SEMICOLON",yytext,logout);
				return SEMICOLON;
			}


{FOR_IDENTIFIER}	{
				SymbolInfo *s= new  SymbolInfo(yytext, "ID");
				s->setAsToken();
				s->setLineNo(line_count);
				yylval.symbolInfo=s;
				lexemeLog(line_count,"ID",yytext,logout);
				return ID;
				///////................................
			}


{FOR_INTEGER}	{
				SymbolInfo *s= new  SymbolInfo(yytext, "CONST_INT");
				s->setAsToken();
				s->setLineNo(line_count);
				s->setVarType("INT");
				yylval.symbolInfo=s;
				lexemeLog(line_count,"CONST_INT",yytext,logout);
				return CONST_INT;
			}

{FOR_FLOAT}		{
				SymbolInfo *s= new  SymbolInfo(yytext, "CONST_FLOAT");
				s->setAsToken();
				s->setLineNo(line_count);
				s->setVarType("FLOAT");
				yylval.symbolInfo=s;
				lexemeLog(line_count,"CONST_FLOAT",yytext,logout);
				return CONST_FLOAT;

			}
{FOR_CHARACTER_LITERALS}	{
				SymbolInfo *s= new  SymbolInfo(yytext, "CONST_CHAR");
				s->setAsToken();
				s->setLineNo(line_count);
				s->setVarType("CHAR");
				yylval.symbolInfo=s;
				lexemeLog(line_count,"CONST_CHAR",yytext,logout);
				return CONST_CHAR;

			}	
{WHITESPACE}	{

			}	
			


%%
