#include <iostream>
#include <string>
#include <vector>

// Type Declarations //

// Enum type to identify different tokens that can exist in the language
enum Token_T {
    // Reserved Words
    MODULE, PROCEDURE, VAR, BEGIN, END, IF, THEN, ELSE, ELSIF, WHILE, DO,
    ARRAY, RECORD, CONST, TYPE,
    // Relational Operators
    EQUAL, HASH, LESST, LESSTE, GREATERT, GREATERTE, 
    // Addition Subtraction Operators
    PLUS, MINUS, OR,
    // Multiplication Operators
    MULT, DIV_SLASH, DIV, MOD, AMPERSAND,
    // Assignment Operator
    ASSIGN,
    // Additional Operators
    SOFT_PARENTHESES_OPEN, SOFT_PARENTHESES_CLOSE, 
    POINTY_PARENTHESES_OPEN, POINTY_PARENTHESES_CLOSE,
    HARD_BRACKET_OPEN, HARD_BRACKET_CLOSE,
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

// Global Variables //
extern Token token;
extern int c_char;
extern int line_no;

class LexicalAnalyzer 
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
    public:
        LexicalAnalyzer(std::string filename);
        ~LexicalAnalyzer();
        void GetNextToken();

    private:
        std::string m_file;

        void ProcessWordToken(std::string sym);
};
