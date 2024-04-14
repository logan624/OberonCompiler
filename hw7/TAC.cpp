#include <string>

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

void printTemp(Token t)
{
    std::cout << "-----------" << std::endl;

    std::stack<Token> * s = temp_map.getTemp(std::stoi(t.m_lexeme));
    
    while (!s->empty())
    {
        Token ele = s->top();
        s->pop();

        if (token.m_token == Token_T::TEMP)
        {
            printTemp(ele);
        }
        else
        {
            DisplayToken(t);
        }
    }


    std::cout << "-----------" << std::endl;
}

void TacWriter::preprocStatement()
{
    // Preprocess Token Stack
    //      Create temp variables for the unary operands, parentheses
    //      since those have the highest precedence
    
    // Entire stack will be reversed and move to this
    std::stack<Token> new_stack;

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
        std::stack<Token> test_one = token_stack;
        Token t = token_stack.top();
        token_stack.pop();
 
        if (t.m_token == Token_T::TEMP_BEGIN)
        {
            std::stack<Token> test = token_stack;
            
            Token temp_token = handleNestedTemp();
            new_stack.push(temp_token);
        }
        else
        {
            new_stack.push(t);
        }
    }

    token_stack = new_stack;

    while (token_stack.empty() != true)
    {
        Token t = token_stack.top();
        token_stack.pop();

        DisplayToken(t);
    }
}

void TacWriter::procStatement()
{
    // Process Token Stack
    //      Break multioperation statements down into ones of two max
}

std::vector<std::stack<Token>> TacWriter::reduceMultiOp(std::stack<Token> s)
{
    std::vector<std::stack<Token>> ret;

    // Flag variables to indicate when to break into a temp

    

    return ret;
}

Token TacWriter::handleNestedTemp(bool recursive)
{
    Token ele;
    std::stack<Token> temp_stack;

    while (!token_stack.empty())
    {
        ele = token_stack.top();
        token_stack.pop();

        if (ele.m_token == Token_T::TEMP_BEGIN)
        {
            Token ret_val = handleNestedTemp(true);
            temp_stack.push(ret_val);
        }
        else if (ele.m_token == Token_T::TEMP_END)
        {
            int temp_key = temp_map.insertTemp(temp_stack);

            Token temp_token;
            temp_token.m_lexeme = std::to_string(temp_key);
            temp_token.m_token = Token_T::TEMP;
            
            return temp_token;
        }
        else
        {
            temp_stack.push(ele);
        }
    }

    // Useless return since it should never get here
    return ele;
}

Token TacWriter::handleNestedTemp()
{
    // Just read second TEMP_BEGIN while processing a temp
    //      Read until TEMP_END
    // Check if encountering another temp:
    //      Call the function again
    std::stack<Token> test = token_stack;
    Token ele;
    std::stack<Token> temp_stack;

    while (!token_stack.empty())
    {
        ele = token_stack.top();
        token_stack.pop();

        if (ele.m_token == Token_T::TEMP_BEGIN)
        {
            Token ret_val = handleNestedTemp(true);
            temp_stack.push(ret_val);
        }
        else if (ele.m_token == Token_T::TEMP_END)
        {
            int temp_key = temp_map.insertTemp(temp_stack);

            Token temp_token;
            temp_token.m_lexeme = std::to_string(temp_key);
            temp_token.m_token = Token_T::TEMP;
            
            return temp_token;
        }
        else
        {
            temp_stack.push(ele);
        }
    }

    // Useless return since it should never get here
    return ele;
}
