#pragma once

#include <variant>
#include "tokenization.h"

namespace node{
    struct ExpressionIntLiteral {
        Token int_literal;
    };
    struct ExpressionIdentifier {
        Token identifier;
    };

    struct Expression {
        std::variant<ExpressionIntLiteral, ExpressionIdentifier> variant;
    };

    struct StatementExit{
        Expression expression;
    };

    struct StatementLet{
        Token identifier;
        Expression expression;
    };

    struct Statement {
        std::variant<StatementExit, StatementLet> variant;
    };

    struct Program {
        std::vector<Statement> statements;
    };
}
class Parser {
public:
    explicit Parser(std::vector<Token> tokens)
        : m_tokens(std::move(tokens))
    {
    }

    std::optional<node::Expression> parse_expression(){
        if (!peek().has_value()) return {};

        if (peek().value().type == TokenType::int_lit){
            return node::Expression{ .variant = node::ExpressionIntLiteral{ .int_literal = consume() } };
        }

        if (peek().value().type == TokenType::identifier){
            return node::Expression{ .variant = node::ExpressionIdentifier{ .identifier = consume() } };
        }

        return {};
    }

    std::optional<node::Statement> parse_statement(){
        if (!peek().has_value()) return {};

        if(peek().value().type == TokenType::exit && peek(1).has_value() && peek(1).value().type == TokenType::open_parenthesis){
            consume();
            consume();

            if(auto node_expression = parse_expression()){
                node::StatementExit statement_exit;
                statement_exit = { .expression = node_expression.value() };
                if (!peek().has_value() || peek().value().type != TokenType::closed_parenthesis){
                    throw std::runtime_error("Expected ')'");
                }
                consume();

                if (!peek().has_value() || peek().value().type != TokenType::semi){
                    throw std::runtime_error("Expected ';'");
                }
                consume();

                return node::Statement{ .variant = statement_exit };
            }

            throw std::runtime_error("Invalid expression");
        }

        if (peek().value().type == TokenType::let){
            if (!peek(1).has_value() || peek(1).value().type != TokenType::identifier) throw std::runtime_error("Expected identifier");
            if (!peek(2).has_value() || peek(2).value().type != TokenType::equals) throw std::runtime_error("Expected '='");
            consume();
            auto statement_let = node::StatementLet {.identifier = consume()};
            consume();
            if (auto expression = parse_expression()){
                statement_let.expression = expression.value();
                if(!peek().has_value() || peek().value().type != TokenType::semi) throw std::runtime_error("Expected ';'");
                consume();

                return node::Statement {.variant = statement_let};
            }

            throw std::runtime_error("Invalid expression");
        }

        return {};
    }

    std::optional<node::Program> parse_program(){
        node::Program program;
        while(peek().has_value()) {
            if (auto statement = parse_statement()){
                program.statements.push_back(statement.value());
                continue;
            }

            throw std::runtime_error("Invalid statement");
        }

        return program;
    }
private:
    [[nodiscard]] std::optional<Token> peek(const int offset = 0) const {
        if (m_current_index + offset >= m_tokens.size()){
            return {};
        }

        return m_tokens.at(m_current_index + offset);
    }

    Token consume() {
        return m_tokens.at(m_current_index++);
    }
    const std::vector<Token> m_tokens;
    size_t m_current_index = 0;

};