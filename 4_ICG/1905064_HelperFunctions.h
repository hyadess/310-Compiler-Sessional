#ifndef HELPER
#define HELPER
#include <bits/stdc++.h>
using namespace std;

#include "1905064_SymbolTable.h"
#include "1905064_OutputStream.h"
string concatElements(vector<SymbolInfo *> symbolInfos);

string typeCast(SymbolInfo *lSide, SymbolInfo *rSide);

bool isVoidTypeSpecifier(SymbolInfo *typeSpecifier, SymbolInfo *id, ofstream &out);
bool isVoidTypeSpecifier(SymbolInfo *typeSpecifier, ofstream &out);
SymbolInfo *parameterToSymbol(ParameterInfo *parameterInfo);
#endif
