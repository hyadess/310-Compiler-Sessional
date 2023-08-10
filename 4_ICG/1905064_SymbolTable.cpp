
#include "1905064_SymbolTable.h"

ParameterInfo::ParameterInfo(string type, string name, string assemblyName, bool is_array)
{
    this->type = type;
    this->name = name;
    this->assemblyName = assemblyName;
    this->is_array = is_array;
}
ParameterInfo::ParameterInfo(string type, string name, bool is_array)
{
    this->type = type;
    this->name = name;
    this->is_array = is_array;
}
ParameterInfo::ParameterInfo(string name, bool is_array)
{
    this->type = "";
    this->name = name;
    this->is_array = is_array;
}
string ParameterInfo::getName()
{
    return name;
}
string ParameterInfo::getType()
{
    return type;
}
void ParameterInfo::setType(string type)
{
    this->type = type;
}
void ParameterInfo::setName(string name)
{
    this->name = name;
}
void ParameterInfo::setAsArray()
{
    this->is_array = true;
}
bool ParameterInfo::isArray()
{
    return this->is_array;
}
string ParameterInfo::getAssemblyName()
{
    return this->assemblyName;
}
void ParameterInfo::setAssemblyName(string assemblyName)
{
    this->assemblyName = assemblyName;
}

///  this class object stores
// 1)function parameters and declaration type ......
// 2) any parameter list or declarationList....

FunctionInfo::FunctionInfo(int type)
{
    parameterList.clear();
    this->func_type = type;
}
int FunctionInfo::noOfParameters()
{
    return parameterList.size();
}

ParameterInfo *FunctionInfo::getParameter(int index) /// 0 indexing
{
    return parameterList[index];
}

void FunctionInfo::addParameter(string parameterType, string parameterName, bool isArray)
{
    ParameterInfo *p = new ParameterInfo(parameterType, parameterName, isArray);
    parameterList.push_back(p);
}
void FunctionInfo::addParameter(string parameterType, string parameterName, string aname, bool isArray)
{
    ParameterInfo *p = new ParameterInfo(parameterType, parameterName, aname, isArray);
    parameterList.push_back(p);
}
void FunctionInfo::addParameter(ParameterInfo *parameterInfo)
{
    parameterList.push_back(parameterInfo);
}

vector<ParameterInfo *> FunctionInfo::getParameterArray()
{
    return parameterList;
}
void FunctionInfo::setParameterArray(vector<ParameterInfo *> parameterList)
{
    this->parameterList = parameterList;
}

void FunctionInfo::setFunc_type(int type)
{
    this->func_type = type;
}
int FunctionInfo::getFunc_type()
{
    return func_type;
}

FunctionInfo::~FunctionInfo()
{
    parameterList.clear();
}

// array info

ArrayInfo::ArrayInfo(int size)
{
    this->size = size;
}
ArrayInfo::ArrayInfo()
{
    this->size = -1;
}

int ArrayInfo::getSize()
{
    return size;
}
void ArrayInfo::setSize(int size)
{
    this->size = size;
}

// assembly info

AssemblyInfo::AssemblyInfo()
{
    trueList = new vector<int>();
    falseList = new vector<int>();
    nextList = new vector<int>();
}
string AssemblyInfo::getAssemblyName()
{
    return assemblyName;
}
void AssemblyInfo::setAssemblyName(string assemblyName, bool global, int arrayStarting)
{
    this->assemblyName = assemblyName;
    this->global = global;
    this->arrayStarting = arrayStarting;
}
bool AssemblyInfo::isGlobal()
{
    return global;
}
void AssemblyInfo::setArrayStarting(int n)
{
    this->arrayStarting = n;
}
int AssemblyInfo::getArrayStarting()
{
    return this->arrayStarting;
}
void AssemblyInfo::addToTrueList(int newlabel)
{
    trueList->push_back(newlabel);
}
void AssemblyInfo::addToFalseList(int newLabel)
{
    falseList->push_back(newLabel);
}
void AssemblyInfo::addToNextList(int newLabel)
{
    nextList->push_back(newLabel);
}
vector<int> *AssemblyInfo::getTrueList()
{
    return trueList;
}
vector<int> *AssemblyInfo::getFalseList()
{
    return falseList;
}
vector<int> *AssemblyInfo::getNextList()
{
    return nextList;
}
string AssemblyInfo::getLabel()
{
    return label;
}
void AssemblyInfo::setLabel(string label)
{
    this->label = label;
}
void AssemblyInfo::setTrueList(vector<int> *trueList)
{
    this->trueList = trueList;
}
void AssemblyInfo::setFalseList(vector<int> *falseList)
{
    this->falseList = falseList;
}
void AssemblyInfo::setNextList(vector<int> *nextList)
{
    this->nextList = nextList;
}
bool AssemblyInfo::isBoolean()
{
    if (trueList->size() > 0 || falseList->size() > 0)
        return true;
    return false;
}

