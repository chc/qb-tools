#include "script.h"
#include <assert.h>
#include <NameToken.h>
#include "engine.h"

#include <NameToken.h>
#include <NameSymbol.h>
#include <IntegerToken.h>
#include <IntegerSymbol.h>
#include <FloatToken.h>
#include <FloatSymbol.h>
#include <StringSymbol.h>
#include <StringToken.h>

#include <ArraySymbol.h>
#include <LocalStringToken.h>
#include <LocalStringSymbol.h>
#include <ReferenceItemSymbol.h>
#include <PairToken.h>
#include <PairSymbol.h>
#include <VectorToken.h>
#include <VectorSymbol.h>
#include <ArgumentPackToken.h>

#include <algorithm>
#include <array>
#include <functional>

//move this...
int get_precedence(QScriptToken *token) {
    switch(token->GetType()) {
        case ESCRIPTTOKEN_OR:
        case ESCRIPTTOKEN_KEYWORD_OR:
            return 1;
        case ESCRIPTTOKEN_AND:
        case ESCRIPTTOKEN_KEYWORD_AND:
            return 2;
        case ESCRIPTTOKEN_ADD:
        case ESCRIPTTOKEN_MINUS:
            return 4;
        case ESCRIPTTOKEN_MULTIPLY:
        case ESCRIPTTOKEN_DIVIDE:
            return 5;
        case ESCRIPTTOKEN_GREATERTHAN:
        case ESCRIPTTOKEN_GREATERTHANEQUAL:
        case ESCRIPTTOKEN_LESSTHAN:
        case ESCRIPTTOKEN_LESSTHANEQUAL:
            return 3;
    }
    return 0;
}
bool is_operand(QScriptToken *token) {
    switch(token->GetType()) {
        case ESCRIPTTOKEN_INTEGER:
        case ESCRIPTTOKEN_STRING:
        case ESCRIPTTOKEN_FLOAT:
        case ESCRIPTTOKEN_PAIR:
        case ESCRIPTTOKEN_VECTOR:
        //what about arrays... startstruct ??
        return true;
    }
    return false;
}
bool is_operator(QScriptToken *token) {
    switch(token->GetType()) {
        case ESCRIPTTOKEN_EQUALS:
        case ESCRIPTTOKEN_KEYWORD_NOT:
        case ESCRIPTTOKEN_GREATERTHAN:
        case ESCRIPTTOKEN_GREATERTHANEQUAL:
        case ESCRIPTTOKEN_LESSTHAN:
        case ESCRIPTTOKEN_LESSTHANEQUAL:
        case ESCRIPTTOKEN_ADD:
        case ESCRIPTTOKEN_MINUS:
        case ESCRIPTTOKEN_MULTIPLY:
        case ESCRIPTTOKEN_DIVIDE:
        case ESCRIPTTOKEN_OR:
        case ESCRIPTTOKEN_KEYWORD_OR:
        case ESCRIPTTOKEN_AND:
        case ESCRIPTTOKEN_KEYWORD_AND:
            return true;
    }
    return false;
}
//

//XXX: move this into lib... taken from qopt
QSymbol *ConvertToken(QScriptToken *token, bool make_reference, ArgumentPackToken *last_argpack) {
    if(make_reference) {      
        assert(last_argpack);
        ReferenceItemSymbol *ref = new ReferenceItemSymbol(last_argpack->GetRefType(), last_argpack->GetIsRequiredParams());
        //for now refs always point to structs... maybe it works this way?
        NameToken *name = reinterpret_cast<NameToken *>(token);
        ref->SetValue(name->GetChecksum());
        return ref;
    }

    PairToken *pt;
    VectorToken *vt;
    switch(token->GetType()) {
        case ESCRIPTTOKEN_INTEGER:
            return new IntegerSymbol(reinterpret_cast<IntegerToken*>(token)->GetValue());
        case ESCRIPTTOKEN_FLOAT:
            return new FloatSymbol(reinterpret_cast<FloatToken*>(token)->GetValue());
        case ESCRIPTTOKEN_NAME:
            return new NameSymbol(reinterpret_cast<NameToken*>(token)->GetChecksum());
        case ESCRIPTTOKEN_STRING:
            return new StringSymbol(reinterpret_cast<StringToken*>(token)->GetValue());
        case ESCRIPTTOKEN_LOCALSTRING:
            return new LocalStringSymbol(reinterpret_cast<LocalStringToken*>(token)->GetValue());
        case ESCRIPTTOKEN_PAIR:
            pt = reinterpret_cast<PairToken*>(token);
            return new PairSymbol(pt->GetX(), pt->GetY());
        case ESCRIPTTOKEN_VECTOR:
            vt = reinterpret_cast<VectorToken*>(token);
            return new VectorSymbol(vt->GetX(), vt->GetY(), vt->GetZ());
            
    }
    assert(false);
    return nullptr;
}

