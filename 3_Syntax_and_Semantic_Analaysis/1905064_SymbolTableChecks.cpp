

#include "1905064_SymbolTableChecks.h"
#include "1905064_OutputStream.h"
void declarationInsertion(SymbolTable *symbolTable, SymbolInfo *symbol, ofstream &out)
{
    bool isInserted = symbolTable->Insert(symbol);
    if (!isInserted)
    {
        SymbolInfo *foundSymbol = symbolTable->lookUp(symbol->get_name());
        if (!foundSymbol->isFunctionDeclaration())
        {
            errorPrint("'" + (string)foundSymbol->get_name() + "' redeclared as different kind of symbol", out);
        }
        else
        {
            int flag = 0;
            if (foundSymbol->getVarType() != symbol->getVarType())
                flag = 1;
            if (foundSymbol->getParameterArray().size() != symbol->getParameterArray().size())
                flag = 1;
            else
            {
                vector<ParameterInfo *> prevParamList = symbol->getParameterArray();
                vector<ParameterInfo *> checkParamList = foundSymbol->getParameterArray();
                for (int i = 0; i < prevParamList.size(); i++)
                {
                    if (prevParamList[i]->getType() != checkParamList[i]->getType())
                    {
                        flag = 1;
                    }
                }
            }
            if (flag == 1)
            {
                errorPrint((string) "Conflicting types for '" + (string)symbol->get_name() + (string) "'", out);
            }
        }
    }
}

void declarationInsertionVar(SymbolTable *symbolTable, SymbolInfo *symbol, ofstream &out)
{
    bool isInserted = symbolTable->Insert(symbol);
    if (!isInserted)
    {

        if (symbol->isArray())
        {
            SymbolInfo *foundSymbol = symbolTable->lookUp(symbol->get_name());
            if (!foundSymbol->isArray())
            {
                errorPrint("'" + (string)foundSymbol->get_name() + "' redeclared as different kind of symbol", out);
            }
            else if (foundSymbol->getVarType() != symbol->getVarType())
            {
                errorPrint((string) "Conflicting types for '" + (string)symbol->get_name() + (string) "'", out);
            }
        }
        else
        {
            SymbolInfo *foundSymbol = symbolTable->lookUp(symbol->get_name());

            if (foundSymbol->getVarType() != symbol->getVarType())
            {
                errorPrint((string) "Conflicting types for '" + (string)symbol->get_name() + (string) "'", out);
            }
            else if (foundSymbol->isFunctionDeclaration() || foundSymbol->isFunctionDefinition())
            {
                errorPrint("'" + (string)foundSymbol->get_name() + "' redeclared as different kind of symbol", out);
            }
        }
    }
}

void insertFunctionDefinition(SymbolTable *symbolTable, SymbolInfo *currentParameterList, SymbolInfo *functionName, SymbolInfo *typeSpecifier, ofstream &out)
{

    functionName->setVarType(typeSpecifier->getVarType());
    // currentParameterList=new SymbolInfo() sets the parameters of the function

    functionName->setParameterArray(currentParameterList->getParameterArray());
    functionName->setAsFunctionDefinition();
    // currentParameterList=new SymbolInfo()->setParameterArray({});

    bool inserted = symbolTable->Insert(functionName);
    if (inserted)
        return;

    // if not inserted there is already a definition or declaration or other of the same name.........
    auto prevDeclaration = symbolTable->lookUp(functionName->get_name());

    // already declared once..................
    if (prevDeclaration->isFunctionDeclaration())
    {
        if (prevDeclaration->getVarType() != functionName->getVarType())
        {
            errorPrint("Conflicting types for '" + (string)functionName->get_name() + (string) "'", out);
            return;
        }

        if (functionName->getParameterArray().size() != prevDeclaration->getParameterArray().size())
        {
            errorPrint("Conflicting types for '" + (string)functionName->get_name() + (string) "'", out);
            return;
        }

        // for non-void functions
        if (functionName->getParameterArray().size() != 0)
        {
            vector<ParameterInfo *> prevParamList = prevDeclaration->getParameterArray();
            vector<ParameterInfo *> checkParamList = functionName->getParameterArray();
            for (int i = 0; i < prevParamList.size(); i++)
            {
                if (prevParamList[i]->getType() != checkParamList[i]->getType())
                {
                    errorPrint((string) "conflicting argument types for argument " + to_string(i + 1), out);
                    return;
                }
            }
        }
    }

    else if (prevDeclaration->isFunctionDefinition())
    {
        errorPrint("re-definition of function '" + (string)functionName->get_name() + (string) "'", out);
    }
    else
    {
        errorPrint((string) "'" + (string)functionName->get_name() + (string) "' not a function", out);
    }
}

void insertVarDeclaration(SymbolTable *symbolTable, SymbolInfo *typeSpecifier, SymbolInfo *declarationList, ofstream &out)
{
    if (!isVoidTypeSpecifier(typeSpecifier, out))
    {
        for (auto parameterInfo : declarationList->getDeclarationList())
        {
            parameterInfo->setType(typeSpecifier->getVarType());
            SymbolInfo *varSymbolInfo = new SymbolInfo(parameterInfo->getName(), parameterInfo->getType());
            if (parameterInfo->isArray() == true)
                varSymbolInfo->setAsArray();
            varSymbolInfo->setVarType(parameterInfo->getType());

            declarationInsertionVar(symbolTable, varSymbolInfo, out);
        }
    }
}
