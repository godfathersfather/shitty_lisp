#include "../include/error.h"

/**
 * @brief Return "desc" when the exception is caught and .what is ran.
 *
 * @return const char*
 */
auto Error::what() const noexcept(true) -> const char* {
    return desc.c_str();
}
