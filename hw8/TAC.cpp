#include <string>
#include <algorithm>

#include "TAC.h"
#include "Parser.h"
#include <sstream>

extern std::ostringstream tac_file;

extern SymbolTable st;
extern TempMap temp_map;
extern std::string curr_procedure;
extern int curr_scope_offset;
extern std::ostringstream asm_file;

using namespace std;

std::string TacWriter::printVar(Token t)
{
    bool is_param = false;
    int param_offset = -1;
    bool is_ref = false;
    // Load in the params for the current procedure
    TableRecord * tr = st.Lookup(curr_procedure);

    if (tr != nullptr)
    {
        ParameterInfo * node = tr->item.procedure.param_info;

        while (node != nullptr)
        {
            if (node->m_token.m_lexeme == t.m_lexeme)
            {
                is_param = true;
                param_offset = node->m_offset;
                if (node->m_mode == Param_Mode::REF)
                {
                    is_ref = true;
                }

                break;
            }

            node = node->next_node;
        }
        // ParameterInfo * node = tr->item.procedure.param_info.m_offset;

    }
    else
    {
        is_param = false;
    }
    
    
    std::string ret = "";

    if (t.m_token == Token_T::TEMP)
    {
        if (global_depth == 2)
        {
            ret = "_t" + t.m_lexeme;
        }
        else
        {
            int offset = curr_scope_offset - (2 * (std::stoi(t.m_lexeme) - 1));
            ret = "_bp" + std::to_string(offset);
        }
    }
    else if (t.m_token == Token_T::IDENTIFIER)
    {
        if (global_depth == 2)
        {
            ret = t.m_lexeme;
        }
        else
        {
            // If it is a normal variabble
            if (!is_param)
            {
                if (st.LookupAtCurrentDepth(t.m_lexeme) == nullptr)
                {
                    tr = st.Lookup(t.m_lexeme);
                    if (tr != nullptr)
                    {
                        return t.m_lexeme;
                    }
                }
                else
                {
                    tr = st.LookupAtCurrentDepth(t.m_lexeme);
                }

                int offset = tr->item.variable.m_offset;

                ret += "_bp" + std::to_string(offset); 
            }
            // If it is a parameter
            else
            {
                if (is_ref)
                {
                    ret += "@";
                }

                ret += "_bp+" + std::to_string(param_offset);
            }
        }
    }
    else if (t.m_token == Token_T::NUMBER)
    {
        if (t.m_is_int)
        {
            ret = std::to_string(t.m_value);
        }
        else
        {
            ret = std::to_string(t.m_valuer);
        }
    }
    else if (t.m_token == Token_T::ASSOP)
    {
        ret = "=";
    }
    else
    {
        // Operations
        ret = t.m_lexeme;
    }

    return ret;
}

void TacWriter::writeFile()
{

}

void TacWriter::addParams(TableRecord * tr, int depth)
{

    // Start at +4
    int current_offset = 4;

    int num_params = tr->item.procedure.num_params;
    int curr_param = 1;

    ParameterInfo * node = tr->item.procedure.param_info;
    while (node != nullptr)
    {
        // Look up in the symbol table
        Token param_token;
        param_token = node->m_token;

        int param_size = typeToSize(node->m_type, param_token);
        node->m_offset =  (2 * (num_params - curr_param)) + current_offset;

        // Set the offset correctly
        // current_offset = current_offset + param_size;
        int offset = node->m_offset;

        std::pair<Token, int> ele(param_token, current_offset);
        offset_map[depth - 1].second.push_back(ele);

        node = node->next_node;
        curr_param++;
    }

    return;
}

void TacWriter::addLocalVars(std::vector<std::string> vars, int depth)
{
    // Start at -2
    int current_offset = -2;

    bool first = true;

    for (const std::string var : vars)
    {
        // Look up in the symbol table
        TableRecord * r = st.LookupAtCurrentDepth(var);
        int var_size = r->item.variable.m_size;
        Token var_token = r->m_token;

        // Set the offset correctly
        // if (first)
        // {
        //     first = false;
        // }
        // else
        // {
        //     current_offset = current_offset - var_size;
        // }
        r->item.variable.m_offset = current_offset;

        current_offset = current_offset - var_size;

        std::pair<Token, int> ele(var_token, current_offset);
        offset_map[depth - 1].second.push_back(ele);
    }

    curr_scope_offset = current_offset;

    return;
}

