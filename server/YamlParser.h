#include <string>
#include <unordered_map>

#include "./VehicleSpec.h"

class YamlParser {
public:
    YamlParser() = default;
    ~YamlParser() = default;

    // Add public methods for parsing YAML files here
    std::unordered_map<std::string, VehicleSpec> parse(const std::string& filename);
};