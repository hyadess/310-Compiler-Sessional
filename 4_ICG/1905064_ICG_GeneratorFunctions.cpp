#include "1905064_ICG_GeneratorFunctions.h"

string newLabel()
{
    return "L" + to_string(labelTagNo++);
}

string newLabelO()
{
    return "INNER_" + to_string(labelTagNo++);
}

void addCommentln(string cmd)
{
    codeSegOut << "; line " << line_count << ": " << cmd << endl;
    codeLineCount++;
}

void genCode(string asmCode, string cmnt)
{
    codeSegOut << asmCode << "\t";
    string lineNo = ";line " + to_string(line_count) + ": ";
    codeSegOut << ((cmnt == "") ? "" : lineNo) << cmnt;
}

void genCodeln(string asmCode, string cmnt)
{
    genCode(asmCode, cmnt);
    codeSegOut << endl;
    codeLineCount++;
}

string getVarAddress(SymbolInfo *var, bool pop)
{
    if (pop)
    {
        if (var->isArray() && !var->isGlobal())
            genCodeln("\t\tPOP BX");
    }
    return var->getAssemblyName();
}

void generateFuncStartCode(string funcName)
{
    codeSegOut << "\t" << funcName << " PROC\n";
    codeLineCount++;
    if (funcName == "main")
    {
        mainFuncTerminateLabel = newLabelO();
        codeSegOut << "\t\tMOV AX, @DATA\n";
        codeSegOut << "\t\tmov DS, AX\n";
        codeSegOut << "\t\t; data segment loaded\n";
        codeLineCount += 3;
        isMain = true;
    }
    else
    {
        isMain = false;
        genCodeln("\t\tPUSH BP"); // SAVE BP
    }
    genCodeln("\t\tMOV BP, SP");
}

void generateFuncEndCode(string funcName)
{
    if (funcName == "main")
    {
        codeSegOut << "\t\t" << mainFuncTerminateLabel << ":" << endl;
        codeSegOut << "\t\tMOV AH, 4CH" << endl;
        codeSegOut << "\t\tINT 21H" << endl;
        codeLineCount += 3;
    }
    else
    {
        // addCommentln("For the case of not returning from a function");
        genCodeln("\t\tPOP BP");
        codeSegOut << "\t\tRET\n";
        isMain = true;
        codeLineCount += 1;
    }
    codeSegOut << "\t" << funcName << " ENDP\n";
    codeLineCount++;
}

//////////////////////////////////////////////////////////////////////////////////////

string assemblyNameForArray(SymbolInfo *var, int index)
{
    if (var->isGlobal())
    {
        return var->getAssemblyName() + "[SI+" + to_string(index) + "]"; // like ARR[SI+2] for 3rd element
    }

    else
    {
        int offset = var->getArrayStarting() + index * 2;
        return "[BP-" + to_string(offset) + "]"; // array starts at [BP-2]...so arraystart is 2, 2nd element is at
                                                 // [BP-4] and so on
    }
}

void generateVarDecCode(SymbolInfo *varInfo, bool globalScope)
{
    if (globalScope)
    {
        dataSegOut << "\t" << varInfo->get_name();

        if (varInfo->isArray())
        {
            dataSegOut << " DW " << varInfo->getSize();
            dataSegOut << " DUP(" << 0 << ")";
            dataSegOut << "\t\t; array " << varInfo->get_name() << " declared";
        }
        else
        {
            dataSegOut << " DW 0   \t\t\t; variable " << varInfo->get_name() << " declared";
        }
        varInfo->setAssemblyName(varInfo->get_name(), true); // global true
        dataSegOut << endl;
    }
    else
    {
        if (varInfo->isArray())
        {
            int n = varInfo->getSize();
            int arrayStart = ((symbolTable->getVarCount() + 1) * 2);
            string baseAddress = "W. [BP-" + to_string(arrayStart) + "]";
            varInfo->setAssemblyName(baseAddress, false, arrayStart);
            symbolTable->addToVarCount(n);
            codeSegOut << "\t\tSUB SP, " << (n * 2) << "\t";
            // comment
            codeSegOut << ";line " << line_count << ": ";
            codeSegOut << "array " << varInfo->get_name();
            codeSegOut << " of size " << n << " declared\t\t";
            codeSegOut << "\t\t; from " << assemblyNameForArray(varInfo, 0);
            codeSegOut << " to " << assemblyNameForArray(varInfo, n - 1) << endl;
            codeLineCount++;
        }
        else
        {
            symbolTable->addToVarCount(1);
            varInfo->setAssemblyName("W. [BP-" + to_string(2 * symbolTable->getVarCount()) + "]");
            codeSegOut << "\t\tSUB SP, 2\t";
            // comment
            codeSegOut << ";line " << line_count << ": ";
            codeSegOut << varInfo->get_name() << " declared: ";
            codeSegOut << varInfo->getAssemblyName() << "\n";
            codeLineCount++;
        }
    }
}

