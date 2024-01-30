
#include <iostream>
#include <string>

#include "LexicalAnalyzer.h"

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
    LexicalAnalyzer lex("test1.txt");
    Token token;
    Token_T token_type;
    lex.GetNextToken();
    
    while (token_type != EOF_T)
    {
        lex.GetNextToken();
        lex.DisplayToken();
    }
}
