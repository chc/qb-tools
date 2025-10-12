#ifndef _SCRIPT_H
#define _SCRIPT_H
#include <stdint.h>
#include <QScriptToken.h>
#include <map>

class QEngine;
class QSymbol;
enum EvaluatorState {
    EvaluatorState_ReadStatement,
    EvaluatorState_ReadArgs,
    EvaluatorState_ReadEquals,
};

class QScript {
    public:
        QScript(QEngine *engine);
        void evaluate(QScriptToken *token);
    private:    
        void evaluate_statement(QScriptToken *token);
        void evaluate_read_args(QScriptToken *token);
        void evaluate_read_equals(QScriptToken *token);
        void finish_read_equals();
        QSymbol *Resolve(uint32_t name);
        QEngine *mp_engine;
        EvaluatorState m_evaluator_state;
        std::vector<QSymbol *> current_args;
        std::map<uint32_t, QSymbol *> current_symbols;
        std::vector<QScriptToken *> current_tokens;
        uint32_t m_callfunc_name;
        bool m_got_arg_token;
};

#endif //_SCRIPT_H