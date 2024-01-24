#include <iostream>
#include <string>
#include <vector>

//// Global Variables ////
std::string Lexeme;
std::string Token;

// Integer Tokens
std::vector<int> Value;
// Float Tokens
std::vector<float> ValueR;
// String Tokens
std::vector<std::string> Literal;

class LexicalAnalyzer 
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
    public:
        LexicalAnalyzer(std::string filename);
        ~LexicalAnalyzer();
        void GetNextToken();

    private:
        void ProcessWordToken(std::string sym);

};
