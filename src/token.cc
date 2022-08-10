#include "../include/token.h"

 /**
 * @brief Construct a new Token object.
 */
Token::Token()
    : type(TokenType::THE_END), value(0) {
}

/**
 * @brief Return a string representation of the token.
 *
 * @return std::string
 */
auto Token::repr() const  -> std::string {
    std::string repr = std::to_string(static_cast<int>(this->type)) +  ", Value: ";

    switch (this->type) {
        case TokenType::SYMBOL:
        case TokenType::STRING:
            repr += std::get<std::string>(this->value);
            break;

        case TokenType::NUMBER:
            repr += std::to_string(std::get<int>(this->value));
            break;

        case TokenType::LPAREN:
            repr += '(';
            break;

        case TokenType::RPAREN:
            repr += ')';
            break;

        default:
            break;
    }
    return repr;
}
