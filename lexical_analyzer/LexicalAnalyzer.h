#include <iostream>
#include <string>
#include <vector>

//// Global Variables ////
std::vector<string> Lexeme;
std::vector<string> Token;

// Integer Tokens
std::vector<int> Value;
// Float Tokens
std::vector<float> ValueR;
// String Tokens
std::vector<std::string> Literal;

class LexicalAnalyzer 
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
    public:
        // Non-default Constructor
        LexicalAnalyzer(std::string filename);

        // Destructor
        ~LexicalAnalyzer();

        void GetNextToken();

    private:


};
