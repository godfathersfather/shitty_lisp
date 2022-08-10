#ifndef LISP_TOKEN_H
#define LISP_TOKEN_H

#include <cstdint>
#include <variant>
#include <string>

/**
 * @brief Enum used to represent the types of tokens within the language.
 */
enum struct TokenType : std::uint8_t {
    SYMBOL,
    STRING,
    NUMBER,
    LPAREN,
    RPAREN,
    THE_END,
};

/**
 * @brief Struct used to represent the constructs that make up the language.
 */
struct Token final {
    TokenType type;
    std::variant<int, std::string> value;

    /**
     * @brief Construct a new Token object.
     */
    Token();
    inline Token(TokenType type, const auto &value)
        : type(type), value(value) {
    }

    /**
     * @brief Return a string representation of the token.
     *
     * @return std::string
     */
    auto repr() const  -> std::string;
};

#endif // LISP_TOKEN_H
