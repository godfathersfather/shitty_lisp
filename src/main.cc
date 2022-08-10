#include "../include/token.h"
#include "../include/text.h"
#include "../include/node.h"
#include "../include/data.h"
#include "../include/error.h"
#include "../include/builtin.h"

#include <iostream>
#include <fstream>
#include <deque>
#include <list>
#include <array>
#include <string>
#include <variant>
#include <functional>
#include <unordered_map>
#include <exception>

static auto run_repl() -> void;
static auto run_file(int argc, char *argv[]) -> void;
static auto expect(const Token &tok, TokenType type)  -> void;
static auto read_source(int argc, char *argv[]) -> std::string;
static auto parse_token(Text &text) -> Token;
static auto parse_ast(Text &text, bool check_lparen = true) -> Node;
static auto call_func(std::deque<Data> &args) -> Node;
static auto eval_node(const Node &node) -> Node;

std::unordered_map<std::string, Data> variables;

/**
 * @brief Global map containing all the built-in functions of the language.
 */
const std::unordered_map<std::string,
                         std::function<Node(std::deque<Data>&)>
                         > built_in_functions {
    {
        {"println", builtin_println},
        {"print", builtin_print},
        {"eprintln", builtin_eprintln},
        {"eprint", builtin_eprint},
        {"concat", builtin_concat},
        {"to_string", builtin_to_string},
        {"to_number", builtin_to_number},
        {"add", builtin_add},
        {"sub", builtin_sub},
        {"mul", builtin_mul},
        {"div", builtin_div},
    }
};

int main(int argc, char *argv[]) {
    if (argc > 1) {
        run_file(argc, argv);
    }
    else {
        run_repl();
    }

    return EXIT_SUCCESS;
}

/**
 * @brief Run a repl and execute commands like a shell.
 */
static auto run_repl() -> void {
    Text text;
    std::string input;
    Node ast;
    Data result;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        text = Text(input);

        try {
            ast = parse_ast(text);
            result = convert_to_data(eval_node(ast));
        }
        catch (const Error &err) {
            std::cerr << "ERROR: " << err.what() << '\n';
            continue;
        }

        std::cout << "\n==> ";
        std::visit([](const auto &v){ std::cout << v << '\n'; }, result.value);
    }
}

/**
 * @brief Execute the code inside of a given file.
 */
static auto run_file(int argc, char *argv[]) -> void {
    Text text(read_source(argc, argv));
    bool first_run = false;

    for (;;) {
        try {
            eval_node(parse_ast(text));
        }
        catch (const Error &err) {
            const auto desc = err.what();

            // this is to fix a bug which I am not interested enough
            // to do so myself because I wish to start over.
            if (std::string(desc) == "Expected ( got nothing") {
                first_run = false;
                continue;
            }
            else if (std::string(desc).find("nothing") != std::string::npos && !first_run) {
                return;
            }
            else {
                std::cerr << "ERROR: " << err.what() << '\n';
                std::exit(EXIT_FAILURE);
            }
        }

        if (first_run) {
            first_run = false;
        }
    }
}

/**
 * @brief Compare the type of "tok" with "type"
 *        and error if they're different. Used
 *        for comparing tokens and creating the AST.
 *
 * @param tok
 * @param type
 */
static auto expect(const Token &tok, TokenType type) -> void {
    static const char *const repr[] {
        "symbol",
        "string constant",
        "number constant",
        "(",
        ")",
        "nothing",
    };

    const auto i_type = static_cast<int>(type);
    const auto i_toktype = static_cast<int>(tok.type);

    if (tok.type != type) {
        quit("Expected ", repr[i_type], " but got ", repr[i_toktype]);
    }
}

/**
 * @brief Take the command line arguments from main
 *        and return the contents of the file of argv[1]
 *        which would be the file containing the script.
 *
 * @param argc
 * @param argv
 * @return std::string
 */
static auto read_source(int argc, char *argv[]) -> std::string {
    if (argc != 2) {
        std::cerr << "Usage: ./lisp <script.lisp>\n";
        std::exit(EXIT_SUCCESS);
    }
    std::ifstream fp(argv[1]);
    return std::string((std::istreambuf_iterator<char>(fp)),
                       (std::istreambuf_iterator<char>()));
}

