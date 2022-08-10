#ifndef LISP_NODE_H
#define LISP_NODE_H

#include <cstdint>
#include <string>
#include <variant>
#include <list>

/**
 * @brief Enum representing all types of node-transformed tokens.
 */
enum struct NodeType : std::uint8_t {
    NUM_CONSTANT,
    STR_CONSTANT,
    SYM_CONSTANT,
    LIST_CONSTANT,
};

/**
 * @brief Struct representing the next form of tokens in the internal
 *        representation of the language when it gets evaluated.
 */
struct Node final {
    NodeType type;
    std::variant<int, std::string, std::list<Node>> exp;

    /**
     * @brief Construct a new Node object.
     *
     */
    Node();
    inline Node(NodeType type, const auto &exp)
        : type(type), exp(exp) {
    }
};

#endif // LISP_NODE_H
