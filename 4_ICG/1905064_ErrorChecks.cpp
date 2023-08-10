#include "1905064_ErrorChecks.h"

void functionCallCheck(SymbolTable *symbolTable, SymbolInfo *factor, SymbolInfo *id, SymbolInfo *argumentList, ofstream &out)
{
    auto foundSymbol = symbolTable->lookUp(id->get_name());
    if (foundSymbol == NULL)
    {
        errorPrint("Undeclared function '" + (string)id->get_name() + "'", out);
        id->setVarType("error");
        factor->setVarType(id->getVarType());
    }
    else
    {
        id->setVarType(foundSymbol->getVarType());
        factor->setVarType(foundSymbol->getVarType());
        if (!foundSymbol->isFunctionDeclaration() and !foundSymbol->isFunctionDefinition())
        {
            errorPrint("'" + (string)id->get_name() + "' is not a function", out);
            id->setVarType("error");
        }
        else if (argumentList->getParameterArray().size() < foundSymbol->getParameterArray().size())
        {
            errorPrint("Too few arguments to function '" + (string)id->get_name() + "'", out);
            id->setVarType("error");
        }
        else if (argumentList->getParameterArray().size() > foundSymbol->getParameterArray().size())
        {
            errorPrint("Too many arguments to function '" + (string)id->get_name() + "'", out);
            id->setVarType("error");
        }
        else
        {
            auto callerParameterList = argumentList->getParameterArray();
            auto calledParameterList = foundSymbol->getParameterArray();
            for (int i = 0; i < callerParameterList.size(); i++)
            {
                if (callerParameterList[i]->getType() != calledParameterList[i]->getType())
                {
                    errorPrint((string) "Type mismatch for argument " + to_string(i + 1) + " of '" + (string)id->get_name() + (string) "'", out);
                    id->setVarType("error");
                }

                else if (callerParameterList[i]->isArray() and !calledParameterList[i]->isArray())
                {
                    string p = (string)id->get_name() + (string) " is called as an array which it is not";
                    id->setVarType("error");
                    errorPrint(p, out);
                }
                else if (!callerParameterList[i]->isArray() and calledParameterList[i]->isArray())
                {
                    errorPrint((string)id->get_name() + (string) " is not called as an array which it is", out);
                    id->setVarType("error");
                }
            }
        }
        factor->setVarType(id->getVarType());
    }
}

void variableCheck(SymbolTable *symbolTable, SymbolInfo *variable, SymbolInfo *id, ofstream &out)
{
    auto foundSymbol = symbolTable->lookUp(id->get_name());
    if (!foundSymbol)
    {
        errorPrint("Undeclared variable '" + (string)id->get_name() + (string) "'", out);
        variable->setVarType("error");
    }
    else if (foundSymbol->isArray())
    {
        string p = (string) "'" + id->get_name() + (string) "' is an array";
        errorPrint(p, out);
        variable->setVarType("error");
    }
    else if (foundSymbol->isFunctionDeclaration() || foundSymbol->isFunctionDefinition())
    {
        string p = (string) "'" + id->get_name() + (string) "' is a function";
        errorPrint(p, out);
        variable->setVarType("error");
    }
    else
    {

        variable->setVarType(foundSymbol->getVarType());
        if (foundSymbol->isArray())
            variable->setAsArray();
        ///////////////////////////////////////////
        variable->setAssemblyName(foundSymbol->getAssemblyName());
    }
}

void arrayCheck(SymbolTable *symbolTable, SymbolInfo *variable, SymbolInfo *id, SymbolInfo *index, ofstream &out)
{
    auto foundSymbol = symbolTable->lookUp(id->get_name());
    if (!foundSymbol)
    {
        errorPrint("Undeclared variable '" + (string)id->get_name() + (string) "'", out);
        variable->setVarType("error");
    }
    else if (!foundSymbol->isArray())
    {
        string p = (string) "'" + id->get_name() + (string) "' is not an array";
        errorPrint(p, out);
        variable->setVarType("error");
    }
    else if (foundSymbol->isFunctionDefinition() || foundSymbol->isFunctionDeclaration())
    {
        string p = (string) "'" + id->get_name() + (string) "' is a function";
        errorPrint(p, out);
        variable->setVarType("error");
    }
    else
    {
        variable->setVarType(foundSymbol->getVarType());
        if (foundSymbol->isArray())
            variable->setAsArray();

        ////////////////////////////////////////
        VarCallForArray(variable, foundSymbol, index);
    }
    if (index->getVarType() != "CONST_INT" and index->getVarType() != "INT" and index->getVarType() != "error")
    {
        errorPrint("Array subscript is not an integer", out);
    }
}

void MULOP_check(SymbolInfo *term, SymbolInfo *operation, SymbolInfo *firstOperand, SymbolInfo *secondOperand, ofstream &out)
{
    term->setVarType(typeCast(firstOperand, secondOperand));
    if (secondOperand->getVarType() == "VOID")
    {
        errorPrint("Void cannot be used in expression", out);
    }
    else if (operation->get_name() == "%" and secondOperand->get_name() == "0")
    {
        errorPrint("Warning: division by zero", out);
        term->setVarType("error");
    }
    else if (operation->get_name() == "%" and (firstOperand->getVarType() != "INT" or secondOperand->getVarType() != "INT"))
    {
        errorPrint("Operands of modulus must be integers", out);
        term->setVarType("error");
    }
}

void ASSIGN_checks(SymbolInfo *rightOperand, SymbolInfo *leftOperand, ofstream &out)
{
    if (rightOperand->getVarType() == "VOID")
    {
        errorPrint("Void cannot be used in expression", out);
    }
    else if (leftOperand->getVarType() == "FLOAT" and rightOperand->getVarType() == "INT")
    {
        // itscool
    }
    else if (leftOperand->getVarType() != rightOperand->getVarType() and
             rightOperand->getVarType() != "error" and leftOperand->getVarType() != "error")
    {
        // out<<leftOperand->getVarType()<<endl;
        errorPrint("Warning: possible loss of data in assignment of FLOAT to INT", out);
    }
}