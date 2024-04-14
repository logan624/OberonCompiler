#pragma once

#include <vector>
#include <string>
#include <stack>
#include "Parser.h"
#include "LexicalAnalyzer.h"
#include "TempMap.h"

extern std::stack<Token> token_stack;

class TacWriter
{
    public:
        void writeFile();
        void writeOperation();
        void writeAssignment();

        static void preprocStatement();
        static void procStatement();         
        static std::vector<std::stack<Token>> reduceMultiOp();
        static Token handleNestedTemp();
        static Token handleNestedTemp(bool recursive);

    private:
        // To hold TAC about the main module
        std::string m_main;
        // To hold TAC about all the procedures
};
