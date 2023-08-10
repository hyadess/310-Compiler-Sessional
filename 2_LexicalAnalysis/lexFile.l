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
#include "1905064.cpp"

using namespace std;

int line_count=1;
int error_count=0;

ofstream logout;
ofstream tokenout;

string logfile_string;
string tokenfile_string;
string comment;
int line_for_string;


SymbolTable* symbolTable;

string capitalize(string keyword)
{
	string upped=keyword;
	int len=upped.length();
	for(int i=0;i<len;i++)
		upped[i]=upped[i]-32;
	return upped;
}

char charTransform(const char* input) {
	
	if(input[1] == '\\') {
		if(input[2] == 'n') {
			return '\n';
		} else if(input[2] == 't') {
			return '\t';
		} else if(input[2] == 'v') {
			return '\v';
		} else if(input[2] == 'f') {
			return '\f';
		} else if(input[2] == 'r') {
			return '\r';
		} else if(input[2] == 'a') {
			return '\a';
		} else if(input[2] == 'b') {
			return '\b';
		} else if(input[2] == '0') {
			return '\0';
		} else if(input[2] == '\\') {
			return '\\';
		} else if(input[2] == '\"') {
			return '\"';
		} else if(input[2] == '\'') {
			return '\'';
		} else if(input[2] == '?') {
			return '\?';
		} else if(input[2] == 'e') {
			return '\e';
		} else{
			return input[2];
		}
	}
	else{
		return input[1];
	}

}


void printAndLogKeyword(string token, string keyword, int line_count)
{
	tokenout<<"<"<<token<<", "<<keyword<<">"<<endl;
	logout<<"Line# "<<line_count<<": Token "<<"<"<<token<<"> Lexeme "<<keyword<<" found"<<endl;
}

void printAndLogCharacter(string token, const char* characterStream, int line_count)
{
	tokenout<<"<"<<token<<", "<<charTransform(characterStream)<<">"<<endl;
	logout<<"Line# "<<line_count<<": Token "<<"<"<<token<<"> Lexeme "<<charTransform(characterStream) <<" found"<<endl;
}

void printAndLogOperator(string token, string opertor, int line_count)
{
	tokenout<<"<"<<token<<", "<<opertor<<">"<<endl;
	logout<<"Line# "<<line_count<<": Token "<<"<"<<token<<"> Lexeme "<<opertor<<" found"<<endl;

	if(opertor=="{")
		symbolTable->enter_scope();
	else if(opertor=="}")
	{
		symbolTable->exit_scope();


	}
}

void printAndLogIdentifier(string token, string identifier, int line_count)
{
	tokenout<<"<"<<token<<", "<<identifier<<">"<<endl;
	logout<<"Line# "<<line_count<<": Token "<<"<"<<token<<"> Lexeme "<<identifier<<" found"<<endl;

	if(symbolTable->Insert(identifier,token,logout)==true)
		symbolTable->print_all(logout);

}

void printAndLogString(string token)
{
	string print_log = "\"";
	print_log += logfile_string;
	print_log += "\"";
							
	tokenout<<"<"<<token<<", "<<tokenfile_string<<">"<<endl;
    logout<<"Line# "<< line_count <<": Token " <<"<"<<token<<">"<<" Lexeme "<<print_log<<" found"<<endl;
                         	
	line_count += line_for_string;
	line_for_string = 0;
							 
	

}

void printAndLogComment(string token)
{
	logout<<"Line# "<< line_count <<": Token " <<"<"<<token<<">"<<" Lexeme "<<comment<<" found"<<endl;
		
	line_count += line_for_string;
	line_for_string = 0;
}

void printAndLogError(string message,string yytext)
{
	logout<<"Error at line# "<<line_count<<": "<<message<<" "<<yytext<<endl;
}



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

"if"|"else"|"for"|"while"|"do"|"int"|"char"|"break"|"float"|"double"|"void"|"return"|"switch"|"case"|"default"|"continue"		{
			printAndLogKeyword(capitalize(yytext),yytext,line_count);
		}
		


"+"|"-"     {
				printAndLogOperator("ADDOP", yytext, line_count);
			}
"*"|"/"|"%"	{
				printAndLogOperator("MULOP", yytext, line_count);
			}
"++"|"--"   {
				printAndLogOperator("INCOP", yytext, line_count);
			}
"<"|"<="|">"|">="|"=="|"!=" 	{
				printAndLogOperator("RELOP", yytext, line_count);
			}
"="         {
				printAndLogOperator("ASSIGNOP", yytext, line_count);
			}
"&&"|"||"   {
				printAndLogOperator("LOGICOP", yytext, line_count);
			}
"&"|"|"|"^"|"<<"|">>"		{
				printAndLogOperator("BITOP", yytext, line_count);
			}		
"!"         {
				printAndLogOperator("NOT", yytext, line_count);
			}
"("         {
				printAndLogOperator("LPAREN", yytext, line_count);
			}
")"         {
				printAndLogOperator("RPAREN", yytext, line_count);
			}
