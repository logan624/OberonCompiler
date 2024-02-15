#include "LexicalAnalyzer.h"

#include <cctype>

// Global Variables //
Token token;
char c_char; // Current Character
char n_char; // Look-ahead character
int file_index;
int line_no;
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

    if (c_char == '\n')
    {
        line_no++;
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
        "RECORD", "CONST", "TYPE", "MOD", "OR", "DIV"};

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
        if (c_char == '*' && n_char == ')') {
            ReadNext();
            ReadNext();
            break;
        }

        if (c_char == '\0') {
            break;
        }

        ReadNext(); // Advance to the next character
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
    std::string file_contents = f_contents;

    Token ret_token;
    std::string identifier = "";
    
    identifier += c_char;
    ReadNext();

    while (std::isalpha(c_char) != 0 || std::isdigit(c_char) != 0) 
    {
        identifier += c_char;
        ReadNext();
    }

    // Read in the entire identifier word
    if (isReservedWord(identifier))
    {
        ret_token.m_lexeme = identifier;
        ret_token.m_token = whichToken(identifier);
    }
    else
    {
        if (identifier.length() > 17)
        {
            std::string temp = "";

            for(int i = 0; i <= 16; i++)
            {
                temp += identifier[i];
            }

            std::cout << "Error - Identifier Length: Identifier '" << identifier <<
                      "' longer than 17 characters, truncating to '" << temp << "'." << std::endl;
            
            identifier = temp;
        }

        ret_token.m_lexeme = identifier;
        ret_token.m_token = Token_T::IDENTIFIER;
    }

    return ret_token;
}

Token processNumber()
{
    std::string raw = "";
    bool is_int = true;
    int int_value;
    int real_value;

    Token ret_token;

    raw += c_char;
    ReadNext();

    while (std::isdigit(c_char) != 0 || c_char == '.')
    {
        if (c_char == '.')
        {
            if (std::isdigit(n_char) != 0)
            {
                is_int = false;
            }
            else
            {
                ret_token.m_value = std::stoi(raw);
                ret_token.m_token = Token_T::NUMBER;
                break;
            }
            
        }

        raw += c_char;
        ReadNext();
    }

    if (is_int)
    {
        ret_token.m_value = std::stoi(raw);
        ret_token.m_token = Token_T::NUMBER;   
    }
    else
    {
        ret_token.m_valuer = std::stof(raw);
        ret_token.m_token = Token_T::NUMBER;
    }

    ret_token.m_is_int = is_int;

    return ret_token;
}

Token processLiteral()
{
    char start_char = c_char;

    Token ret_token;
    bool finished_good = false;

    ret_token.m_literal = "";

    // Check to see if the starting character is either a ' or a ", and handle accordingly
    if (c_char == '\'')
    {
        ReadNext();

        while (c_char != '\'' && c_char != '\0' && c_char != '\n')
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
        ReadNext();

        while (c_char != '\"' && c_char != '\0' && c_char != '\n')
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
        ret_token.m_token = Token_T::LITERAL;
    }
    else
    {
        std::cout << "ERRROR - SYNTAX - Unterminated literal string '" << ret_token.m_literal << std::endl;
        ret_token.m_token = Token_T::UNKNOWN;
        ret_token.m_lexeme = start_char + ret_token.m_literal;
    }

    return ret_token;
}

