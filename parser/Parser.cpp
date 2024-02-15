#ifndef PARSER_H

#include "Parser.h"

bool prev_empty;

void checkNextToken(Token_T expected, bool empty_ok)
{
    if (!prev_empty)
    {
        LexicalAnalyzer::GetNextToken();
    }
    else
    {
        prev_empty = false;
    }

    // std::cout << token.m_lexeme << std::endl;

    if (token.m_token == expected)
    {
        return;
    }
    else
    {
        if (empty_ok)
        {
            prev_empty = true;
            return;
        }
        else
        {
            std::cout << "ERROR - PARSING - LINE " << line_no << " - Got Token: " << tokenTypeToString(token.m_token) << ", Expected Token: " << tokenTypeToString(expected) << std::endl;
            exit(120);
        }
    }
    
    return;
}

void checkNextToken(std::vector<Token_T> expected_options, bool empty_ok)
{
    if (!prev_empty)
    {
        LexicalAnalyzer::GetNextToken();
    }
    else
    {
        prev_empty = false;
    }

    bool accepted = false;

    for (const Token_T poss_type: expected_options)
    {
        if (token.m_token == poss_type)
        {
            accepted = true;
            break;
        }
    }   

    if (accepted)
    {
        return;
    }
    else
    {
        if (empty_ok)
        {
            prev_empty = true;
            return;
        }
        else
        {
            std::cout << "ERROR - PARSING - LINE " << line_no << " - Got Token: " << tokenTypeToString(token.m_token) << ", Expected Token: ";
            
            for (long int i = 0; i < expected_options.size(); i++)
            {
                std::cout << tokenTypeToString(expected_options[i]);
                
                if (i != expected_options.size() - 1)
                {
                    std::cout << ", ";
                }
                else
                {
                    std::cout << std::endl;
                }
            }

            exit(120);
        }
    }

    Token_T test_t = token.m_token;
    
    return;
}

// Prog -> modulet idt ; DeclarativePart StatementPart endt idt
void Prog()
{
    prev_empty = false;

    checkNextToken(Token_T::MODULE, false);
    checkNextToken(Token_T::IDENTIFIER, false);
    checkNextToken(Token_T::SEMICOLON, false);
    DeclarativePart();
    StatementPart();
    checkNextToken(Token_T::END, false);
    checkNextToken(Token_T::IDENTIFIER, false);
}

// DeclarativePart -> ConstPart VarPart ProcPart
void DeclarativePart()
{
    ConstPart();
    VarPart();
    ProcPart();
}

// ConstPart -> constt ConstTail | e
void ConstPart()
{
    checkNextToken(Token_T::CONST, true);

    if (prev_empty)
    {
        return;
    }

    ConstTail();
}

// ConstTail -> idt = Value  ConstTail | e
void ConstTail()
{
    checkNextToken(Token_T::IDENTIFIER, true);

    if (prev_empty)
    {
        return;
    }

    checkNextToken(Token_T::EQUAL_SIGN, false);
    Value();
    ConstTail();
}

// VarPart -> vart VarTail | e
void VarPart()
{
    checkNextToken(Token_T::VAR, true);

    if (prev_empty)
    {
        return;
    }

    VarTail();

}

// THIS NEEDS CHECKED -- NOT SURE IF THE CASE WHERE IdentifierList is EMPTY IS BEING HANDLED CORRECLTY
// VarTail -> IdentifierList : TypeMark ; VarTail | e
void VarTail()
{
    LexicalAnalyzer::GetNextToken();
    prev_empty = true;

    Token t = token;

    if (token.m_token != Token_T::IDENTIFIER)
    {
        return;
    }

    IdentifierList();

    // if (prev_empty)
    // {
    //     return;
    // }

    checkNextToken(Token_T::COLON, false);
    TypeMark();
    checkNextToken(Token_T::SEMICOLON, false);
    VarTail();

}

// NEEDS TESTED
// IdentifierList -> idt | IdentifierList , idt
void IdentifierList()
{
    checkNextToken(Token_T::IDENTIFIER, false);
    // TODO - Condition might be wrong
    while (token.m_token == Token_T::IDENTIFIER)
    {
        checkNextToken(Token_T::COMMA, true);
        if (prev_empty)
        {
            return;
        }
        checkNextToken(Token_T::IDENTIFIER, false);
    }

}

// TypeMark -> integert | realt | chart
void TypeMark()
{
    std::vector<Token_T> types_to_check = { Token_T::INTEGER, Token_T::REAL, Token_T::CHAR };
    checkNextToken(types_to_check, false);
}

// Value -> NumericalLiteral
void Value()
{
    checkNextToken(Token_T::NUMBER, false);
}

// ProcPart -> ProcedureDecl ProcPart | e
void ProcPart()
{
    // Token t = token;
    // // Check if ProcPart is empty
    // LexicalAnalyzer::GetNextToken();
    // Token t2 = token;
    // prev_empty = true;

    if (token.m_token != Token_T::PROCEDURE)
    {
        return;
    } 

    ProcedureDecl();
    ProcPart();


}

// ProcedureDecl -> ProcHeading ; ProcBody idt ;
void ProcedureDecl()
{
    ProcHeading();
    checkNextToken(Token_T::SEMICOLON, false);
    ProcBody();
    checkNextToken(Token_T::IDENTIFIER, false);
    checkNextToken(Token_T::SEMICOLON, false);
}

// ProcHeading -> proct idt Args
void ProcHeading()
{
    checkNextToken(Token_T::PROCEDURE, false);
    checkNextToken(Token_T::IDENTIFIER, false);
    Args();
}

// ProcBody -> DeclarativePart StatementPart endt
void ProcBody()
{
    DeclarativePart();
    prev_empty = true;
    StatementPart();

    Token t = token;
    checkNextToken(Token_T::END, false);
}

// Args -> ( ArgList ) | e
void Args()
{
    checkNextToken(Token_T::L_SYMBOL, true);
    
    if (prev_empty)
    {
        return;
    }

    ArgList();
    checkNextToken(Token_T::R_SYMBOL, false);
}

// ArgList -> Mode IdentifierList : TypeMark MoreArgs
void ArgList()
{
    Mode();
    IdentifierList();
    checkNextToken(Token_T::COLON, false);
    TypeMark();
    MoreArgs();
}

// MoreArgs -> ; ArgList | e
void MoreArgs()
{
    checkNextToken(Token_T::SEMICOLON, true);

    if (prev_empty == true)
    {
        return;
    }

    ArgList();
}

// Mode -> vart | e
void Mode()
{
    checkNextToken(Token_T::VAR, true);
}

// StatementPart -> begint SeqOfStatements | e
void StatementPart()
{
    // Token t = token;
    // prev_empty = true;
    checkNextToken(Token_T::BEGIN, true);

    if (prev_empty)
    {
        return;
    }

    SeqOfStatements();
}

// SeqOfStatements -> e
void SeqOfStatements()
{
    while(token.m_token != Token_T::END)
    {
        LexicalAnalyzer::GetNextToken();

        if (token.m_token == Token_T::EOF_T)
        {
            break;
        }
    }

    prev_empty = true;
    
    return;

}


#endif