"{"         {
				printAndLogOperator("LCURL", yytext, line_count);
			}
"}"         {
				printAndLogOperator("RCURL", yytext, line_count);
			}
"["         {
				printAndLogOperator("LSQUARE", yytext, line_count);
			}
"]"         {
				printAndLogOperator("RSQUARE", yytext, line_count);
			}
","         {
				printAndLogOperator("COMMA", yytext, line_count);
			}
";"         {
				printAndLogOperator("SEMICOLON", yytext, line_count);
			}


{FOR_IDENTIFIER}	{
				printAndLogIdentifier("ID",yytext,line_count);

			}


{FOR_INTEGER}	{
				printAndLogKeyword("CONST_INT",yytext,line_count);
			}

{FOR_FLOAT}		{
				printAndLogKeyword("CONST_FLOAT",yytext,line_count);

			}
{FOR_CHARACTER_LITERALS}	{
				printAndLogCharacter("CONST_CHAR",yytext,line_count);

			}	
{WHITESPACE}	{

			}	


\"			{
				tokenfile_string.clear();
				logfile_string.clear();
				line_for_string=0;
				BEGIN SINGLE_LINE_STRING;
	
			}

\/\/		{
				comment.clear();
				comment += "//";

				line_for_string = 0;

				BEGIN SINGLE_LINE_COMMENT;
				
}	

\/\*		{
				comment.clear();
				comment += "/*";

				line_for_string = 0;

				BEGIN MULTI_LINE_COMMENT;
	
}	



<SINGLE_LINE_STRING>\\\\	{
				tokenfile_string += "\\";
				logfile_string +="\\\\";
			}
<SINGLE_LINE_STRING>\\\'	{
				tokenfile_string += "\'";
				logfile_string +="\\\'";
			}
<SINGLE_LINE_STRING>\\\"	{
				tokenfile_string += "\"";
				logfile_string +="\\\"";
			}

<SINGLE_LINE_STRING>\\a		{
				tokenfile_string += "\a";
				logfile_string +="\\a";
			}
<SINGLE_LINE_STRING>\\b		{
				tokenfile_string += "\b";
				logfile_string +="\\b";
			}
<SINGLE_LINE_STRING>\\f		{
				tokenfile_string += "\f";
				logfile_string +="\\f";
			}
<SINGLE_LINE_STRING>\\n		{
				tokenfile_string += "\n";
				logfile_string +="\\n";
			}	
<SINGLE_LINE_STRING>\\r		{
				tokenfile_string += "\r";
				logfile_string +="\\r";
			}
<SINGLE_LINE_STRING>\\t		{
				tokenfile_string += "\t";
				logfile_string +="\\t";
			}
<SINGLE_LINE_STRING>\\v		{
				tokenfile_string += "\v";
				logfile_string +="\\v";
			}
<SINGLE_LINE_STRING>\\0		{
				tokenfile_string += "\0";
				logfile_string +="\\0";
			}


<SINGLE_LINE_STRING>\\\r\n         { 
				line_for_string++;
				logfile_string += "\\\r\n" ;
				BEGIN MULTI_LINE_STRING;
			}
<SINGLE_LINE_STRING>\\\n         { 
				line_for_string++;
				logfile_string += "\\\n" ;
				BEGIN MULTI_LINE_STRING;
			}


<SINGLE_LINE_STRING>[\r]?\n { 
		line_for_string++;
		error_count++;


		///single line string, so when enter is pressed, error is in the prev line, so to 
		///print error , need not to add line_for_string....
		logfile_string='"'+logfile_string;
		
		printAndLogError("UNFINISHED_STRING",logfile_string);

		line_count += line_for_string;
		


		line_for_string = 0;

		BEGIN INITIAL;
	}
<SINGLE_LINE_STRING><<EOF>>      { 
		error_count++;

		logfile_string='"'+logfile_string;
	
		/// was a single line string, need not to increment line_count
		printAndLogError("UNFINISHED_STRING",logfile_string);

		
		line_for_string = 0;
		 
		BEGIN INITIAL;
	}

<SINGLE_LINE_STRING>\"           {
		printAndLogString("SINGLE LINE STRING");
		BEGIN INITIAL;
							
    }

<SINGLE_LINE_STRING>.            {
		tokenfile_string += yytext[0]; logfile_string+= yytext[0];
	}





<MULTI_LINE_STRING>\\\\	{
				tokenfile_string += "\\";
				logfile_string +="\\\\";
			}
<MULTI_LINE_STRING>\\\'	{
				tokenfile_string += "\'";
				logfile_string +="\\\'";
			}
<MULTI_LINE_STRING>\\\"	{
				tokenfile_string += "\"";
				logfile_string +="\\\"";
			}

<MULTI_LINE_STRING>\\a		{
				tokenfile_string += "\a";
				logfile_string +="\\a";
			}
<MULTI_LINE_STRING>\\b		{
				tokenfile_string += "\b";
				logfile_string +="\\b";
			}
<MULTI_LINE_STRING>\\f		{
				tokenfile_string += "\f";
				logfile_string +="\\f";
			}
