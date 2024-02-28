#include "SymTable.h"

#include <list>
#include <vector>

// For this assignment you are to write a module that will maintain a symbol table for your compiler.  Use the linked implementation of a hash table.  The unit should provide the following operations which must be named exactly as listed below:

// Constructor & Destructor
SymbolTable::SymbolTable()
{
    m_table = std::vector<std::list<TableRecord *>>(TABLE_SIZE);
    
}

SymbolTable::~SymbolTable()
{
    for (auto& list : m_table)
    {
            for (auto & record : list) 
            {
                delete record;
            }

            list.clear();
    }
}

// Insert(lex,token,depth) - insert the lexeme, token and depth into a record in the symbol table.
void SymbolTable::Insert(std::string lexeme, Token token, int depth)
{
    // unsigned int hash = hash(lexeme);

    // TableRecord * new_entry = new TableRecord();
    
}

// Lookup(lex) - lookup uses the lexeme to find the entry and returns a pointer to that entry.
TableRecord * SymbolTable::Lookup(std::string lex)
{
    TableRecord * ret;


    return ret;
}

// DeleteDepth(depth) - delete is passed the depth and deletes all records that are in the table at that depth.
void SymbolTable::DeleteDepth(int depth)
{

}

// WriteTable(depth) - include a procedure that will write out all variables (lexeme only) that are in the table at a specified depth. [ this will be useful for debugging your compiler ]
void SymbolTable::WriteTable(int depth)
{

}

// hash(lexeme) - (private)passed a lexeme and return the location for that lexeme. (this should be an internal routine only, do not list in the interface section). 
unsigned int SymbolTable::hash(std::string lexeme)
{
    unsigned int hashValue = 0;

    for (size_t i = 0; i < lexeme.length(); ++i) 
    {
        hashValue += (lexeme[i] * (1 << i));
    }

    return hashValue % TABLE_SIZE;
}
