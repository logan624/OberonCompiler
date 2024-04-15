#pragma once

#include <vector>
#include <stack>

#include "TAC.h"
#include "LexicalAnalyzer.h"

class TempMap
{
    private:
        int highest_index;
        std::vector<std::pair<int, std::stack<Token>>> temp_map;

    public:
        TempMap()
        {
            highest_index = 0;
        }

        ~TempMap()
        {

        }

        int insertTemp(std::stack<Token> stack)
        {
            highest_index++;
            int key = highest_index;

            temp_map.push_back(std::pair<int, std::stack<Token>>(key, stack));

            return key;
        }

        std::stack<Token> * getTemp(int index)
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

        void writeTemp(int index, bool recursive)
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

            std::cout << "_t" << index << " := ";

            for (int i = 0; i < vec.size(); i++)
            {
                if (vec[i].m_token == Token_T::TEMP)
                {
                    // Key
                    int key = std::stoi(vec[i].m_lexeme);
                    std::cout << "_t" << std::to_string(key);
                }
                else
                {
                    // Print the lexeme of the top token.
                    DisplayToken(vec[i]);  
                }

                std::cout << " ";
            }

            // while (tokenStack != nullptr && !tokenStack->empty())
            // {
            //     // Access the top token from the stack.
            //     Token &topToken = tokenStack->top();
            //     tokenStack->pop();

            //     // Check if the token type is TEMP and perform the desired action.
            //     if (topToken.m_token == Token_T::TEMP)
            //     {
            //         // Key
            //         int key = std::stoi(topToken.m_lexeme);
            //         writeTemp(key, true);
            //         std::cout << "_t" << std::to_string(key) << " " << ":=" << " ";
            //     }
            //     else
            //     {
            //         // Print the lexeme of the top token.
            //         std::cout << topToken.m_lexeme;  
            //     }

            //     std::cout << " ";
            // }

            std::cout << std::endl;
        }
};