<MULTI_LINE_STRING>\\n		{
				tokenfile_string += "\n";
				logfile_string +="\\n";
			}	
<MULTI_LINE_STRING>\\r		{
				tokenfile_string += "\r";
				logfile_string +="\\r";
			}
<MULTI_LINE_STRING>\\t		{
				tokenfile_string += "\t";
				logfile_string +="\\t";
			}
<MULTI_LINE_STRING>\\v		{
				tokenfile_string += "\v";
				logfile_string +="\\v";
			}
<MULTI_LINE_STRING>\\0		{
				tokenfile_string += "\0";
				logfile_string +="\\0";
			}


<MULTI_LINE_STRING>\\\r\n         { 
				line_for_string++;
				logfile_string += "\\\r\n" ;
				
			}
<MULTI_LINE_STRING>\\\n         { 
				line_for_string++;
				logfile_string += "\\\n" ;
				
			}


<MULTI_LINE_STRING>[\r]?\n { 
	
		error_count++;

		///at first, print the error, then increment line count for current newline.....
		logfile_string='"'+logfile_string;
		line_count += line_for_string;
		printAndLogError("UNFINISHED_STRING",logfile_string);
		
		line_count++;
		line_for_string = 0;

		BEGIN(INITIAL);
	}
<MULTI_LINE_STRING><<EOF>>      { 
		error_count++;

		logfile_string='"'+logfile_string;
		line_count += line_for_string;
		printAndLogError("UNFINISHED_STRING",logfile_string);

		
		line_for_string = 0;
		 
		BEGIN(INITIAL);
	}

<MULTI_LINE_STRING>\"           {
		printAndLogString("MULTI LINE STRING");
		BEGIN(INITIAL);
							
    }

<MULTI_LINE_STRING>.            {
		tokenfile_string += yytext[0]; logfile_string+= yytext[0];
	}





<SINGLE_LINE_COMMENT>\\\r\n		{ 
		line_for_string++;
		comment += "\\\r\n";	 
	}

<SINGLE_LINE_COMMENT>\\\n		{ 
		line_for_string++;
		comment += "\\\n";
	}

<SINGLE_LINE_COMMENT>[\r]?\n           { 

		line_for_string++;
		printAndLogComment("SINGLE LINE COMMENT");
		
		BEGIN(INITIAL);
	}
<SINGLE_LINE_COMMENT><<EOF>>      { 

		printAndLogComment("SINGLE LINE COMMENT");
		
		BEGIN(INITIAL);
	}

<SINGLE_LINE_COMMENT>\\           { comment += "\\";}
<SINGLE_LINE_COMMENT>.            { comment += yytext[0];} 


<MULTI_LINE_COMMENT>\*\/       { 
		
		comment += "*/";
		printAndLogComment("MULTI LINE COMMENT");
		
		BEGIN(INITIAL);
}

<MULTI_LINE_COMMENT><<EOF>>       { 

		error_count++;
		line_count += line_for_string;
		printAndLogError("UNFINISHED_COMMENT",comment);
		
		
		line_for_string = 0;
		
		BEGIN(INITIAL);
	}

<MULTI_LINE_COMMENT>\r            { comment += "\r";} 
<MULTI_LINE_COMMENT>\n            { line_for_string++; comment += "\n";} 
<MULTI_LINE_COMMENT>.            { comment += yytext[0];} 



{FOR_TOO_MANY_DECIMAL_POINTS}	{
		error_count++;
		printAndLogError("TOO_MANY_DECIMAL_POINTS",yytext);
	}
{FOR_ILLFORMED_NUMBER}	{
		error_count++;
		printAndLogError("ILLFORMED_NUMBER",yytext);
	}
{FOR_INVALID_ID_SUFFIX_NUM_PREFIX}	{
		error_count++;
		printAndLogError("INVALID_ID_SUFFIX_NUM_PREFIX",yytext);
	}
{FOR_MULTICHAR_CONST_CHAR}	{
		error_count++;
		printAndLogError("MULTICHAR_CONST_CHAR",yytext);
	}
{FOR_UNFINISHED_CONST_CHAR}		{
		error_count++;
		
		printAndLogError("UNFINISHED_CONST_CHAR",yytext);
	}
{FOR_EMPTY_CONST_CHAR}	{
		error_count++;
		printAndLogError("EMPTY_CONST_CHAR",yytext);
	}
{FOR_UNRECOGNIZED_CHAR}		{
		error_count++;
		printAndLogError("UNRECOGNIZED_CHAR",yytext);
	}

%%




int main(int argc, char** argv) {
	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}
	
	logout.open("1905064_log.txt", ios::out);
	tokenout.open("1905064_token.txt", ios::out);

	symbolTable=new SymbolTable(10);


	yyin= fin;
	yylex();
	fclose(yyin);

	symbolTable->print_all(logout);

	logout<<"Total lines: "<<line_count<<endl;
	logout<<"Total errors: "<<error_count<<endl;

	logout.close();
	tokenout.close();
	


	return 0;
}
