#ifndef TABLE
#define TABLE

#include <bits/stdc++.h>
using namespace std;

struct ParseTreeInfo
{
    bool isToken = false;
    int startLineNo;
    int endLineNo;
};

class ParameterInfo
{
    string type; /// variable type....
    string name;
    bool is_array = false;

public:
    ParameterInfo(string type, string name, bool is_array = false);

    ParameterInfo(string name, bool is_array = false);

    string getName();

    string getType();

    void setType(string type);

    void setName(string name);

    void setAsArray();

    bool isArray();
};

///  this class object stores
// 1)function parameters and declaration type ......
// 2) any parameter list or declarationList....
class FunctionInfo
{
    /// can be two types of function, declaration(1) or definition(2)
    /// we can also store a parameter list as type 3
    int func_type;
    vector<ParameterInfo *> parameterList;

public:
    FunctionInfo(int type);

    int noOfParameters();

    ParameterInfo *getParameter(int index); /// 0 indexing

    void addParameter(string parameterType, string parameterName, bool isArray = false);

    void addParameter(ParameterInfo *parameterInfo);

    vector<ParameterInfo *> getParameterArray();

    void setParameterArray(vector<ParameterInfo *> parameterList);

    void setFunc_type(int type);

    int getFunc_type();

    ~FunctionInfo();
};

class ArrayInfo
{
    int size;

public:
    ArrayInfo(int size);

    ArrayInfo();

    int getSize();

    void setSize(int size);
};

class SymbolInfo
{
    string name;
    string type;
    SymbolInfo *nextSymbol;
    FunctionInfo *functionInfo;
    ArrayInfo *arrayInfo;
    vector<SymbolInfo *> childList;
    string varType = ""; /// for variable and array.....for function, it is treated as returnType
    ParseTreeInfo parseTreeInfo;

public:
    SymbolInfo();

    SymbolInfo(string name, string type); // general symbolInfo

    SymbolInfo(string name, string type, string varType); /// for variable

    SymbolInfo(string name, string type, string varType, int arrSize); /// for array

    SymbolInfo(string name, string type, string varType, bool isDeclaration); /// for function

    bool isVariable();

    bool isFunctionDeclaration();

    bool isFunctionDefinition();

    bool isArray();

    string get_name();

    string get_type();

    SymbolInfo *get_next_symbol();

    void set_name(string name);

    void set_type(string type);

    void set_next_symbol(SymbolInfo *symbol);

    void setAsFunctionDeclaration(); /// called if this is a function declaration

    void setAsFunctionDefinition();

    void setAsParameterList();

    void setAsArray();

    void setParameterArray(vector<ParameterInfo *> parameterList);

    vector<ParameterInfo *> getParameterArray();

    void addParameter(ParameterInfo *parameterInfo);

    void addParameter(string parameterName, string parameterType);

    void setVarType(string varType);

    string getVarType();

    vector<ParameterInfo *> getDeclarationList();

    void setDeclarationList(vector<ParameterInfo *> declarationList);

    void addDeclaration(string name, bool isArray = false);

    void setAsToken();
    void setLineNo(int lineNo);
    void setStartLineNo(int lineNo);
    void setEndLineNo(int lineNo);

    void addChild(vector<SymbolInfo *> childs);
    void printParseTree(ofstream &out, int depth);
};

class ScopeTable
{
    int table_id;
    int bucket_size;
    SymbolInfo **symbolInfos;
    ScopeTable *parent_scope;

    int sdbm_hash_function(string name);

    // return that symbolInfo which have the same name, otherwise return null...
    SymbolInfo *check_name(SymbolInfo *current_symbol, string symbol_name);

    // returns true if a symbolInfo is found in the scope table with same name....
    bool match_found(string symbol_name);

public:
    ScopeTable(int id, int bucket_size);

    ScopeTable *get_parent_scope();

    void set_parent_scope(ScopeTable *parent_scope);

    int get_id();

    SymbolInfo *lookUp(string symbol_name);

    bool Insert(string symbol_name, string symbol_type);
    bool Insert(SymbolInfo *symbolInfo);

    bool Delete(string symbol_name);

    void print(ofstream &output);

    ~ScopeTable();
};

class SymbolTable
{
    int current_id;
    int bucket_size;
    ScopeTable *current_scope_table;

public:
    SymbolTable(int bucket_size);

    void enter_scope();

    void exit_scope();

    bool Insert(string symbol_name, string symbol_type);

    bool Insert(SymbolInfo *symbolInfo);

    bool Delete(string symbol_name);

    SymbolInfo *lookUp(string symbol_name);

    void print_current_scope(ofstream &output);

    void print_all(ofstream &output);

    ~SymbolTable();
};
#endif