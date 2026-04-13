#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include "./parser.hpp"

class Generator {
public:
    inline Generator(NodeProgram prog)
        : m_prog(prog)
    {
    }

    void gen_expr(const NodeExpr& expr) {
        struct ExprVisitor {
            Generator* gen;
            ExprVisitor(Generator* gen)
                : gen(gen)
            {  
            };

            void operator()(const NodeExprIntLit& expr_int_lit){
                gen->m_output << "    mov rax, " << expr_int_lit.int_lit.value.value() << "\n";
                gen->push("rax");
            }
            void operator()(const NodeExprIdent& expr_ident){
                
                if (gen->m_vars.find(expr_ident.ident.value.value()) == gen->m_vars.end()){
                    std::cerr << "Undeclared identifier: " << expr_ident.ident.value.value() << std::endl;
                    exit(EXIT_FAILURE);
                }

                const auto& var = gen->m_vars.at(expr_ident.ident.value.value());
                std::stringstream offset;
                offset << "QWORD [rsp + " << (gen->m_stack_size - var.stack_loc - 1) * 8 << "]\n";
                gen->push(offset.str());
            }
        };

        ExprVisitor visitor(this);
        std::visit(visitor, expr.var);
    }

    void gen_stmt(const NodeStmt& stmt) {
        struct StmtVisitor {
            Generator* gen;

            StmtVisitor(Generator* gen)
                : gen(gen)
            {
            };

            void operator()(const NodeStmtExit& stmt_exit){
                gen->gen_expr(stmt_exit.expr);
                gen->m_output << "    mov rax, 60\n";
                gen->pop("rdi");
                gen->m_output << "    syscall\n";
            }
            
            void operator()(const NodeStmtLet& stmt_let){
                if(gen->m_vars.find(stmt_let.ident.value.value()) != gen->m_vars.end()){
                    std::cerr << "Identifier already used: " << stmt_let.ident.value.value();
                    exit(EXIT_FAILURE);
                }

                
                gen->m_vars.insert({stmt_let.ident.value.value(), Var {gen->m_stack_size}});
                gen->gen_expr(stmt_let.expr);
            }
        };

        StmtVisitor visitor(this);
        std::visit(visitor, stmt.var);
    }

    [[nodiscard]]std::string gen_prog() {
        m_output << "global _start\n_start:\n";

        for (const NodeStmt& stmt : m_prog.stmts) {
            gen_stmt(stmt);
        }

        m_output << "    mov rax, 60\n";
        m_output << "    mov rdi, 0\n";
        m_output << "    syscall\n";

        return m_output.str();
    }

private:

    void push(const std::string& reg){
        m_output << "    push " << reg << "\n";
        m_stack_size++;
    }

    void pop(const std::string& reg){
        m_output << "    pop " << reg << "\n";
        m_stack_size--;
    }

    struct Var {
        size_t stack_loc;
    };

    const NodeProgram m_prog;
    std::stringstream m_output;
    size_t m_stack_size = 0;
    std::unordered_map<std::string, Var> m_vars {};
};