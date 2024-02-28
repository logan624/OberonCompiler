#pragma once

#include <string>
#include <list>
#include <vector>

#include "LexicalAnalyzer.h"

const int TABLE_SIZE = 256;

enum class Var_T {INTEGER, REAL, CHAR};
enum class Param_Mode {VAL, REF};

struct Variable {
    Var_T m_type;
    int m_offset;
    int m_size;
};

struct Constant {
    bool m_is_int;
    union {
        int m_int_val;
        double m_real_val;
    } m_value;
};

struct Procedure {
    int localVariablesSize;
    int numParams;
    std::vector<Var_T> paramTypes;
    std::vector<Param_Mode> paramPassingModes;
};

struct TableRecord {
    std::string m_lexeme;
    Token_T m_token;

    int m_depth;
    
    union {
        Variable m_var;
        Constant m_const;
        Procedure* m_proc;
    } m_type;
    
    TableRecord* m_next;

    TableRecord(std::string lexeme, Token_T token, int depth, Variable var)
        : m_lexeme(lexeme), m_token(token), m_depth(depth), m_next(nullptr)
    {
        // Initialize union member based on the token type
        if (token == Token_T::VAR) {
            m_type.m_var = var; // Direct initialization for Variable
        } else if (token == Token_T::CONST) {
            // For Constant, you would need parameters passed in the constructor. Skipping for brevity.
        } else if (token == Token_T::PROCEDURE) {
            // For Procedure, allocate dynamically and assign the pointer
            m_type.m_proc = new Procedure(); // Consider using std::make_unique<Procedure>() if using C++14 or later for smart pointers
            // You would also need to pass in Procedure-specific parameters or initialize them separately.
        }
        // For other members, initialize them directly in the initialization list or within the constructor body
    }

    // Constructor for Constant case (example with parameters for initialization)
    TableRecord(std::string lexeme, Token_T token, int depth, bool is_int, int int_val)
        : m_lexeme(lexeme), m_token(token), m_depth(depth), m_next(nullptr)
    {
        if (token == Token_T::CONST) {
            m_type.m_const.m_is_int = is_int;
            m_type.m_const.m_value.m_int_val = int_val; // Assuming it's an int for simplicity
        }
        // Initialize other members as necessary
    }

    // Constructor for dynamically allocating Procedure
    TableRecord(std::string lexeme, Token_T token, int depth, Procedure* proc)
    : m_lexeme(lexeme), m_token(token), m_depth(depth), m_type{.m_proc = proc}, m_next(nullptr) {
        // Ensure token is PROCEDURE, otherwise, this constructor shouldn't be used
        if (token != Token_T::PROCEDURE) {
            throw std::invalid_argument("Token type must be PROCEDURE for this constructor.");
        }
        // Note: The Procedure object should be allocated before or within this constructor call.
    }

    ~TableRecord() {
        switch(m_token) {
            case Token_T::PROCEDURE:
                delete m_type.m_proc;
                break;
        }
    }

    // Copy/Move if necessary for the TableRecord
    TableRecord(const TableRecord& other) = delete;
    TableRecord& operator=(const TableRecord& other) = delete;
    TableRecord(TableRecord&& other) = delete;
    TableRecord& operator=(TableRecord&& other) = delete;
};

// For this assignment you are to write a module that will maintain a symbol table for your compiler.  Use the linked implementation of a hash table.  The unit should provide the following operations which must be named exactly as listed below:

class SymbolTable
{
    public:
        // Constructor & Destructor
        SymbolTable();
        ~SymbolTable();

        // Insert(lex,token,depth) - insert the lexeme, token and depth into a record in the symbol table.
        void Insert(std::string, Token token, int depth);
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
        std::vector<std::list<TableRecord *>> m_table;
};

// 1. Insert(lex,token,depth) - insert the lexeme, token and depth into a record in the symbol table.
// 2. Lookup(lex) - lookup uses the lexeme to find the entry and returns a pointer to that entry.
// 3. DeleteDepth(depth) - delete is passed the depth and deletes all records that are in the table at that depth.
// 4. WriteTable(depth) - include a procedure that will write out all variables (lexeme only) that are in the table at a specified depth. [ this will be useful for debugging your compiler ]
// 5. hash(lexeme) - (private)passed a lexeme and return the location for that lexeme. (this should be an internal routine only, do not list in the interface section). 

// The module should automatically initialize the table to empty and the table itself should be a variable declared in the module.
// Store the following information for each record type in the symbol table:
// 1. VARIABLE type of variable (use an enumerated data type), offset (use an integer variable) and size (use an integer variable).
// 2. CONSTANT appropriate fields to store either a real or integer value.
// 3. PROCEDURE size of local variables (this is the total required for all locals), number of parameters, type of each parameter, parameter passing mode of each parameter.

// You must use a union in C/C++ to store the information, for each item in the table the fields token, lexeme, depth and next will be needed. C# and Java programmers will need to use appropriate object oriented techniques to store the correct object type for each of the three record types.
