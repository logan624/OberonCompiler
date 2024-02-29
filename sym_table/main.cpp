/**
 * Project Name: Gregg1
 * File Name: main.cpp
 * Author: Logan Gregg
 * Date: 2/16/2024
 *
 * Description:
 * Lexical Analyzer Assignment for CSC 446
 *
 * Build Instructions:
 * This project can be built using Make with the following command:
 * make
 *
 * Run Instructions:
 * After building, the project can be run from the command line as follows:
 * ./Gregg2 <Name of file to read from> 
 * Example: ./Gregg2 test1.txt
 */

#include <iostream>
#include <string>

#include "LexicalAnalyzer.h"
#include "Parser.h"
#include "SymTable.h"

int main(int argc, char* argv[])
{

    // Initialize test data to test with
    Token t1;
    t1.m_lexeme = "cat";
    t1.m_token = Token_T::IDENTIFIER;
    
    Token t2;
    t2.m_lexeme = "dog";
    t2.m_token = Token_T::IDENTIFIER;
    
    Token t3;
    t3.m_lexeme = "lizard";
    t3.m_token = Token_T::IDENTIFIER;
    
    Token t4;
    t4.m_lexeme = "otter";
    t4.m_token = Token_T::IDENTIFIER;
    
    Token t5;
    t5.m_lexeme = "platypus";
    t5.m_token = Token_T::IDENTIFIER;
    
    Token t6;
    t6.m_lexeme = "wasp";
    t6.m_token = Token_T::IDENTIFIER;

    // Initalize the symbol table
    SymbolTable st = SymbolTable();

    std::cout << "Inserting two symbols at depth 0 and two at depth 1" << std::endl;
    st.Insert(t1.m_lexeme, t1, 0);
    st.Insert(t2.m_lexeme, t2, 0);
    st.Insert(t3.m_lexeme, t3, 1);
    st.Insert(t4.m_lexeme, t4, 1);

    std::cout << std::endl << "Printing out all symbols at depth 0!" << std::endl;
    st.WriteTable(0);
    std::cout << std::endl << "Printing out all symbols at depth 1!" << std::endl;
    st.WriteTable(1);

    std::cout << std::endl << "Deleting all symbols at depth 0!" << std::endl << std::endl;
    st.DeleteDepth(0);
    std::cout << "Printing out all symbols at depth 0!" << std::endl;
    st.WriteTable(0);

    std::cout << std::endl << "Printing out all symbols at depth 1!" << std::endl;
    st.WriteTable(1);

    std::cout << std::endl << "Inserting four symbols at depth 0" << std::endl;
    st.Insert(t1.m_lexeme, t1, 0);
    st.Insert(t2.m_lexeme, t2, 0);
    st.Insert(t1.m_lexeme, t1, 1);
    st.Insert(t2.m_lexeme, t2, 1);

    std::cout << std::endl << "Printing out all symbols at depth 0!" << std::endl;
    st.WriteTable(0);

    std::cout << std::endl << "Deleting all symbols at depth 0!" << std::endl << std::endl;
    st.DeleteDepth(0);
    std::cout << "Printing out all symbols at depth 0!" << std::endl;
    st.WriteTable(0);

    std::cout << std::endl << "Inserting four symbols at depth 2" << std::endl;
    st.Insert(t5.m_lexeme, t5, 2);
    st.Insert(t6.m_lexeme, t6, 2);

    std::cout << std::endl << "Printing out all symbols at depth 0" << std::endl;
    st.WriteTable(0);
    std::cout << std::endl << "Printing out all symbols at depth 1" << std::endl;
    st.WriteTable(1);
    std::cout << std::endl << "Printing out all symbols at depth 2" << std::endl;
    st.WriteTable(2);

}
