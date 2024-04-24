#pragma once

#include <map>
#include <string>

class LiteralTable {
public:
    std::string insertLiteral(const std::string& literal);

    std::string getLiteral(const std::string& key) const;

private:
    std::map<std::string, std::string> table;
    int nextIndex = 0;
};