// symbol info

SymbolInfo::SymbolInfo()
{
}
SymbolInfo::SymbolInfo(string name, string type) // general symbolInfo
{
    this->name = name;
    this->type = type;

    this->nextSymbol = NULL;
    this->functionInfo = NULL;
    this->arrayInfo = NULL;
}
SymbolInfo::SymbolInfo(string name, string type, string varType) /// for variable
{
    this->name = name;
    this->type = type;
    this->varType = varType;
    this->nextSymbol = NULL;
    this->functionInfo = NULL;
    this->arrayInfo = NULL;
}
SymbolInfo::SymbolInfo(string name, string type, string varType, int arrSize) /// for array
{
    this->name = name;
    this->type = type;
    this->varType = varType;
    this->nextSymbol = NULL;
    this->functionInfo = NULL;
    this->arrayInfo = new ArrayInfo(arrSize);
}
SymbolInfo::SymbolInfo(string name, string type, string varType, bool isDeclaration) /// for function
{
    this->name = name;
    this->type = type;
    this->varType = varType;
    this->nextSymbol = NULL;
    this->functionInfo = new FunctionInfo(isDeclaration);
    this->arrayInfo = NULL;
}

bool SymbolInfo::isVariable()
{
    if (varType == "")
        return false;
    else
        return true;
}
bool SymbolInfo::isFunctionDeclaration()
{
    if (functionInfo == NULL)
        return false;
    else if (functionInfo->getFunc_type() != 1)
        return false;
    else
        return true;
}
bool SymbolInfo::isFunctionDefinition()
{
    if (functionInfo == NULL)
        return false;
    else if (functionInfo->getFunc_type() != 2)
        return false;
    else
        return true;
}
bool SymbolInfo::isArray()
{
    if (arrayInfo == NULL)
        return false;
    else
        return true;
}

string SymbolInfo::get_name()
{
    return name;
}
string SymbolInfo::get_type()
{
    return type;
}
SymbolInfo *SymbolInfo::get_next_symbol()
{
    return nextSymbol;
}
void SymbolInfo::set_name(string name)
{
    this->name = name;
}
void SymbolInfo::set_type(string type)
{
    this->type = type;
}
void SymbolInfo::set_next_symbol(SymbolInfo *symbol)
{
    this->nextSymbol = symbol;
}

void SymbolInfo::setAsFunctionDeclaration() /// called if this is a function declaration
{
    if (this->functionInfo == NULL)
        functionInfo = new FunctionInfo(1);
    else
        functionInfo->setFunc_type(1);
}
void SymbolInfo::setAsFunctionDefinition()
{

    if (this->functionInfo == NULL)
        functionInfo = new FunctionInfo(2);
    else
        functionInfo->setFunc_type(2);
}
void SymbolInfo::setAsParameterList()
{
    if (this->functionInfo == NULL)
        functionInfo = new FunctionInfo(3);
    else
        functionInfo->setFunc_type(3);
}

void SymbolInfo::setAsArray()
{
    this->arrayInfo = new ArrayInfo();
}

void SymbolInfo::setParameterArray(vector<ParameterInfo *> parameterList)
{
    if (this->functionInfo == NULL)
        this->functionInfo = new FunctionInfo(3);
    this->functionInfo->setParameterArray(parameterList);
}
vector<ParameterInfo *> SymbolInfo::getParameterArray()
{
    if (this->functionInfo == NULL)
        return {};
    return this->functionInfo->getParameterArray();
}

void SymbolInfo::addParameter(ParameterInfo *parameterInfo)
{
    if (this->functionInfo == NULL)
        this->functionInfo = new FunctionInfo(3);
    functionInfo->addParameter(parameterInfo);
}
void SymbolInfo::addParameter(string parameterName, string parameterType)
{
    if (this->functionInfo == NULL)
        this->functionInfo = new FunctionInfo(3);
    functionInfo->addParameter(parameterType, parameterName);
}

void SymbolInfo::setVarType(string varType)
{
    this->varType = varType;
}
string SymbolInfo::getVarType()
{
    return varType;
}

