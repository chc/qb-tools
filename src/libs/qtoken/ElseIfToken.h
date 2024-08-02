#ifndef _ELSEIFTOKEN_H
#define _ELSEIFTOKEN_H
#include "QScriptToken.h"

class ElseIfToken : public QScriptToken {
    public:
        ElseIfToken();
        ~ElseIfToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
        int GetPreTabOffset();
        int GetPostTabOffset();
        void Write(IStream *stream);
    private:
        uint32_t m_offset;
};
#endif //_ELSEIFTOKEN_H