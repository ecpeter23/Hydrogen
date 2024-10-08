#pragma once

#include <unordered_map>
#import <unordered_map>

class Generator{
public:
    explicit Generator(node::Program program)
        : m_program(std::move(program))
    {
    }

    void generate_expression(const node::Expression& expression){
        struct ExpressionVisitor{
            Generator* generator;

            void operator()(const node::ExpressionIntLiteral& expression_int_literal) const{
                generator->m_output << "    mov w16, " << expression_int_literal.int_literal.value.value() << "\n";
                generator->push("w16");
            }
            void operator()(const node::ExpressionIdentifier& expression_identifier) const{
                const auto& var = expression_identifier.identifier.value.value();
                if (!generator->m_variables.contains(var)) throw std::runtime_error("Undeclared identifier: " + expression_identifier.identifier.value.value());
                generator->m_output << "    ldr w16, [sp, #" << (generator->m_variables.at(var).stack_location * 8) << "]\n";
                generator->push("w16");
            }
        };

        ExpressionVisitor visitor { .generator = this };
        std::visit(visitor, expression.variant);
    }

    void generate_statement(const node::Statement& statement){
        struct StatementVisitor{
            Generator* generator;

            void operator()(const node::StatementExit& statement_exit) const{
                generator->generate_expression(statement_exit.expression);
                generator->m_output << "    mov w16, 1\n";  // Load system call number into x8
                generator->pop("w0");
                generator->m_output << "    add sp, sp, #" << generator->m_stack_size * 8 << "\n";  // De-allocate 16 bytes from the stack at the end
                generator->m_output << "    svc 0\n";  // System call instruction
            }
            void operator()(const node::StatementLet& statement_let) const{
                if (generator->m_variables.contains(statement_let.identifier.value.value())) throw std::runtime_error("Identifier already used: " + statement_let.identifier.value.value());

                generator->m_variables.insert({statement_let.identifier.value.value(), Variable { .stack_location = generator->m_stack_size } });
                generator->generate_expression(statement_let.expression);
            }
        };

        StatementVisitor visitor { .generator = this};
        std::visit(visitor, statement.variant);

    }

    [[nodiscard]] std::string generate_program()
    {
        m_output << ".global _main\n";  // Global directive to specify entry point
        m_output << ".align 2\n";        // Align directive for 64-bit boundary
        m_output << "_main:\n";         // Label for program entry point

        m_output << "    sub sp, sp, #16\n";  // Allocate 16 bytes on the stack at the beginning

        for(const node::Statement& statement : m_program.statements){
            generate_statement(statement);
        }

        m_output << "    add sp, sp, #16\n";  // De-allocate 16 bytes from the stack at the end
        m_output << "    mov w16, 1\n";  // Load system call number into x8
        m_output << "    mov w0, 0\n";  // Move return value into x0
        m_output << "    svc 0\n";  // System call instruction


        return m_output.str();
    }
private:

    void push(const std::string& _register){
        m_output << "    str " << _register << ", [sp, #" << m_stack_size * 8 << "]\n";
        m_stack_size++;
    }

    void pop(const std::string& _register){
        m_stack_size--;
        m_output << "    ldr " << _register << ", [sp, #" << m_stack_size * 8 << "]\n";
    }

    void pop(const std::string& _register, const size_t stack_position){
        m_stack_size--;
        m_output << "    ldr " << _register << ", [sp, #" << stack_position * 8 << "]\n";
    }

    struct Variable {
        size_t stack_location;
    };

    const node::Program m_program;
    std::stringstream m_output;
    size_t m_stack_size = 0;
    std::unordered_map<std::string, Variable> m_variables {};
};
