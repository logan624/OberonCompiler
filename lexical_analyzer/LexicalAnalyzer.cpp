#include "LexicalAnalyzer.h"

// Global Variables //
Token token;
char c_char; // Current Character
char n_char; // Look-ahead character
int file_index;
std::string f_contents;

std::map<std::string, Token_T> reserved_words_dict = {
        // Reserved Words
        {"MODULE", Token_T::MODULE},
        {"PROCEDURE", Token_T::PROCEDURE},
        {"VAR", Token_T::VAR},
        {"BEGIN", Token_T::BEGIN},
        {"END", Token_T::END},
        {"IF", Token_T::IF},
        {"THEN", Token_T::THEN},
        {"ELSE", Token_T::ELSE},
        {"ELSIF", Token_T::ELSIF},
        {"WHILE", Token_T::WHILE},
        {"DO", Token_T::DO},
        {"ARRAY", Token_T::ARRAY},
        {"RECORD", Token_T::RECORD},
        {"CONST", Token_T::CONST},
        {"TYPE", Token_T::TYPE},
        
        // Type Operators
        {"INTEGER", Token_T::INTEGER},
        {"REAL", Token_T::REAL},
        {"CHAR", Token_T::CHAR},
        
        // Relational Operators
        {"=", Token_T::RELOP},
        {"#", Token_T::RELOP},
        {"<", Token_T::RELOP},
        {"<=", Token_T::RELOP},    
        {">", Token_T::RELOP},     
        {">=", Token_T::RELOP},

        // Addition Subtraction Operators
        {"+", Token_T::ADDOP},
        {"-", Token_T::ADDOP},
        {"OR", Token_T::ADDOP},

        // Multiplication Operators
        {"*", Token_T::MULOP},
        {"/", Token_T::MULOP},
        {"DIV", Token_T::MULOP},
        {"MOD", Token_T::MULOP},
        {"&", Token_T::MULOP},

        // Assignment Operators
        {":=", Token_T::ASSOP},

        // Additional Operators
        {"(", Token_T::L_SYMBOL},
        {")", Token_T::R_SYMBOL},
        {"{", Token_T::L_SYMBOL},
        {"}", Token_T::R_SYMBOL},
        {"[", Token_T::L_SYMBOL},
        {"]", Token_T::R_SYMBOL},
        {",", Token_T::COMMA},
        {":", Token_T::COLON},
        {";", Token_T::SEMICOLON},
        {".", Token_T::PERIOD},
        {"\'", Token_T::APOSTROPHE},
        {"~", Token_T::TILDAE},

        // Comments
        {"(*", Token_T::COMMENT},
        {"*)", Token_T::COMMENT},

        // End of File
        {"EOF", Token_T::EOF_T},

        // Literal
        {"LITERAL", Token_T::LITERAL}
    };

void ReadNext()
{
    file_index++;
    
    if (file_index + 1 > f_contents.length() - 1)
    {
        c_char = f_contents[file_index];
        n_char = '\0';
    }
    else
    {
        c_char = n_char;
        n_char = f_contents[file_index + 1];
    }
    
    return;
}

// Procedural Function Definitons
bool isReservedWord(std::string s)
{
    std::vector<std::string> reserved_words = 
    {
        "MODULE", "PROCEDURE", "VAR", "BEGIN", "END", "INTEGER", "REAL",
        "CHAR", "IF", "THEN", "ELSE", "ELSIF", "WHILE", "DO", "ARRAY",
        "RECORD", "CONST", "TYPE"};

    for (const std::string word : reserved_words)
    {
        if (s == word)
        {
            return true;
        }
    }
    
    return false;
}

Token_T whichToken(std::string input)
{
    Token_T ret_value;

    auto it = reserved_words_dict.find(input);

    if (it != reserved_words_dict.end()) 
    {
        ret_value = it->second;
    }
    else 
    {
        ret_value = Token_T::UNKNOWN;
    }

    return ret_value;
}

void processComment()
{
    // Assuming you already read `(*`, so start looking for the ending `*)`
    while (true)
    {
        ReadNext(); // Advance to the next character
        if (c_char == '*' && n_char == ')') {
            ReadNext(); // Consume '*'
            ReadNext(); // Consume ')' and exit the comment
            break;
        }

        if (c_char == '\0') {
            // End of file reached without closing comment
            // Handle error or exit
            break;
        }
    }
    return;
}

void processWhitespace()
{
    while (std::isspace(c_char) && c_char != '\0')
        ReadNext();

    return;
}

Token processIdentifier()
{
    Token ret_token;
    std::string identifier = "";
    
    identifier += c_char;
    ReadNext();

    while (std::isalpha(c_char) == true || std::isdigit(c_char) == true)
    {
        identifier += c_char;
        ReadNext();
    }

    // Read in the entire identifier word

    return ret_token;
}

