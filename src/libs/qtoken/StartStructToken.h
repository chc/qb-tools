#ifndef _STARTSTRUCTTOKEN_H
#define _STARTSTRUCTTOKEN_H
#include "QScriptToken.h"

class StartStructToken : public QScriptToken {
    public:
        StartStructToken() {

        }
        ~StartStructToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_STARTSTRUCT;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        std::string ToString() {
            return "{";
        }
    private:
};
#endif //_STARTSTRUCTTOKEN_H