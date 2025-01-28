#ifndef _BREAKTOKEN_H
#define _BREAKTOKEN_H
#include "QScriptToken.h"

class BreakToken : public QScriptToken {
    public:
        BreakToken() {
            
        }
        ~BreakToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_BREAK;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        std::string ToString() {
            return AppendSpaceToString ? "break " : "break";
        }
    private:
};
#endif //_BREAKTOKEN_H