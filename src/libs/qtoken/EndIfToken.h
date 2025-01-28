#ifndef _ENDIFTOKEN_H
#define _ENDIFTOKEN_H
#include "QScriptToken.h"

class EndIfToken : public QScriptToken {
    public:
        EndIfToken() {

        }
        ~EndIfToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_ENDIF;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_KEYWORD_ENDIF);
        }
        std::string ToString() {
            return "endif";
        }
        int GetPreTabOffset() { return -1; }
    private:
};
#endif //_ENDIFTOKEN_H