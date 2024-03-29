#include "SymTable.h"

#include <list>
#include <vector>

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
            curr_node = curr_node->m_next;
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
}

// WriteTable(depth) - include a procedure that will write out all variables (lexeme only) that are in the table at a specified depth. [ this will be useful for debugging your compiler ]
void SymbolTable::WriteTable(int depth)
{
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
                std::cout << "Lexeme: " << node->m_lexeme << " , Depth: " << node->m_depth << std::endl;
            }

            node = node->m_next;
         }
    }
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
