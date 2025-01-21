#include "QScriptToken.h"
#include <cassert>

#include <stdio.h>

#include "EndOfFileToken.h"
#include "EndOfLineToken.h"
#include "EndOfLineNumberToken.h"
#include "NameToken.h"
#include "IntegerToken.h"
#include "StringToken.h"
#include "LocalStringToken.h"
#include "FloatToken.h"
#include "PairToken.h"
#include "VectorToken.h"
#include "EqualsToken.h"
#include "ChecksumNameToken.h"
#include "ScriptToken.h"
#include "EndScriptToken.h"
#include "IfToken.h"
#include "ElseToken.h"
#include "EndIfToken.h"
#include "NotToken.h"
#include "NotEqualToken.h"
#include "AndToken.h"
#include "OrToken.h"
#include "LessThanToken.h"
#include "GreaterThanToken.h"
#include "AllArgsToken.h"
#include "ArgToken.h"
#include "StartArrayToken.h"
#include "StartStructToken.h"
#include "EndArrayToken.h"
#include "EndStructToken.h"
#include "OpenParenthesisToken.h"
#include "CloseParenthesisToken.h"
#include "BeginToken.h"
#include "RepeatToken.h"
#include "SwitchToken.h"
#include "CaseToken.h"
#include "DefaultToken.h"
#include "EndSwitchToken.h"
#include "BreakToken.h"
#include "ColonToken.h"
#include "CommaToken.h"
#include "ReturnToken.h"
#include "RandomToken.h"
#include "RandomRangeToken.h"
#include "RandomNoRepeatToken.h"
#include "RandomPermuteToken.h"
#include "MinusToken.h"
#include "AddToken.h"
#include "MultiplyToken.h"
#include "DivideToken.h"
#include "JumpToken.h"
#include "DotToken.h"
#include "FastIfToken.h"
#include "FastElseToken.h"
#include "ArgumentPackToken.h"
#include "InlinePackStructToken.h"
#include "WideStringToken.h"
#include "ShortJumpToken.h"
#include "ElseIfToken.h"
#include "LessThanEqualToken.h"
#include "GreaterThanEqualToken.h"
#include "StringQSToken.h"
#include "RandomIntegerToken.h"
#include "RandomFloatToken.h"
QScriptToken::QScriptToken() {

}
QScriptToken::~QScriptToken() {

}
QScriptToken *QScriptToken::Resolve(uint8_t token) {
    switch(token) {
        case ESCRIPTTOKEN_ENDOFLINE:
            return new EndOfLineToken();
        case ESCRIPTTOKEN_ENDOFLINENUMBER:
            return new EndOfLineNumberToken();
        case ESCRIPTTOKEN_NAME:
            return new NameToken();
        case ESCRIPTTOKEN_INTEGER:
            return new IntegerToken();
        case ESCRIPTTOKEN_FLOAT:
            return new FloatToken();
        case ESCRIPTTOKEN_PAIR:
            return new PairToken();
        case ESCRIPTTOKEN_VECTOR:
            return new VectorToken();
        case ESCRIPTTOKEN_STRING:
            return new StringToken();
        case ESCRIPTTOKEN_LOCALSTRING:
            return new LocalStringToken();
        case ESCRIPTTOKEN_EQUALS:
            return new EqualsToken();
        case ESCRIPTTOKEN_KEYWORD_SCRIPT:
            return new ScriptToken();
        case ESCRIPTTOKEN_KEYWORD_ENDSCRIPT:
            return new EndScriptToken();
        case ESCRIPTTOKEN_KEYWORD_IF:
            return new IfToken();
        case ESCRIPTTOKEN_KEYWORD_ELSE:
            return new ElseToken();
        case ESCRIPTTOKEN_KEYWORD_ELSEIF:
            return new ElseIfToken();
        case ESCRIPTTOKEN_KEYWORD_ENDIF:
            return new EndIfToken();
        case ESCRIPTTOKEN_KEYWORD_NOT:
            return new NotToken();
        case ESCRIPTTOKEN_NOTEQUAL:
            return new NotEqualToken();            
        case ESCRIPTTOKEN_AND:
            return new AndToken();
        case ESCRIPTTOKEN_OR:
            return new OrToken();
        case ESCRIPTTOKEN_LESSTHAN:
            return new LessThanToken();
        case ESCRIPTTOKEN_LESSTHANEQUAL:
            return new LessThanEqualToken();
        case ESCRIPTTOKEN_GREATERTHAN:
            return new GreaterThanToken();
        case ESCRIPTTOKEN_GREATERTHANEQUAL:
            return new GreaterThanEqualToken();
        case ESCRIPTTOKEN_MINUS:
            return new MinusToken();
        case ESCRIPTTOKEN_ADD:
            return new AddToken();
        case ESCRIPTTOKEN_MULTIPLY:
            return new MultiplyToken();
        case ESCRIPTTOKEN_DIVIDE:
            return new DivideToken();
        case ESCRIPTTOKEN_KEYWORD_BEGIN:
            return new BeginToken();
        case ESCRIPTTOKEN_KEYWORD_REPEAT:
            return new RepeatToken();
        case ESCRIPTTOKEN_ARG:
            return new ArgToken();
        case ESCRIPTTOKEN_KEYWORD_ALLARGS:
            return new AllArgsToken();
        case ESCRIPTTOKEN_STARTARRAY:
            return new StartArrayToken();
        case ESCRIPTTOKEN_ENDARRAY:
            return new EndArrayToken();
        case ESCRIPTTOKEN_STARTSTRUCT:
            return new StartStructToken();
        case ESCRIPTTOKEN_ENDSTRUCT:
            return new EndStructToken();
        case ESCRIPTTOKEN_OPENPARENTH:
            return new OpenParenthesisToken();
        case ESCRIPTTOKEN_CLOSEPARENTH:
            return new CloseParenthesisToken();
        case ESCRIPTTOKEN_CHECKSUM_NAME:
            return new ChecksumNameToken();
        case ESCRIPTTOKEN_ENDOFFILE:
            return new EndOfFileToken();
        case ESCRIPTTOKEN_KEYWORD_SWITCH:
            return new SwitchToken();
        case ESCRIPTTOKEN_KEYWORD_ENDSWITCH:
            return new EndSwitchToken();
        case ESCRIPTTOKEN_KEYWORD_CASE:
            return new CaseToken();
        case ESCRIPTTOKEN_KEYWORD_DEFAULT:
            return new DefaultToken();
        case ESCRIPTTOKEN_KEYWORD_BREAK:
            return new BreakToken();
        case ESCRIPTTOKEN_COLON:
            return new ColonToken();
        case ESCRIPTTOKEN_COMMA:
            return new CommaToken();
        case ESCRIPTTOKEN_KEYWORD_RETURN:
            return new ReturnToken();
        case ESCRIPTTOKEN_KEYWORD_RANDOM:
            return new RandomToken();
        case ESCRIPTTOKEN_KEYWORD_RANDOM_RANGE:
            return new RandomRangeToken();
        case ESCRIPTTOKEN_KEYWORD_RANDOM_NO_REPEAT:
            return new RandomNoRepeatToken();
        case ESCRIPTTOKEN_KEYWORD_RANDOM_PERMUTE:
            return new RandomPermuteToken();            
        case ESCRIPTTOKEN_JUMP:
            return new JumpToken();
        case ESCRIPTTOKEN_DOT:
            return new DotToken();
        case ESCRIPTTOKEN_KEYWORD_FASTIF:
            return new FastIfToken();
        case ESCRIPTTOKEN_KEYWORD_FASTELSE:
            return new FastElseToken();
        case ESCRIPTTOKEN_ARGUMENTPACK:
            return new ArgumentPackToken();
#ifdef WITH_SYMBOL_SUPPORT
        case ESCRIPTTOKEN_INLINEPACKSTRUCT:
            return new InlinePackStructToken();
#endif
        case ESCRIPTTOKEN_WIDESTRING:
            return new WideStringToken();
        case ESCRIPTTOKEN_SHORTJUMP:
            return new ShortJumpToken();
        case ESCRIPTTOKEN_STRINGQS:
            return new StringQSToken();
        case ESCRIPTTOKEN_KEYWORD_RANDOMFLOAT:
            return new RandomFloatToken();
        case 81:
        case 82:
        case 83:
        case 84:
        case 85:
        case 86:
        case 87:
        case 88:
        case 89:
        case 90:
        printf("** unhandled type: %d\n", token);
        case ESCRIPTTOKEN_KEYWORD_RANDOMINTEGER:
            return new RandomIntegerToken();
            
    }
    fprintf(stderr, "Failed to resolve token with id: %d - %02x\n", token, token);
    assert(false);
    return nullptr;
}
int	QScriptToken::GetPreTabOffset() {
    return 0;
}
int QScriptToken::GetPostTabOffset() {
    return 0;
}
std::vector<TokenInjection> QScriptToken::GetInjections() {
    return std::vector<TokenInjection>();
}
void QScriptToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(GetType());
 }