QScript::QScript(QEngine *engine) : m_evaluator_state(EvaluatorState_ReadStatement), m_callfunc_name(0), mp_engine(engine), m_got_arg_token(false) {

}

void QScript::evaluate(QScriptToken *token) {
    switch(m_evaluator_state) {
        case EvaluatorState_ReadStatement:
            evaluate_statement(token);
        break;
        case EvaluatorState_ReadArgs:
            evaluate_read_args(token);
        break;
        case EvaluatorState_ReadEquals:
            evaluate_read_equals(token);
        break;
    }
}
void QScript::evaluate_read_equals(QScriptToken *token) {
    switch(token->GetType()) {
        /*case ESCRIPTTOKEN_INTEGER:
        case ESCRIPTTOKEN_FLOAT:
        case ESCRIPTTOKEN_NAME:
        case ESCRIPTTOKEN_STRING:
        case ESCRIPTTOKEN_LOCALSTRING:
        case ESCRIPTTOKEN_PAIR:
        case ESCRIPTTOKEN_VECTOR:*/
        default:
            current_tokens.push_back(token);
            break;
        case ESCRIPTTOKEN_ENDOFLINE:
        case ESCRIPTTOKEN_ENDOFLINENUMBER:
            m_evaluator_state = EvaluatorState_ReadStatement;
            finish_read_equals();
            break;
    }
}

int get_integer(QScriptToken *token) {
    switch(token->GetType()) {
        case ESCRIPTTOKEN_INTEGER:
            return (int)((IntegerToken*)token)->GetValue();
        default:
            assert(false);
    }
}
QScriptToken * perform_add(EScriptToken output_type, std::stack<QScriptToken *> &stack) {
    QScriptToken *lh, *rh;
    rh = stack.top(); stack.pop();
    lh = stack.top(); stack.pop();
    switch(output_type) {
        case ESCRIPTTOKEN_INTEGER:
            return new IntegerToken(get_integer(rh)+get_integer(lh));
        break;
    }
    return nullptr;
}

QScriptToken * perform_subtract(EScriptToken output_type, std::stack<QScriptToken *> &stack) {
    QScriptToken *lh, *rh;
    rh = stack.top(); stack.pop();
    lh = stack.top(); stack.pop();
    switch(output_type) {
        case ESCRIPTTOKEN_INTEGER:
            return new IntegerToken(get_integer(rh)-get_integer(lh));
        break;
    }
    return nullptr;
}
QScriptToken * perform_multiply(EScriptToken output_type, std::stack<QScriptToken *> &stack) {
    QScriptToken *lh, *rh;
    rh = stack.top(); stack.pop();
    lh = stack.top(); stack.pop();
    switch(output_type) {
        case ESCRIPTTOKEN_INTEGER:
            return new IntegerToken(get_integer(rh)*get_integer(lh));
        break;
    }
    return nullptr;
}
QScriptToken * perform_divide(EScriptToken output_type, std::stack<QScriptToken *> &stack) {
    QScriptToken *lh, *rh;
    rh = stack.top(); stack.pop();
    lh = stack.top(); stack.pop();
    switch(output_type) {
        case ESCRIPTTOKEN_INTEGER:
            if(get_integer(lh) == 0) {
                return new IntegerToken(0);    
            }
            return new IntegerToken(get_integer(rh)/get_integer(lh));
        break;
    }
    return nullptr;
}

