#include "1905064_ICG_HelperFunctions.h"

void genPrintNewLine()
{
    string str = "\n\tPRINT_NEWLINE PROC\r"
                 "\n"
                 "        ; PRINTS A N"
                 "EW LINE WITH CARRIAG"
                 "E RETURN\r\n"
                 "        PUSH AX\r\n"
                 "        PUSH DX\r\n"
                 "        MOV AH, 2\r"
                 "\n"
                 "        MOV DL, 0Dh"
                 "\r\n"
                 "        INT 21h\r\n"
                 "        MOV DL, 0Ah"
                 "\r\n"
                 "        INT 21h\r\n"
                 "        POP DX\r\n"
                 "        POP AX\r\n"
                 "        RET\r\n"
                 "    PRINT_NEWLINE EN"
                 "DP";
    codeSegOut << str << endl;
}

void genMyFunctionForPrintln()
{
    char *str = "\tPRINT_NUM_FROM_BX PR"
                "OC\r\n"
                "       PUSH CX  \r\n"
                "       ; push to stack "
                "to \r\n"
                "       ; check the end "
                "   of the number  \r\n"
                "       MOV AX, \'X\'\r"
                "\n"
                "        PUSH AX\r\n"
                "    \r\n"
                "       CMP BX, 0  \r\n"
                "       JE ZERO_NUM\r\n"
                "       JNL NON_NEGATIVE"
                " \r\n"
                "    \r\n"
                "       NEG BX\r\n"
                "       ; print - for ne"
                "gative number\r\n"
                "       MOV DL, \'-\'\r"
                "\n"
                "       MOV AH, 2\r\n"
                "       INT 21H\r\n"
                "       JMP NON_NEGATIVE"
                "  \r\n"
                "    \r\n"
                "       ZERO_NUM:\r\n"
                "        MOV DX, 0\r"
                "\n"
                "        PUSH DX\r\n"
                "        JMP POP_PRIN"
                "T_LOOP\r\n"
                "    \r\n"
                "        NON_NEGATIVE:\r"
                "\n"
                "    \r\n"
                "        MOV CX, 10 \r\n"
                "    \r\n"
                "        MOV AX, BX\r\n"
                "        PRINT_LOOP:\r\n"
                "        ; if AX == 0"
                "\r\n"
                "        CMP AX, 0\r"
                "\n"
                "        JE END_PRINT"
                "_LOOP\r\n"
                "        ; else\r\n"
                "        MOV DX, 0 ; "
                "DX:AX = 0000:AX\r\n"
                "        \r\n"
                "        ; AX = AX / "
                "10 ; store reminder "
                "in DX \r\n"
                "        DIV CX\r\n"
                "    \r\n"
                "        PUSH DX\r\n"
                "        \r\n"
                "        JMP PRINT_LO"
                "OP\r\n\r\n"
                "        END_PRINT_LOOP:"
                "\r\n"
                "    \r\n"
                "    \r\n"
                "    \r\n"
                "        POP_PRINT_LOOP:"
                "\r\n"
                "        POP DX\r\n"
                "        ; loop endin"
                "g condition\r\n"
                "        ; if DX == "
                "\'X\'\r\n"
                "        CMP DX, \'X"
                "\'\r\n"
                "        JE END_POP_P"
                "RINT_LOOP\r\n"
                "        \r\n"
                "        ; if DX == "
                "\'-\'\r\n"
                "        CMP DX, \'-"
                "\'\r\n"
                "        JE PRINT_TO_"
                "CONSOLE\r\n"
                "        \r\n"
                "        ; convert to"
                " ascii\r\n"
                "        ADD DX, 30H "
                "      \r\n"
                "        ; print the "
                "digit\r\n"
                "        PRINT_TO_CON"
                "SOLE:\r\n"
                "        MOV AH, 2\r"
                "\n"
                "        INT 21H\r\n"
                "        \r\n"
                "        JMP POP_PRIN"
                "T_LOOP\r\n"
                "    \r\n"
                "       END_POP_PRINT_LO"
                "OP: \r\n"
                "       CALL PRINT_NEWLINE\r\n"
                "       POP CX\r\n"
                "       RET\r\n"
                "\tPRINT_NUM_FROM_BX EN"
                "DP";
    codeSegOut << str << endl;
}

