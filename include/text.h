#ifndef LISP_TEXT_H
#define LISP_TEXT_H

#include <string>
#include <functional>

/**
 * @brief Struct used to represent an easily
 *        tokenizable piece of text.
 */
struct Text final {
    std::string contents;
    std::size_t position, size;

    /**
     * @brief Construct a new Text object.
     *
     * @param contents
     */
    Text(const std::string &contents = {});

    /**
     * @brief Operator overloading just to type less when accessing an index.
     *        Throws std::out_of_range exception if the index is out of bounds.
     *
     * @param index
     * @return char&
     */
    auto operator[](const std::size_t index) -> char&;

    /**
     * @brief Return the current character in the string.
     *        Immutable return value.
     *
     * @return char
     */
    auto curr() const -> char;

    /**
     * @brief Begin from "this->position" and stop
     *        once the lambda "check" returns false.
     *        Great for discovering where specific
     *        portions of text end.
     *
     * @param check
     * @return std::size_t
     */
    auto find(std::function<bool(char)> check)  -> std::size_t;

    /**
     * @brief Return a substring from "this->position" up until "end".
     *
     * @param end
     * @return std::string
     */
    auto substr(std::size_t end) const  -> std::string;
};

#endif // LISP_TEXT_H