/**
 * @brief Take a Text object and return the first token found within.
 *
 * @param text
 * @return Token
 */
static auto parse_token(Text &text) -> Token {
    while (text.position < text.size) {
        switch (text.curr()) {
            case  ' ':
            case '\n':
            case '\t':
                ++text.position;
                break;

            case '#':
                while (text.curr() != '\n') {
                    ++text.position;
                }
                break;

            case '(':
                ++text.position;
                return Token(TokenType::LPAREN, 0);

            case ')':
                ++text.position;
                return Token(TokenType::RPAREN, 0);

            case '"': {
                ++text.position;

                auto new_index = text.find([](char c){ return c != '"'; });
                auto string = text.substr(new_index);
                text.position = new_index + 1;

                return Token(TokenType::STRING, string);
            }

            default:
                if (std::isdigit(text.curr()) || text.curr() == '-') {
                    bool is_minus = false;

                    if (text.curr() == '-') {
                        is_minus = true;
                        text.position += 1;
                    }

                    auto new_index = text.find([](char c){ return std::isdigit(c); });
                    auto number = std::stoi(text.substr(new_index));
                    text.position = new_index;

                    return Token(TokenType::NUMBER, is_minus ? -number : number);
                }
                else if (std::isalpha(text.curr()) || text.curr() == '_') {
                    auto new_index = text.find(
                        [](char c){ return std::isalnum(c) || c == '_'; });

                    auto symbol = text.substr(new_index);
                    text.position = new_index;

                    return Token(TokenType::SYMBOL, symbol);
                }
                else {
                    quit("Failed To Get Next Token!\n");
                }
        }
    }

    // never runs, used just to silence compiler warnings
    return Token(); // THE_END, 0
}

/**
 * @brief Convert a series of tokens into an abstract syntax tree.
 *
 * @param text
 * @return Node
 */
static auto parse_ast(Text &text, bool check_lparen) -> Node {
    std::list<Node> elements;
    Token curr_tok;

    if (check_lparen) {
        expect(parse_token(text), TokenType::LPAREN);
    }

    curr_tok = parse_token(text);
    while (curr_tok.type != TokenType::RPAREN) {
        switch (curr_tok.type) {
            case TokenType::NUMBER:
                elements.emplace_back(Node(NodeType::NUM_CONSTANT,
                                           std::get<int>(curr_tok.value)));
                break;

            case TokenType::STRING:
                elements.emplace_back(Node(NodeType::STR_CONSTANT,
                                           std::get<std::string>(curr_tok.value)));
                break;

            case TokenType::SYMBOL:
                elements.emplace_back(Node(NodeType::SYM_CONSTANT,
                                           std::get<std::string>(curr_tok.value)));
                break;

            case TokenType::LPAREN:
                elements.emplace_back(parse_ast(text, false));
                break;

            default:
                quit("Unterminated list!");
        }

        curr_tok = parse_token(text);
    }

    expect(curr_tok, TokenType::RPAREN);
    return Node(NodeType::LIST_CONSTANT, elements);
}

/**
 * @brief Take function/list as a deque of Data and call it using the global "functions" map.
 *
 * @param args
 * @return Node
 */
static auto call_func(std::deque<Data> &args) -> Node {
    Node return_value;
    const auto func_name = std::get<std::string>(args[0].value);

    // remove function name from args
    args.pop_front();

    if (built_in_functions.contains(func_name)) {
        return_value = built_in_functions.at(func_name)(args);
    }
    else {
        quit("Tried to call an unknown function and failed!");
    }

    return return_value; // return 0 by default
}

/**
 * @brief Evaluate a node and slowly collapse an abstract syntax tree into a single node.
 *
 * @param node
 * @return Node
 */
static auto eval_node(const Node &node) -> Node {
    switch (node.type) {
        case NodeType::LIST_CONSTANT: {
            std::deque<Data> args;
            const auto body = std::get<std::list<Node>>(node.exp);

            for (const auto &param : body) {
                args.emplace_back(convert_to_data(eval_node(param)));
            }

            return call_func(args);
        }

        default:
            return node;
    }
}
