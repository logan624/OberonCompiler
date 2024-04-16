#include "TempMap.h"

#include <string>
#include <vector>
#include <stack>

#include "TAC.h"
#include "LexicalAnalyzer.h"
#include "Parser.h"

extern int curr_scope_offset;

TempMap::TempMap()
{
    highest_index = 0;
}

int TempMap::insertTemp(std::stack<Token> stack)
{
    highest_index++;
    int key = highest_index;

    temp_map.push_back(std::pair<int, std::stack<Token>>(key, stack));

    return key;
}

std::stack<Token> * TempMap::getTemp(int index)
{
    std::stack<Token> * ret = nullptr;

    for (auto & temp : temp_map)
    {
        if (temp.first == index)
        {
            ret = &temp.second;
        }
    }

    return ret;
}

void TempMap::writeTemp(int index, bool recursive)
{
    // Find the stack associated with the given index.
    std::stack<Token> *tokenStack = getTemp(index);

    // Convert the token stack to a vector
    std::vector<Token> vec;
    while (tokenStack->empty() == false)
    {
        if (tokenStack->top().m_token == Token_T::TEMP)
        {
            writeTemp(std::stoi(tokenStack->top().m_lexeme), true);
        }

        vec.push_back(tokenStack->top());
        tokenStack->pop();
    }

    // std::cout << "_t" << index << " := ";
    if (global_depth == 2)
    {
        std::cout << "_t" << index << " := ";
    }
    else
    {
        std::cout << "_bp" << std::to_string(curr_scope_offset - (index * 2)) << " := ";
    }

    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i].m_token == Token_T::TEMP)
        {
            // Key
            int key = std::stoi(vec[i].m_lexeme);
            // std::cout << "_t" << std::to_string(key);

            if (global_depth == 2)
            {
                std::cout << "_t" << std::to_string(key) << " := ";
            }
            else
            {
                std::cout << "_bp" << std::to_string(curr_scope_offset - (key * 2));
            }
        }
        else
        {
            // Print the lexeme of the top token.
            // DisplayToken(vec[i]);  
            std::cout << TacWriter::printVar(vec[i]);  
        }

        std::cout << " ";
    }

    std::cout << std::endl;
}
