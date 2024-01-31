#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <fstream>

// Type Declarations //

// Enum type to identify different tokens that can exist in the language
enum Token_T {
    // Reserved Words
    MODULE, PROCEDURE, VAR, BEGIN, END, IF, THEN, ELSE, ELSIF, WHILE, DO,
    ARRAY, RECORD, CONST, TYPE,

    // Type Operators
    INTEGER, REAL, CHAR,

    // Relational Operators
    RELOP,

    // Addition Subtraction Operators
    ADDOP,

    // Multiplication Operators
    MULOP,

    // Assignment Operator
    ASSOP,

    // Additional Operators
    L_SYMBOL, R_SYMBOL,
    COMMA,
    COLON,
    SEMICOLON,
    PERIOD,
    APOSTROPHE,
    TILDAE,

    // Comments
    COMMENT,

    // End of File
    EOF_T,

    // Unknown
    UNKNOWN,

    // Literal
    LITERAL,

    // Identifier
    IDENTIFIER,

    // Number
    NUMBER
};

// Token Struct Type
struct Token
{
    Token_T m_token;
    std::string m_lexeme;
    int m_value;
    float m_valuer;
    std::string m_literal;
    bool m_is_int;
};

extern std::map<std::string, Token_T> reserved_words_dict;

// Global Variables //
extern Token token;
extern char c_char; // Current Character
extern char n_char; // Look-ahead character
extern int file_index;
extern std::string f_contents;

// Procedural Functions
void ReadNext();
bool isReservedWord(std::string s);
Token_T whichToken(std::string input);
void processComment();
void processWhitespace();
std::string tokenTypeToString(Token_T t);
Token processIdentifier();
Token processNumber();
Token processLiteral();

// Class Declaration
class LexicalAnalyzer 
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
    public:
        LexicalAnalyzer(std::string filename);
        ~LexicalAnalyzer();
        Token GetNextToken();
        void DisplayToken();

    private:
        std::string m_file;

        void ProcessWordToken(char sym);
};
