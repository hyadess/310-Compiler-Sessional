
#include "1905064_OutputStream.h"

void lexemeLog(int line_count, string token, string lexeme, ofstream &out)
{
    out << "Line# " << line_count << ": Token <" << token << "> Lexeme " << lexeme << " found" << endl;
}
void grammarRuleLog(int line_count, string lSide, string rSide, ofstream &out)
{
    out << lSide << " : " << rSide << " " << endl;
}
void errorPrint(string errMessage, ofstream &out)
{
    error_count++;
    out << "Line# " << line_count << ": " << errMessage << endl;
}
