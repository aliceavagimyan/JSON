#ifndef JSONPARSER
#define JSONPARSER

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <map>

class JsonValue {
public:
    virtual ~JsonValue() = default;
    virtual std::string getValue() const = 0;  
};

class JsonString : public JsonValue {
public:
    explicit JsonString(std::string value) : value(std::move(value)) {}
    
    std::string getValue() const override {
        return value;  
    }

private:
    std::string value;
};

class JsonNumber : public JsonValue {
public:
    explicit JsonNumber(double value) : value(value) {}
    
    std::string getValue() const override {
        return std::to_string(value); 
    }

private:
    double value;
};

class JsonBool : public JsonValue {
public:
    explicit JsonBool(bool value) : value(value) {}
    
    std::string getValue() const override {
        return value ? "true" : "false";  
    }

private:
    bool value;
};

class JsonNull : public JsonValue {
public:
    std::string getValue() const override { 
        return "null"; 
    }
};

class JsonArray : public JsonValue {
public:
    void addElement(std::unique_ptr<JsonValue> element) {
        elements.push_back(std::move(element));
    }

    const std::vector<std::unique_ptr<JsonValue>>& getElements() const {
        return elements;
    }

    std::string getValue() const override {
        std::string result = "[";
        for (size_t i = 0; i < elements.size(); ++i) {
            result += elements[i]->getValue();
            if (i < elements.size() - 1) {
                result += ", ";
            }
        }
        result += "]";
        return result;
    }

private:
    std::vector<std::unique_ptr<JsonValue>> elements;
};

class JsonObject : public JsonValue {
public:
    void addPair(const std::string& key, std::unique_ptr<JsonValue> value) {
        pairs.push_back({key, std::move(value)});  
    }

    const std::vector<std::pair<std::string, std::unique_ptr<JsonValue>>>& getPairs() const {
        return pairs;
    }

    std::string getValue() const override {
        std::string result = "{";
        for (auto it = pairs.begin(); it != pairs.end(); ++it) {
            result += "\"" + it->first + "\": " + it->second->getValue();
            if (std::next(it) != pairs.end()) {
                result += ", ";
            }
        }
        result += "}";
        return result;
    }

private:
    std::vector<std::pair<std::string, std::unique_ptr<JsonValue>>> pairs; 
};

class JsonParser {
public:
    static std::unique_ptr<JsonValue> parse(const std::string& str);

private:
    static void skipWhitespace(const std::string& str, size_t& index);
    static std::unique_ptr<JsonValue> parseString(const std::string& str, size_t& index);
    static std::unique_ptr<JsonValue> parseNumber(const std::string& str, size_t& index);
    static std::unique_ptr<JsonValue> parseBool(const std::string& str, size_t& index);
    static std::unique_ptr<JsonValue> parseNull(const std::string& str, size_t& index);
    static std::unique_ptr<JsonValue> parseArray(const std::string& str, size_t& index);
    static std::unique_ptr<JsonValue> parseObject(const std::string& str, size_t& index);
    static std::unique_ptr<JsonValue> parseValue(const std::string& str, size_t& index);
};


#endif // JSONPARSER