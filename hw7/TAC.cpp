#include <string>
#include <algorithm>

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

// Returns vector of tokens if it could reduce stuff, otherwise retrns nothing
std::vector<Token> TacWriter::reduceMultiOp()
{
    std::vector<Token> ret;

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
    ret.push_back(token_stack.top());
    token_stack.pop();
    ret.push_back(token_stack.top());
    token_stack.pop();

    std::stack<Token> initial_state = token_stack;

    // Flag variables to indicate when to break into a temp
    bool first = false;
    bool first_op = false;
    bool first_op_second = false;
    bool additional_op = false;
    bool insert_next = false;

    // First identifier
    Token first_identifier;
    first_identifier.m_token = Token_T::UNKNOWN;
    // Operation
    Token operation;
    operation.m_token = Token_T::UNKNOWN;
    // Second Identifier
    Token second_identifier;
    second_identifier.m_token = Token_T::UNKNOWN;

    std::stack<Token> test = token_stack;
    Token last_temp;

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

        if (additional_op)
        {
            // Right after reading in an additional operation
            if (!token_stack.empty())
            {
                if (is_var)
                {
                    first_identifier = last_temp;
                    operation = operation;
                    second_identifier = t;

                    std::stack<Token> temp_s;
                    temp_s.push(second_identifier);
                    temp_s.push(operation);
                    temp_s.push(first_identifier);
                    
                    int temp_key = temp_map.insertTemp(temp_s);
                    last_temp.m_lexeme = std::to_string(temp_key);
                    last_temp.m_token = Token_T::TEMP;

                    ret.push_back(last_temp);

                    first_identifier.m_token = Token_T::UNKNOWN;
                    second_identifier.m_token = Token_T::UNKNOWN;
                    operation.m_token = Token_T::UNKNOWN;
                }
            }
            else
            {
                if (insert_next == true)
                {
                    insert_next = false;
                    ret.push_back(last_temp);
                }

                ret.push_back(operation);
                ret.push_back(t);
            }

            additional_op = false;

            continue;
        }

        // If there is a token left after reading 'var op var'
        if (first_op_second)
        {
            first = false;
            first_op = false;
            first_op_second = false;

            std::stack<Token> temp_s;
            temp_s.push(second_identifier);
            temp_s.push(operation);
            temp_s.push(first_identifier);
            
            int temp_key = temp_map.insertTemp(temp_s);
            last_temp.m_lexeme = std::to_string(temp_key);
            last_temp.m_token = Token_T::TEMP;

            // Might be wrong
            if (token_stack.empty() || token_stack.size() == 2)
            {
                ret.push_back(last_temp);
            }

            first_identifier.m_token = Token_T::UNKNOWN;
            second_identifier.m_token = Token_T::UNKNOWN;
            operation.m_token = Token_T::UNKNOWN;
        }

        // If nothing has been read in yet
        if (!first)
        {
            if (is_var)
            {
                first_identifier = t;
            }
            else
            {
                additional_op = true;
            }
        }
        else
        {
            // If something has been read in already
            if (first == true && first_op == false)
            {
                if (is_var)
                {
                    
                }
                else if (is_multi_op)
                {
                    operation = t;
                }
            }
            else if (first_op == true && first_op_second == false)
            {
                if (is_var)
                {
                    second_identifier = t;
                }
                else if (is_multi_op)
                {
                    
                }
            }
        }

        if (additional_op == true)
        {
            operation = t;

            continue;
        }

        // Logic to set the flags
        if (first_identifier.m_token == Token_T::UNKNOWN)
        {

        }
        else
        {
            first = true;

            if (operation.m_token == Token_T::UNKNOWN)
            {
            
            }
            else
            {
                first_op = true;

                if (second_identifier.m_token == Token_T::UNKNOWN)
                {

                }
                else
                {
                    first_op_second = true;

                    if (token_stack.size() == 2)
                    {
                        insert_next = true;
                    }
                }
            }
        }
    }

    if (initial_state.size() < 5)
    {
        while(initial_state.empty() == false)
        {
            ret.push_back(initial_state.top());
            initial_state.pop();
        }

        // reverse(ret.begin(), ret.end());
    }

    return ret;
}

