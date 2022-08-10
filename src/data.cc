#include "../include/data.h"
#include "../include/node.h"

/**
 * @brief Construct a new Data object.
 */
Data::Data()
    : type(DataType::NUMBER), value(0) {
}

Data::Data(DataType type, const auto &value)
    : type(type), value(value) {
}

/**
 * @brief Take a node and return its Data equivalent.
 *
 * @param node
 * @return Data
 */
auto convert_to_data(const Node &node)  -> Data {
    switch (node.type) {
        case NodeType::NUM_CONSTANT:
            return Data(DataType::NUMBER, std::get<int>(node.exp));

        case NodeType::STR_CONSTANT:
            return Data(DataType::STRING, std::get<std::string>(node.exp));

        case NodeType::SYM_CONSTANT:
            return Data(DataType::SYMBOL, std::get<std::string>(node.exp));

        default:
            return Data(); // NUM_CONSTANT, 0
    }
}
