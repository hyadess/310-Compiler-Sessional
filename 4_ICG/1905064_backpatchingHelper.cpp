#include "1905064_backpatchingHelper.h"

vector<int> *mergeList(vector<int> *firstList, vector<int> *secondList)
{

    vector<int> *newVector = new vector<int>();
    for (auto it = firstList->begin(); it != firstList->end(); ++it)
    {
        newVector->push_back(*it);
    }
    for (auto it = secondList->begin(); it != secondList->end(); ++it)
    {
        newVector->push_back(*it);
    }

    return newVector;
}

void backPatch(vector<int> *list, string label)
{
    if (list == nullptr)
        return;

    int n = list->size();
    for (auto it = list->begin(); it != list->end(); ++it)
    {
        int p = *it;
        labelMap.insert({p, label});
    }
}

void needValueCode(SymbolInfo *expression, bool beforePOP) // boolean ->  stack
{

    if (expression->isBoolean())
    {
        string trueLabel = newLabel();
        string falseLabel = newLabel();
        string nextLabel = newLabel();
        backPatch(expression->getTrueList(), trueLabel);
        backPatch(expression->getFalseList(), falseLabel);
        genCodeln(trueLabel + ":");
        if (beforePOP)
            genCodeln("POP AX");
        genCodeln("\t\tPUSH 1");
        genCodeln("\t\tJMP " + nextLabel);
        genCodeln(falseLabel + ":");
        if (beforePOP)
            genCodeln("POP AX");
        genCodeln("\t\tPUSH 0");
        genCodeln(nextLabel + ":");
        if (beforePOP)
            genCodeln("PUSH AX");
    }
}

void needListCode(SymbolInfo *expression) // STACK -> create true false list
{
    if (!expression->isBoolean())
    {
        genCodeln("\t\tPOP AX");
        genCodeln("\t\tCMP AX, 0");
        genCodeln("\t\tJE ");
        expression->addToFalseList(codeLineCount);
        genCodeln("\t\tJMP ");
        expression->addToTrueList(codeLineCount);
    }
}

void forLOGICOP(SymbolInfo *logicExpression, SymbolInfo *firstExpression, SymbolInfo *LOGICOP,
                SymbolInfo *label, SymbolInfo *secondExpression)
{

    if (firstExpression->isBoolean() != true || secondExpression->isBoolean() != true)
    {

        if (firstExpression->isBoolean() == true)
        {

            needValueCode(firstExpression, 1);
        }
        else if (secondExpression->isBoolean() == true)
        {
            needValueCode(secondExpression, 0);
        }

        string code = firstExpression->get_name() + LOGICOP->get_name() + secondExpression->get_name();
        genCodeln("\t\tPOP BX", "load " + secondExpression->get_name());
        genCodeln("\t\tPOP AX", "load " + firstExpression->get_name());
        if (LOGICOP->get_name() == "||")
        {

            genCodeln("\t\tCMP AX, 1");
            genCodeln("\t\tJE ");
            logicExpression->addToTrueList(codeLineCount);
            genCodeln("\t\tCMP BX, 1");
            genCodeln("\t\tJE ");
            logicExpression->addToTrueList(codeLineCount);
            genCodeln("\t\tJMP ");
            logicExpression->addToFalseList(codeLineCount);
        }
        else
        {
            genCodeln("\t\tCMP AX, 0");
            genCodeln("\t\tJE ");
            logicExpression->addToFalseList(codeLineCount);
            genCodeln("\t\tCMP BX, 0");
            genCodeln("\t\tJE ");
            logicExpression->addToFalseList(codeLineCount);
            genCodeln("\t\tJMP ");
            logicExpression->addToTrueList(codeLineCount);
        }
    }
    else
    {
        if (LOGICOP->get_name() == "||")
        {

            backPatch(firstExpression->getFalseList(), label->getLabel());
            logicExpression->setTrueList(mergeList(firstExpression->getTrueList(), secondExpression->getTrueList()));
            logicExpression->setFalseList(secondExpression->getFalseList());
        }
        else if (LOGICOP->get_name() == "&&")
        {
            backPatch(firstExpression->getTrueList(), label->getLabel());
            logicExpression->setTrueList(secondExpression->getTrueList());
            logicExpression->setFalseList(mergeList(firstExpression->getFalseList(), secondExpression->getFalseList()));
        }
    }
}

