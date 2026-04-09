#pragma once
#include <string>
#include <sstream>
#include <iostream>
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
                gen->m_output << "   mov rax, " << expr_int_lit.int_lit.value.value() << "\n";
                gen->m_output << "   push rax\n";
            }
            void operator()(const NodeExprIdent& expr_ident){

            }
        };

        ExprVisitor visitor(this);
        std::visit(visitor, expr.var);
    }

    void gen_stmt(const NodeStmt& stmt) {
        struct StmtVisitor {
            Generator* gen;
            void operator()(const NodeStmtExit& stmt_exit){
                gen->gen_expr(stmt_exit.expr);
                gen->m_output << "    mov rax, 60\n";
                gen->m_output << "    pop rdi\n";
                gen->m_output << "    mov rdi, 0\n"; //TODO
                gen->m_output << "    syscall\n";
            }
            
            void operator()(const NodeStmtLet& stmt_let){

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
    const NodeProgram m_prog;
    std::stringstream m_output;
};