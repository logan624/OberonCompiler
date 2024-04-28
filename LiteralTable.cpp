#include "LiteralTable.h"

#include <vector>
#include <string>

std::string LiteralTable::insertLiteral(const std::string& literal) {
    std::string key = "_S" + std::to_string(nextIndex++);
    table[key] = literal;
    return key;
}

std::string LiteralTable::getLiteral(const std::string& key)
{
    auto it = table.find(key);
    if (it != table.end()) {
        return it->second;
    } else {
        return "";
    }
}

void LiteralTable::addLiterals(std::vector<std::string> & vec)
{
    for (const auto& entry : table)
    {
        vec.push_back(entry.first);
    }
}
