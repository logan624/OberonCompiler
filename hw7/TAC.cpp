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
    reduceMultiOp();

    // while (token_stack.empty() != true)
    // {
    //     Token t = token_stack.top();
    //     token_stack.pop();

    //     DisplayToken(t);
    // }
}

void TacWriter::procStatement()
{
    // Process Token Stack
    //      Break multioperation statements down into ones of two max
}

std::vector<std::stack<Token>> TacWriter::reduceMultiOp()
{
    std::vector<std::stack<Token>> ret;

    std::stack<Token> new_stack;

    // Have to reverse the token_stack
    // -------------------------------
    std::stack<Token> rev_stack;

    while (!token_stack.empty())
    {
        Token tok = token_stack.top();
        token_stack.pop();

        rev_stack.push(tok);
    }

    token_stack = rev_stack;
    // -------------------------------

    // Pop the identifier and the assignment operator
    new_stack.push(token_stack.top());
    token_stack.pop();
    new_stack.push(token_stack.top());
    token_stack.pop();

    // Flag variables to indicate when to break into a temp
    bool first = false;
    bool first_op = false;
    bool first_op_second = false;

    // First identifier
    Token first_identifier;
    first_identifier.m_token = Token_T::UNKNOWN;
    // Operation
    Token operation;
    operation.m_token = Token_T::UNKNOWN;
    // Second Identifier
    Token second_identifier;
    second_identifier.m_token = Token_T::UNKNOWN;

    // Reverse the token stack
    // -------------------------
    std::stack<Token> temp;

    while (!token_stack.empty())
    {
        temp.push(token_stack.top());
        token_stack.pop();
    }
    token_stack = temp;
    // -------------------------

    std::stack<Token> test = token_stack;

    while(!token_stack.empty())
    {
        Token t = token_stack.top();
        token_stack.pop();

        bool is_var = false;
        bool is_multi_op = false;
        
        // Determine what type the current token is
        if (t.m_token == Token_T::MULOP || t.m_token == Token_T::ADDOP)
        {
            is_multi_op = true;
        }
        else if (t.m_token == Token_T::IDENTIFIER || t.m_token == Token_T::NUMBER || t.m_token == Token_T::TEMP)
        {
            is_var = true;
        }

        // If nothing has been read in yet
        if (!first)
        {
            if (is_var)
            {

            }
            else if (is_multi_op)
            {

            }
        }
        else
        {
            // If something has been read in already
            if (first)
            {
                if (is_var)
                {

                }
                else if (is_multi_op)
                {
                    
                }
            }
            else if (first_op)
            {
                if (is_var)
                {

                }
                else if (is_multi_op)
                {
                    
                }
            }
            else if (first_op_second)
            {
                if (is_var)
                {

                }
                else if (is_multi_op)
                {
                    
                }
            }
        }

        // Logic to set the flags
        if (first_identifier.m_token == Token_T::UNKNOWN)
        {
            if (operation.m_token == Token_T::UNKNOWN)
            {
                if (second_identifier.m_token == Token_T::UNKNOWN)
                {

                }
                else
                {
                    first_op_second = true;
                }
            }
            else
            {
                first_op = true;
            }
        }
        else
        {
            first = true;
        }
    }
    

    // Logic

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
