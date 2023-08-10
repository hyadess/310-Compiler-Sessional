#ifndef CHECKER
#define CHECKER

#include <bits/stdc++.h>
using namespace std;

#include "1905064_HelperFunctions.h"
#include "1905064_OutputStream.h"
void declarationInsertion(SymbolTable *symbolTable, SymbolInfo *symbol, ofstream &out, int offset = -1);
void declarationInsertionVar(SymbolTable *symbolTable, SymbolInfo *symbol, ofstream &out);
void insertFunctionDefinition(SymbolTable *symbolTable, SymbolInfo *currentParameterList, SymbolInfo *functionName, SymbolInfo *typeSpecifier, ofstream &out);

void insertVarDeclaration(SymbolTable *symbolTable, SymbolInfo *typeSpecifier, SymbolInfo *declarationList, ofstream &out);

#endif