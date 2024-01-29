#include "LexicalAnalyzer.h"

#include <fstream>

// Global Variables //
Token token;
char c_char; // Current Character
char n_char; // Look-ahead character
int line_no;

// Procedural Function Definitons
bool isReservedWord(std::string s)
{
    std::vector<std::string> reserved_words = {"MODULE", "PROCEDURE", "VAR",
    "BEGIN", "END", "INTEGER", "REAL", "CHAR", "IF", "THEN", "ELSE", "ELSIF", "WHILE", "DO",
     "ARRAY", "RECORD", "CONST", "TYPE"};

    for (const std::string word : reserved_words)
    {
        if (s == word)
        {
            return true;
        }
    }
    
    return false;
}

// Member Function Definitions
LexicalAnalyzer::LexicalAnalyzer(std::string filename)
{
    m_file = filename;
    m_f_contents = "";
    line_no = -1;
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
    else{
        line_no = 0;
    }

    // Read the first character
    input_file.get(c_char);
    
    std::cout << c_char << std::endl;

    // Start a while loop to read characters until the end of the file
    while (input_file.get(n_char)) {

        c_char = n_char;

        std::cout << c_char << std::endl;

        if (c_char = '\n')
        {
            line_no++;
        }
    }

    input_file.close();
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
