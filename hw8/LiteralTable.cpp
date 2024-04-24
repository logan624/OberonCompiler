#include "LiteralTable.h"

std::string LiteralTable::insertLiteral(const std::string& literal) {
    std::string key = "_S" + std::to_string(nextIndex++);
    table[key] = literal;
    return key;
}

std::string LiteralTable::getLiteral(const std::string& key) const {
    auto it = table.find(key);
    if (it != table.end()) {
        return it->second;
    } else {
        return "";
    }
}
