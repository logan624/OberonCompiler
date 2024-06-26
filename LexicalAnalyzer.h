#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <fstream>

// Type Declarations //

// Enum type to identify different tokens that can exist in the language
enum class Token_T {
    // Reserved Words
    MODULE, PROCEDURE, VAR, BEGIN, END, IF, THEN, ELSE, ELSIF, WHILE, DO,
    ARRAY, RECORD, CONST, TYPE,

    // Type Operators
    INTEGER, REAL, CHAR,

    // Relational Operators
    EQUAL_SIGN,
    RELOP,

    // Addition Subtraction Operators
    ADDOP,

    // Sign Operators
    MINUS,

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
    NUMBER,

    // Temp - Used in intermediate code generation
    TEMP_BEGIN,
    TEMP_END,
    TEMP,

    // READ / WRITE
    READ,
    WRITE,
    WRITELN
};

enum class WriteReadType {
    CHAR,
    INT,
    STR,
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
    WriteReadType write;
};

extern std::map<std::string, Token_T> reserved_words_dict;

// Global Variables //
extern Token token;
extern char c_char; // Current Character
extern char n_char; // Look-ahead character
extern int file_index;
extern int line_no;
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
void DisplayToken(Token t);

// Class Declaration
class LexicalAnalyzer 
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
    public:
        LexicalAnalyzer(std::string filename);
        ~LexicalAnalyzer();
        static Token GetNextToken();
        void DisplayToken();

    private:
        std::string m_file;

        void ProcessWordToken(char sym);
};
