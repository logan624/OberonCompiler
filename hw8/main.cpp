/**
 * Project Name: Gregg1
 * File Name: main.cpp
 * Author: Logan Gregg
 * Date: 4/26/2024
 *
 * Description:
 * Assignment 8 for CSC 446
 *
 * Build Instructions:
 * This project can be built using Make with the following command:
 * make
 *
 * Run Instructions:
 * After building, the project can be run from the command line as follows:
 * ./Gregg5 <Name of file to read from> 
 * Example: ./Gregg8 test1.txt
 */

#include <iostream>
#include <string>
#include <fstream>

#include "LexicalAnalyzer.h"
#include "Parser.h"
#include "Asm.h"

extern SymbolTable st;
extern std::ostringstream tac_file;
extern std::ostringstream asm_file;
extern Asm asm_writer;

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

    prog_file = "oberon_ex2.txt";

    // Create instance of the lexical analyzer
    LexicalAnalyzer* lex = new LexicalAnalyzer(prog_file);
    // LexicalAnalyzer* lex = new LexicalAnalyzer("oberon_ex2.txt");
    
    while (token.m_token != Token_T::EOF_T)
    {
        lex->GetNextToken();
        // lex->DisplayToken();
    }

    // Delete and ReAllocate the Lexical Analyzer to reset file pos
    delete lex;
    lex = new LexicalAnalyzer(prog_file);
    // lex = new LexicalAnalyzer("oberon_ex2.txt");

    st = SymbolTable();

    // Open file stream for the TAC file
    // ---------------------------------
    std::string tac_filename = prog_file;

    if (tac_filename.size() >= 3) {
        // Remove the last three characters
        tac_filename.erase(tac_filename.end() - 3, tac_filename.end());
    }
    // Append "TAC" to the end of the file name
    tac_filename += "TAC";
    std::ofstream tac_stream(tac_filename);
    if (!tac_stream.is_open()) 
    {
        std::cerr << "Failed to open " << tac_filename << std::endl;
        exit(105);
    }

    asm_writer = Asm(tac_filename);

    // OPEN SS FOR THE ASM FILE
    // ------------------------
    std::string asm_filename = prog_file;
    if (asm_filename.size() >= 3) {
        // Remove the last three characters
        asm_filename.erase(asm_filename.end() - 3, asm_filename.end());
    }
    // Append "asm" to the end of the file name
    asm_filename += "asm";
    std::ofstream asm_stream(asm_filename);
    if (!asm_stream.is_open()) 
    {
        std::cerr << "Failed to open " << asm_filename << std::endl;
        exit(105);
    }

    // Begin parsing
    Prog();

    // Write TAC/ASM contents to appropriate file
    // ------------------------------------------
    tac_stream << tac_file.str();

    if (tac_stream.fail()) {
        std::cerr << "Error occurred while writing to the file " << tac_filename << std::endl;
    }

    // Close the file stream
    tac_stream.close();

    asm_writer.readTACFile();
    asm_writer.writeCode();

    asm_stream << asm_file.str();

    if (asm_stream.fail()) {
        std::cerr << "Error occurred while writing to the file " << asm_filename << std::endl;
    }

    // -----------------------------------------

    std::cout << "Program parsed successfully!" << std::endl;

    delete lex;
    lex = nullptr;

}
