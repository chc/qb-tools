#ifndef _FASTIFTOKEN_H
#define _FASTIFTOKEN_H
#include "QScriptToken.h"

class FastIfToken : public QScriptToken {
    public:
        FastIfToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        std::string ToString();
        int GetPostTabOffset();
    private:
        uint16_t m_offset;
};
#endif //_FASTIFTOKEN_H