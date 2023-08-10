#ifndef CHECKER
#define CHECKER

#include <bits/stdc++.h>
using namespace std;

#include "1905064_HelperFunctions.h"

void declarationInsertion(SymbolTable *symbolTable, SymbolInfo *symbol, ofstream &out);
void declarationInsertionVar(SymbolTable *symbolTable, SymbolInfo *symbol, ofstream &out);
void insertFunctionDefinition(SymbolTable *symbolTable, SymbolInfo *currentParameterList, SymbolInfo *functionName, SymbolInfo *typeSpecifier, ofstream &out);

void insertVarDeclaration(SymbolTable *symbolTable, SymbolInfo *typeSpecifier, SymbolInfo *declarationList, ofstream &out);

#endif