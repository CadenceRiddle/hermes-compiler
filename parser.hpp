#pragma once

#include <vector>
#include <string>
#include <optional>
#include <iostream>
#include "./tokenization.hpp"


struct NodeExpr {
    Token int_lit;
};

struct NodeExit {
    NodeExpr expr;
};

class Parser {
public:
    inline explicit Parser(std::vector<Token> tokens)
        : m_tokens(std::move(tokens))
    {
    }

    NodeExit parse() {
        
    }

private:
    [[nodiscard]] inline std::optional<Token> peak(int ahead = 1) const {
        if (m_index + ahead > m_tokens.size()){
            return {};
        }
        else {
            return m_tokens.at(m_index);
        }
    }

    inline Token consume(){
        return m_tokens.at(m_index++);
    }

    const std::vector<Token> m_tokens;
    size_t m_index = 0;
}