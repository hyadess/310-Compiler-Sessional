#include <bits/stdc++.h>
using namespace std;
#include "1905064_SymbolTable.h"
#include "1905064_OutputStream.h"
#include "1905064_HelperFunctions.h"
void functionCallCheck(SymbolTable *symbolTable, SymbolInfo *factor, SymbolInfo *id, SymbolInfo *argumentList, ofstream &out);

void variableCheck(SymbolTable *symbolTable, SymbolInfo *variable, SymbolInfo *id, ofstream &out);

void arrayCheck(SymbolTable *symbolTable, SymbolInfo *variable, SymbolInfo *id, SymbolInfo *index, ofstream &out);

void MULOP_check(SymbolInfo *term, SymbolInfo *operation, SymbolInfo *firstOperand, SymbolInfo *secondOperand, ofstream &out);

void ASSIGN_checks(SymbolInfo *rightOperand, SymbolInfo *leftOperand, ofstream &out);