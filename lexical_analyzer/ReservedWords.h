#include <string>

vector<std::string> reserved_words = {"MODULE", "PROCEDURE", "VAR",
    "BEGIN", "END", "IF", "THEN", "ELSE", "ELSIF", "WHILE", "DO",
     "ARRAY", "RECORD", "CONST", "TYPE"};

bool isReservedWord(std::string s)
{
    for (const &auto word : reserved_words)
    {
        if (s == word)
        {
            return true;
        }
    }
    
    return false;
}