#pragma once

#include <vector>
#include <string>
#include <stack>
#include "Parser.h"
#include "LexicalAnalyzer.h"
#include "TempMap.h"
#include "SymTable.h"

extern std::stack<Token> token_stack;
extern SymbolTable st;

class TacWriter
{
    public:
        TacWriter()
        {
            // setupOffsetMap();
        }

        void writeFile();

        // Functions to setup offset info correctly
        // ------------------------------------------

        void addParams(TableRecord * tr, int depth);

        void addLocalVars(std::vector<std::string> vars, int depth);
        
        void addTemp();

        void getHighestOffset()
        {

        }

        void setupOffsetMap()
        {
            for (int i = 0; i < 4; i++)
            {
                std::vector<std::pair<Token, int>> vec;
                offset_map.push_back(std::make_pair(i + 1, vec));
            }
        }

        // ------------------------------------------

        // Functions to process assignment statements
        // ------------------------------------------
        static void preprocStatement();      
        static std::vector<Token> reduceMultiOp();
        static std::vector<Token> reduceTempMultiOp(std::stack<Token> p_stack);
        static Token handleNestedTemp();
        static Token handleNestedTemp(bool recursive);
        // ------------------------------------------

        // Functions for Procedure calls
        // ------------------------------------------
        
        // ------------------------------------------

    private:
        // To hold TAC about the main module
        std::string m_main;
        // Data structure to hold offset information
        std::vector<std::pair<int, std::vector<std::pair<Token, int>>>> offset_map;
};
