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

    Token_T test_t = token.m_token;
    
    return;
}

// Prog -> modulet idt DeclarativePart StatementPart endt idt
void Prog()
{
    prev_empty = false;

    checkNextToken(Token_T::MODULE, false);
    checkNextToken(Token_T::IDENTIFIER, false);
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

    // Figure out if = should be :=??????
    // checkNextToken(Token_T::)
}

// VarPart -> vart VarTail | e
void VarPart()
{
    
}

// VarTail -> IdentifierList : TypeMark  VarTail | e
void VarTail()
{
    
}

// IdentifierList -> idt | IdentifierList , idt
void IdentifierList()
{
    
}

// TypeMark -> integert | realt | chart
void TypeMark()
{
    
}

// Value -> NumericalLiteral
void Value()
{
    
}

// ProcPart -> ProcedureDecl ProcPart | e
void ProcPart()
{
    
}

// ProcedureDecl -> ProcHeading  ProcBody idt 
void ProcedureDecl()
{
    
}

// ProcHeading -> proct idt Args
void ProcHeading()
{
    
}

// ProcBody -> DeclarativePart StatementPart endt
void ProcBody()
{
    
}

// Args -> ( ArgList ) | e
void Args()
{
    
}

// ArgList -> Mode IdentifierList : TypeMark MoreArgs
void ArgList()
{
    
}

// MoreArgs ->  ArgList | e
void MoreArgs()
{
    
}

// Mode -> vart | e
void Mode()
{
    
}

// StatementPart -> begint SeqOfStatements | e
void StatementPart()
{
    
}

// SeqOfStatements -> e
void SeqOfStatements()
{
    
}


#endif