vector<ParameterInfo *> SymbolInfo::getDeclarationList()
{
    if (this->functionInfo == NULL)
        return {};
    return this->functionInfo->getParameterArray();
}
void SymbolInfo::setDeclarationList(vector<ParameterInfo *> declarationList)
{
    if (this->functionInfo == NULL)
        this->functionInfo = new FunctionInfo(3);
    functionInfo->setParameterArray(declarationList);
}
void SymbolInfo::addDeclaration(string name, string aname, bool isArray)
{
    if (this->functionInfo == NULL)
        functionInfo = new FunctionInfo(3);
    functionInfo->addParameter("", name, aname, isArray);
}

void SymbolInfo::setAsToken()
{
    this->parseTreeInfo.isToken = true;
}
void SymbolInfo::setLineNo(int lineNo)
{
    this->parseTreeInfo.startLineNo = this->parseTreeInfo.endLineNo = lineNo;
}
void SymbolInfo::setStartLineNo(int lineNo)
{
    this->parseTreeInfo.startLineNo = lineNo;
}
void SymbolInfo::setEndLineNo(int lineNo)
{
    this->parseTreeInfo.endLineNo = lineNo;
}
void SymbolInfo::addChild(vector<SymbolInfo *> childs)
{
    this->childList = childs;
    this->parseTreeInfo.isToken = false;
    if (childs.size() == 0)
    {
        this->parseTreeInfo.startLineNo = this->parseTreeInfo.endLineNo = -1;
        return;
    }
    this->parseTreeInfo.startLineNo = childs[0]->parseTreeInfo.startLineNo;
    this->parseTreeInfo.endLineNo = childs[childs.size() - 1]->parseTreeInfo.endLineNo;
}
void SymbolInfo::printParseTree(ofstream &out, int depth)
{
    for (int i = 0; i < depth; i++)
        out << " ";

    if (this->parseTreeInfo.isToken == true) // base case...................
    {
        out << this->get_type() << " : " << this->get_name() << "\t<Line: " << this->parseTreeInfo.startLineNo << ">" << endl;
        return;
    }

    out << this->get_type() << " :";
    for (int i = 0; i < this->childList.size(); i++)
    {
        out << " " << this->childList[i]->get_type();
    }
    out << " ";
    out << "\t<Line: " << this->parseTreeInfo.startLineNo << "-" << this->parseTreeInfo.endLineNo << ">";

    out << endl;
    for (int i = 0; i < this->childList.size(); i++)
        this->childList[i]->printParseTree(out, depth + 1);
}

int SymbolInfo::getSize()
{
    return arrayInfo->getSize();
}
void SymbolInfo::setSize(int size)
{
    arrayInfo->setSize(size);
}

string SymbolInfo::getAssemblyName()
{
    return assemblyInfo.getAssemblyName();
}
void SymbolInfo::setAssemblyName(string assemblyName, bool global, int arrayStarting)
{
    assemblyInfo.setAssemblyName(assemblyName, global, arrayStarting);
}
bool SymbolInfo::isGlobal()
{
    return assemblyInfo.isGlobal();
}
void SymbolInfo::setArrayStarting(int n)
{
    assemblyInfo.setArrayStarting(n);
}
int SymbolInfo::getArrayStarting()
{
    return assemblyInfo.getArrayStarting();
}
void SymbolInfo::addToTrueList(int newlabel)
{
    assemblyInfo.addToTrueList(newlabel);
}
void SymbolInfo::addToFalseList(int newLabel)
{
    assemblyInfo.addToFalseList(newLabel);
}
void SymbolInfo::addToNextList(int newLabel)
{
    assemblyInfo.addToNextList(newLabel);
}
vector<int> *SymbolInfo::getTrueList()
{
    return assemblyInfo.getTrueList();
}
vector<int> *SymbolInfo::getFalseList()
{
    return assemblyInfo.getFalseList();
}
vector<int> *SymbolInfo::getNextList()
{
    return assemblyInfo.getNextList();
}
string SymbolInfo::getLabel()
{
    return assemblyInfo.getLabel();
}
void SymbolInfo::setLabel(string label)
{
    assemblyInfo.setLabel(label);
}
void SymbolInfo::setTrueList(vector<int> *trueList)
{
    assemblyInfo.setTrueList(trueList);
}
void SymbolInfo::setFalseList(vector<int> *falseList)
{
    assemblyInfo.setFalseList(falseList);
}
void SymbolInfo::setNextList(vector<int> *nextList)
{
    assemblyInfo.setNextList(nextList);
}
bool SymbolInfo::isBoolean()
{
    return assemblyInfo.isBoolean();
}
// scope table
int ScopeTable::sdbm_hash_function(string name)
{
    uint64_t num = 0;
    int len = name.length();
    for (int i = 0; i < len; i++)
    {
        num = name[i] + (num << 16) + (num << 6) - num;
    }
    num = num % ((uint64_t)bucket_size);

    return (int)num;
}

