#ifndef LISP_DATA_H
#define LISP_DATA_H

#include "node.h"

#include <cstdint>
#include <variant>
#include <string>

/**
 * @brief Enum representing the three types of values
 *        that can be returned by a function.
 */
enum struct DataType : std::uint8_t {
    NUMBER,
    STRING,
    SYMBOL,
};

/**
 * @brief Struct representing the physical wrapper around
 *        a value that can be returned by a function.
 */
struct Data final {
    DataType type;
    std::variant<int, std::string> value;

    /**
     * @brief Construct a new Data object.
     */
    Data();
    Data(DataType type, const auto &value);
};

/**
 * @brief Take a node and return its Data equivalent.
 *
 * @param node
 * @return Data
 */
extern auto convert_to_data(const Node &node) -> Data;
#endif // LISP_DATA_H
