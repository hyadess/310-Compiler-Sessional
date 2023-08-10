#ifndef BACKPATCH
#define BACKPATCH

#include <bits/stdc++.h>
using namespace std;
#include "1905064_SymbolTable.h"
#include "1905064_ICG_GeneratorFunctions.h"
extern int codeLineCount;
extern map<int, string> labelMap;
extern SymbolTable *symbolTable;
vector<int> *mergeList(vector<int> *firstList, vector<int> *secondList);

void backPatch(vector<int> *list, string label);

void needValueCode(SymbolInfo *expression, bool beforePOP = false);

void needListCode(SymbolInfo *expression);

void forLOGICOP(SymbolInfo *logicExpression, SymbolInfo *firstExpression, SymbolInfo *LOGICOP,
                SymbolInfo *label, SymbolInfo *secondExpression);

void forRELOP(SymbolInfo *relExpression, SymbolInfo *firstSimExp, SymbolInfo *relop, SymbolInfo *secondSimExp);

// inside unary_expression.......................................

void forUnaryNot(SymbolInfo *ans, SymbolInfo *NOT, SymbolInfo *unaryExp);

void forUnaryADDOP(SymbolInfo *ans, SymbolInfo *ADDOP, SymbolInfo *unaryExp);

void ifStatement(SymbolInfo *ans, SymbolInfo *p);

void ifElseStatement(SymbolInfo *ans, SymbolInfo *p, SymbolInfo *jmpCode, SymbolInfo *secondLabel, SymbolInfo *secondStatement);
void whileStatement(SymbolInfo *ans, SymbolInfo *firstLabel, SymbolInfo *expression, SymbolInfo *secondLabel, SymbolInfo *statement);

void forRETURN(SymbolInfo *expression);
void forPRINTLN(SymbolInfo *id);

#endif BACKPATCH