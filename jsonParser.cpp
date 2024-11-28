#include "jsonParser.hpp"
#include <cctype>
#include <sstream>
#include <iostream>

void JsonParser::skipWhitespace(const std::string& str, size_t& index) {
    while (index < str.size() && std::isspace(str[index])) {
        ++index;
    }
}

std::unique_ptr<JsonValue> JsonParser::parseString(const std::string& str, size_t& index) {
    if (str[index] != '"') {
        throw std::runtime_error("Waiting for string");
    }
    ++index;  

    std::string result;
    while (index < str.size() && str[index] != '"') {
        result.push_back(str[index++]);
    }

    if (index < str.size() && str[index] == '"') {
        ++index;  
    } else {
        throw std::runtime_error("non close string");
    }
    return std::make_unique<JsonString>(result);
}

std::unique_ptr<JsonValue> JsonParser::parseNumber(const std::string& str, size_t& index) {
    size_t start = index;
    while (index < str.size() && (std::isdigit(str[index]) || str[index] == '.' || str[index] == '-' || str[index] == '+')) {
        ++index;
    }

    std::string numStr = str.substr(start, index - start);
    double value = std::stod(numStr);
    return std::make_unique<JsonNumber>(value);
}

std::unique_ptr<JsonValue> JsonParser::parseBool(const std::string& str, size_t& index) {
    if (str.substr(index, 4) == "true") {
        index += 4;
        return std::make_unique<JsonBool>(true);
    } else if (str.substr(index, 5) == "false") {
        index += 5;
        return std::make_unique<JsonBool>(false);
    }
    throw std::runtime_error("waiting for 'true' or 'false'");
}

std::unique_ptr<JsonValue> JsonParser::parseNull(const std::string& str, size_t& index) {
    if (str.substr(index, 4) == "null") {
        index += 4;
        return std::make_unique<JsonNull>();
    }
    throw std::runtime_error("waiting for 'null'");
}

std::unique_ptr<JsonValue> JsonParser::parseArray(const std::string& str, size_t& index) {
    ++index; 
    auto arr = std::make_unique<JsonArray>();
    skipWhitespace(str, index);
    if (str[index] == ']') {
        ++index; 
        return arr;
    }

    while (true) {
        arr->addElement(parseValue(str, index));
        skipWhitespace(str, index);
        if (str[index] == ']') {
            ++index;
            break;
        }
        if (str[index] == ',') {
            ++index;  
            skipWhitespace(str, index);
        } else {
            throw std::runtime_error("waiting for ',' or ']' ");
        }
    }
    return arr;
}

std::unique_ptr<JsonValue> JsonParser::parseObject(const std::string& str, size_t& index) {
    ++index;  
    auto obj = std::make_unique<JsonObject>();
    skipWhitespace(str, index);
    if (str[index] == '}') {
        ++index; 
        return obj;
    }

    while (true) {
        skipWhitespace(str, index);
        
       
        std::string key = parseString(str, index)->getValue();  
        
        skipWhitespace(str, index);
        if (str[index] != ':') {
            throw std::runtime_error("waiting for ':'");
        }
        ++index;  
        skipWhitespace(str, index);
        obj->addPair(key, parseValue(str, index));  
        skipWhitespace(str, index);
        
        if (str[index] == '}') {
            ++index;
            break;
        }
        
        if (str[index] == ',') {
            ++index; 
            skipWhitespace(str, index);
        } else {
            throw std::runtime_error("waiting for ',' or '}' ");
        }
    }
    return obj;
}

std::unique_ptr<JsonValue> JsonParser::parseValue(const std::string& str, size_t& index) {
    skipWhitespace(str, index);
    if (index >= str.size()) {
        throw std::runtime_error("non full JSON");
    }
    char currentChar = str[index];
    if (currentChar == '"') {
        return parseString(str, index);
    } else if (std::isdigit(currentChar) || currentChar == '-') {
        return parseNumber(str, index);
    } else if (currentChar == 't' || currentChar == 'f') {
        return parseBool(str, index);
    } else if (currentChar == 'n') {
        return parseNull(str, index);
    } else if (currentChar == '[') {
        return parseArray(str, index);
    } else if (currentChar == '{') {
        return parseObject(str, index);
    }
    throw std::runtime_error("error symbol in JSON");
}

std::unique_ptr<JsonValue> JsonParser::parse(const std::string& str) {
    size_t index = 0;
    return parseValue(str, index);
}
