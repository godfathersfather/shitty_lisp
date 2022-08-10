#include "../include/text.h"

/**
 * @brief Construct a new Text object.
 *
 * @param contents
 */
Text::Text(const std::string &contents)
    : contents(contents), position(0), size(contents.size()) {
}

/**
 * @brief Operator overloading just to type less when accessing an index.
 *        Throws std::out_of_range exception if the index is out of bounds.
 *
 * @param index
 * @return char&
 */
auto Text::operator[](const std::size_t index) -> char& {
    if (index < this->size) {
        return contents[index];
    }
    std::abort();
}

/**
 * @brief Return the current character in the string.
 *        Immutable return value.
 *
 * @return char
 */
auto Text::curr() const -> char {
    return this->contents[this->position];
}

/**
 * @brief Begin from "this->position" and stop
 *        once the lambda "check" returns false.
 *        Great for discovering where specific
 *        portions of text end.
 *
 * @param check
 * @return std::size_t
 */
auto Text::find(std::function<bool(char)> check)  -> std::size_t {
    std::size_t i;
    for (i = this->position; check(this->contents[i]); ++i) {
        if (i >= this->size) {
            return std::string::npos;
        }
    }
    return i;
}

/**
 * @brief Return a substring from "this->position" up until "end".
 *
 * @param end
 * @return std::string
 */
auto Text::substr(std::size_t end) const  -> std::string {
    std::string sub;
    for (std::size_t i = this->position; i < end; ++i) {
        sub += this->contents[i];
    }
    return sub;
}
