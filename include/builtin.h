#ifndef LISP_BUILTIN_H
#define LISP_BUILTIN_H

#include "data.h"
#include "node.h"

#include <deque>

extern auto builtin_println(std::deque<Data> &args) -> Node;
extern auto builtin_print(std::deque<Data> &args) -> Node;
extern auto builtin_eprintln(std::deque<Data> &args) -> Node;
extern auto builtin_eprint(std::deque<Data> &args) -> Node;
extern auto builtin_concat(std::deque<Data> &args) -> Node;
extern auto builtin_to_string(std::deque<Data> &args) -> Node;
extern auto builtin_to_number(std::deque<Data> &args) -> Node;
extern auto builtin_add(std::deque<Data> &args) -> Node;
extern auto builtin_sub(std::deque<Data> &args) -> Node;
extern auto builtin_mul(std::deque<Data> &args) -> Node;
extern auto builtin_div(std::deque<Data> &args) -> Node;

#endif // LISP_BUILTIN_H
