#pragma once

#include <string>
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
        TempMap();

        int insertTemp(std::stack<Token> stack);

        std::stack<Token> * getTemp(int index);

        void writeTemp(int index, bool recursive);
};