void genUnaryOerationCode(SymbolInfo *info, bool inc)
{
    string op = inc ? "INC" : "DEC";
    string address = getVarAddress(info, true);
    genCodeln("\t\tPUSH " + address);
    genCodeln("\t\t" + op + " " + address, info->get_name() + (inc ? "++" : "--"));
}

string relopToJumpIns(string relop)
{
    if (relop == "<")
        return "JL";
    if (relop == "<=")
        return "JLE";
    if (relop == ">")
        return "JG";
    if (relop == ">=")
        return "JGE";
    if (relop == "==")
        return "JE";
    if (relop == "!=")
        return "JNE";
}

//////////////////////////////////////////////////////////////////////////////////////
void genAddCode(SymbolInfo *simple_expression, SymbolInfo *ADDOP, SymbolInfo *term)
{
    if (simple_expression->isBoolean() == true && term->isBoolean() == true)
    {
        // at first, push boolean values to AX or/and BX

        if (simple_expression->isBoolean())
        {
            needValueCode(simple_expression, 0);
        }
        if (term->isBoolean())
            needValueCode(term, 0);
    }
    else if (simple_expression->isBoolean() == true)
    {

        needValueCode(simple_expression, 1);
    }
    else if (term->isBoolean() == true)
    {
        needValueCode(term, 0);
    }
    genCodeln("\t\tPOP BX", "load " + term->get_name());
    genCodeln("\t\tPOP AX", "load " + simple_expression->get_name());
    string op = ADDOP->get_name() == "+" ? "ADD" : "SUB";
    genCodeln("\t\t" + op + " AX, BX");
    genCodeln("\t\tPUSH AX", "save " + simple_expression->get_name());
}

void genMulCode(SymbolInfo *term, SymbolInfo *MULOP, SymbolInfo *unaryExpression)
{
    if (term->isBoolean() == true && unaryExpression->isBoolean() == true)
    {
        // at first, push boolean values to AX or/and BX

        if (term->isBoolean())

            needValueCode(term, 0);

        if (unaryExpression->isBoolean())
            needValueCode(unaryExpression, 0);
    }
    else if (term->isBoolean() == true)
    {

        needValueCode(term, 1);
    }
    else if (unaryExpression->isBoolean() == true)
    {
        needValueCode(unaryExpression, 0);
    }
    string code = term->get_name() + MULOP->get_name() + unaryExpression->get_name();
    genCodeln("\t\tPOP BX", "load " + unaryExpression->get_name());
    genCodeln("\t\tPOP AX", "load " + term->get_name());
    genCodeln("\t\tXOR DX, DX", "clear DX");
    string op = MULOP->get_name() == "*" ? "IMUL" : "IDIV";
    genCodeln("\t\t" + op + " BX", code);
    string result = MULOP->get_name() == "%" ? "DX" : "AX";
    genCodeln("\t\tPUSH " + result, "save " + code);
}

// after assigning, we are pushing assigned value into stack........
void getAssignCode(SymbolInfo *variable, SymbolInfo *logic_expression)
{
    if (variable->isBoolean() == true && logic_expression->isBoolean() == true)
    {
        // at first, push boolean values to AX or/and BX

        if (variable->isBoolean())
        {
            needValueCode(variable, 0);
        }
        if (logic_expression->isBoolean())
            needValueCode(logic_expression, 0);
    }
    else if (variable->isBoolean() == true)
    {

        needValueCode(variable, 1);
    }
    else if (logic_expression->isBoolean() == true)
    {
        needValueCode(logic_expression, 0);
    }
    string code = variable->get_name() + "=" + logic_expression->get_name();
    if (variable->isArray() && !variable->isGlobal())
    {
        genCodeln("\t\tPOP AX", "load " + logic_expression->get_name());
        genCodeln("\t\tPOP BX", "load " + variable->get_name() + "'s address of that index");
        genCodeln("\t\tMOV " + variable->getAssemblyName() + ", AX", code);
        genCodeln("\t\tPUSH AX", "save " + variable->get_name());
    }
    genCodeln("\t\tPOP AX", "load " + logic_expression->get_name());
    genCodeln("\t\tMOV " + variable->getAssemblyName() + ", AX", code);
    genCodeln("\t\tPUSH AX", "save " + variable->get_name());
}