// return that symbolInfo which have the same name, otherwise return null...
SymbolInfo *ScopeTable::check_name(SymbolInfo *current_symbol, string symbol_name)
{
    if (current_symbol == NULL)
        return NULL;
    if (current_symbol->get_name() == symbol_name)
        return current_symbol;
    if (current_symbol->get_next_symbol() == NULL)
        return NULL;

    return check_name(current_symbol->get_next_symbol(), symbol_name);
}

// returns true if a symbolInfo is found in the scope table with same name....
bool ScopeTable::match_found(string symbol_name)
{
    int hash_value = sdbm_hash_function(symbol_name);

    if (check_name(symbolInfos[hash_value], symbol_name) != NULL)
        return true;
    else
        return false;
}

ScopeTable::ScopeTable(int id, int bucket_size)
{

    this->table_id = id;
    this->bucket_size = bucket_size;
    this->symbolInfos = new SymbolInfo *[bucket_size];
    for (int i = 0; i < bucket_size; i++)
    {
        symbolInfos[i] = NULL;
    }
    this->parent_scope = NULL;

    // output << "\t"
    //<< "ScopeTable# " << id << " created" << endl;
}

ScopeTable *ScopeTable::get_parent_scope()
{
    return parent_scope;
}
void ScopeTable::set_parent_scope(ScopeTable *parent_scope)
{
    this->parent_scope = parent_scope;
}
int ScopeTable::get_id()
{
    return table_id;
}

SymbolInfo *ScopeTable::lookUp(string symbol_name)
{
    int hash_value = sdbm_hash_function(symbol_name);
    SymbolInfo *current_symbol = symbolInfos[hash_value];

    if (current_symbol == NULL)
    {
        return NULL;
    }

    else
    {

        int chain = 1;
        if (current_symbol->get_name() == symbol_name)
        {
            chain = 1;
        }
        else
        {
            while (current_symbol->get_name() != symbol_name && current_symbol->get_next_symbol() != NULL)
            {
                chain++;
                current_symbol = current_symbol->get_next_symbol();
            }
        }
        if (current_symbol->get_name() != symbol_name)
            return NULL;

        // output << "\t"
        //  << "'" << symbol_name << "' found in ScopeTable# " << this->table_id << " at position "
        //  << hash_value + 1 << ", " << chain << endl;
        return current_symbol;
    }
}

bool ScopeTable::Insert(string symbol_name, string symbol_type)
{

    bool check = match_found(symbol_name);

    if (check == true)
    {
        return false;
    }

    else
    {
        SymbolInfo *new_symbol = new SymbolInfo(symbol_name, symbol_type);
        int chain = 1;
        int hash_value = sdbm_hash_function(symbol_name);
        if (symbolInfos[hash_value] == NULL)
            symbolInfos[hash_value] = new_symbol;
        else
        {
            SymbolInfo *currSymbol = symbolInfos[hash_value];
            while (currSymbol->get_next_symbol() != NULL)
            {
                chain++;
                currSymbol = currSymbol->get_next_symbol();
            }
            chain++;
            currSymbol->set_next_symbol(new_symbol);
        }

        return true;
    }
}
bool ScopeTable::Insert(SymbolInfo *symbolInfo)
{
    bool check = match_found(symbolInfo->get_name());

    if (check == true)
    {
        return false;
    }

    else
    {
        int chain = 1;
        int hash_value = sdbm_hash_function(symbolInfo->get_name());
        if (symbolInfos[hash_value] == NULL)
            symbolInfos[hash_value] = symbolInfo;
        else
        {
            SymbolInfo *currSymbol = symbolInfos[hash_value];
            while (currSymbol->get_next_symbol() != NULL)
            {
                chain++;
                currSymbol = currSymbol->get_next_symbol();
            }
            chain++;
            currSymbol->set_next_symbol(symbolInfo);
        }

        return true;
    }
}

