#pragma once

#include <string>
#include <list>
#include <vector>

#include "LexicalAnalyzer.h"

const int TABLE_SIZE = 211;

enum class Var_T {INTEGER, REAL, CHAR};
enum class Param_Mode {VAL, REF};

struct TableRecord {
    TableRecord * m_next;

    std::string m_lexeme;
    Token m_token;

    int m_depth;
    
    union
    {
        struct 
        {
            Var_T m_type;
            int m_offset;
            int m_size;
        } variable;

        struct
        {
            bool m_is_int;
            
            union
            {
                int m_int_val;
                double m_real_val;
            } m_value;
        } constant;

        struct
        {
            int local_vars_size;
            int num_params;
            std::vector<Var_T> * m_param_types;
            std::vector<Param_Mode> * m_param_modes;

            // Procedure()
            // {
            //     m_param_types = new std::vector<Var_T>();
            //     m_param_modes = new std::vector<Param_Mode>();
            // }

            // ~Procedure()
            // {
            //     delete m_param_types;
            //     delete m_param_modes;
            //     m_param_types = nullptr;
            //     m_param_modes = nullptr;
            // }
        } procedure;
    };
};

// For this assignment you are to write a module that will maintain a symbol table for your compiler.  Use the linked implementation of a hash table.  The unit should provide the following operations which must be named exactly as listed below:

class SymbolTable
{
    public:
        // Constructor & Destructor
        SymbolTable();
        ~SymbolTable();

        // Insert(lex,token,depth) - insert the lexeme, token and depth into a record in the symbol table.
        void Insert(std::string p_lexeme, Token p_token, int p_depth);
        // Lookup(lex) - lookup uses the lexeme to find the entry and returns a pointer to that entry.
        TableRecord * Lookup(std::string lex);
        // DeleteDepth(depth) - delete is passed the depth and deletes all records that are in the table at that depth.
        void DeleteDepth(int depth);
        // WriteTable(depth) - include a procedure that will write out all variables (lexeme only) that are in the table at a specified depth. [ this will be useful for debugging your compiler ]
        void WriteTable(int depth);
        
    private:
        // hash(lexeme) - (private)passed a lexeme and return the location for that lexeme. (this should be an internal routine only, do not list in the interface section). 
        unsigned int hash(std::string lexeme);

        // Internal Data Structure for Entries
        std::vector<TableRecord *> m_table;
};
