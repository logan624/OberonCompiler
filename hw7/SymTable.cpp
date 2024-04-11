#include "SymTable.h"

#include <list>
#include <vector>

extern int global_depth;

// For this assignment you are to write a module that will maintain a symbol table for your compiler.  Use the linked implementation of a hash table.  The unit should provide the following operations which must be named exactly as listed below:

// Constructor & Destructor
SymbolTable::SymbolTable() : m_table(TABLE_SIZE)
{

}

SymbolTable::~SymbolTable()
{
    for (int i = 0; i < m_table.size(); i++) 
    {
        TableRecord* curr_node = m_table[i];
        
        while (curr_node != nullptr)
        {
            TableRecord* temp = curr_node;
            TableRecord* node;
            curr_node = curr_node->m_next;

            if (temp->m_entry == Entry_Type::FUNCTION)
            {
                delete temp;
                node = curr_node;
                temp = node;
            }

            delete temp;
        }
    }
}

// Insert(lex,token,depth) - insert the lexeme, token and depth into a record in the symbol table.
void SymbolTable::Insert(std::string p_lexeme, Token p_token, int p_depth)
{
    int hash = this->hash(p_lexeme);

    TableRecord * current_node = m_table[hash];

    TableRecord * new_node = new TableRecord();
    new_node->m_lexeme = p_lexeme;
    new_node->m_token = p_token;
    new_node->m_depth = p_depth;

    if (current_node == nullptr)
    {
        m_table[hash] = new_node;
    }
    else
    {
        while(current_node->m_next != nullptr)
        {
            current_node = current_node->m_next;
        }
        
        current_node->m_next = new_node;
    }
}

// Lookup(lex) - lookup uses the lexeme to find the entry and returns a pointer to that entry.
TableRecord * SymbolTable::LookupAtCurrentDepth(std::string lex)
{
    int hash = this->hash(lex);

    TableRecord* ret = m_table[hash];
    
    while (ret != nullptr)
    {
        if (ret->m_lexeme == lex && ret->m_depth == global_depth)
        {
            return ret;
        }

        ret = ret->m_next;
    }

    return nullptr;
}

TableRecord * SymbolTable::Lookup(std::string lex)
{
    int hash = this->hash(lex);

    TableRecord* ret = m_table[hash];
    
    while (ret != nullptr)
    {
        if (ret->m_lexeme == lex)
        {
            return ret;
        }

        ret = ret->m_next;
    }

    return nullptr;
}

// DeleteDepth(depth) - delete is passed the depth and deletes all records that are in the table at that depth.
void SymbolTable::DeleteDepth(int depth)
{
    TableRecord * curr;
    TableRecord * prev;
    TableRecord * temp;

    for (int i = 0; i < m_table.size(); i++)
    {
        
        curr = m_table[i];
        prev = nullptr;

        while (curr != nullptr)
        {
            if (curr->m_depth == depth)
            {
                if (prev == nullptr)
                {
                    m_table[i] = curr->m_next;
                }
                else
                {
                    prev->m_next = curr->m_next;
                }

                temp = curr;
                curr = curr->m_next;

                delete temp;
            }
            else
            {
                prev = curr;
                curr = curr->m_next;
            }
        }
    }

    // if (depths_offset_map[])
}

// WriteTable(depth) - include a procedure that will write out all variables (lexeme only) that are in the table at a specified depth. [ this will be useful for debugging your compiler ]
void SymbolTable::WriteTable(int depth)
{
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "CONTENTS OF SYMBOL TABLE AT DEPTH: " << global_depth << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    for (int i = 0; i < TABLE_SIZE; i++) {
        
        TableRecord * node = m_table[i];

        if(node == nullptr)
        {
            continue;
        }

        while (node != nullptr)
        {
            if (node->m_depth == depth)
            {
                // std::cout << "Lexeme: " << node->m_lexeme << " , Depth: " << node->m_depth << std::endl;
                
                if (node->m_entry == Entry_Type::CONST)
                {
                    PrintConstant(node);
                }
                else if (node->m_entry == Entry_Type::VAR)
                {
                    PrintVar(node);
                }
                else if (node->m_entry == Entry_Type::FUNCTION)
                {
                    PrintProc(node);
                }
            }

            node = node->m_next;
         }
    }
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << std::endl;
}

