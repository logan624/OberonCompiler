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
    EQUAL, HASH, LESST, LESSTE, GREATERT, GREATERTE, 
    // EQUAL, HASH, LESST, LESSTE, GREATERT, GREATERTE, 

    // Addition Subtraction Operators
    ADDOP,
    // PLUS, MINUS, OR,

    // Multiplication Operators
    MULOP,
    // MULT, DIV_SLASH, DIV, MOD, AMPERSAND,

    // Assignment Operator
    ASSOP,

    // Additional Operators
    L_SYMBOL, R_SYMBOL,
    // SOFT_PARENTHESES_OPEN, SOFT_PARENTHESES_CLOSE, 
    // POINTY_PARENTHESES_OPEN, POINTY_PARENTHESES_CLOSE,
    // HARD_BRACKET_OPEN, HARD_BRACKET_CLOSE,

    COMMA,
    COLON,
    SEMICOLON,
    PERIOD,
    APOSTROPHE,
    TILDAE,

    // Comments
    OPEN_COMMENT, CLOSE_COMMENT,

    // End of File
    EOF_T,

    // Unknown
    UNKNOWN
};

// Token Struct Type
struct Token
{
    Token_T m_token;
    std::string m_lexeme;
    int m_value;
    int m_valuer;
    std::string m_literal;
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
Token processIdentifier();
Token processNumber();
Token processLiteral();
Token processAddOperator();
Token processAssOperator();
Token processMultOperator();
Token processRelOperator();
Token processBrackets();

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