// Member Function Definitions
LexicalAnalyzer::LexicalAnalyzer(std::string filename)
{
    m_file = filename;
    file_index = 0;
    line_no = 0;

    std::ifstream input_file(m_file);

    if (!input_file.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        exit(101); // Return an error code
    }
    
    std::string line;
        
    while (std::getline(input_file, line))
    {
        line += '\n';
        f_contents += line;
    }

    f_contents += '\0';

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
    std::string temp = "";

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
                    ret_token.m_lexeme = temp;
                    ret_token.m_token = Token_T::ADDOP;
                    ReadNext();
                    break;

                // MultOps -- Make sure to handle DIV and MOD in the identifier function
                case '*':
                case '/':
                case '&':
                    temp += c_char;
                    ret_token.m_lexeme = temp;
                    ret_token.m_token = Token_T::MULOP;
                    ReadNext();
                    break;

                // Brackets
                case '(':
                    if (c_char == '(' && n_char == '*')
                    {
                        ReadNext(); // Consume '('
                        ReadNext(); // Consume '*'
                        processComment(); // Now inside a comment
                        continue;
                        break;
                    }
                case '{':
                case '[':
                    temp += c_char;
                    ret_token.m_lexeme = temp;
                    ret_token.m_token = Token_T::L_SYMBOL;
                    ReadNext();
                    break;
                case ')':
                case '}':
                case ']':
                    temp += c_char;
                    ret_token.m_lexeme = temp;
                    ret_token.m_token = Token_T::R_SYMBOL;
                    ReadNext();
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
                    temp += c_char;
                    ret_token.m_lexeme = temp;
                    ret_token.m_token = whichToken(temp);
                    ReadNext();
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
                case ';':
                case ',':
                case '~':
                    temp += c_char;
                    ret_token.m_token = whichToken(temp);
                    ret_token.m_lexeme = temp;
                    ReadNext();
                    break;

                // Default - When unknown
                default:
                    ReadNext();
                    ret_token.m_token = Token_T::UNKNOWN;
                    ret_token.m_lexeme = "";
            }
        }

        token = ret_token;
        return ret_token;
    }

    if (c_char == '\0')
    {
        ret_token.m_token = Token_T::EOF_T;
        ret_token.m_lexeme = "";
    }

    token = ret_token;
    return ret_token;
}

std::string tokenTypeToString(Token_T t)
{
    std::map<Token_T, std::string> dict = 
    {
        {Token_T::MODULE, "MODULET"},
        {Token_T::PROCEDURE, "PROCT"},
        {Token_T::VAR, "VART"},
        {Token_T::BEGIN, "BEGINT"},
        {Token_T::END, "ENDT"},
        {Token_T::IF, "IFT"},
        {Token_T::THEN, "THENT"},
        {Token_T::ELSE, "ELSET"},
        {Token_T::ELSIF, "ELSIFT"},
        {Token_T::WHILE, "WHILET"},
        {Token_T::DO, "DOT"},
        {Token_T::ARRAY, "ARRAYT"},
        {Token_T::RECORD, "RECT"},
        {Token_T::CONST, "CONSTT"},
        {Token_T::TYPE, "TYPET"},
        {Token_T::INTEGER, "INTEGERT"},
        {Token_T::REAL, "REALT"},
        {Token_T::CHAR, "CHART"},
        {Token_T::RELOP, "RELOPT"},
        {Token_T::ADDOP, "ADDOPT"},
        {Token_T::MULOP, "MULOPT"},
        {Token_T::ASSOP, "ASSIGNOPT"},
        {Token_T::L_SYMBOL, "LPARENT"},
        {Token_T::R_SYMBOL, "RPARENT"},
        {Token_T::COMMA, "COMMAT"},
        {Token_T::COLON, "COLONT"},
        {Token_T::SEMICOLON, "SEMIT"},
        {Token_T::PERIOD, "PERIODT"},
        {Token_T::APOSTROPHE, "APOSTROPHET"},
        {Token_T::TILDAE, "TILDAET"},
        {Token_T::COMMENT, "COMMENTT"},
        {Token_T::UNKNOWN, "UNKNOWNT"},
        {Token_T::LITERAL, "LITERALT"},
        {Token_T::IDENTIFIER, "IDENTT"},
        {Token_T::NUMBER, "NUMBT"},
        {Token_T::EOF_T, "EOFT"}
    };

    return dict[t];
}

void LexicalAnalyzer::DisplayToken()
{
    switch (token.m_token)
    {
        case Token_T::LITERAL:

            std::cout << "Token Type: " << tokenTypeToString(token.m_token) << ", Literal: '" << token.m_literal << "'" << std::endl;
            break;

        case Token_T::NUMBER:

            if(token.m_is_int)
            {
                std::cout << "Token Type: " << tokenTypeToString(token.m_token) << ", Value: " << token.m_value << std::endl;
            }
            else
            {
                std::cout << "Token Type: " << tokenTypeToString(token.m_token) << ", Valuer: " << token.m_valuer << std::endl;
            }

            break;

        default:
            std::cout << "Token Type: " << tokenTypeToString(token.m_token) << ", Lexeme: " << token.m_lexeme << std::endl;
            break;
    }
}
