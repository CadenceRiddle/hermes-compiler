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

    [[nodiscard]]std::string generate() const{
        std::stringstream output;
        output << "global _start\n_start:\n";
        output << "    mov rax, 60\n";
        output << "    mov rdi, " << m_prog.expr.int_lit.value.value() << "\n";
        output << "    syscall\n";

        return output.str();
    }

private:
    const NodeProgram m_prog;
};