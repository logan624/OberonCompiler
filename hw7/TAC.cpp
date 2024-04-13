#include "TAC.h"
#include "Parser.h"

extern TempMap temp_map;

void TacWriter::writeFile()
{

}

void TacWriter::writeOperation()
{

}

void TacWriter::writeAssignment()
{

}

void TacWriter::preprocStatement()
{
    // Preprocess Token Stack
    //      Create temp variables for the unary operands, parentheses
    //      since those have the highest precedence
    
    // Entire stack will be reversed and move to this
    std::stack<Token> new_stack;
    // Will be used to wrap 'temp' variables into a single token
    std::stack<Token> temp_stack;

    // Used as a LCV to indicate if current tokens are part of the current temp variable
    bool creating_temp = false;

    // Reverse the token stack (needed for it to be in left to right order)
    // ---------------------------------------------------------------------
    while (token_stack.empty() != true)
    {
        Token t = token_stack.top();
        token_stack.pop();
        new_stack.push(t);
    }

    token_stack = new_stack;

    while (new_stack.empty() != true)
    {
        new_stack.pop();
    }

    // ---------------------------------------------------------------------

    while (token_stack.empty() != true)
    {
        Token t = token_stack.top();
        token_stack.pop();

        if (creating_temp == true)
        {
            if (t.m_token == Token_T::UNKNOWN)
            {
                creating_temp = false;

                // Create a temp for the current stack
                int temp_key = temp_map.insertTemp(temp_stack);

                Token temp_token;
                temp_token.m_token = Token_T::TEMP;
                temp_token.m_lexeme = std::to_string(temp_key);

                new_stack.push(temp_token);
            }
        }   
        else if (t.m_token == Token_T::UNKNOWN)
        {
            creating_temp = true;
        }
        else
        {
            new_stack.push(t);
        }

        temp_stack = new_stack;
    }

    
    std::cout << "STATEMENT:" << std::endl;
    std::cout << "----------" << std::endl;

    while (new_stack.empty() != true)
    {
        Token t = new_stack.top();
        new_stack.pop();

        if (token.m_token == Token_T::TEMP)
        {
            std::stack<Token> * sp = temp_map.getTemp(std::stoi(token.m_lexeme));
            std::stack<Token> s = *sp;

            std::cout << "";
        }

        DisplayToken(t);
    }
}

void TacWriter::procStatement()
{
    // Process Token Stack
    //      Break multioperation statements down into ones of two max
}
