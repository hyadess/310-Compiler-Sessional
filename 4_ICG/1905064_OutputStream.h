#ifndef OUTPUT
#define OUTPUT

#include <bits/stdc++.h>
using namespace std;
extern int line_count;
extern int error_count;
void lexemeLog(int line_count, string token, string lexeme, ofstream &out);

void grammarRuleLog(int line_count, string lSide, string rSide, ofstream &out);

void errorPrint(string errMessage, ofstream &out);

#endif