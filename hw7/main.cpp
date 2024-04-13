/**
 * Project Name: Gregg1
 * File Name: main.cpp
 * Author: Logan Gregg
 * Date: 4/17/2024
 *
 * Description:
 * Assignment 7 for CSC 446
 *
 * Build Instructions:
 * This project can be built using Make with the following command:
 * make
 *
 * Run Instructions:
 * After building, the project can be run from the command line as follows:
 * ./Gregg5 <Name of file to read from> 
 * Example: ./Gregg7 test1.txt
 */

#include <iostream>
#include <string>

#include "LexicalAnalyzer.h"
#include "Parser.h"

extern SymbolTable st;

int main(int argc, char* argv[])
{
    std::string prog_file;

    // if (argc != 2)
    // {
    //     std::cout << "Usage: " << argv[0] << " filename" << std::endl;
    //     return 100;
    // }
    // else
    // {
    //     prog_file = argv[1];
    // }

    // Create instance of the lexical analyzer
    // LexicalAnalyzer* lex = new LexicalAnalyzer(prog_file);
    LexicalAnalyzer* lex = new LexicalAnalyzer("oberon_ex2.txt");
    
    while (token.m_token != Token_T::EOF_T)
    {
        lex->GetNextToken();
        // lex->DisplayToken();
    }

    // Delete and ReAllocate the Lexical Analyzer to reset file pos
    delete lex;
    // lex = new LexicalAnalyzer(prog_file);
    lex = new LexicalAnalyzer("oberon_ex2.txt");

    st = SymbolTable();

    Prog();

    std::cout << "Program parsed successfully!" << std::endl;

    delete lex;
    lex = nullptr;

}