std::vector<QScriptToken *> convertToRPN(std::vector<QScriptToken *> token_list) {
	std::vector<QScriptToken *> tokens;
	std::vector<QScriptToken *>::iterator it = token_list.begin();
	std::stack<QScriptToken *> operator_stack;
	while(it != token_list.end()) {
		QScriptToken *cur_token = *it;
		EScriptToken type = cur_token->GetType();
		if(is_operand(cur_token)) { //catch all operands
			tokens.push_back(cur_token);
		} else if(is_operator(cur_token)) {
			while(!operator_stack.empty() && is_operator(operator_stack.top())) {
				if(get_precedence(operator_stack.top()) >= get_precedence(cur_token)) {
					tokens.push_back(operator_stack.top());
					operator_stack.pop();
				} else break;
			}
			operator_stack.push(cur_token);
		} else if(type == ESCRIPTTOKEN_OPENPARENTH) {
			operator_stack.push(cur_token);
		} else if(type == ESCRIPTTOKEN_CLOSEPARENTH) {
			while(!operator_stack.empty() && operator_stack.top()->GetType() != ESCRIPTTOKEN_OPENPARENTH) {
				tokens.push_back(operator_stack.top());
				operator_stack.pop();
			}

			if(!operator_stack.empty()) { //if this doesn't pass then there is a mismatch
				operator_stack.pop();
			}
		}
		it++;

	}

	while(!operator_stack.empty()) {
		tokens.push_back(operator_stack.top());
		operator_stack.pop();
	}
	return tokens;
}
void QScript::finish_read_equals() {
    std::vector<QScriptToken *> converted_tokens = convertToRPN(current_tokens);
    std::vector<QScriptToken *>::iterator it = converted_tokens.begin();
    EScriptToken output_type = ESCRIPTTOKEN_INTEGER;
    EScriptToken operation = ESCRIPTTOKEN_ENDOFFILE;

    std::stack<QScriptToken *> operand_stack;


    while(it != converted_tokens.end()) {
        QScriptToken *t = *it;
        if(is_operand(t)) {
            operand_stack.push(t);
        } else if(is_operator(t)) {
            switch(t->GetType()) {
                case ESCRIPTTOKEN_KEYWORD_NOT:
                break;
                case ESCRIPTTOKEN_GREATERTHAN:
                break;
                case ESCRIPTTOKEN_GREATERTHANEQUAL:
                break;
                case ESCRIPTTOKEN_LESSTHAN:
                break;
                case ESCRIPTTOKEN_LESSTHANEQUAL:
                break;
                case ESCRIPTTOKEN_ADD:
                    operand_stack.push(perform_add(output_type, operand_stack));
                break;
                case ESCRIPTTOKEN_MINUS:
                    operand_stack.push(perform_subtract(output_type, operand_stack));
                break;
                case ESCRIPTTOKEN_MULTIPLY:
                    operand_stack.push(perform_multiply(output_type, operand_stack));
                break;
                case ESCRIPTTOKEN_DIVIDE:
                    operand_stack.push(perform_divide(output_type, operand_stack));
                break;
                case ESCRIPTTOKEN_OR:
                break;
                case ESCRIPTTOKEN_KEYWORD_OR:
                break;
                case ESCRIPTTOKEN_AND:
                break;
                case ESCRIPTTOKEN_KEYWORD_AND:
                break;
            }
        }
        it++;
    }

    assert(operand_stack.size() == 1);
    QScriptToken *root_symbol = operand_stack.top();
    current_symbols[m_callfunc_name] = ConvertToken(root_symbol, false, nullptr);

    current_tokens.clear();
}
QSymbol *QScript::Resolve(uint32_t name) {
    if(current_symbols.find(name) != current_symbols.end()) {
        return current_symbols[name];
    }
    return nullptr;
}
void QScript::evaluate_read_args(QScriptToken *token) {
    StructureSymbol *ssym = nullptr;
    NameToken *name = nullptr;
    QSymbol *resolved_symbol;
    switch(token->GetType()) {
        case ESCRIPTTOKEN_NAME:
            if(m_got_arg_token) {
                name = reinterpret_cast<NameToken*>(token);
                resolved_symbol = Resolve(name->GetChecksum());
                assert(resolved_symbol);
                current_args.push_back(resolved_symbol);
                m_got_arg_token = false;
                return;
            }
        case ESCRIPTTOKEN_INTEGER:
        case ESCRIPTTOKEN_FLOAT:
        case ESCRIPTTOKEN_STRING:
        case ESCRIPTTOKEN_LOCALSTRING:
        case ESCRIPTTOKEN_PAIR:
        case ESCRIPTTOKEN_VECTOR:
            current_args.push_back(ConvertToken(token, false, nullptr));
        break;
        case ESCRIPTTOKEN_ARG:
            m_got_arg_token = true;
        break;
        case ESCRIPTTOKEN_EQUALS:
            m_evaluator_state = EvaluatorState_ReadEquals;
        break;
        case ESCRIPTTOKEN_ENDOFLINE:
        case ESCRIPTTOKEN_ENDOFLINENUMBER:
            printf("evalulate eol... RUN %08x\n", m_callfunc_name);
            m_evaluator_state = EvaluatorState_ReadStatement;
            ssym = new StructureSymbol(current_args);
            
            mp_engine->run_script(m_callfunc_name, ssym);
            current_args.clear();
            return;
        break;
        default:
            assert(false);
    }
}
void QScript::evaluate_statement(QScriptToken *token) {
    switch(token->GetType()) {
        case ESCRIPTTOKEN_NAME:
            m_evaluator_state = EvaluatorState_ReadArgs;
            m_callfunc_name = reinterpret_cast<NameToken*>(token)->GetChecksum();
        break;

        case ESCRIPTTOKEN_KEYWORD_ENDSCRIPT:
            //exit... should not proceed further
        break;
        // case ESCRIPTTOKEN_KEYWORD_FASTIF:
        
        // break;
        case ESCRIPTTOKEN_ENDOFLINE:
        case ESCRIPTTOKEN_ENDOFLINENUMBER:
            m_evaluator_state = EvaluatorState_ReadStatement;
        break;
        default:
            assert(false);
    }
}
