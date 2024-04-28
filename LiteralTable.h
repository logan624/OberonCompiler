#pragma once

#include <map>
#include <string>
#include <vector>

class LiteralTable {
public:
    std::string insertLiteral(const std::string& literal);

    std::string getLiteral(const std::string& key);

    void addLiterals(std::vector<std::string> & vec);

private:
    std::map<std::string, std::string> table;
    int nextIndex = 0;
};
