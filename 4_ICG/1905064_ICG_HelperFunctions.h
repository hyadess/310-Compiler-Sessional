#ifndef ICG_HELPER
#define ICG_HELPER

#include <bits/stdc++.h>
using namespace std;
#include "1905064_SymbolTable.h"
#include "1905064_ICG_GeneratorFunctions.h"
#include "1905064_backpatchingHelper.h"

void genPrintNewLine();

void genMyFunctionForPrintln();

void genFunctionForPrintln();

void openFiles();

void optimizeCode();

void generateCodeDotAsmFile();

void VarCallForArray(SymbolInfo *varInfo, SymbolInfo *arrayId, SymbolInfo *expression);

#endif ICG_HELPER