// Returns vector of tokens if it could reduce stuff, otherwise retrns nothing
std::vector<Token> TacWriter::reduceTempMultiOp(std::stack<Token> p_stack)
{
    std::vector<Token> ret;

    // Have to reverse the token_stack
    // -------------------------------
    std::stack<Token> rev_stack;

    while (!p_stack.empty())
    {
        Token tok = p_stack.top();
        p_stack.pop();

        rev_stack.push(tok);
    }

    p_stack = rev_stack;
    // -------------------------------

    std::stack<Token> initial_state = p_stack;

    // Flag variables to indicate when to break into a temp
    bool first = false;
    bool first_op = false;
    bool first_op_second = false;
    bool additional_op = false;
    bool insert_next = false;

    // First identifier
    Token first_identifier;
    first_identifier.m_token = Token_T::UNKNOWN;
    // Operation
    Token operation;
    operation.m_token = Token_T::UNKNOWN;
    // Second Identifier
    Token second_identifier;
    second_identifier.m_token = Token_T::UNKNOWN;

    std::stack<Token> test = p_stack;
    Token last_temp;

    while(!p_stack.empty())
    {
        Token t = p_stack.top();
        p_stack.pop();

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

        if (additional_op)
        {
            // Right after reading in an additional operation
            if (!p_stack.empty())
            {
                if (is_var)
                {
                    first_identifier = last_temp;
                    operation = operation;
                    second_identifier = t;

                    std::stack<Token> temp_s;
                    temp_s.push(second_identifier);
                    temp_s.push(operation);
                    temp_s.push(first_identifier);
                    
                    int temp_key = temp_map.insertTemp(temp_s);
                    last_temp.m_lexeme = std::to_string(temp_key);
                    last_temp.m_token = Token_T::TEMP;

                    ret.push_back(last_temp);

                    first_identifier.m_token = Token_T::UNKNOWN;
                    second_identifier.m_token = Token_T::UNKNOWN;
                    operation.m_token = Token_T::UNKNOWN;
                }
            }
            else
            {
                if (insert_next == true)
                {
                    insert_next = false;
                    ret.push_back(last_temp);
                }

                ret.push_back(operation);
                ret.push_back(t);
            }

            additional_op = false;

            continue;
        }

        // If there is a token left after reading 'var op var'
        if (first_op_second)
        {
            first = false;
            first_op = false;
            first_op_second = false;

            std::stack<Token> temp_s;
            temp_s.push(second_identifier);
            temp_s.push(operation);
            temp_s.push(first_identifier);
            
            int temp_key = temp_map.insertTemp(temp_s);
            last_temp.m_lexeme = std::to_string(temp_key);
            last_temp.m_token = Token_T::TEMP;

            // Might be wrong
            if (p_stack.empty() || p_stack.size() == 2)
            {
                ret.push_back(last_temp);
            }

            first_identifier.m_token = Token_T::UNKNOWN;
            second_identifier.m_token = Token_T::UNKNOWN;
            operation.m_token = Token_T::UNKNOWN;
        }

        // If nothing has been read in yet
        if (!first)
        {
            if (is_var)
            {
                first_identifier = t;
            }
            else
            {
                additional_op = true;
            }
        }
        else
        {
            // If something has been read in already
            if (first == true && first_op == false)
            {
                if (is_var)
                {
                    
                }
                else if (is_multi_op)
                {
                    operation = t;
                }
            }
            else if (first_op == true && first_op_second == false)
            {
                if (is_var)
                {
                    second_identifier = t;
                }
                else if (is_multi_op)
                {
                    
                }
            }
        }

        if (additional_op == true)
        {
            operation = t;

            continue;
        }

        // Logic to set the flags
        if (first_identifier.m_token == Token_T::UNKNOWN)
        {

        }
        else
        {
            first = true;

            if (operation.m_token == Token_T::UNKNOWN)
            {
            
            }
            else
            {
                first_op = true;

                if (second_identifier.m_token == Token_T::UNKNOWN)
                {

                }
                else
                {
                    first_op_second = true;

                    if (p_stack.size() == 2)
                    {
                        insert_next = true;
                    }
                }
            }
        }
    }

    if (initial_state.size() < 4)
    {
        while(initial_state.empty() == false)
        {
            ret.push_back(initial_state.top());
            initial_state.pop();
        }

        // reverse(ret.begin(), ret.end());
    }

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
            std::vector<Token> reduced_stack = reduceTempMultiOp(temp_stack);
            
            std::stack<Token> ns;

            for (long int i = 0; i < reduced_stack.size(); i++)
            {
                ns.push(reduced_stack[i]);
                // reduced_stack.pop_back();
            }

            int temp_key = temp_map.insertTemp(ns);

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
            std::vector<Token> reduced_stack = reduceTempMultiOp(temp_stack);
            
            std::stack<Token> ns;

            for (long int i = 0; i < reduced_stack.size(); i++)
            {
                ns.push(reduced_stack[i]);
                // reduced_stack.pop_back();
            }

            int temp_key = temp_map.insertTemp(ns);

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
