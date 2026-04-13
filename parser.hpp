#pragma once

#include <vector>
#include <string>
#include <optional>
#include <iostream>
#include <variant>
#include "./tokenization.hpp"

struct NodeExprIntLit {
    Token int_lit;
};

struct NodeExprIdent {
    Token ident;
};

struct NodeExpr;

struct BinExprAdd {
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct BinExprMulti{
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct BinExpr {
    std::variant<BinExprAdd*, BinExprMulti*> var;
};

struct NodeExpr {
    std::variant<NodeExprIntLit*, NodeExprIdent*, BinExpr*> var;
};

struct NodeStmtExit{
    NodeExpr expr;
};

struct NodeStmtLet{
    Token ident;
    NodeExpr expr;
};

struct NodeStmt {
    std::variant<NodeStmtExit, NodeStmtLet> var;
};

struct NodeProgram {
    std::vector<NodeStmt> stmts;
};

class Parser {
public:
    inline explicit Parser(std::vector<Token> tokens)
        : m_tokens(std::move(tokens))
    {
    }

    std::optional<NodeExpr> parse_expr() {
        if (peak().has_value() && peak().value().type == TokenType::int_lit){
            return NodeExpr{NodeExprIntLit{consume()}};
        }
        else if (peak().has_value() && peak().value().type == TokenType::ident){
            return NodeExpr{NodeExprIdent{consume()}};
        }
        else{
            return {};
        }
    }

    std::optional<NodeStmt> parse() {
            if (peak().value().type == TokenType::exit && peak(1).has_value() && peak(1).value().type == TokenType::open_paren){
                consume();
                consume();
                NodeStmtExit stmt_exit;
                if(auto node_expr = parse_expr()){
                    stmt_exit = {node_expr.value()};
                }
                else{
                    std::cerr << "invalid expression" << std::endl;
                    exit(EXIT_FAILURE);
                }
                if (peak().has_value() && peak().value().type == TokenType::close_paren){
                    consume();
                }
                else{
                    std::cerr << "expected ')'" << std::endl;
                    exit(EXIT_FAILURE);
                }
                if(peak().has_value() && peak().value().type == TokenType::semi){
                    consume();
                }
                else{
                    std::cerr << "expected ';'" << std::endl;
                    exit(EXIT_FAILURE);
                }
                return NodeStmt{stmt_exit};
            } else if (peak().has_value() && peak().value().type == TokenType::let 
                        && peak(1).has_value() && peak(1).value().type == TokenType::ident 
                        && peak(2).has_value() && peak(2).value().type == TokenType::equal){
                consume();
                auto stmt_let = NodeStmtLet{consume()};
                consume();
                if (auto expr = parse_expr()) {
                    stmt_let.expr = expr.value();
                } else {
                    std::cerr <<"Invalid Expression" <<std::endl;
                    exit(EXIT_FAILURE);
                }
                if (peak().has_value() && peak().value().type == TokenType::semi){
                    consume();
                } else {
                    std::cerr << "Expected ;" << std::endl;
                    exit(EXIT_FAILURE);
                }
                return NodeStmt{stmt_let};
            } else {
                return {};
            }
            
    }

    std::optional<NodeProgram> parse_prog(){
        NodeProgram prog;

        while(peak().has_value()){
            if (auto stmt = parse()){
                prog.stmts.push_back(stmt.value());    
            } else {
            std::cerr << "Invalid Statement" <<std::endl;
            exit(EXIT_FAILURE);
            } 
        }

        return prog;
    }

private:
    [[nodiscard]] inline std::optional<Token> peak(int offset = 0) const {
        if (m_index + offset >= m_tokens.size()){
            return {};
        }
        else {
            return m_tokens.at(m_index + offset);
        }
    }

    inline Token consume(){
        return m_tokens.at(m_index++);
    }
    
    const std::vector<Token> m_tokens;
    size_t m_index = 0;
};