void forRELOP(SymbolInfo *relExpression, SymbolInfo *firstSimExp, SymbolInfo *relop, SymbolInfo *secondSimExp)
{
    if (firstSimExp->isBoolean() == true && secondSimExp->isBoolean() == true)
    {
        // at first, push boolean values to AX or/and BX

        if (firstSimExp->isBoolean())
        {
            needValueCode(firstSimExp, 0);
        }
        if (secondSimExp->isBoolean())
            needValueCode(secondSimExp, 0);
    }
    else if (firstSimExp->isBoolean() == true)
    {
        needValueCode(firstSimExp, 1);
    }
    else if (secondSimExp->isBoolean() == true)
    {
        needValueCode(secondSimExp, 0);
    }

    string code = firstSimExp->get_name() + relop->get_name() + secondSimExp->get_name();
    genCodeln("\t\tPOP BX", "load " + secondSimExp->get_name());
    genCodeln("\t\tPOP AX", "load " + firstSimExp->get_name());
    genCodeln("\t\tCMP AX, BX");
    string op = relopToJumpIns(relop->get_name());
    genCodeln("\t\t" + op + " ");
    relExpression->addToTrueList(codeLineCount);
    genCodeln("\t\tJMP ");
    relExpression->addToFalseList(codeLineCount);
}

// inside unary_expression.......................................

void forUnaryNot(SymbolInfo *ans, SymbolInfo *NOT, SymbolInfo *unaryExp)
{
    if (unaryExp->isBoolean())
    {
        ans->setTrueList(unaryExp->getFalseList());
        ans->setFalseList(unaryExp->getTrueList());
    }
    else
    {
        genCodeln("\t\tPOP AX\t\t;load " + unaryExp->get_name());
        genCodeln("\t\tCMP AX, 0");
        genCodeln("\t\tJE ");
        ans->addToFalseList(codeLineCount);

        genCodeln("\t\t\tJMP ");
        ans->addToTrueList(codeLineCount);
    }
}

void forUnaryADDOP(SymbolInfo *ans, SymbolInfo *ADDOP, SymbolInfo *unaryExp)

{
    needValueCode(unaryExp, 0);
    if (ADDOP->get_name() == "-")
    {
        // addCommentln("-" + unaryExp->get_name());
        genCodeln("\t\tPOP AX");
        genCodeln("\t\tNEG AX");
        genCodeln("\t\tPUSH AX");
    }
}

// statements.............................................

void ifStatement(SymbolInfo *ans, SymbolInfo *p)
{

    backPatch(p->getTrueList(), p->getLabel());
    ans->setNextList(mergeList(p->getFalseList(), p->getNextList()));

    // for (auto it = p->getTrueList()->begin(); it != p->getTrueList()->end(); ++it)
    // {
    //     dataSegOut << *it << endl
    //                << endl;
    // }
    // for (auto it = p->getFalseList()->begin(); it != p->getFalseList()->end(); ++it)
    // {
    //     dataSegOut << *it << endl;
    // }
}

void ifElseStatement(SymbolInfo *ans, SymbolInfo *p, SymbolInfo *jmpCode, SymbolInfo *secondLabel, SymbolInfo *secondStatement)
{

    backPatch(p->getTrueList(), p->getLabel());
    backPatch(p->getFalseList(), secondLabel->getLabel());
    ans->setNextList(mergeList(mergeList(p->getNextList(), jmpCode->getNextList()), secondStatement->getNextList()));
}

void whileStatement(SymbolInfo *ans, SymbolInfo *firstLabel, SymbolInfo *expression, SymbolInfo *secondLabel, SymbolInfo *statement)
{
    backPatch(statement->getNextList(), firstLabel->getLabel());
    backPatch(expression->getTrueList(), secondLabel->getLabel());
    ans->setNextList(expression->getFalseList());
    genCodeln("\t\tJMP " + firstLabel->getLabel());
}

void forRETURN(SymbolInfo *expression)
{
    needValueCode(expression, 0);
    if (!isMain)
    {
        // pop value to be returned from the top of the stack to AX
        genCodeln("\t\tPOP AX", "load " + expression->get_name());
        genCodeln("\t\tMOV [BP+4], AX"); // move value from AX to return value location
        // remove all the local variable decalared in the function scope
        // addCommentln("removing all local variables from the stack");
        genCodeln("\t\tADD SP, " + to_string(symbolTable->getVarCount() * 2));
        genCodeln("\t\tPOP BP"); // restore BP for the caller function
        genCodeln("\t\tRET");    // return control to the caller funcdtion
    }
}

void forPRINTLN(SymbolInfo *id)
{
    SymbolInfo *foundSymbol = symbolTable->lookUp(id->get_name());
    if (foundSymbol != NULL)
    {
        // addCommentln("println(" + id->get_name() + ")");
        genCodeln("\t\tMOV BX, " + foundSymbol->getAssemblyName(), "load " + id->get_name());
        genCodeln("\t\tCALL PRINT_NUM_FROM_BX");
    }
}
