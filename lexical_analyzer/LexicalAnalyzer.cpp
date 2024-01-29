#include "LexicalAnalyzer.h"
#include "ReservedWords.h"

#include <fstream>
 
LexicalAnalyzer::LexicalAnalyzer(std::string filename)
{
    m_file = filename;
}
 
LexicalAnalyzer::~LexicalAnalyzer()
{

}

void LexicalAnalyzer::GetNextToken()
{
    std::ifstream input_file(m_file);

    if (!input_file.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        exit(101); // Return an error code
    }

    std::string word;
    while (input_file >> word) {
        // Process each word, for example, print it
        std::cout << "Read word: " << word << std::endl;
    }
}

void LexicalAnalyzer::ProcessWordToken(std::string sym)
{




    // // ReadRest; {Fill the Lexeme}
    // if (isReservedWord(sym) == true)
    // {
    //     Token.push_back(sym);
    // }
    // // else if ()
    // // Code for special cases: and, or, not, div, mod, etc.
    // else
    // {
    //     // Token = idt
    // }
}
