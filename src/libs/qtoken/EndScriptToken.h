#ifndef _ENDSCRIPTTOKEN_H
#define _ENDSCRIPTTOKEN_H
#include "QScriptToken.h"

class EndScriptToken : public QScriptToken {
    public:
        EndScriptToken() {

        }
        ~EndScriptToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_ENDSCRIPT;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        std::string ToString() {
            return "endscript";
        }
        int GetPreTabOffset() { return -1; }
    private:
};
#endif //_ENDSCRIPTTOKEN_H