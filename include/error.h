#ifndef LISP_ERROR_H
#define LISP_ERROR_H

#include <string>
#include <exception>
#include <array>

/**
 * @brief Struct representing an error with a descriptive message.
 *        Used for error handling when parsing and evaluating code.
 */
struct Error final : public std::exception {
    /**
     * @brief Construct a new Error object.
     *
     * @param desc
     */
    inline Error(const auto& ...desc) {
        std::array<std::string, sizeof...(desc)> unpacked = {desc...};
        for (const auto &v : unpacked) {
            this->desc += v;
        }
    }

    /**
     * @brief Return "desc" when the exception is caught and .what is ran.
     *
     * @return const char*
     */
    auto what() const noexcept(true) -> const char* override;

private:
    std::string desc;
};

/**
 * @brief Throw Error with desc as its description.
 *
 * @param desc
 */
inline auto quit(const auto& ...desc) -> void {
    throw Error(desc...);
}

#endif // LISP_ERROR_H
