#pragma once
#include <string>
#include <vector>
#include <optional>
#include <iostream>


enum class TokenType {
    exit, 
    int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value {}; 
}; 

class Tokenizer {
public:
    inline Tokenizer(std::string& src)
       : m_src(std::move(src))
    {
    }

    inline std::vector<Token> tokenize(){

        std::string buf;
        while(peak().has_value()){
            if (std::isalpha(peak().value()))
        }
        for(int i = 0; i < str.length(); i++){
            char c = str.at(i);
            if (std::isalpha(c)){
                buf.push_back(c);
                i++;
                while (std::isalnum(str.at(i))){
                    buf.push_back(str.at(i));
                    i++;
                }
                i--;

                if(buf == "return"){
                    tokens.push_back(Token{TokenType::exit});
                    buf.clear();
                    continue;
                }
                else{
                    std::cerr << "You Messed Up" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else if (std::isdigit(c)){
                buf.push_back(c);
                i++;
                while (std::isdigit(str.at(i))){
                    buf.push_back(str.at(i));
                    i++;
                }
                i--;
                tokens.push_back(Token{TokenType::int_lit, buf});
                buf.clear();
            }
            else if (c == ';'){
                tokens.push_back(Token{TokenType::semi});
            }
            else if (std::isspace(c)){
                continue;
            }
            else{
                std::cerr << "You Messed Up" << std::endl;
                exit(EXIT_FAILURE);
            }

        }
        return tokens;
    }

private:

    [[nodiscard]] std::optional<char> peak(int ahead = 1) const {
        if (m_index + ahead >= m_src.length()){
            return {};
        }
        else {
            return m_src.at(m_index);
        }
    }

    char consume(){
        return m_src.at(m_index++);
    }

    const std::string m_src;
    int m_index;

};