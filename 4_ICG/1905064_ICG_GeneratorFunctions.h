#ifndef ICG_GENERATOR
#define ICG_GENERATOR

#include <bits/stdc++.h>
using namespace std;
#include "1905064_backpatchingHelper.h"
#include "1905064_SymbolTable.h"
extern int labelTagNo;
extern int line_count;
extern ofstream dataSegOut;
extern ofstream codeSegOut;
extern ofstream logout;
extern ofstream errout;
extern ofstream parsetree;
extern string mainFuncTerminateLabel;
extern bool isMain;

extern SymbolTable *symbolTable;
extern int codeLineCount;

string newLabel();

string newLabelO();

void addCommentln(string cmd);

void genCode(string asmCode, string cmnt = "");

void genCodeln(string asmCode, string cmnt = "");

string getVarAddress(SymbolInfo *var, bool pop = false);

void generateFuncStartCode(string funcName);

void generateFuncEndCode(string funcName);

//////////////////////////////////////////////////////////////////////////////////////

string assemblyNameForArray(SymbolInfo *var, int index);

void generateVarDecCode(SymbolInfo *varInfo, bool globalScope = false);

void genUnaryOerationCode(SymbolInfo *info, bool inc = true);

string relopToJumpIns(string relop);

//////////////////////////////////////////////////////////////////////////////////////
void genAddCode(SymbolInfo *simple_expression, SymbolInfo *ADDOP, SymbolInfo *term);

void genMulCode(SymbolInfo *term, SymbolInfo *MULOP, SymbolInfo *unaryExpression);

// after assigning, we are pushing assigned value into stack........
void getAssignCode(SymbolInfo *variable, SymbolInfo *logic_expression);

#endif ICG_GENERATOR