Token processNumber()
{
    Token ret_token;

    return ret_token;
}

Token processLiteral()
{
    Token ret_token;
    bool finished_good = false;

    ret_token.m_literal = "";

    // Check to see if the starting character is either a ' or a ", and handle accordingly
    if (c_char == '\'')
    {
        while (c_char != '\'' && c_char != '\0')
        {
            ret_token.m_literal += c_char;
            ReadNext();
        }

        if (c_char == '\'')
        {
            ReadNext();
            finished_good = true;
        }
    }
    else if (c_char == '\"')
    {
        while (c_char != '\"' && c_char != '\0')
        {
            ret_token.m_literal += c_char;
            ReadNext();
        }
        
        if (c_char == '\"')
        {
            ReadNext();
            finished_good = true;
        }
    }

    if (finished_good)
    {
        ret_token.m_lexeme = ret_token.m_literal;
        ret_token.m_token = Token_T::LITERAL;
    }
    else
    {
        std::cout << "ERRROR - SYNTAX - Unterminated literal string.\nTerminating Compilation."
                  << std::endl;
    }

    return ret_token;
}

Token processBrackets()
{
    Token ret_token;

    return ret_token;
}

// Member Function Definitions
LexicalAnalyzer::LexicalAnalyzer(std::string filename)
{
    m_file = filename;
    file_index = 0;

    std::ifstream input_file(m_file);

    if (!input_file.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        exit(101); // Return an error code
    }
    
    std::string line;
        
    while (std::getline(input_file, line))
    {
        f_contents += line;
    }

    input_file.close();

    c_char = f_contents[file_index];
    n_char = f_contents[file_index + 1];
}
 
LexicalAnalyzer::~LexicalAnalyzer()
{

}

Token LexicalAnalyzer::GetNextToken()
{
    Token ret_token;
    std::string temp;

    while (c_char != '\0') 
    {  
        // Check if the current character is part of whitespace
        if (isspace(c_char)) 
        {
            processWhitespace();
            continue;
        }
        // Check if the current character is part of an identifier
        else if (isalpha(c_char)) 
        {
            ret_token = processIdentifier();
        }
        // Check if the current character is part of a number
        else if (std::isdigit(c_char)) 
        {
            ret_token = processNumber();
        }
        else
        {
            switch (c_char)
            {
                // AddOps
                case '+':    
                case '-':
                // Need to Include OR in the Add_Ops
                //      - Probably call from identifier?
                    temp += c_char;
                    token.m_lexeme = temp;
                    token.m_token = Token_T::ADDOP;
                    break;

                // MultOps -- Make sure to handle DIV and MOD in the identifier function
                case '*':
                case '/':
                case '&':
                    temp += c_char;
                    token.m_lexeme = temp;
                    token.m_token = Token_T::MULOP;
                    break;

                // Brackets
                case '(':
                    if (c_char == '(' && n_char == '*')
                    {
                        ReadNext(); // Consume '('
                        ReadNext(); // Consume '*'
                        processComment(); // Now inside a comment
                        ReadNext();

                        break;
                    }
                case ')':
                case '{':
                case '}':
                case '[':
                case ']':
                    ret_token = processBrackets();
                    break;

                // Literals
                case '\'':
                case '\"':
                    ret_token = processLiteral();
                    break;

                // Relational Operators
                case '<':
                case '>':
                case '=':
                case '#':

                    break;

                // Assign Operator and Symbols
                case ':':
                    // Need to check if it is the assignment operator
                    if (n_char == '=')
                    {
                        ret_token.m_token = Token_T::ASSOP;
                        ret_token.m_lexeme = ":=";
                        ReadNext();
                        ReadNext();
                    }
                    else
                    {
                        ret_token.m_token = Token_T::COLON;
                        ret_token.m_lexeme = ":";
                        ReadNext();
                    }

                    break;
                case '.':
                    ReadNext();
                    ret_token.m_token = Token_T::PERIOD;
                    ret_token.m_lexeme  = ".";
                    break;
                case ';':
                    ReadNext();
                    ret_token.m_token = Token_T::SEMICOLON;
                    ret_token.m_lexeme  = ";";
                    break;
                case ',':
                    ReadNext();
                    ret_token.m_token = Token_T::COMMA;
                    ret_token.m_lexeme  = ",";
                    break;
                case '~':
                    ReadNext();
                    ret_token.m_token = Token_T::TILDAE;
                    ret_token.m_lexeme  = "~";
                     break;

                // Default - When unknown
                default:
                    ReadNext();
                    ret_token.m_token = Token_T::UNKNOWN;
                    ret_token.m_lexeme = "";
            }
        }

        return ret_token;
    }

    if (c_char == '\0')
    {
        ret_token.m_token = Token_T::EOF_T;
        ret_token.m_lexeme = "";
    }

    return ret_token;
}

void LexicalAnalyzer::DisplayToken()
{

}
