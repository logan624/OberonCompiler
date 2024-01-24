#include "LexicalAnalyzer.h"
#include "ReservedWords.h"
 
LexicalAnalyzer::LexicalAnalyzer(std::string filename)
{

}
 
LexicalAnalyzer::~LexicalAnalyzer()
{

}

void LexicalAnalyzer::GetNextToken()
{
    
}

void LexicalAnalyzer::ProcessWordToken(std::string sym)
{
    // ReadRest; {Fill the Lexeme}
    if (isReservedWord(sym) == true)
    {
        Token.push_back(sym);
    }
    // else if ()
    // Code for special cases: and, or, not, div, mod, etc.
    else
    {
        // Token = idt
    }
}
