#pragma once

#include "parser.hpp"
#include <sstream>

class Generator{
    public:
        inline Generator(NodeProgram prog): m_prog(std::move(prog))
        {
        }

        void gen_expr(const NodeExpr& expr){
            struct ExprVisitor {
                Generator* gen;

                void operator()(const NodeExprIntLit& expr_int_lit) const {
                    gen->m_output << "    mov rax, " << expr_int_lit.int_lit.value.value() << "\n";
                    gen->m_output << "    push rax\n";
                }
                void operator()(const NodeExprIdent& expr_ident){
                    // TODO
                }
            };

            ExprVisitor visitor{.gen = this};
            std::visit(visitor, expr.var);
        }

        void gen_stmt(const NodeStmt& stml) {
            struct StmtVistor {
                Generator* gen;
                void operator()(const NodeStmtExit& stmt_exit) const {
                    gen->gen_expr(stmt_exit.expr);
                    gen->m_output << "    mov rax, 60\n";
                    gen->m_output << "    pop rdi\n";
                    gen->m_output << "    syscall \n";
                }
                void operator()(const NodeStmtLet& stmt_let){
                    
                }
            };

            StmtVistor visitor{.gen = this};
            std::visit(visitor, stml.var);
        }

        [[nodiscard]] std::string gen_prog() {
            m_output << "global _start\n_start:\n";

            for(const NodeStmt& stmt : m_prog.stmts){
                gen_stmt(stmt);
            }

            return m_output.str();
        }
    private:
        const NodeProgram m_prog;
        std::stringstream m_output;
};