// hash(lexeme) - (private)passed a lexeme and return the location for that lexeme. (this should be an internal routine only, do not list in the interface section). 
unsigned int SymbolTable::hash(std::string lexeme)
{
    std::string s = lexeme;

    const int PRIME = TABLE_SIZE;

    unsigned int h = 0, g;

    for (char c : s) {
        h = (h << 24) + c;

        if ((g = h & 0xF0000000) != 0) {
            h = h ^ (g >> 24);
            h ^= g;
        }
    }

    return h % PRIME;
}

void SymbolTable::PrintConstant(TableRecord * tr)
{
    // Print Lexeme, Entry Type, Number Type, Value of constant
    std::cout << tr->m_lexeme << std::endl;
    std::cout << "\tEntry Type: " << EntryTypeToString(tr->m_entry) << std::endl;
    if (tr->item.constant.m_is_int)
    {
        std::cout << "\tType:       INTEGER" << std::endl;
        std::cout << "\tValue:      " << tr->item.constant.m_value.m_int_val << std::endl;
    }
    else
    {
        std::cout << "\tType:       REAL" << std::endl;
        std::cout << "\tValue:      " << tr->item.constant.m_value.m_real_val << std::endl;
    }
} 

void SymbolTable::PrintProc(TableRecord * tr)
{
    // Print Lexeme, Entry Type, Size of Local Variables, Number of Parameters
    std::cout << tr->m_lexeme << std::endl;
    std::cout << "\tEntry Type: " << EntryTypeToString(tr->m_entry) << std::endl;
    std::cout << "\tSize of Local Variables: " << tr->item.procedure.local_vars_size << std::endl;
    std::cout << "\tNumber of Parameters: " << tr->item.procedure.num_params << std::endl;
    std::cout << "\tParams: " << std::endl;
    
    ParameterInfo* node = tr->item.procedure.param_info;
    while(node != nullptr) // Changed to check node itself, not node->next_node
    {
        std::cout << "\t\t" << node->m_token.m_lexeme << std::endl;
        std::cout << "\t\t\tMode: " << ParamModeToString(node->m_mode) << std::endl;
        std::cout << "\t\t\tType: " << VarTypeToString(node->m_type) << std::endl;
        node = node->next_node;
    }

}

void SymbolTable::PrintVar(TableRecord * tr)
{
    // Print Lexeme, Entry Type, Type of Var, Offset
    std::cout << tr->m_lexeme << std::endl;
    std::cout << "\tEntry Type: " << EntryTypeToString(tr->m_entry) << std::endl;
    std::cout << "\tType:       " << VarTypeToString(tr->item.variable.m_type) << std::endl;
    std::cout << "\tOffset:     " << tr->item.variable.m_offset << std::endl;
}

std::string VarTypeToString(Var_T vt)
{
    switch(vt)
    {
        case Var_T::INTEGER:
            return "INTEGER";
            break;
        case Var_T::REAL:
            return "REAL";
            break;
        case Var_T::CHAR:
            return "CHAR";
            break;
        default:
            return "";
    }
}

std::string ParamModeToString(Param_Mode pm)
{
    switch(pm)
    {
        case Param_Mode::VAL:
            return "VALUE";
            break;
        case Param_Mode::REF:
            return "REFERENCE";
            break;
        default:
            return "";
    }
}

std::string EntryTypeToString(Entry_Type et)
{
    switch(et)
    {
        case Entry_Type::VAR:
            return "VAR      ";
            break;
        case Entry_Type::CONST:
            return "CONST    ";
            break;
        case Entry_Type::FUNCTION:
            return "PROCEDURE";
            break;
        default:
            return "";
    }
}