void TacWriter::addTemp()
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
    std::vector<Token> stats = reduceMultiOp();

    for(int i = 0 ; i < stats.size(); i++)
    {
        if (stats[i].m_token == Token_T::TEMP)
        {
            temp_map.writeTemp(std::stoi(stats[i].m_lexeme), false);
        }
    }

    for(int i = 0 ; i < stats.size(); i++)
    {
        if (stats[i].m_token == Token_T::TEMP)
        {
            if (global_depth == 2)
            {
                tac_file << "_t" << stats[i].m_lexeme;
            }
            else
            {
                int test = curr_scope_offset - ((std::stoi(stats[i].m_lexeme) - 1) * 2);
                tac_file << "_bp" << std::to_string(curr_scope_offset - ((std::stoi(stats[i].m_lexeme) - 1) * 2));
            }
        }
        else
        {
            // DisplayToken(stats[i]);
            tac_file << TacWriter::printVar(stats[i]);
        }

        tac_file << " ";
    }

    tac_file << std::endl << std::endl;
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

    if (initial_state.size() == 1)
    {
        std::stack<Token> d;
        d.push(initial_state.top());
        initial_state.pop();

        Token temp_token;

        int temp_key = temp_map.insertTemp(d);
        temp_token.m_lexeme = std::to_string(temp_key);
        temp_token.m_token = Token_T::TEMP;

        ret.push_back(temp_token);

        while (!token_stack.empty())
        {
            token_stack.pop();
        }

        return ret;
    }

    while(!token_stack.empty())
    {
        // if (token_stack.size() == 0)
        // {
        //     std::stack<Token> temp_s;
        //     temp_s.push(second_identifier);
        //     temp_s.push(operation);
        //     temp_s.push(first_identifier);
            
        //     int temp_key = temp_map.insertTemp(temp_s);
        //     last_temp.m_lexeme = std::to_string(temp_key);
        //     last_temp.m_token = Token_T::TEMP;

        //     ret.push_back(last_temp);

        //     while(!token_stack.empty())
        //     {
        //         token_stack.pop();
        //     }

        //     return ret;
        // }


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

                    // ret.push_back(last_temp);

                    first_identifier.m_token = Token_T::UNKNOWN;
                    second_identifier.m_token = Token_T::UNKNOWN;
                    operation.m_token = Token_T::UNKNOWN;
                }
            }
            else
            {
                std::stack<Token> s;

                Token temp_token;
                temp_token.m_token = Token_T::TEMP;

                s.push(t);
                s.push(operation);
                s.push(last_temp);

                temp_token.m_lexeme = std::to_string(temp_map.insertTemp(s));

                // if (insert_next == true)
                // {
                //     insert_next = false;
                //     ret.push_back(last_temp);
                // }

                // ret.push_back(operation);
                // ret.push_back(t);

                ret.push_back(temp_token);
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

                    if (token_stack.empty())
                    {
                        Token tk;
                        std::stack<Token> tk_s;
                        tk.m_token = Token_T::TEMP;
                        
                        tk_s.push(second_identifier);
                        tk_s.push(operation);
                        tk_s.push(first_identifier);

                        tk.m_lexeme = std::to_string(temp_map.insertTemp(tk_s));                    

                        ret.push_back(tk);

                        return ret;
                    }

                    if (token_stack.size() == 2)
                    {
                        insert_next = true;
                    }
                }
            }
        }
    }

    if (initial_state.size() < 3)
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

    if (initial_state.size() == 1)
    {
        std::stack<Token> d;
        d.push(initial_state.top());
        initial_state.pop();

        Token temp_token;

        int temp_key = temp_map.insertTemp(d);
        temp_token.m_lexeme = std::to_string(temp_key);
        temp_token.m_token = Token_T::TEMP;

        ret.push_back(temp_token);

        while (!p_stack.empty())
        {
            p_stack.pop();
        }

        return ret;
    }

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
                ret.push_back(operation);
                ret.push_back(t);  
                
                std::reverse(ret.begin(), ret.end()); 

                if (insert_next == true)
                {
                    insert_next = false;
                    ret.push_back(last_temp);
                }

                
                
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
