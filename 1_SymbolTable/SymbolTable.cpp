#include <bits/stdc++.h>
using namespace std;

ifstream input("in.txt");
ofstream output("out.txt");

class SymbolInfo
{

    string name;
    string type;
    SymbolInfo *nextSymbol;

public:
    SymbolInfo(string name, string type)
    {
        this->name = name;
        this->type = type;
        this->nextSymbol = NULL;
    }

    string get_name()
    {
        return name;
    }
    string get_type()
    {
        return type;
    }
    SymbolInfo *get_next_symbol()
    {
        return nextSymbol;
    }
    void set_name(string name)
    {
        this->name = name;
    }
    void set_type(string type)
    {
        this->type = type;
    }
    void set_next_symbol(SymbolInfo *symbol)
    {
        this->nextSymbol = symbol;
    }
};

class ScopeTable
{
    int table_id;
    int bucket_size;
    SymbolInfo **symbolInfos;
    ScopeTable *parent_scope;

    int sdbm_hash_function(string name)
    {
        int num = 0;
        int len = name.length();
        for (int i = 0; i < len; i++)
        {
            num = (name[i] + (num << 6) + (num << 16) - num) % bucket_size;
        }
        num = num % bucket_size;

        return num;
    }

    // return that symbolInfo which have the same name, otherwise return null...
    SymbolInfo *check_name(SymbolInfo *current_symbol, string symbol_name)
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
    bool match_found(string symbol_name)
    {
        int hash_value = sdbm_hash_function(symbol_name);

        if (check_name(symbolInfos[hash_value], symbol_name) != NULL)
            return true;
        else
            return false;
    }

public:
    ScopeTable(int id, int bucket_size)
    {

        this->table_id = id;
        this->bucket_size = bucket_size;
        this->symbolInfos = new SymbolInfo *[bucket_size];
        for (int i = 0; i < bucket_size; i++)
        {
            symbolInfos[i] = NULL;
        }
        this->parent_scope = NULL;

        output << "\t"
               << "ScopeTable# " << id << " created" << endl;
    }

    ScopeTable *get_parent_scope()
    {
        return parent_scope;
    }
    void set_parent_scope(ScopeTable *parent_scope)
    {
        this->parent_scope = parent_scope;
    }
    int get_id()
    {
        return table_id;
    }

    SymbolInfo *lookUp(string symbol_name)
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

            output << "\t"
                   << "'" << symbol_name << "' found in ScopeTable# " << this->table_id << " at position "
                   << hash_value + 1 << ", " << chain << endl;
            return current_symbol;
        }
    }

    bool Insert(string symbol_name, string symbol_type)
    {

        bool check = match_found(symbol_name);

        if (check == true)
        {
            output << "\t"
                   << "'" << symbol_name << "'"
                   << " already exists in the current ScopeTable" << endl;
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
            output << "\t"
                   << "Inserted in ScopeTable# " << this->table_id << " at position " << hash_value + 1 << ", " << chain << endl;

            return true;
        }
    }

    bool Delete(string symbol_name)
    {
        int hash_value = sdbm_hash_function(symbol_name);
        SymbolInfo *found_symbol = check_name(symbolInfos[hash_value], symbol_name);
        if (found_symbol == NULL)
        {
            output << "\t"
                   << "Not found in the current ScopeTable" << endl;
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
            output << "\t"
                   << "Deleted '" << symbol_name << "' from ScopeTable# " << this->table_id << " at position "
                   << hash_value + 1 << ", " << chain << endl;
            return true;
        }
    }

    void print()
    {
        output << "\t"
               << "ScopeTable# " << this->table_id << endl;
        for (int i = 0; i < this->bucket_size; i++)
        {
            output << "\t" << i + 1 << "--> ";
            SymbolInfo *current = symbolInfos[i];
            while (current != NULL)
            {
                output << "<" << current->get_name() << "," << current->get_type() << "> ";
                current = current->get_next_symbol();
            }
            output << endl;
        }
    }

    ~ScopeTable()
    {
        output << "\t"
               << "ScopeTable# " << this->table_id << " removed" << endl;
        delete[] symbolInfos;
    }
};

