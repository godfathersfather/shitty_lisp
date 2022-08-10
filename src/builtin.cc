#include "../include/builtin.h"
#include "../include/error.h"

#include <iostream>

auto builtin_println(std::deque<Data> &args) -> Node {
    if (args.empty()) {
        quit("Invalid amount of arguments passed to (println x y ...)");
    }
    builtin_print(args);
    std::cout << '\n';
    return Node();
}

auto builtin_print(std::deque<Data> &args) -> Node {
    if (args.empty()) {
        quit("Invalid amount of arguments passed to (print x y ...)");
    }
    for (const auto &arg : args) {
        std::visit([](const auto &v){ std::cout << v; }, arg.value);
    }
    return Node();
}

auto builtin_eprintln(std::deque<Data> &args) -> Node {
    if (args.empty()) {
        quit("Invalid amount of arguments passed to (eprintln x y ...)");
    }
    builtin_eprint(args);
    std::cerr << '\n';
    return Node();
}

auto builtin_eprint(std::deque<Data> &args) -> Node {
    if (args.empty()) {
        quit("Invalid amount of arguments passed to (eprint x y ...)");
    }
    for (const auto &arg : args) {
        std::visit([](const auto &v){ std::cerr << v; }, arg.value);
    }
    return Node();
}

auto builtin_concat(std::deque<Data> &args) -> Node {
    if (args.size() < 2) {
        quit("Invalid amount of arguments passed to (concat x y ...)");
    }
    std::string whole;
    for (const auto &arg : args) {
        whole += std::get<std::string>(arg.value);
    }
    return Node(NodeType::STR_CONSTANT, whole);
}

auto builtin_to_string(std::deque<Data> &args) -> Node {
    if (args.size() != 1) {
        quit("Invalid amount of arguments passed to (to_number x)");
    }
    return Node(NodeType::STR_CONSTANT,
                std::to_string(std::get<int>(args[0].value)));
}

auto builtin_to_number(std::deque<Data> &args) -> Node {
    if (args.size() != 1) {
        quit("Invalid amount of arguments passed to (to_number x)");
    }
    return Node(NodeType::NUM_CONSTANT,
                std::stoi(std::get<std::string>(args[0].value)));
}

auto builtin_add(std::deque<Data> &args) -> Node {
    if (args.size() < 2) {
        quit("Invalid amount of arguments passed to (add x y ...)");
    }
    int total = 0;
    for (const auto &arg : args) {
        total += std::get<int>(arg.value);
    }
    return Node(NodeType::NUM_CONSTANT, total);
}

auto builtin_sub(std::deque<Data> &args) -> Node {
    if (args.size() < 2) {
        quit("Invalid amount of arguments passed to (sub x y ...)");
    }
    int total = std::get<int>(args[0].value);
    args.pop_front();

    for (const auto &arg : args) {
        total -= std::get<int>(arg.value);
    }
    return Node(NodeType::NUM_CONSTANT, total);
}

auto builtin_mul(std::deque<Data> &args) -> Node {
    if (args.size() < 2) {
        quit("Invalid amount of arguments passed to (mul x y ...)");
    }
    int total = std::get<int>(args[0].value);
    args.pop_front();

    for (const auto &arg : args) {
        total *= std::get<int>(arg.value);
    }
    return Node(NodeType::NUM_CONSTANT, total);
}

auto builtin_div(std::deque<Data> &args) -> Node {
    if (args.size() < 2) {
        quit("Invalid amount of arguments passed to (div x y ...)");
    }
    int total = std::get<int>(args[0].value);
    args.pop_front();

    for (const auto &arg : args) {
        total /= std::get<int>(arg.value);
    }
    return Node(NodeType::NUM_CONSTANT, total);
}
