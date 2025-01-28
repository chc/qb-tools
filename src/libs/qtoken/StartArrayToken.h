#ifndef _STARTARRAYTOKEN_H
#define _STARTARRAYTOKEN_H
#include "QScriptToken.h"

class StartArrayToken : public QScriptToken {
    public:
        StartArrayToken() {

        }
        ~StartArrayToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_STARTARRAY;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        std::string ToString() {
            return "[";
        }
    private:
};
#endif //_STARTARRAYTOKEN_H