#pragma once
#include <string>
#include <vector>
#include <optional>
#include <iostream>


enum class TokenType {
    exit, 
    int_lit,
    semi,
    open_paren,
    close_paren,
    ident,
    let,
    equal, 
    plus,
    star, 
    sub, 
    div, 
    open_curly,
    close_curly,
    if_
};

std::optional<int> bin_prec(TokenType type){
    switch(type){
        case TokenType::plus:
        case TokenType::sub:
            return 0;
        case TokenType::div:
        case TokenType::star:
            return 1;
        default: 
            return {};
    }
}

struct Token {
    TokenType type;
    std::optional<std::string> value {}; 
};

class Tokenizer {
public:
    inline Tokenizer(const std::string& src)
       : m_src(std::move(src))
    {
    }

    inline std::vector<Token> tokenize(){
        std::vector<Token> m_tokens;
        std::string buf;
        while(peak().has_value()){
            if (std::isalpha(peak().value())){
                buf.push_back(consume());
                while (peak().has_value() && std::isalnum(peak().value())){
                    buf.push_back(consume());
                }

                if (buf == "exit"){
                    m_tokens.push_back(Token{TokenType::exit});
                    buf.clear();
                }
                else if(buf == "let"){
                    m_tokens.push_back(Token{TokenType::let});
                    buf.clear();
                }
                else if(buf == "if"){
                    m_tokens.push_back(Token{TokenType::if_});
                    buf.clear();
                }
                else{
                    m_tokens.push_back({TokenType::ident, buf});
                    buf.clear();
                }
            }
            else if(std::isdigit(peak().value())){
                buf.push_back(consume());
                while(peak().has_value() && std::isdigit(peak().value())){
                    buf.push_back(consume());
                }
                m_tokens.push_back(Token{TokenType::int_lit, buf});
                buf.clear();
            }
            else if(peak().value() == '('){
                consume();
                m_tokens.push_back(Token{TokenType::open_paren});
            }
            else if(peak().value() == ')'){
                consume();
                m_tokens.push_back(Token{TokenType::close_paren});
            }
            else if(peak().value() == '{'){
                consume();
                m_tokens.push_back(Token{TokenType::open_curly});
            }
            else if(peak().value() == '}'){
                consume();
                m_tokens.push_back(Token{TokenType::close_curly});
            }
            else if(peak().value() == ';'){
                consume();
                m_tokens.push_back(Token{TokenType::semi});
            }
            else if(peak().value() == '+'){
                consume();
                m_tokens.push_back(Token{TokenType::plus});
            }
            else if(peak().value() == '*'){
                consume();
                m_tokens.push_back(Token{TokenType::star});
            }
            else if(peak().value() == '-'){
                consume();
                m_tokens.push_back(Token{TokenType::sub});
            }
            else if(peak().value() == '/'){
                consume();
                m_tokens.push_back(Token{TokenType::div});
            }
            else if(peak().value() == '='){
                consume(); 
                m_tokens.push_back(Token{TokenType::equal});
            }
            else if(std::isspace(peak().value())){
                consume();
            }
            else {
                std::cerr << "You messed up" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        m_index = 0;
        return m_tokens;
    }

private:

    [[nodiscard]] inline std::optional<char> peak(int offset = 0) const {
        if (m_index + offset >= m_src.length()){
            return {};
        }
        else {
            return m_src.at(m_index + offset);
        }
    }

    inline char consume(){
        return m_src.at(m_index++);
    }

    const std::string m_src;
    size_t m_index = 0;

};