bool ScopeTable::Delete(string symbol_name)
{
    int hash_value = sdbm_hash_function(symbol_name);
    SymbolInfo *found_symbol = check_name(symbolInfos[hash_value], symbol_name);
    if (found_symbol == NULL)
    {
        // output << "\t"
        //<< "Not found in the current ScopeTable" << endl;
        return false;
    }

    else
    {

        // now we need to find the previous pointer of found_symbol....

        SymbolInfo *current = symbolInfos[hash_value];
        int chain = 1;
        if (current == found_symbol) /// found_symbol is the first one in that place
        {
            symbolInfos[hash_value] = found_symbol->get_next_symbol();
        }
        else
        {
            while (current->get_next_symbol() != found_symbol)
            {
                chain++;
                current = current->get_next_symbol();
            }
            chain++;
            current->set_next_symbol(found_symbol->get_next_symbol());
        }
        delete found_symbol;
        // output << "\t"
        //  << "Deleted '" << symbol_name << "' from ScopeTable# " << this->table_id << " at position "
        //<< hash_value + 1 << ", " << chain << endl;
        return true;
    }
}

void ScopeTable::print(ofstream &output)
{
    output << "\t"
           << "ScopeTable# " << this->table_id << endl;
    int flag = 0;
    for (int i = 0; i < this->bucket_size; i++)
    {
        if (symbolInfos[i] != NULL)
            flag = 1;
    }
    if (flag == 0)
        return;

    for (int i = 0; i < this->bucket_size; i++)
    {
        if (symbolInfos[i] == NULL)
            continue;
        output << "\t" << i + 1 << "--> ";
        SymbolInfo *current = symbolInfos[i];
        while (current != NULL)
        {
            output << "<" << current->get_name() << ", ";
            if (current->isArray())
                output << "ARRAY, ";
            else if (current->isFunctionDeclaration() || current->isFunctionDefinition())
                output << "FUNCTION, ";
            if (current->getVarType() == "")
                output << current->get_type() << "> ";
            else
                output << current->getVarType() << "> ";
            current = current->get_next_symbol();
        }
        output << endl;
    }
}

ScopeTable::~ScopeTable()
{
    // output << "\t"
    //  << "ScopeTable# " << this->table_id << " removed" << endl;
    delete[] symbolInfos;
}

// symbol table
SymbolTable::SymbolTable(int bucket_size)
{
    this->current_id = 1;
    this->bucket_size = bucket_size;
    current_scope_table = new ScopeTable(current_id, bucket_size);
}
void SymbolTable::enter_scope()
{
    current_id++;
    ScopeTable *new_scope_table = new ScopeTable(current_id, bucket_size);
    new_scope_table->set_parent_scope(current_scope_table);
    current_scope_table = new_scope_table;
}
void SymbolTable::exit_scope()
{
    ScopeTable *parent_scope_table = current_scope_table->get_parent_scope();
    if (parent_scope_table == NULL)
    {
        // output << "\t"
        //<< "ScopeTable# " << current_scope_table->get_id() << " cannot be removed" << endl;
        return;
    }
    delete current_scope_table;
    current_scope_table = parent_scope_table;
}

bool SymbolTable::Insert(string symbol_name, string symbol_type)
{
    return current_scope_table->Insert(symbol_name, symbol_type);
}

bool SymbolTable::Insert(SymbolInfo *symbolInfo)
{
    return current_scope_table->Insert(symbolInfo);
}

bool SymbolTable::Delete(string symbol_name)
{
    return current_scope_table->Delete(symbol_name);
}

SymbolInfo *SymbolTable::lookUp(string symbol_name)
{
    SymbolInfo *ans = NULL;
    ScopeTable *current = current_scope_table;
    while (current != NULL)
    {
        ans = current->lookUp(symbol_name);
        if (ans != NULL)
            break;
        current = current->get_parent_scope();
    }
    // if (ans == NULL)
    //  output << "\t"
    //<< "'" << symbol_name << "' not found in any of the ScopeTables" << endl;
    return ans;
}

void SymbolTable::print_current_scope(ofstream &output)
{
    current_scope_table->print(output);
}
void SymbolTable::print_all(ofstream &output)
{
    ScopeTable *current = current_scope_table;
    while (current != NULL)
    {
        current->print(output);
        current = current->get_parent_scope();
    }
}
SymbolTable::~SymbolTable()
{
    ScopeTable *current = current_scope_table;
    ScopeTable *next;
    while (current != NULL)
    {
        next = current->get_parent_scope();
        delete current;
        current = next;
    }
}
void SymbolTable::addToVarCount(int n)
{
    this->variableCount += n;
}
int SymbolTable::getVarCount()
{
    return this->variableCount;
}