#ifndef _ELSETOKEN_H
#define _ELSETOKEN_H
#include "QScriptToken.h"

class ElseToken : public QScriptToken {
    public:
        ElseToken() {

        }
        ~ElseToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_ELSE;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        std::string ToString() {
            return "else";
        }
        int GetPreTabOffset() { return -1; }
        int GetPostTabOffset() { return 1; }
    private:
};
#endif //_ELSETOKEN_H