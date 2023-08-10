
#include "1905064_HelperFunctions.h"

string concatElements(vector<SymbolInfo *> symbolInfos)
{
    string name = "";
    for (auto symbolInfo : symbolInfos)
    {
        name += symbolInfo->get_name();
        if (symbolInfo->get_type() == "TYPE_SPECIFIER" or symbolInfo->get_type() == "KEYWORD")
            name += " ";
        else if (symbolInfo->get_name() == ";" or symbolInfo->get_name() == "{")
            name += "\n";
        else if (symbolInfo->get_name() == "}")
            name += "\n\n";
    }
    return name;
}
string typeCast(SymbolInfo *lSide, SymbolInfo *rSide)
{
    string leftType = lSide->getVarType();
    string rightType = rSide->getVarType();
    if (leftType == "VOID" or rightType == "VOID")
        return "error"; // won't occur :/
    if (leftType == "error" or rightType == "error")
        return "error";
    if (leftType == "FLOAT" or rightType == "FLOAT")
        return "FLOAT";
    return "INT";
}
bool isVoidTypeSpecifier(SymbolInfo *typeSpecifier, SymbolInfo *id, ofstream &out)
{
    if (typeSpecifier->getVarType() == "VOID")
    {
        errorPrint("Variable or field '" + (string)id->get_name() + "' declared void", out);
        return true;
    }
    return false;
}
bool isVoidTypeSpecifier(SymbolInfo *typeSpecifier, ofstream &out)
{
    if (typeSpecifier->getVarType() == "VOID")
    {
        errorPrint("Variable or field declared void", out);
        return true;
    }
    return false;
}
SymbolInfo *parameterToSymbol(ParameterInfo *parameterInfo)
{
    SymbolInfo *s = new SymbolInfo(parameterInfo->getName(), parameterInfo->getType()); ///////////////
    s->setVarType(parameterInfo->getType());
    if (parameterInfo->isArray())
        s->setAsArray();

    return s;
}