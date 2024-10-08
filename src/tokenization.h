#pragma once

#include <string>
#include <utility>
#include <vector>

enum class TokenType {
    exit,
    int_lit,
    semi,
    open_parenthesis,
    closed_parenthesis,
    identifier,
    let,
    equals
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer {
public:
    explicit Tokenizer(std::string src)
        : m_src(std::move(src))
    {
    }

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        std::string buffer;
        while(peek().has_value()){
            if(std::isalpha(peek().value())){
                buffer.push_back(consume());
                while (peek().has_value() && std::isalnum(peek().value())){
                    buffer.push_back(consume());
                }

                if (buffer == "exit") {
                    tokens.push_back({.type = TokenType::exit});
                    buffer.clear();
                    continue;
                }

                if (buffer == "let") {
                    tokens.push_back({.type = TokenType::let});
                    buffer.clear();
                    continue;
                }

                tokens.push_back({.type = TokenType::identifier, .value = buffer});
                buffer.clear();
                continue;
            }

            if(std::isdigit(peek().value())){
                buffer.push_back(consume());

                while (peek().has_value() && std::isdigit(peek().value())){
                    buffer.push_back(consume());
                }

                tokens.push_back({ .type = TokenType::int_lit, .value = buffer});
                buffer.clear();
                continue;
            }
            if(peek().value() == '('){
                consume();
                tokens.push_back({ .type= TokenType::open_parenthesis});
                continue;
            }
            if(peek().value() == ')'){
                consume();
                tokens.push_back({ .type= TokenType::closed_parenthesis});
                continue;
            }

            if(peek().value() == '='){
                consume();
                tokens.push_back({ .type= TokenType::equals});
                continue;
            }

            if(peek().value() == ';'){
                consume();
                tokens.push_back({ .type= TokenType::semi});
                continue;
            }

            if (std::isspace(peek().value())) {
                consume();
                continue;
            }

            throw std::runtime_error("Unexpected character: " + std::string(1, peek().value()));
        }

        m_current_index = 0;

        return tokens;
    }
private:
    [[nodiscard]] std::optional<char> peek (const int offset = 0) const {
        if (m_current_index + offset >= m_src.length()){
            return {};
        }

        return m_src.at(m_current_index + offset);
    }

    char consume() {
        return m_src.at(m_current_index++);
    }

    const std::string m_src;
    int m_current_index = 0;
};
