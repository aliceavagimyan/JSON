#include "jsonParser.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

int main() {
    try {
    
        std::ifstream file("example.json");
        if (!file) {
            throw std::runtime_error("Failed to open file: example.json");
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string jsonStr = buffer.str();

        auto root = JsonParser::parse(jsonStr);

        std::cout << "Parsing completed successfully!" << std::endl;

        
        auto object = dynamic_cast<JsonObject*>(root.get());
        if (object) {
            for (const auto& pair : object->getPairs()) {
                std::cout << "Key: " << pair.first << ", Value: " << pair.second->getValue() << std::endl;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
