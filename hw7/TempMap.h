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
};