void genFunctionForPrintln()
{
    genPrintNewLine();
    codeSegOut << endl;
    genMyFunctionForPrintln();
}

void openFiles()
{
    logout.open("log.txt", ios::out);
    errout.open("error.txt", ios::out);
    parsetree.open("tree.txt", ios::out);
    dataSegOut.open("dataseg.txt", ios::out);
    codeSegOut.open("codeseg.txt", ios::out);
}

void optimizeCode()
{

    ifstream codeSegmentIn("code.asm");

    // optimize the code
    ofstream optimizeOut("optimized_code.asm", ios::out);
    vector<string> lines;
    string line;
    while (getline(codeSegmentIn, line))
    {
        lines.push_back(line);
    }

    for (int i = 0; i < lines.size(); i++)
    {
        if (i + 1 >= lines.size() || lines[i].size() < 4 || lines[i + 1].size() < 4)
        {
        }
        else if (lines[i].substr(1, 3) == "MOV" && lines[i + 1].substr(1, 3) == "MOV")
        {
            string line1 = lines[i].substr(4);
            string line2 = lines[i + 1].substr(4);

            int delIndex1 = line1.find(",");
            int delIndex2 = line2.find(",");

            if (line1.substr(1, delIndex1 - 1) == line2.substr(delIndex2 + 2))
                if (line1.substr(delIndex1 + 2) == line2.substr(1, delIndex2 - 1))
                {
                    optimizeOut << "\t; Redundant MOV optimized" << endl;
                    i++;
                    continue;
                }
        }
        else if (lines[i].substr(2, 4) == "PUSH" && lines[i + 1].substr(2, 3) == "POP")
        {
            if (lines[i].substr(7, 2) == lines[i + 1].substr(6, 2))
                i += 2;
        }
        else if (lines[i].substr(2, 3) == "POP" && lines[i + 1].substr(2, 4) == "PUSH")
        {
            if (lines[i].substr(6, 2) == lines[i + 1].substr(7, 2))
                i += 2;
        }
        optimizeOut << lines[i] << endl;
    }

    optimizeOut.close();
    codeSegmentIn.close();
}

void generateCodeDotAsmFile()
{
    // close the files first
    if (dataSegOut.is_open())
    {
        dataSegOut.close();
    }
    if (codeSegOut.is_open())
    {
        codeSegOut.close();
    }
    ifstream dataSegmentIn("dataseg.txt");
    ifstream codeSegmentIn("codeseg.txt");
    ofstream codeDotAsmOut("code.asm", ios::out);
    string line;
    while (getline(dataSegmentIn, line))
    {
        codeDotAsmOut << line << endl;
    }
    codeDotAsmOut << endl;
    int linec = 0;
    while (getline(codeSegmentIn, line))
    {
        linec++;
        if (labelMap.find(linec) == labelMap.end())
            codeDotAsmOut << line << endl;
        else
            codeDotAsmOut << line << labelMap[linec] << endl;
    }

    codeDotAsmOut << endl
                  << "END MAIN" << endl;

    dataSegmentIn.close();
    codeSegmentIn.close();
    codeDotAsmOut.close();
    string r = "dataseg.txt";
    string p = "codeseg.txt";
    remove(r.c_str());
    remove(p.c_str());

    optimizeCode();
}

void VarCallForArray(SymbolInfo *varInfo, SymbolInfo *arrayId, SymbolInfo *expression)
{
    needValueCode(expression);

    genCodeln("\t\tPOP AX", "pop index " + expression->get_name() + " of array " + arrayId->get_name());
    genCodeln("\t\tSHL AX, 1", "multiply by 2 to get offset");
    genCodeln("\t\tLEA BX, " + arrayId->getAssemblyName(), "get array base address");
    genCodeln("\t\tSUB BX, AX");
    genCodeln("\t\tPUSH BX");
    varInfo->setAssemblyName("[BX]");
}