class SymbolTable
{
    int current_id;
    int bucket_size;
    ScopeTable *current_scope_table;

public:
    SymbolTable(int bucket_size)
    {
        this->current_id = 1;
        this->bucket_size = bucket_size;
        current_scope_table = new ScopeTable(current_id, bucket_size);
    }
    void enter_scope()
    {
        current_id++;
        ScopeTable *new_scope_table = new ScopeTable(current_id, bucket_size);
        new_scope_table->set_parent_scope(current_scope_table);
        current_scope_table = new_scope_table;
    }
    void exit_scope()
    {
        ScopeTable *parent_scope_table = current_scope_table->get_parent_scope();
        if (parent_scope_table == NULL)
        {
            output << "\t"
                   << "ScopeTable# " << current_scope_table->get_id() << " cannot be removed" << endl;
            return;
        }
        delete current_scope_table;
        current_scope_table = parent_scope_table;
    }

    bool Insert(string symbol_name, string symbol_type)
    {
        return current_scope_table->Insert(symbol_name, symbol_type);
    }
    bool Delete(string symbol_name)
    {
        return current_scope_table->Delete(symbol_name);
    }

    SymbolInfo *lookUp(string symbol_name)
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
        if (ans == NULL)
            output << "\t"
                   << "'" << symbol_name << "' not found in any of the ScopeTables" << endl;
        return ans;
    }

    void print_current_scope()
    {
        current_scope_table->print();
    }
    void print_all()
    {
        ScopeTable *current = current_scope_table;
        while (current != NULL)
        {
            current->print();
            current = current->get_parent_scope();
        }
    }
    ~SymbolTable()
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
};

string *devide_string(string s)
{
    string *strings = new string[10];

    for (int i = 0; i < 10; i++)
        strings[i] = "";

    int cur = 0;
    stringstream temp(s);
    string inside;

    while (getline(temp, inside, ' '))
    {
        if (inside.length() != 0)
        {
            strings[cur] = inside;
            cur++;
        }
        if (cur == 5)
            break;
    }

    return strings;
}

void input_stream()
{

    int size;
    input >> size;

    SymbolTable *symbolbucket = new SymbolTable(size);

    /// we will get an array of strings; string a[10]
    string *strings;
    string line;
    int curPos = 0, flag = 0;

    int command_count = 0;
    while (getline(input, line))
    {
        // to ignore the first command...........
        if (flag == 0)
        {
            flag = 1;
            continue;
        }

        command_count++;

        output << "Cmd " << command_count << ": " << line << endl;

        strings = devide_string(line);
        curPos = 0;
        int i = 0;

        // count the number of strings..........
        while (strings[i] != "")
        {
            curPos++;
            i++;
        }

        if (strings[0] == "I")
        {
            if (curPos != 3)
            {
                output << "\t"
                       << "Number of parameters mismatch for the command " << strings[0] << endl;
            }
            else
            {

                symbolbucket->Insert(strings[1], strings[2]);
            }
        }
        else if (strings[0] == "L")
        {
            if (curPos != 2)
            {
                output << "\t"
                       << "Number of parameters mismatch for the command " << strings[0] << endl;
            }
            else
            {
                symbolbucket->lookUp(strings[1]);
            }
        }
        else if (strings[0] == "P")
        {
            if (curPos != 2)
            {
                output << "\t"
                       << "Number of parameters mismatch for the command " << strings[0] << endl;
            }
            else
            {
                if (strings[1] == "C")
                    symbolbucket->print_current_scope();
                else if (strings[1] == "A")
                    symbolbucket->print_all();
                else
                    output << "\t"
                           << "unrecognized command" << endl;
            }
        }
        else if (strings[0] == "D")
        {
            if (curPos != 2)
            {
                output << "\t"
                       << "Number of parameters mismatch for the command " << strings[0] << endl;
            }
            else
            {
                symbolbucket->Delete(strings[1]);
            }
        }
        else if (strings[0] == "S")
        {
            if (curPos != 1)
            {
                output << "\t"
                       << "Number of parameters mismatch for the command " << strings[0] << endl;
            }
            else
            {
                symbolbucket->enter_scope();
            }
        }
        else if (strings[0] == "E")
        {
            if (curPos != 1)
            {
                output << "\t"
                       << "Number of parameters mismatch for the command " << strings[0] << endl;
            }
            else
            {
                symbolbucket->exit_scope();
            }
        }
        else if (strings[0] == "Q")
        {
            if (curPos != 1)
            {
                output << "\t"
                       << "Number of parameters mismatch for the command " << strings[0] << endl;
            }
            else
            {
                delete symbolbucket;
                break;
            }
        }
        else
        {
            output << "\t"
                   << "unrecognized command" << endl;
        }
    }
}

int main()
{

    input_stream();
}