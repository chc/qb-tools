#ifndef _ENDOFFILETOKEN_H
#define _ENDOFFILETOKEN_H
#include "QScriptToken.h"

class EndOfFileToken : public QScriptToken {
    public:
        EndOfFileToken() {

        }
        ~EndOfFileToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_ENDOFFILE;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        std::string ToString() {
            return "\n";
        }
    private:
};
#endif //_ENDOFFILETOKEN_H